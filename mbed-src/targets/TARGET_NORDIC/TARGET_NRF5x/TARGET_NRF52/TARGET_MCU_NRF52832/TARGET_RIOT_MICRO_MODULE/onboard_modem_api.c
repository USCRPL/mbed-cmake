/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#if MBED_CONF_NSAPI_PRESENT

#include "mbed-trace/mbed_trace.h"
#ifndef TRACE_GROUP
#define TRACE_GROUP  "RIOT"
#endif // TRACE_GROUP


#include "onboard_modem_api.h"
#include "gpio_api.h"
#include "PinNames.h"
#include "hal/serial_api.h"
#include "platform/mbed_thread.h"

#if MODEM_ON_BOARD

void onboard_modem_init()
{
    char promptEnd;

    tr_debug("onboard_modem_init");

    gpio_t gpio;

    gpio_init_out_ex(&gpio, MDMCHEN, 0);
    gpio_init_out_ex(&gpio, MDMREMAP, 0);
    // Take us out of reset
    gpio_init_out_ex(&gpio, MDMRST, 0);
    thread_sleep_for(100);
    gpio_write(&gpio, 1);

    /* Initialize UART1 pins to allow collecting logs from a PC */
    gpio_init_in_ex(&gpio, MDM_UART1_TXD, PullNone);
    gpio_init_in_ex(&gpio, MDM_UART1_RXD, PullNone);

    serial_t bootrom_uart;
    serial_init(&bootrom_uart, MDM_UART0_TXD, MDM_UART0_RXD);
    serial_baud(&bootrom_uart, 115200);

    tr_debug("%s: MODEM RESET", __func__);

    serial_getc(&bootrom_uart);
    tr_debug("%s: MODEM first activity after reset", __func__);
    /* Wait for some dots */
    for (int i = 0; i < 3; i++) {
        do {
            promptEnd = serial_getc(&bootrom_uart);
        } while ('.' != promptEnd);
    }
    serial_putc(&bootrom_uart, ' ');
    /* Wait for bootrom prompt */
    for (int i = 0; i < 2; i++) {
        do {
            promptEnd = serial_getc(&bootrom_uart);
        } while ('>' != promptEnd);
    }
    serial_putc(&bootrom_uart, '6');
    serial_free(&bootrom_uart);

    /* Wait for stack prompt */
    tr_debug("%s: Wait for stack prompt", __func__);
    thread_sleep_for(100);
    serial_t cli_uart;
    serial_init(&cli_uart, MDM_UART3_TXD, MDM_UART3_RXD);
    serial_baud(&cli_uart, 230400); /* TODO make baud rate configurable */

    do {
        promptEnd = serial_getc(&cli_uart);
    } while ('>' != promptEnd);

    serial_free(&cli_uart);

    tr_debug("%s: MODEM CLI prompt reached", __func__);

    tr_debug("Reset RM1000 completed");
}

void onboard_modem_deinit()
{
    tr_debug("onboard_modem_deinit");

    gpio_t gpio;

    // Back into reset
    gpio_init_out_ex(&gpio, MDMRST, 0);
}

#endif //MODEM_ON_BOARD
#endif //MBED_CONF_NSAPI_PRESENT
