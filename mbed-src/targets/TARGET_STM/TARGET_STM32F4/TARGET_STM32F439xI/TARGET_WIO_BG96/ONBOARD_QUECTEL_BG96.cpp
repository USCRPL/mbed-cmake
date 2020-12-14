/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "ONBOARD_QUECTEL_BG96.h"

#include "cellular/onboard_modem_api.h"
#include "UARTSerial.h"

using namespace mbed;

ONBOARD_QUECTEL_BG96::ONBOARD_QUECTEL_BG96(FileHandle *fh) : QUECTEL_BG96(fh)
{
    ::onboard_modem_init();
}

nsapi_error_t ONBOARD_QUECTEL_BG96::hard_power_on()
{
    ::onboard_modem_init();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_BG96::hard_power_off()
{
    ::onboard_modem_deinit();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_BG96::soft_power_on()
{
    ::onboard_modem_power_up();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_BG96::soft_power_off()
{
    ::onboard_modem_power_down();
    return NSAPI_ERROR_OK;
}

CellularDevice *CellularDevice::get_target_default_instance()
{
    static UARTSerial serial(MDMTXD, MDMRXD, 115200);
    static ONBOARD_QUECTEL_BG96 device(&serial);
    return &device;
}

#endif // MBED_CONF_NSAPI_PRESENT
