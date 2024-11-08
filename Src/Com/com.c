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

comPacket_t comNewPacket(uint8_t type, uint8_t len, uint8_t* pBuf){
	comPacket_t pk = {.start = COM_STA, .type = type,.len = len};
	memcpy(pk.payload, pBuf, len);
	return pk;
}

comPacket_t comNewCommand(uint8_t cmd){
	comPacket_t pk = {
		.type = cmd,
		.len = 0
	};
	return pk;
}

/* Configured for UART */
int8_t comReadPacket(comPacket_t* packet){
	int8_t rslt;
	packet->start = 0;

	uartRead(&COM_UART, &packet->start, 1);
	if(start != COM_STA) return E_NOT_FOUND;

	rslt  = uartRead(&COM_UART, &packet->type, 2);
	if(packet->len > 0) rslt |= uartRead(&COM_UART, packet->payload, packet->len);
	rslt |= uartRead(&COM_UART, &packet->crc, 1);

	if(packet->crc != comCrc(packet->payload, packet->len)) return E_CONF_FAIL;
	return rslt;
}

/* Configured for UART */
int8_t comWritePacket(comPacket_t* packet){
	int8_t rslt;

	rslt = uartWrite(&COM_UART, (uint8_t*)&packet, 3);
	if(packet->len > 0) rslt |= uartWrite(&COM_UART, packet->payload, packet->len);
	packet->crc = comCrc(packet->payload, packet->len);
	rslt |= uartWrite(&COM_UART, &packet->crc, 1);

	return rslt;
}

int8_t comFReadPacket(comPacket_t* packet, uint8_t ack){
	int8_t rslt;
	uint8_t retry = 0;
	while(retry++ < 3){ /* Send NAK for each wrong crc */
		rslt = comReadPacket(packet);
		if(rslt != E_CONF_FAIL) break;
		comWriteCmd(COM_NAK);
	}
	if(rslt == OK && ack) comWriteCmd(COM_ACK);
	return rslt;
}

int8_t comFWritePacket(comPacket_t* packet){
	int8_t rslt;
	uint8_t retry = 0;
	while(retry++ < 3){ /* Send packet until receive ACK */
		rslt = comWritePacket(packet);
		if(rslt != OK) break; /* Hardware problem */
		rslt = comReadCmd(COM_ACK);
		if(rslt == E_NOT_FOUND) continue; /* Retry if ACK not found */
		break; /* Received ACK or Hardware problem */
	}
	return rslt;
}

int8_t comReadCmd(uint8_t cmd){
	comPacket_t pk;
	int8_t rslt = comReadPacket(&pk);
	if(rslt != OK) return rslt;
	if(pk.type != cmd) return E_NOT_FOUND;
	return OK;
}

int8_t comWriteCmd(uint8_t cmd){
	comPacket_t pk = comNewCommand(cmd);
	return comWritePacket(&pk);
}

uint8_t comCrc(uint8_t* buffer, uint8_t len){
    uint8_t crc = 0x00;
    uint8_t polynomial = 0x07;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= buffer[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & (1U<<7)) crc = (crc << 1) ^ polynomial;
            else crc <<= 1;
        }
    }
    return crc;
}

