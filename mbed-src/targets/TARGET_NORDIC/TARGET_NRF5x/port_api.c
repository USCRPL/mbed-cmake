/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "port_api.h"
#include "pinmap.h"

#if defined(TARGET_MCU_NRF51822) || defined(TARGET_MCU_NRF52832)
    #define GPIO_REG_LIST  {NRF_GPIO}
#endif

static NRF_GPIO_Type * const m_ports[] = GPIO_REG_LIST;

#if defined(TARGET_MCU_NRF51822)
    static const uint32_t m_gpio_pin_count[] = {31};
#elif defined(TARGET_MCU_NRF52832)
    static const uint32_t m_gpio_pin_count[] = {32};
#elif defined(TARGET_MCU_NRF52840)
    static const uint32_t m_gpio_pin_count[] = {32, 16};
#else
    #error not recognized gpio count for mcu
#endif

#define GPIO_PORT_COUNT (sizeof(m_gpio_pin_count)/sizeof(m_gpio_pin_count[0]))


PinName port_pin(PortName port, int pin_n)
{
#if defined(TARGET_MCU_NRF51822) || defined(TARGET_MCU_NRF52832)
    return (PinName)pin_n;
#else    
    return (PinName)NRF_GPIO_PIN_MAP(port, pin_n);
#endif
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    MBED_ASSERT((uint32_t)port < GPIO_PORT_COUNT);
    
    obj->port = port;
    obj->mask = mask;

    port_dir(obj, dir);
}

void port_mode(port_t *obj, PinMode mode)
{
    uint32_t i;
    // The mode is set per pin: reuse pinmap logic
    for (i = 0; i<31; i++) {
        if (obj->mask & (1 << i)) {
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    uint32_t i;
    
    volatile uint32_t *reg_cnf = (volatile uint32_t*) m_ports[obj->port]->PIN_CNF;

    switch (dir) {
        case PIN_INPUT:

            for (i = 0; i < m_gpio_pin_count[obj->port]; i++) {
                if (obj->mask & (1 << i)) {
                    reg_cnf[i] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
                                 | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                                 | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                                 | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
                }
            }
            break;

        case PIN_OUTPUT:

            for (i = 0; i < m_gpio_pin_count[obj->port]; i++) {
                if (obj->mask & (1 << i)) {
                    reg_cnf[i] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
                                 | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                                 | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                                 | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                                 | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
                }
            }
            break;
    }
}

void port_write(port_t *obj, int value)
{
    m_ports[obj->port]->OUTSET = value & obj->mask;
    m_ports[obj->port]->OUTCLR = (~value) & obj->mask;
}

int port_read(port_t *obj)
{
    return ((m_ports[obj->port]->IN) & obj->mask);
}
