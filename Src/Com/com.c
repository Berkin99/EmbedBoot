/*
 *       ______          __             ____    _       __
 *      / ____/___ ___  / /_  ___  ____/ / /   (_)___  / /__
 *     / __/ / __ `__ \/ __ \/ _ \/ __  / /   / / __ \/ //_/
 *    / /___/ / / / / / /_/ /  __/ /_/ / /___/ / / / / ,<
 *   /_____/_/ /_/ /_/_.___/\___/\__,_/_____/_/_/ /_/_/|_|
 *
 *  EmbedLink Firmware
 *  Copyright (c) 2024 Yeniay RD, All rights reserved.
 *  _________________________________________________________
 *
 *  EmbedLink Firmware is free software: you can redistribute
 *  it and/or  modify it under  the  terms of the  GNU Lesser
 *  General Public License as  published by the Free Software
 *  Foundation,  either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  EmbedLink  Firmware is  distributed  in the  hope that it
 *  will be useful, but  WITHOUT  ANY  WARRANTY; without even
 *  the implied warranty of MERCHANTABILITY or FITNESS FOR A
 *  PARTICULAR PURPOSE.  See  the GNU  Lesser  General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with EmbedLink Firmware. If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <string.h>
#include "sysconfig.h"
#include "com.h"
#include "uart.h"

static comPacket_t txPacket; /* Stores the last transmitted packet */
static comPacket_t rxPacket; /* Stores the last readed packet w hanlder */

comPacket_t comNewPacket(uint8_t type, uint8_t* payload, uint8_t len){
	comPacket_t pk = {
			.start = COM_STA,
			.type = type,
			.len = len
	};
	memcpy(pk.payload, payload, len);
	pk.crc = comCrc(pk.payload, len);
	return pk;
}

comPacket_t comNewCmd(uint8_t cmd){
	comPacket_t pk = {
		.start = COM_STA,
		.type = cmd,
		.len = 0,
	};
	pk.crc = comCrc(pk.payload, 0);
	return pk;
}

/* Configured for Testing : UART */
int8_t comReadPacket(comPacket_t* packet){
	int8_t rslt;

	/* Start Byte */
	rslt = uartRead(&COM_UART, &packet->start, 1);
	if(packet->start != COM_STA) return E_NOT_FOUND; /* No Start Byte */

	/* Type Byte */
	rslt |= uartRead(&COM_UART, &packet->type, 1);
	rslt |= uartRead(&COM_UART, &packet->len, 1);

	if (packet->len > (COM_PACKET_SIZE - 4)) return E_OVERFLOW;
	if (packet->len > 0) rslt |= uartRead(&COM_UART, packet->payload, packet->len);

	/* Crc */
	rslt |= uartRead(&COM_UART, &packet->crc, 1);

	/* Crc Control */
	if(packet->crc != comCrc(packet->payload, packet->len)) return E_CONF_FAIL;
	return rslt;
}

/* Configured for UART */
int8_t comWritePacket(comPacket_t* packet){
	int8_t rslt;
	uint8_t buffer[COM_PACKET_SIZE];

	if (packet->len > (COM_PACKET_SIZE - 4)) return E_OVERFLOW;
	memcpy(buffer, packet, 3);
	if(packet->len > 0) memcpy(&(buffer[3]), packet->payload, packet->len);
	buffer[3 + packet->len] = packet->crc;

	/* Send with one write */
	rslt = uartWrite(&COM_UART, buffer, 4 + packet->len);

	return rslt;
}

int8_t comReadF(comPacket_t* packet, uint8_t ack){
	int8_t rslt;
	uint8_t retry = 0;
	while(retry++ < 3){
		rslt = comReadPacket(packet);
		if(rslt != E_CONF_FAIL) break; /* Error does not caused by CRC calculation : Exit */
		comWriteCmd(COM_NAK);          /* Send NAK for each wrong crc : Packet Request */
	}
	if(rslt == OK && ack) comWriteCmd(COM_ACK);
	return rslt;
}

int8_t comWriteF(comPacket_t* packet, uint8_t ack){
	txPacket = *packet;
	if(ack){
		comPacket_t pk;
		return comWriteMaster(packet, &pk, COM_ACK);
	}
	else return comWritePacket(&txPacket);
}

int8_t comWriteCmd(uint8_t cmd){
	comPacket_t pk = comNewCmd(cmd);
	return comWritePacket(&pk);
}

uint8_t comCrc(uint8_t* buffer, uint8_t len){
    uint8_t crc = 0x00;
    uint8_t polynomial = 0x07;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= buffer[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & (1U << 7)) crc = (crc << 1) ^ polynomial;
            else crc <<= 1;
        }
    }
    return crc;
}

/*
 * @brief Response waiting write command. Master usecase
 * Write and wait desired response :
 * -> NAK : retransmit 3x
 * -> ACK : exit
 * -> NoResponse : REQ last packet :
 * --> Same with transmitted : exit
 * --> Not same : retransmit, go start
 * --> NoResponse : exit
 */
int8_t comWriteMaster(comPacket_t* packet, comPacket_t* pRxPacket, uint8_t response){
	int8_t rslt = comWritePacket(packet);
	if(rslt != OK) return rslt;

	if(response){
		int8_t retry = 0;
		while(retry++ < 3){
			rslt = comReadPacket(pRxPacket);
			if(rslt == E_CONF_FAIL){
				comWriteCmd(COM_NAK);
				continue; /* NAK : retransit */
			}
		}
	}

	return rslt;
}

/*
 * @brief Successfully readed packet handler.
 * It is nescessary to use  this api after  reading.
 * ReadF function does not use this apriori because
 * main CMD funcitons does not count as last readed
 * packet.
 */
void comPacketHandler(comPacket_t* packet){
	switch (packet->type){
		case COM_ACK: break; /* ACK */
		case COM_NAK: comWritePacket(&txPacket); break; /* Retransmit last transmitted packet */
		case COM_GET: break; /* Get Function Handler Needed */
		case COM_SET: break; /* Set Function Handler Needed */
		case COM_REQ: comWritePacket(&rxPacket); break; /* Transmit last received packet*/
		case COM_MSG: break;
		default : rxPacket = *packet; break; /* Store the last packet */
	}
}

