/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include "i2c_api.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "gpio_include.h"

static const PinMap PinMap_I2C_SDA[] = {
    {PC1, I2C_0, PIN_DATA(1, 2)},
    {PA5, I2C_1, PIN_DATA(1, 2)},
    {PL1, I2C_2, PIN_DATA(3, 2)},
    {NC,  NC,    0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PC0, I2C_0, PIN_DATA(1, 2)},
    {PA4, I2C_1, PIN_DATA(1, 2)},
    {PL0, I2C_2, PIN_DATA(3, 2)},
    {NC,  NC,    0}
};

// Clock setting structure definition
typedef struct {
    uint32_t sck;
    uint32_t prsck;
} I2C_clock_setting_t;

// SCK Divider value table
static const uint32_t I2C_SCK_DIVIDER_TBL[8] = {
    20, 24, 32, 48, 80, 144, 272, 528
};

I2C_clock_setting_t clk;
static uint32_t start_flag = 0;

static int32_t wait_status(i2c_t *p_obj);
static void i2c_start_bit(i2c_t *obj);

// Initialize the I2C peripheral. It sets the default parameters for I2C
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    MBED_ASSERT(obj != NULL);

    I2CName i2c_sda  = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl  = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    I2CName i2c_name = (I2CName)pinmap_merge(i2c_sda, i2c_scl);

    MBED_ASSERT((int)i2c_name != NC);

    switch (i2c_name) {
        case I2C_0:
            TSB_CG_FSYSENA_IPENA20 = ENABLE;
            TSB_CG_FSYSENA_IPENA02 = ENABLE;
            obj->i2c = TSB_I2C0;
            break;
        case I2C_1:
            TSB_CG_FSYSENA_IPENA21 = ENABLE;
            TSB_CG_FSYSENA_IPENA00 = ENABLE;
            obj->i2c = TSB_I2C1;
            break;
        case I2C_2:
            TSB_CG_FSYSENA_IPENA22 = ENABLE;
            TSB_CG_FSYSENA_IPENA10 = ENABLE;
            obj->i2c = TSB_I2C2;
            break;
        default:
            error("I2C is not available");
            break;
    }

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pin_mode(sda, OpenDrain);
    pin_mode(sda, PullUp);

    pinmap_pinout(scl, PinMap_I2C_SCL);
    pin_mode(scl, OpenDrain);
    pin_mode(scl, PullUp);

    i2c_reset(obj);
    i2c_frequency(obj, 100000);
    obj->i2c->CR2 = (I2CxCR2_I2CM_ENABLE | I2CxCR2_TRX | I2CxCR2_PIN_CLEAR |
                     I2CxCR2_INIT);
    obj->i2c->OP  = I2CxOP_INIT;
    obj->i2c->IE  = I2CxIE_CLEAR;
}

// Configure the I2C frequency
void i2c_frequency(i2c_t *obj, int hz)
{
    uint64_t sck;
    uint64_t tmp_sck;
    uint64_t prsck;
    uint64_t tmp_prsck;
    uint64_t fscl;
    uint64_t tmp_fscl;
    uint64_t fx;

    SystemCoreClockUpdate();

    if (hz <= 1000000) {
        sck = tmp_sck = 0;
        prsck = tmp_prsck = 1;
        fscl = tmp_fscl = 0;
        for (prsck = 1; prsck <= 32; prsck++) {
            fx = ((uint64_t)SystemCoreClock / prsck);
            if ((fx < 20000000U) && (fx > 6666666U)) {
                for (sck = 0; sck <= 7; sck++) {
                    fscl = (fx / (uint64_t)I2C_SCK_DIVIDER_TBL[sck]);
                    if ((fscl <= (uint64_t)hz) && (fscl > tmp_fscl)) {
                        tmp_fscl = fscl;
                        tmp_sck = sck;
                        tmp_prsck = (prsck < 32) ? prsck : 0;
                    }
                }
            }
        }
        clk.sck = (uint32_t)tmp_sck;
        clk.prsck = (tmp_prsck < 32) ? (uint32_t)(tmp_prsck - 1) : 0;
    }

    obj->i2c->CR1 = (I2CxCR1_ACK | clk.sck);
    obj->i2c->PRS = (I2CxPRS_PRCK & clk.prsck);
}

int i2c_start(i2c_t *obj)
{
    start_flag = 1;  // Start Condition
    return 0;
}

int i2c_stop(i2c_t *obj)
{
    uint32_t timeout = I2C_TIMEOUT;

    obj->i2c->CR2 = I2CxCR2_STOP_CONDITION;

    while ((obj->i2c->SR & I2CxSR_BB) == I2CxSR_BB) {
        if (timeout == 0) {
            break;
        }
        timeout--;
    }

    return 0;
}

void i2c_reset(i2c_t *obj)
{
    obj->i2c->CR2 = I2CxCR2_SWRES_10;
    obj->i2c->CR2 = I2CxCR2_SWRES_01;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int32_t result = 0;
    int32_t count  = 0;
    int32_t pdata  = 0;

    if (length > 0) {
        start_flag = 1;  // Start Condition
        if (i2c_byte_write(obj, (int32_t)((uint32_t)address | 1U)) == I2C_ACK) {
            while (count < length) {
                pdata = i2c_byte_read(obj, ((count < (length - 1)) ? 0 : 1));
                if (pdata < 0) {
                    break;
                }
                data[count++] = (uint8_t)pdata;
            }
            result = count;
        } else {
            stop = 1;
            result = I2C_ERROR_NO_SLAVE;
        }

        if (stop) {  // Stop Condition
            i2c_stop(obj);
        }
    }

    return result;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    int32_t result = 0;
    int32_t count  = 0;

    start_flag = 1; // Start Condition

    if (i2c_byte_write(obj, address) == I2C_ACK) {
        while (count < length) {
            if (i2c_byte_write(obj, (int32_t)data[count++]) < I2C_ACK) {
                break;
            }
        }
        result = count;
    } else {
        stop = 1;
        result = I2C_ERROR_NO_SLAVE;
    }

    if (stop) {  // Stop Condition
        i2c_stop(obj);
    }

    return result;
}

int i2c_byte_read(i2c_t *obj, int last)
{
    int32_t result = 0;

    obj->i2c->ST = I2CxST_CLEAR;

    if (last) {
        obj->i2c->OP |= I2CxOP_MFACK;
    } else {
        obj->i2c->OP &= ~I2CxOP_MFACK;
    }

    obj->i2c->DBR = (0 & I2CxDBR_DB_MASK);

    if (wait_status(obj) < 0) {
        result = -1;
    } else {
        result = (int32_t)(obj->i2c->DBR & I2CxDBR_DB_MASK);
    }

    return result;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    int32_t result = 0;

    obj->i2c->ST = I2CxST_CLEAR;
    if (start_flag == 1) {
        obj->i2c->DBR = (data & I2CxDBR_DB_MASK);
        i2c_start_bit(obj);
        start_flag = 0;
    } else {
        obj->i2c->DBR = (data & I2CxDBR_DB_MASK);
    }

    if (wait_status(obj) < 0) {
        return -1;
    }

    if (!((obj->i2c->SR & I2CxSR_LRB) == I2CxSR_LRB)) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

static void i2c_start_bit(i2c_t *obj)  // Send START command
{
    uint32_t opreg = 0;

    opreg = obj->i2c->OP;
    opreg &= ~(I2CxOP_RSTA | I2CxOP_SREN);

    if ((obj->i2c->SR & I2CxSR_BB)) {
        opreg |= I2CxOP_SREN;
    }

    obj->i2c->OP = opreg;
    obj->i2c->CR2 |= I2CxCR2_START_CONDITION;
}

static int32_t wait_status(i2c_t *p_obj)
{
    volatile int32_t timeout;
    timeout = I2C_TIMEOUT;

    while (!((p_obj->i2c->ST & I2CxST_I2C) == I2CxST_I2C)) {
        if ((timeout--) == 0) {
            return (-1);
        }
    }

    return 0;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    if (enable_slave) {
        obj->i2c->OP  = I2CxOP_SLAVE_INIT;
        obj->i2c->CR1 = (I2CxCR1_ACK | clk.sck);
        obj->i2c->CR2 = (I2CxCR2_INIT | I2CxCR2_PIN_CLEAR);
        obj->i2c->PRS = (I2CxPRS_PRCK & clk.prsck);
        obj->i2c->AR  = (obj->address & I2CAR_SA_MASK);
        obj->i2c->IE  = I2CxIE_INTI2C;
    } else {
        i2c_reset(obj);
        obj->i2c->CR2 = (I2CxCR2_I2CM_ENABLE | I2CxCR2_TRX | I2CxCR2_PIN_CLEAR |
                         I2CxCR2_INIT);
        obj->i2c->OP  = I2CxOP_INIT;
        obj->i2c->CR1 = (I2CxCR1_ACK | clk.sck);
        obj->i2c->PRS = (I2CxPRS_PRCK & clk.prsck);
        NVIC_DisableIRQ(obj->IRQn);
        NVIC_ClearPendingIRQ(obj->IRQn);
        obj->i2c->ST  = I2CxST_CLEAR;
    }
}

int i2c_slave_receive(i2c_t *obj)
{
    int32_t result = I2C_NO_DATA;

    if ((obj->i2c->ST & I2CxST_I2C) && (obj->i2c->OP & I2CxOP_SAST)) {
        if ((obj->i2c->SR & I2CxSR_TRX) == I2CxSR_TRX) {
            result = I2C_READ_ADDRESSED;
        } else {
            result = I2C_WRITE_ADDRESSED;
        }
    }

    return (result);
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    int32_t count = 0;

    while (count < length) {
        int32_t pdata = i2c_byte_read(obj, ((count < (length - 1)) ? 0 : 1));
        if ((obj->i2c->SR & I2CxSR_TRX)) {
            return (count);
        } else {
            if (pdata < 0) {
                break;
            }
            data[count++] = (uint8_t)pdata;
        }
    }

    i2c_slave_mode(obj,1);

    return (count);
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    int32_t count = 0;

    while (count < length) {
        if (i2c_byte_write(obj, (int32_t)data[count++]) < I2C_ACK) {
            break;
        }
    }

    i2c_slave_mode(obj,1);

    return (count);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    obj->address = address & I2CAR_SA_MASK;
    i2c_slave_mode(obj,1);
}
