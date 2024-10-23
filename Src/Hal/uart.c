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

#include <stdint.h>
#include <stdio.h>
#include "system.h"
#include "sysconfig.h"
#include "uart.h"

#define UART_TIMEOUT (1000)

uart_t uart2;

void uartInit(void){
#ifdef HUART1
    uart1.handle = &HUART1;
#endif
#ifdef HUART2
    uart2.handle = &HUART2;
#endif
#ifdef HUART3
    uart3.handle = &HUART3;
#endif
#ifdef HUART4
    uart4.handle = &HUART4;
#endif
}

void uartSetBaudRate(uart_t* uart, uint32_t rate){
    HAL_UART_DeInit(uart->handle);
    ((UART_HandleTypeDef*)uart->handle)->Init.BaudRate = rate;
    HAL_UART_Init(uart->handle);
}

uint32_t uartGetBaudRate(uart_t* uart){
    return ((UART_HandleTypeDef*) uart->handle)->Init.BaudRate;
}

int8_t uartRead(uart_t* uart, uint8_t* pRxData, uint16_t len){
    int8_t status = HAL_UART_Receive(uart->handle, pRxData, len, UART_TIMEOUT);
    if(status != HAL_OK) return E_CONNECTION;
    return OK;
}

int8_t uartReadToIdle (uart_t* uart, uint8_t* pRxData, uint16_t len){
    int8_t status = HAL_UARTEx_ReceiveToIdle(uart->handle, pRxData, len, &uart->received, UART_TIMEOUT);
    if(status != HAL_OK) return E_CONNECTION;
    return OK;
}

int8_t uartWrite (uart_t* uart, const uint8_t* pTxData, uint16_t len){
    int8_t status = HAL_UART_Transmit(uart->handle, pTxData, len, UART_TIMEOUT);
    if(status != HAL_OK) return E_CONNECTION;
    return OK;
}

int8_t uartPrint(uart_t* uart, const char* format, ...) {
    va_list args;
    char buffer[128];
    int len;

    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

   return uartWrite(uart, (uint8_t*)buffer, len);
}

#ifdef SERIAL_UART
void serialPrint (const char* format, ...){
	va_list args;
	char buffer[128];
	uint16_t len;

	va_start(args, format);
	len = vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	uartWrite(&SERIAL_UART, (uint8_t*)buffer, len);
}
#endif
