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
#include "drivers/Serial.h"

#if DEVICE_SERIAL

namespace mbed {

Serial::Serial(PinName tx, PinName rx, const char *name, int baud) : SerialBase(tx, rx, baud), Stream(name)
{
}

Serial::Serial(const serial_pinmap_t &static_pinmap, const char *name, int baud) : SerialBase(static_pinmap, baud), Stream(name)
{
}

Serial::Serial(PinName tx, PinName rx, int baud): SerialBase(tx, rx, baud), Stream(NULL)
{
}

Serial::Serial(const serial_pinmap_t &static_pinmap, int baud): SerialBase(static_pinmap, baud), Stream(NULL)
{
}

int Serial::_getc()
{
    // Mutex is already held
    return _base_getc();
}

int Serial::_putc(int c)
{
    // Mutex is already held
    return _base_putc(c);
}

void Serial::lock()
{
    _mutex.lock();
}

void Serial::unlock()
{
    _mutex.unlock();
}

} // namespace mbed

#endif
