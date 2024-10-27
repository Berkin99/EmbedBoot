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

#include "main.h"
#include "sysconfig.h"
#include "systime.h"
#include "sysdefs.h"
#include "gpio.h"
#include "uart.h"
#include "bootloader.h"
#include "com.h"

static BL_State_e  state;
static uint32_t    fwIndex;
static comPacket_t rxPacket;

void bootLaunch(void){
	uartInit();

	uint8_t i = 0;
	while(i++ < 8){
		pinToggle(LED1_PIN);
		delay(50);
	}

	/* Check GPIO Pin for HIGH */
	state = BL_STATE_SYNC;
	fwIndex = BL_APPLICATION_BASE;

	while(1){
		bootTask();
	}
}

void bootTask(void){

	switch (state) {
		case BL_STATE_FAULT:     bootFault(); break;
		case BL_STATE_SYNC:      bootSync(); break;
		case BL_STATE_FW_INIT:   bootFirmwareInit(); break;
		case BL_STATE_FW_UPDATE: bootFirmwareUpdate(); break;
		case BL_STATE_FW_COMPLETE: default: bootJumpApp(); break;
	}
}

void bootSync(void){
	while(comReadCmd(COM_SYC) != OK);
	comWriteCmd(COM_ACK);
	state = BL_STATE_FW_INIT;
}

void bootFirmwareInit(void){
	if(comFReadPacket(&rxPacket, 0) != OK){
		state = BL_STATE_FAULT; return;
	}

	if(rxPacket.type == COM_IDD && *(uint16_t*)rxPacket.payload == DEVICE_ID){
		/* Erase all Flash Sectors */
		if(bootFirmwareErase() == OK){
			fwIndex = 0;
			state = BL_STATE_FW_UPDATE;
			comWriteCmd(COM_ACK);
		}
		else{
			state = BL_STATE_FAULT;
			comWriteCmd(COM_ERR);
		}
	}
	else comWriteCmd(COM_ERR);
}

void bootFirmwareUpdate(void){
	if(comFReadPacket(&rxPacket, 0) != OK){
		state = BL_STATE_FAULT; return;
	}

	switch (rxPacket.type) {
		case COM_FWE:
			state = BL_STATE_FW_COMPLETE; break;
		case COM_FWP:
			bootLoad(rxPacket.payload, rxPacket.len); break;
		default:
			comWriteCmd(COM_NAK); return; break;
	}

	comWriteCmd(COM_ACK);
}

int8_t bootFirmwareErase(void){
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;

    if(HAL_FLASH_Unlock() != HAL_OK) return E_ERROR;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Banks = FLASH_BANK_1;               /* Bank 1 */
    EraseInitStruct.Sector = FLASH_SECTOR_1;            /* Start sector to be erased. */
    EraseInitStruct.NbSectors = FLASH_SECTOR_TOTAL - 1; /* Number of sectors to be erased. */
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        HAL_FLASH_Lock();
        return E_ERROR;
    }

    EraseInitStruct.Banks = FLASH_BANK_2;
    EraseInitStruct.Sector = FLASH_SECTOR_0;  /* First Sector */
    EraseInitStruct.NbSectors = 4; 			  /* Total number of sectors in Bank 2. */

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        HAL_FLASH_Lock();
        return E_ERROR;
    }

    HAL_FLASH_Lock();
    return OK;
}

int8_t bootLoad(uint8_t* payload, uint8_t len){

	if(HAL_FLASH_Unlock() != HAL_OK) return E_ERROR;

	__IO uint32_t* firmware = (__IO uint32_t*)fwIndex;


	HAL_FLASH_Lock();
	fwIndex += len;
    return OK;
}


int8_t bootGetStamp(void){
	uint32_t stamp = *(__IO uint32_t*)BL_STAMP_BASE;
	if(stamp == BL_STAMP) return OK;
	return E_NOT_FOUND;
}

/* Preerase Needed */
void bootSetStamp(void){
	HAL_FLASH_Unlock();
	__IO uint32_t* pStamp = (__IO uint32_t*)BL_STAMP_BASE;
	*pStamp = BL_STAMP;
	HAL_FLASH_Lock();
}

void bootJumpApp(void){
	if(bootGetStamp() != OK) return;
	typedef void(*void_t)(void);
	uint32_t jumpAddr = *(uint32_t*)(BL_APPLICATION_BASE + 4); /* Reset Handler */
	void_t mainapp = (void_t)jumpAddr;
	__set_MSP(*(__IO uint32_t*)BL_APPLICATION_BASE);	/* Stack Pointer */
	mainapp();
}
