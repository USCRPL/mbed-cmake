/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _PINNAMES_H_
#define _PINNAMES_H_

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PORT_A = 0,
    PORT_B = 1,
    PORT_C = 2,
    PORT_D = 3,
    PORT_E = 4,
    PORT_F = 5,
    PORT_G = 6,
    PORT_H = 7,
    PORT_I = 8,
    PORT_J = 9,
    PORT_K = 10,

    PORT_V = 11,
    PORT_U = 12,
    PORT_MAX
} GPIO_PORT;

#define RTL_PIN_PERI(FUN, IDX, SEL)      ((int)(((FUN) << 8) | ((IDX)<<4) | (SEL)))
#define RTL_PIN_FUNC(FUN, SEL)      ((int)(((FUN) << 7) | (SEL)))
#define RTL_GET_PERI_SEL(peri)      ((int)((peri)&0x0F))
#define RTL_GET_PERI_IDX(peri)      ((int)(((peri) >> 4)&0x0F))

typedef enum {
    PIN_INPUT=0,
    PIN_OUTPUT
} PinDirection;


typedef enum {
    PA_0  = (PORT_A<<4|0),
    PA_1  = (PORT_A<<4|1),
    PA_2  = (PORT_A<<4|2),
    PA_3  = (PORT_A<<4|3),
    PA_4  = (PORT_A<<4|4),
    PA_5  = (PORT_A<<4|5),
    PA_6  = (PORT_A<<4|6),
    PA_7  = (PORT_A<<4|7),

    PB_0  = (PORT_B<<4|0),
    PB_1  = (PORT_B<<4|1),
    PB_2  = (PORT_B<<4|2),
    PB_3  = (PORT_B<<4|3),
    PB_4  = (PORT_B<<4|4),
    PB_5  = (PORT_B<<4|5),
    PB_6  = (PORT_B<<4|6),
    PB_7  = (PORT_B<<4|7),

    PC_0  = (PORT_C<<4|0),
    PC_1  = (PORT_C<<4|1),
    PC_2  = (PORT_C<<4|2),
    PC_3  = (PORT_C<<4|3),
    PC_4  = (PORT_C<<4|4),
    PC_5  = (PORT_C<<4|5),
    PC_6  = (PORT_C<<4|6),
    PC_7  = (PORT_C<<4|7),
    PC_8  = (PORT_C<<4|8),
    PC_9  = (PORT_C<<4|9),

    PD_0  = (PORT_D<<4|0),
    PD_1  = (PORT_D<<4|1),
    PD_2  = (PORT_D<<4|2),
    PD_3  = (PORT_D<<4|3),
    PD_4  = (PORT_D<<4|4),
    PD_5  = (PORT_D<<4|5),
    PD_6  = (PORT_D<<4|6),
    PD_7  = (PORT_D<<4|7),
    PD_8  = (PORT_D<<4|8),
    PD_9  = (PORT_D<<4|9),
  
    PE_0  = (PORT_E<<4|0),
    PE_1  = (PORT_E<<4|1),
    PE_2  = (PORT_E<<4|2),
    PE_3  = (PORT_E<<4|3),
    PE_4  = (PORT_E<<4|4),
    PE_5  = (PORT_E<<4|5),
    PE_6  = (PORT_E<<4|6),
    PE_7  = (PORT_E<<4|7),
    PE_8  = (PORT_E<<4|8),
    PE_9  = (PORT_E<<4|9),
    PE_A  = (PORT_E<<4|10),
  
    PF_0  = (PORT_F<<4|0),
    PF_1  = (PORT_F<<4|1),
    PF_2  = (PORT_F<<4|2),
    PF_3  = (PORT_F<<4|3),
    PF_4  = (PORT_F<<4|4),
    PF_5  = (PORT_F<<4|5),
    /* unavailable pins */
//    PF_6  = (PORT_F<<4|6),
//    PF_7  = (PORT_F<<4|7),

    PG_0  = (PORT_G<<4|0),
    PG_1  = (PORT_G<<4|1),
    PG_2  = (PORT_G<<4|2),
    PG_3  = (PORT_G<<4|3),
    PG_4  = (PORT_G<<4|4),
    PG_5  = (PORT_G<<4|5),
    PG_6  = (PORT_G<<4|6),
    PG_7  = (PORT_G<<4|7),

    PH_0  = (PORT_H<<4|0),
    PH_1  = (PORT_H<<4|1),
    PH_2  = (PORT_H<<4|2),
    PH_3  = (PORT_H<<4|3),
    PH_4  = (PORT_H<<4|4),
    PH_5  = (PORT_H<<4|5),
    PH_6  = (PORT_H<<4|6),
    PH_7  = (PORT_H<<4|7),

    PI_0  = (PORT_I<<4|0),
    PI_1  = (PORT_I<<4|1),
    PI_2  = (PORT_I<<4|2),
    PI_3  = (PORT_I<<4|3),
    PI_4  = (PORT_I<<4|4),
    PI_5  = (PORT_I<<4|5),
    PI_6  = (PORT_I<<4|6),
    PI_7  = (PORT_I<<4|7),

    PJ_0  = (PORT_J<<4|0),
    PJ_1  = (PORT_J<<4|1),
    PJ_2  = (PORT_J<<4|2),
    PJ_3  = (PORT_J<<4|3),
    PJ_4  = (PORT_J<<4|4),
    PJ_5  = (PORT_J<<4|5),
    PJ_6  = (PORT_J<<4|6),
    /* unavailable pins */
//    PJ_7  = (PORT_J<<4|7),

    PK_0  = (PORT_K<<4|0),
    PK_1  = (PORT_K<<4|1),
    PK_2  = (PORT_K<<4|2),
    PK_3  = (PORT_K<<4|3),
    PK_4  = (PORT_K<<4|4),
    PK_5  = (PORT_K<<4|5),
    PK_6  = (PORT_K<<4|6),
    /* unavailable pins */
    //    PK_7  = (PORT_K<<4|7),

    AD_1  = (PORT_V<<4|1),
    AD_2  = (PORT_V<<4|2),
    AD_3  = (PORT_V<<4|3),

    DA_0  = (PORT_U<<4|0),
    DA_1  = (PORT_U<<4|1),

    // Not connected
    NC = (int)0xFFFFFFFF,

    // Generic signals namings
    /* LED1~4 are defined as alias of GPIO pins, they are not the LEDs on board*/
    LED1        = PB_4,
    LED2        = PB_5,
    LED3        = PB_6,
    LED4        = PB_7,
    SERIAL_TX   = PA_7,
    SERIAL_RX   = PA_6,
    USBTX       = PB_0,
    USBRX       = PB_1,
    I2C_SCL     = PC_5,
    I2C_SDA     = PC_4,
    SPI_MOSI    = PC_2,
    SPI_MISO    = PC_3,
    SPI_SCK     = PC_1,
    SPI_CS      = PC_0,
    PWM_OUT     = PD_4,

    // Arduino connector namings

    A0          = AD_2,//A0 and A1 are connected
    A1          = AD_2,
    A2          = AD_3,
    A3          = NC,
    A4          = NC,
    A5          = NC,

    D0          = PA_6,
    D1          = PA_7,
    D2          = PA_5,
    D3          = PD_4,
    D4          = PD_5,
    D5          = PA_4,
    D6          = PA_3,
    D7          = PA_2,
    D8          = PB_4,
    D9          = PB_5,
    D10         = PC_0,
    D11         = PC_2,
    D12         = PC_3,
    D13         = PC_1,
    D14         = PB_3,
    D15         = PB_2,
    D16         = PA_1,
    D17         = PA_0,
    D18         = PE_5

} PinName;

typedef enum {
    PullNone  = 0,
    PullUp    = 1,
    PullDown  = 2,
    OpenDrain = 3,
    PullDefault = PullNone
} PinMode;

#define PORT_NUM(pin) (((uint32_t)(pin) >> 4) & 0xF)
#define PIN_NUM(pin)  ((uint32_t)(pin) & 0xF)

#ifdef __cplusplus
}
#endif

#endif
