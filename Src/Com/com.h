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

#define COM_STA    0x20    /* Packet Start Byte */
#define COM_SYC    "SYNC"  /* Sync */

#define COM_ACK    0x22    /* Acknowledge */
#define COM_NAK    0x23    /* Not Acknowledge */
#define COM_GET    0x24    /* Read Register + Register */
#define COM_SET    0x25    /* Write Register + Register + Data */
#define COM_REQ    0x26    /* Request Last Given Packet */
#define COM_MSG    0x27    /* String Message */

#define COM_FID    0xF0    /* Firmware ID */
#define COM_FWP    0xF1    /* Firmware Packet */
#define COM_FWE    0xF2    /* Firmware End Command */
#define COM_ERR    0xEE    /* Error */

#define COM_PACKET_PAYLOAD_SIZE (16)
#define COM_PACKET_SIZE         (COM_PACKET_PAYLOAD_SIZE + 4)

typedef struct{
    uint8_t start;
    uint8_t type;
    uint8_t len;
    uint8_t payload[COM_PACKET_PAYLOAD_SIZE];
    uint8_t crc;
}comPacket_t;

comPacket_t comNewPacket(uint8_t type, uint8_t* payload, uint8_t len);
comPacket_t comNewCmd(uint8_t cmd);
int8_t  comReadPacket(comPacket_t* packet);
int8_t  comWritePacket(comPacket_t* packet);
int8_t  comReadF(comPacket_t* packet, uint8_t ack);
int8_t  comWriteF(comPacket_t* packet, uint8_t ack);
int8_t  comWriteCmd(uint8_t cmd);
int8_t  comWriteMaster(comPacket_t* packet, comPacket_t* pRxPacket, uint8_t response);
uint8_t comCrc(uint8_t* buffer, uint8_t len);

void comPacketHandler(comPacket_t* packet);

#endif /* COM_H_ */
