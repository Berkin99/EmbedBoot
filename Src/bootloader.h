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

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <stdint.h>
#include "stm32h7xx.h"

#define BL_FLASH_BASE         (FLASH_BANK1_BASE)
#define BL_FLASH_SIZE         (FLASH_SECTOR_SIZE)
#define BL_STAMP_BASE         (0U)
#define BL_STAMP              (uint32_t)(0xCAFA1500)
#define BL_APP_BANK_1         (uint32_t)(FLASH_BANK1_BASE + BL_FLASH_SIZE)
#define BL_APP_BANK_2         (uint32_t)(FLASH_BANK2_BASE)
#define BL_APP_BANK_SIZE_1    (uint32_t)(0x)
#define BL_APP_BANK_SIZE_2    (uint32_t)(0x1EE000U)
#define BL_FLASH_PROGRAM_SIZE (32)

typedef enum{
	BL_STATE_FAULT,
	BL_STATE_SYNC,
	BL_STATE_FW_INIT,
	BL_STATE_FW_UPDATE,
	BL_STATE_FW_COMPLETE,
}BL_State_e;

typedef struct{
	uint32_t    address;
	uint8_t     packet[BL_PROGRAM_SIZE];
	uint8_t     pIndex;
}BL_Firmware_t;

void bootLaunch(void);
void bootTask(void);
void bootSync(void);
void bootFirmwareInit(void);
void bootFirmwareUpdate(void);
int8_t bootFirmwareErase(void);
int8_t bootLoad256(uint8_t* payload, uint8_t len, uint32_t fwAddress);
void bootFault(void);
int8_t bootGetStamp(void);
void bootSetStamp(void);
void bootJumpApp(void);

#endif /* BOOTLOADER_H_ */
