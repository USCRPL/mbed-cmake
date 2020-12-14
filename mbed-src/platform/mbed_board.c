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
#include <stdio.h>
#include "hal/gpio_api.h"
#include "platform/mbed_wait_api.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_interface.h"
#include "platform/mbed_critical.h"
#include "hal/serial_api.h"

#if DEVICE_SERIAL
extern int stdio_uart_inited;
extern serial_t stdio_uart;
#endif

WEAK void mbed_die(void)
{
#if !defined (NRF51_H) && !defined(TARGET_EFM32)
    core_util_critical_section_enter();
#endif
    gpio_t led_err;
    gpio_init_out(&led_err, LED1);

    while (1) {
        for (int i = 0; i < 4; ++i) {
            gpio_write(&led_err, 1);
            wait_ms(150);
            gpio_write(&led_err, 0);
            wait_ms(150);
        }

        for (int i = 0; i < 4; ++i) {
            gpio_write(&led_err, 1);
            wait_ms(400);
            gpio_write(&led_err, 0);
            wait_ms(400);
        }
    }
}

void mbed_error_printf(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    mbed_error_vfprintf(format, arg);
    va_end(arg);
}

void mbed_error_vfprintf(const char *format, va_list arg)
{
#if DEVICE_SERIAL
#define ERROR_BUF_SIZE      (128)
    core_util_critical_section_enter();
    char buffer[ERROR_BUF_SIZE];
    int size = vsnprintf(buffer, ERROR_BUF_SIZE, format, arg);
    if (size > 0) {
        if (!stdio_uart_inited) {
            serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
        }
#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
        char stdio_out_prev = '\0';
        for (int i = 0; i < size; i++) {
            if (buffer[i] == '\n' && stdio_out_prev != '\r') {
                serial_putc(&stdio_uart, '\r');
            }
            serial_putc(&stdio_uart, buffer[i]);
            stdio_out_prev = buffer[i];
        }
#else
        for (int i = 0; i < size; i++) {
            serial_putc(&stdio_uart, buffer[i]);
        }
#endif
    }
    core_util_critical_section_exit();
#endif
}
