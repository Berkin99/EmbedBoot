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

#ifndef COM_H_
#define COM_H_

#include <stdint.h>
#include "sysdefs.h"

#define COM_STA         0x20

#define COM_SYC			0x21
#define COM_ACK			0x22
#define COM_NAK			0x23
#define COM_RET			0x24
#define COM_RQE		    0x25

typedef struct{
	uint8_t type;
	uint8_t len;
	uint8_t buffer[16];
}comPacket_t;

comPacket_t comNewPacket(uint8_t type, uint8_t len, uint8_t* pBuf);
comPacket_t comNewCommand(uint8_t cmd);

void    comSyncronize(void);
int8_t  comPacketTransmit(comPacket_t* packet);
int8_t  comPacketReceive(comPacket_t* packet);
uint8_t comCrc(uint8_t* buffer, uint8_t len);

#endif /* COM_H_ */
