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
#include "com.h"
#include "com_defs.h"

comPacket_t comNewPacket(uint8_t type, uint8_t len, uint8_t* pBuf){
	comPacket_t pk = {
		.type = type,
		.len = len
	};
	memcpy(pk.buffer, pBuf, len);
	return pk;
}

comPacket_t comNewCommand(uint8_t cmd){
	comPacket_t pk = {
		.type = cmd,
		.len = 0
	};
	return pk;
}

void comSyncronize(void){
	/* Wait Syncronize Packet From Master */
	comPacket_t pk;
	while(1){
		int8_t rslt = comPacketReceive(&pk);
		if((rslt == OK) && (pk.type == COM_SYC)) break;
	}
}

int8_t comPacketTransmit(comPacket_t* packet){
	int8_t rslt;
	uint8_t start = COM_STA;
	uint8_t crc = comCrc(packet->buffer, packet->len);
	rslt  = comTransmit(&start, 1);
	rslt |= comTransmit(&packet->type, 1);
	rslt |= comTransmit(&packet->len, 1);
	rslt |= comTransmit(packet->buffer, packet->len);
	rslt |= comTransmit(&crc, 1);
	return rslt;
}

int8_t comPacketReceive(comPacket_t* packet){
	int8_t rslt;
	uint8_t start = 0;
	uint8_t crc;
	comReceive(&start, 1);
	if(start != COM_STA) return E_NOT_FOUND;
	rslt  = comReceive(&packet->type, 1);
	rslt |= comReceive(&packet->len, 1);
	rslt |= comReceive(packet->buffer, packet->len);
	rslt |= comReceive(&crc, 1);
	if(comCrc(packet->buffer, packet->len) != crc) return E_CONF_FAIL;
	return rslt;
}

uint8_t comCrc(uint8_t* buffer, uint8_t len){

	return 0;
}

