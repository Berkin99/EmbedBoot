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

#define BL_BOOT_BASE          (FLASH_BASE)
#define BL_BOOT_SIZE          (uint32_t)(0x6000U)
#define BL_STAMP_BASE         (BL_BOOT_BASE + BL_BOOT_SIZE - 4)
#define BL_STAMP              (uint32_t)(0xDEADBEEF)
#define BL_APPLICATION_BASE   (uint32_t)(BL_BOOT_BASE + BL_BOOT_SIZE)
#define BL_APPLICATION_SIZE   (uint32_t)(0x1EE000U)

typedef enum{
	BL_STATE_FAULT,
	BL_STATE_SYNC,
	BL_STATE_FW_DEVICE,
	BL_STATE_FW_UPDATE,
	BL_STATE_FW_COMPLETE,
}BL_State_e;

void bootLaunch(void);
void bootTask(void);

void bootSync(void);
void bootDeviceID(void);
void bootFirmwareUpdate(void);
void bootErase(void);
void bootLoad(uint8_t* payload, uint8_t len);

int8_t bootGetStamp(void);
void bootSetStamp(void);
void bootJumpApp(void);

#endif /* BOOTLOADER_H_ */
