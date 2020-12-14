/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 Nuvoton
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

#include "flash_api.h"

#if DEVICE_FLASH

#include <string.h>
#include "flash_data.h"
#include "mbed_critical.h"
#include "partition_M2351.h"

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

#ifndef MBED_ROM_START
#define MBED_ROM_START      0x0
#endif

#ifndef MBED_ROM_SIZE
#define MBED_ROM_SIZE       0x40000
#endif

#define NU_SECURE_FLASH_START       MBED_ROM_START
#define NU_SECURE_FLASH_SIZE        MBED_ROM_SIZE

// This is a flash algo binary blob. It is PIC (position independent code) that should be stored in RAM
// NOTE: On ARMv7-M/ARMv8-M, instruction fetches are always little-endian.
static uint32_t FLASH_ALGO[] = {
    0x460ab085, 0x90034603, 0x7001a802, 0x93009201, 0xf24ce7ff, 0xf2c400c0, 0x68000000, 0x42082101, 
    0xe7ffd001, 0xf24ce7f5, 0xf2c40000, 0x68010000, 0x43112240, 0xf24c6001, 0xf2c4000c, 0x21220000, 
    0x98036001, 0x0104f24c, 0x0100f2c4, 0xa8026008, 0x28007800, 0xe7ffd108, 0x43c02000, 0x0108f24c, 
    0x0100f2c4, 0xe0096008, 0x0008f24c, 0x0000f2c4, 0x2103f64a, 0x0155f2c0, 0xe7ff6001, 0x0010f24c, 
    0x0000f2c4, 0x60012101, 0x8f6ff3bf, 0xf24ce7ff, 0xf2c400c0, 0x68000000, 0x42082101, 0xe7ffd001, 
    0xf24ce7f5, 0xf2c40000, 0x68000000, 0x42082140, 0xe7ffd00b, 0x0000f24c, 0x0000f2c4, 0x22406801, 
    0x60014311, 0x90042001, 0x2000e002, 0xe7ff9004, 0xb0059804, 0xb5b04770, 0x4613b087, 0x4605460c, 
    0x91049005, 0xf2409203, 0xf2c41000, 0x21590000, 0x21166001, 0x21886001, 0x68006001, 0x42082101, 
    0x94019302, 0xd1039500, 0x2001e7ff, 0xe0389006, 0x2000f240, 0x0000f2c4, 0x22046801, 0x60014311, 
    0x2004f240, 0x0000f2c4, 0x43116801, 0xe7ff6001, 0x2050f240, 0x0000f2c4, 0x21106800, 0xd1014208, 
    0xe7f5e7ff, 0x0000f24c, 0x0000f2c4, 0x22016801, 0x60014311, 0x011cf24c, 0x0100f2c4, 0x6800600a, 
    0xd1034210, 0x2001e7ff, 0xe00a9006, 0x0000f24c, 0x0000f2c4, 0x22406801, 0x60014311, 0x90062000, 
    0x9806e7ff, 0xbdb0b007, 0x4601b082, 0x91009001, 0xf24ce7ff, 0xf2c400c0, 0x68000000, 0x42082101, 
    0xe7ffd001, 0xf24ce7f5, 0xf2c40000, 0x68010000, 0x43912201, 0xf24c6001, 0xf2c4001c, 0x21000000, 
    0x46086001, 0x4770b002, 0xe7ffb081, 0x00c0f24c, 0x0000f2c4, 0x21016800, 0xd0014208, 0xe7f5e7ff, 
    0x0000f24c, 0x0000f2c4, 0x22406801, 0x60014311, 0x000cf24c, 0x0000f2c4, 0x60012126, 0x0004f24c, 
    0x0000f2c4, 0x60012100, 0xf24c43c8, 0xf2c40108, 0x60080100, 0x0010f24c, 0x0000f2c4, 0x60012101, 
    0x8f6ff3bf, 0xf24ce7ff, 0xf2c400c0, 0x68000000, 0x42082101, 0xe7ffd001, 0xf24ce7f5, 0xf2c40000, 
    0x68000000, 0x42082140, 0xe7ffd00b, 0x0000f24c, 0x0000f2c4, 0x22406801, 0x60014311, 0x90002001, 
    0x2000e002, 0xe7ff9000, 0xb0019800, 0xb5804770, 0x4601b084, 0x98029002, 0x72fff64f, 0x72fff6ce, 
    0x90024010, 0xf64f9802, 0xf6cf0200, 0x401072ff, 0x98029002, 0x0512220f, 0x22014010, 0x42900552, 
    0xd10b9101, 0x9802e7ff, 0x0100f240, 0x71e0f6cf, 0x21011840, 0xfea4f7ff, 0xe0059003, 0x21009802, 
    0xfe9ef7ff, 0xe7ff9003, 0xb0049803, 0xb580bd80, 0x460ab086, 0x90044603, 0x20009103, 0x92019002, 
    0xe7ff9300, 0x99039802, 0xd20f4288, 0x9804e7ff, 0x1c4a9902, 0x00899202, 0xf7ff5840, 0x2800ffb8, 
    0xe7ffd003, 0x90052001, 0xe7ebe003, 0x90052000, 0x9805e7ff, 0xbd80b006, 0xb088b5b0, 0x460c4613, 
    0x90064605, 0x92049105, 0x1cc09805, 0x43882103, 0x98069005, 0x71fff64f, 0x71fff6ce, 0x90064008, 
    0x94029303, 0xe7ff9501, 0x00c0f24c, 0x0000f2c4, 0x21016800, 0xd0014208, 0xe7f5e7ff, 0x0000f24c, 
    0x0000f2c4, 0x22406801, 0x60014311, 0x000cf24c, 0x0000f2c4, 0x60012121, 0x9805e7ff, 0xd03f2800, 
    0x9806e7ff, 0x0104f24c, 0x0100f2c4, 0x98046008, 0xf83cf000, 0x0108f24c, 0x0100f2c4, 0xf24c6008, 
    0xf2c40010, 0x21010000, 0xf3bf6001, 0xe7ff8f6f, 0x00c0f24c, 0x0000f2c4, 0x21016800, 0xd0014208, 
    0xe7f5e7ff, 0x0000f24c, 0x0000f2c4, 0x21406800, 0xd00b4208, 0xf24ce7ff, 0xf2c40000, 0x68010000, 
    0x43112240, 0x20016001, 0xe00c9007, 0x1d009806, 0x98049006, 0x90041d00, 0x1f009805, 0xe7bc9005, 
    0x90072000, 0x9807e7ff, 0xbdb0b008, 0x4601b083, 0x98029002, 0x92021c42, 0x90017800, 0x1c429802, 
    0x78009202, 0x9a010200, 0x90011810, 0x1c429802, 0x78009202, 0x9a010400, 0x90011810, 0x1c429802, 
    0x78009202, 0x9a010600, 0x90011810, 0x91009801, 0x4770b003, 0xb088b5b0, 0x460c4613, 0x90064605, 
    0x92049105, 0x1cc09805, 0x43882103, 0x93039005, 0x95019402, 0xf24ce7ff, 0xf2c400c0, 0x68000000, 
    0x42082101, 0xe7ffd001, 0xf24ce7f5, 0xf2c40000, 0x68010000, 0x43112240, 0xf24c6001, 0xf2c4000c, 
    0x21000000, 0xe7ff6001, 0x28009805, 0xe7ffd053, 0xf64f9806, 0xf6ce71ff, 0x400871ff, 0x0104f24c, 
    0x0100f2c4, 0xf24c6008, 0xf2c40008, 0x21000000, 0xf24c6001, 0xf2c40010, 0x21010000, 0xf3bf6001, 
    0xe7ff8f6f, 0x00c0f24c, 0x0000f2c4, 0x21016800, 0xd0014208, 0xe7f5e7ff, 0x0000f24c, 0x0000f2c4, 
    0x21406800, 0xd00b4208, 0xf24ce7ff, 0xf2c40000, 0x68010000, 0x43112240, 0x98066001, 0xe01d9007, 
    0x0008f24c, 0x0000f2c4, 0x99046800, 0x46089000, 0xff6cf7ff, 0x42819900, 0xe7ffd003, 0x90079806, 
    0x9806e00c, 0x90061d00, 0x1d009804, 0x98059004, 0x90051f00, 0x9806e7a8, 0xe7ff9007, 0xb0089807, 
    0x0000bdb0, 0x00000000, 
};

static const flash_algo_t flash_algo_config = {
    .init = 0x000000d7,
    .uninit = 0x00000189,
    .erase_sector = 0x0000026f,
    .program_page = 0x00000319,
    .static_base = 0x00000564,
    .algo_blob = FLASH_ALGO
};

/* Secure flash */
static const sector_info_t sectors_info[] = {
    {NU_SECURE_FLASH_START, 0x800},                         // (start, sector size)
};

/* Secure flash */
static const flash_target_config_t flash_target_config = {
    .page_size  = 4,                                        // 4 bytes
                                                            // Here page_size is program unit, which is different
                                                            // than FMC definition.
    .flash_start = NU_SECURE_FLASH_START,
    .flash_size = NU_SECURE_FLASH_SIZE,
    .sectors = sectors_info,
    .sector_info_count = sizeof(sectors_info) / sizeof(sector_info_t)
};

/* Non-secure flash */
static const sector_info_t sectors_info_ns[] = {
    {(NS_OFFSET + NU_SECURE_FLASH_SIZE), 0x800},            // (start, sector size)
};

/* Non-secure flash */
static const flash_target_config_t flash_target_config_ns = {
    .page_size  = 4,                                        // 4 bytes
                                                            // Here page_size is program unit, which is different
                                                            // than FMC definition.
    .flash_start = NS_OFFSET + NU_SECURE_FLASH_SIZE,
    .flash_size = 0x80000 - NU_SECURE_FLASH_SIZE,
    .sectors = sectors_info_ns,
    .sector_info_count = sizeof(sectors_info_ns) / sizeof(sector_info_t)
};

void flash_set_target_config(flash_t *obj)
{
    obj->flash_algo = &flash_algo_config;
    obj->target_config = &flash_target_config;
    obj->target_config_ns = &flash_target_config_ns;
}

#endif  // #if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#endif  // #if DEVICE_FLASH
