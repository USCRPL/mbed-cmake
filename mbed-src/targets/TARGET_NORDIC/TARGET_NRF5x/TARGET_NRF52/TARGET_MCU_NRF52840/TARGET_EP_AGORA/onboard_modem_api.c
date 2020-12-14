/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

#include "cellular/onboard_modem_api.h"
#include "gpio_api.h"
#include "platform/mbed_thread.h"
#include "PinNames.h"

#if MODEM_ON_BOARD

static void press_power_button(int time_ms)
{
    gpio_t gpio;

    gpio_init_out_ex(&gpio, PIN_NAME_CELL_ON_OFF, 1);
    gpio_write(&gpio, 0);
    thread_sleep_for(time_ms);
    gpio_write(&gpio, 1);
}

void onboard_modem_init()
{
    gpio_t gpio;

    gpio_init_out_ex(&gpio, PIN_NAME_CELL_POWER_ENABLE, 0);
    gpio_write(&gpio, 1);
}

void onboard_modem_deinit()
{
    gpio_t gpio;

    gpio_init_out_ex(&gpio, PIN_NAME_CELL_POWER_ENABLE, 1);
    gpio_write(&gpio, 0);
}

void onboard_modem_power_up()
{
    /* keep the power line low for 5 seconds */
    press_power_button(5000);
    /* give modem a little time to respond */
    thread_sleep_for(20 * 1000);
}

void onboard_modem_power_down()
{
    gpio_t gpio;

    gpio_init_out_ex(&gpio, PIN_NAME_CELL_ON_OFF, 0);
    /* keep the power line low for more than 3 seconds.
     * If 3G_ON_OFF pin is kept low for more than a second, a controlled disconnect and shutdown takes
     * place, Due to the network disconnect, shut-off can take up to 30 seconds. However, we wait for 10
     * seconds only   */
    thread_sleep_for(10 * 1000);
}
#endif //MODEM_ON_BOARD
#endif //MBED_CONF_NSAPI_PRESENT
