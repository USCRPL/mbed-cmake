/***************************************************************************//**
 * @file crc_api.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2018 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#include "clocking.h"
#include <stdio.h>

#if DEVICE_CRC
#include "mbed_assert.h"
#include "crc_api.h"
#include "em_cmu.h"

#if defined(GPCRC_PRESENT) && (GPCRC_COUNT > 0)
#include "em_gpcrc.h"

static bool revOutput = false;
static uint32_t final_xor;

bool hal_crc_is_supported(const crc_mbed_config_t *config)
{
    //GPCRC supports any 16-bit poly, but only the CCITT 32-bit poly
    if (config == NULL) {
        return false;
    }

    if (config->width == 16) {
        return true;
    } else if (config->width == 32) {
        if (config->polynomial == POLY_32BIT_ANSI) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void hal_crc_compute_partial_start(const crc_mbed_config_t *config)
{
    if (!hal_crc_is_supported(config)) {
        return;
    }

    CMU_ClockEnable(cmuClock_GPCRC, true);
    GPCRC_Reset(GPCRC);

    GPCRC_Init_TypeDef crc_init;

    crc_init.autoInit = false;
    crc_init.enable = true;
    crc_init.crcPoly = config->polynomial;

    // GPCRC operates on bit-reversed inputs and outputs vs the standard
    // defined by the mbed API. Emlib does the reversal on the poly, but
    // not on the initial value.
    if (config->width == 16) {
        crc_init.initValue = __RBIT(config->initial_xor) >> 16;
    } else {
        crc_init.initValue = __RBIT(config->initial_xor);
    }

    // GPCRC operates on bit-reversed inputs and outputs vs the standard
    // defined by the mbed API, so reflect_in/out needs to be negated.
    if (config->reflect_in) {
        crc_init.reverseByteOrder = false;
        crc_init.reverseBits = false;
    } else {
        crc_init.reverseByteOrder = true;
        crc_init.reverseBits = true;
    }

    // Disable byte mode to be able to run a faster U32 input version
    crc_init.enableByteMode = false;

    // GPCRC does not support hardware output bit-reversal, nor finalisation.
    // Since the mbed API does not pass the config struct when fetching the
    // CRC calculation result, we need to keep track of these locally.
    revOutput = config->reflect_out;
    final_xor = config->final_xor;

    GPCRC_Init(GPCRC, &crc_init);
    GPCRC_Start(GPCRC);
}

void hal_crc_compute_partial(const uint8_t *data, const size_t size)
{
    if (data == NULL || size <= 0) {
        return;
    }

    if (((uint32_t)data & 0x3) != 0 || size < 4) {
        // Unaligned or very small input, run a bytewise CRC
        for (size_t i = 0; i < size; i++) {
            GPCRC_InputU8(GPCRC, data[i]);
        }
    } else {
        // Aligned input, run 32-bit inputs as long as possible to make go faster.
        size_t i = 0;
        for (; i < (size & (~0x3)); i+=4) {
            GPCRC_InputU32(GPCRC, *((uint32_t*)(&data[i])));
        }
        for (; i < size; i++) {
            GPCRC_InputU8(GPCRC, data[i]);
        }
    }
}

uint32_t hal_crc_get_result(void)
{
    uint32_t result;

    // GPCRC operates on bit-reversed inputs and outputs vs the standard
    // defined by the mbed API.
    if (!revOutput) {
        result = GPCRC_DataReadBitReversed(GPCRC);
    } else {
        result = GPCRC_DataRead(GPCRC);
    }

    GPCRC_Enable(GPCRC, false);

    return result ^ final_xor;
}

#endif //GPCRC_PRESENT
#endif //DEVICE_CRC
