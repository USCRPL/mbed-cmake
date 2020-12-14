/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2019, STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#include "cmsis.h"
#include "gpio_irq_device.h"

// Used to return the index for channels array.
const exti_lines_t pin_lines_desc[16] = {
    // EXTI0
    {.gpio_idx = 0, .irq_index = 0, .irq_n = EXTI0_IRQn}, // pin 0
    // EXTI1
    {.gpio_idx = 0, .irq_index = 1, .irq_n = EXTI1_IRQn}, // pin 1
    // EXTI2
    {.gpio_idx = 0, .irq_index = 2, .irq_n = EXTI2_IRQn}, // pin 2
    // EXTI3
    {.gpio_idx = 0, .irq_index = 3, .irq_n = EXTI3_IRQn}, // pin 3
    // EXTI4
    {.gpio_idx = 0, .irq_index = 4, .irq_n = EXTI4_IRQn}, // pin 4
    // EXTI5_9
    {.gpio_idx = 0, .irq_index = 5, .irq_n = EXTI9_5_IRQn},// pin 5
    {.gpio_idx = 1, .irq_index = 5, .irq_n = EXTI9_5_IRQn},// pin 6
    {.gpio_idx = 2, .irq_index = 5, .irq_n = EXTI9_5_IRQn},// pin 7
    {.gpio_idx = 3, .irq_index = 5, .irq_n = EXTI9_5_IRQn},// pin 8
    {.gpio_idx = 4, .irq_index = 5, .irq_n = EXTI9_5_IRQn},// pin 9
    // EXTI10_15
    {.gpio_idx = 0, .irq_index = 6, .irq_n = EXTI15_10_IRQn},// pin 10
    {.gpio_idx = 1, .irq_index = 6, .irq_n = EXTI15_10_IRQn},// pin 11
    {.gpio_idx = 2, .irq_index = 6, .irq_n = EXTI15_10_IRQn},// pin 12
    {.gpio_idx = 3, .irq_index = 6, .irq_n = EXTI15_10_IRQn},// pin 13
    {.gpio_idx = 4, .irq_index = 6, .irq_n = EXTI15_10_IRQn},// pin 14
    {.gpio_idx = 5, .irq_index = 6, .irq_n = EXTI15_10_IRQn}// pin 15
};

