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
#include "spi_api.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "gpio_include.h"
#include "txz_tspi.h"

static const PinMap PinMap_SPI_SCLK[] = {
    {PM0, SPI_0, PIN_DATA(3, 2)},
    {PP0, SPI_1, PIN_DATA(1, 2)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {PM1, SPI_0, PIN_DATA(3, 1)},
    {PP1, SPI_1, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {PM2, SPI_0, PIN_DATA(3, 0)},
    {PP2, SPI_1, PIN_DATA(1, 0)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {PM3, SPI_0, PIN_DATA(3, 1)},
    {PL6, SPI_1, PIN_DATA(1, 2)},
    {NC,  NC,    0}
};

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // Check pin parameters
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);

    obj->module = (SPIName)pinmap_merge(spi_data, spi_sclk);
    obj->module = (SPIName)pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->module!= NC);

    // Identify SPI module to use
    switch ((int)obj->module) {
        case SPI_0:
            obj->p_obj.p_instance = TSB_TSPI0;
            TSB_CG_FSYSENA_IPENA18 = ENABLE;
            TSB_CG_FSYSENA_IPENA11 = ENABLE;
            break;
        case SPI_1:
            obj->p_obj.p_instance = TSB_TSPI1;
            TSB_CG_FSYSENA_IPENA19 = ENABLE;
            TSB_CG_FSYSENA_IPENA13 = ENABLE;
            TSB_CG_FSYSENA_IPENA10 = ENABLE;
            break;
        default:
            error("Cannot found SPI module corresponding with input pins.");
            break;
    }

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);

    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }

    //Control 1 configurations
    obj->p_obj.init.id = (uint32_t)obj->module;
    obj->p_obj.init.cnt1.trgen  = TSPI_TRGEN_DISABLE;       // Trigger disabled
    obj->p_obj.init.cnt1.trxe   = TSPI_DISABLE;             // Enable Communication
    obj->p_obj.init.cnt1.tspims = TSPI_SPI_MODE;            // SPI mode
    obj->p_obj.init.cnt1.mstr   = TSPI_MASTER_OPEARTION;    // master mode operation
    obj->p_obj.init.cnt1.tmmd   = TSPI_TWO_WAY;             // Full-duplex mode (Transmit/receive)
    obj->p_obj.init.cnt1.cssel  = TSPI_TSPIxCS0_ENABLE;     // Chip select of pin CS0 is valid
    obj->p_obj.init.cnt1.fc     = TSPI_TRANS_RANGE_SINGLE;  // transfer single frame at a time continuously

    //Control 2 configurations
    obj->p_obj.init.cnt2.tidle   = TSPI_TIDLE_HI;
    obj->p_obj.init.cnt2.txdemp  = TSPI_TXDEMP_HI;          // when slave underruns TxD fixed to low
    obj->p_obj.init.cnt2.rxdly   = TSPI_RXDLY_40MHz_OVER;
    obj->p_obj.init.cnt2.til     = TSPI_TX_FILL_LEVEL_0;    // transmit FIFO Level
    obj->p_obj.init.cnt2.ril     = TSPI_RX_FILL_LEVEL_1;    // receive FIFO Level
    obj->p_obj.init.cnt2.inttxwe = TSPI_TX_INT_DISABLE;
    obj->p_obj.init.cnt2.intrxwe = TSPI_RX_INT_DISABLE;
    obj->p_obj.init.cnt2.inttxfe = TSPI_TX_FIFO_INT_DISABLE;
    obj->p_obj.init.cnt2.intrxfe = TSPI_RX_FIFO_INT_DISABLE;
    obj->p_obj.init.cnt2.interr  = TSPI_ERR_INT_DISABLE;
    obj->p_obj.init.cnt2.dmate   = TSPI_TX_DMA_INT_DISABLE;
    obj->p_obj.init.cnt2.dmare   = TSPI_RX_DMA_INT_DISABLE;

    //Control 3 configurations
    obj->p_obj.init.cnt3.tfempclr = TSPI_TX_BUFF_CLR_DONE;  // transmit buffer clear
    obj->p_obj.init.cnt3.rffllclr = TSPI_RX_BUFF_CLR_DONE;  // receive buffer clear

    //baudrate settings
    spi_frequency(obj, (int)INITIAL_SPI_FREQ);

    //Format Control 0 settings
    obj->p_obj.init.fmr0.dir  = TSPI_DATA_DIRECTION_MSB;    // MSB bit first
    obj->p_obj.init.fmr0.fl   = TSPI_DATA_LENGTH_8;
    obj->p_obj.init.fmr0.fint = TSPI_INTERVAL_TIME_0;

    //Special control on polarity of signal and generation timing
    obj->p_obj.init.fmr0.cs3pol = TSPI_TSPIxCS3_NEGATIVE;
    obj->p_obj.init.fmr0.cs2pol = TSPI_TSPIxCS2_NEGATIVE;
    obj->p_obj.init.fmr0.cs1pol = TSPI_TSPIxCS1_NEGATIVE;
    obj->p_obj.init.fmr0.cs0pol = TSPI_TSPIxCS0_NEGATIVE;

    obj->p_obj.init.fmr0.ckpha   = TSPI_SERIAL_CK_1ST_EDGE;
    obj->p_obj.init.fmr0.ckpol   = TSPI_SERIAL_CK_IDLE_LOW;
    obj->p_obj.init.fmr0.csint   = TSPI_MIN_IDLE_TIME_1;
    obj->p_obj.init.fmr0.cssckdl = TSPI_SERIAL_CK_DELAY_1;
    obj->p_obj.init.fmr0.sckcsdl = TSPI_NEGATE_1;

    //Format Control 1 settings tspi_fmtr1_t
    obj->p_obj.init.fmr1.vpe = TSPI_PARITY_DISABLE;
    obj->p_obj.init.fmr1.vpm = TSPI_PARITY_BIT_ODD;

    obj->bits = (uint8_t)TSPI_DATA_LENGTH_8;
    //initialize SPI
    tspi_init(&obj->p_obj);
}

void spi_free(spi_t *obj)
{
    tspi_deinit(&obj->p_obj);
    obj->module = (SPIName)NC;
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    MBED_ASSERT((slave == 0U) || (slave == 1U));   // 0: master mode, 1: slave mode
    MBED_ASSERT((bits >= 8) && (bits <= 32));

    obj->bits = bits;
    obj->p_obj.init.fmr0.fl = (bits << 24);

    if ((mode >> 1) & 0x1) {
        obj->p_obj.init.fmr0.ckpol = TSPI_SERIAL_CK_IDLE_HI;
    } else {
        obj->p_obj.init.fmr0.ckpol = TSPI_SERIAL_CK_IDLE_LOW;
    }

    if (mode & 0x1) {
        obj->p_obj.init.fmr0.ckpha = TSPI_SERIAL_CK_2ND_EDGE;
    } else {
        obj->p_obj.init.fmr0.ckpha = TSPI_SERIAL_CK_1ST_EDGE;
    }

    tspi_init(&obj->p_obj);
}

void spi_frequency(spi_t *obj, int hz)
{
    uint8_t  brs      = 0;
    uint8_t  brck     = 0;
    uint16_t prsck    = 1;
    uint64_t fscl     = 0;
    uint64_t tmp_fscl = 0;
    uint64_t fx       = 0;
    uint64_t tmpvar   = SystemCoreClock;

    SystemCoreClockUpdate();

    tmpvar = tmpvar / 2;

    for (prsck = 1; prsck <= 512; prsck *= 2) {
        fx = ((uint64_t)tmpvar / prsck);
        for (brs = 1; brs <= 16; brs++) {
            fscl = fx /brs;
            if ((fscl <= (uint64_t)hz) && (fscl > tmp_fscl)) {
                tmp_fscl = fscl;
                obj->p_obj.init.brd.brck = (brck << 4);
                if (brs == 16) {
                    obj->p_obj.init.brd.brs = 0;
                } else {
                    obj->p_obj.init.brd.brs = brs;
                }
            }
        }
        brck ++;
    }
    tspi_init(&obj->p_obj);
}

int spi_master_write(spi_t *obj, int value)
{
    uint8_t ret_value = 0;

    tspi_transmit_t send_obj;
    tspi_receive_t rec_obj;

    // Transmit data
    send_obj.tx8.p_data = (uint8_t *)&value;
    send_obj.tx8.num    = 1;
    tspi_master_write(&obj->p_obj, &send_obj, TIMEOUT);

    // Read received data
    rec_obj.rx8.p_data = &ret_value;
    rec_obj.rx8.num    = 1;
    tspi_master_read(&obj->p_obj, &rec_obj, TIMEOUT);

    return ret_value;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    for (int i = 0; i < total; i++) {
        char out = (i < tx_length) ? tx_buffer[i] : write_fill;
        char in  = spi_master_write(obj, out);
        if (i < rx_length) {
            rx_buffer[i] = in;
        }
    }

    return total;
}

int spi_busy(spi_t *obj)
{
    int      ret    = 1;
    uint32_t status = 0;

    tspi_get_status(&obj->p_obj, &status);

    if ((status & (TSPI_TX_FLAG_ACTIVE | TSPI_RX_FLAG_ACTIVE)) == 0) {
        ret = 0;
    }

    return ret;
}

uint8_t spi_get_module(spi_t *obj)
{
    return (uint8_t)(obj->module);
}
