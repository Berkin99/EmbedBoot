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

#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_

/// SYSTIME /////////////////////////////////////////////////
#define SYSTIME                htim2

/// SYSDEBUG ////////////////////////////////////////////////
#define SERIAL_UART            uart2

/// BOOTLOADER //////////////////////////////////////////////
#define COM_UART               uart2
#define DEVICE_ID              (0x0F35U)

/// LED /////////////////////////////////////////////////////
#define LED1_PIN               PE11
#define LED2_PIN               PE10
#define LED3_PIN               PD3
#define LED4_PIN               PD4

/// SPI /////////////////////////////////////////////////////
//#define RF24_SPI               spi1
//#define RF24_CE                PE5
//#define RF24_CS                PE6
//#define RF24_RX_ADDRESS        {0xE7, 0xE7, 0xE7, 0xE3, 0x04}
//#define RF24_TX_ADDRESS        {0xE7, 0xE7, 0xE7, 0xE3, 0x05}

/////////////////////////////////////////////////////////////
#endif /* SYSCONFIG_H_ */
