/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
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
 */

#include "fsl_flexio_spi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief FLEXIO SPI transfer state, which is used for SPI transactiaonl APIs' internal state. */
enum _flexio_spi_transfer_states
{
    kFLEXIO_SPI_Idle = 0x0U, /*!< Nothing in the transmitter/receiver's queue. */
    kFLEXIO_SPI_Busy,        /*!< Transmiter/Receive's queue is not finished. */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Send a piece of data for SPI.
 *
 * This function computes the number of data to be written into D register or Tx FIFO,
 * and write the data into it. At the same time, this function updates the values in
 * master handle structure.
 *
 * @param base pointer to FLEXIO_SPI_Type structure
 * @param handle Pointer to SPI master handle structure.
 */
static void FLEXIO_SPI_TransferSendTransaction(FLEXIO_SPI_Type *base, flexio_spi_master_handle_t *handle);

/*!
 * @brief Receive a piece of data for SPI master.
 *
 * This function computes the number of data to receive from D register or Rx FIFO,
 * and write the data to destination address. At the same time, this function updates
 * the values in master handle structure.
 *
 * @param base pointer to FLEXIO_SPI_Type structure
 * @param handle Pointer to SPI master handle structure.
 */
static void FLEXIO_SPI_TransferReceiveTransaction(FLEXIO_SPI_Type *base, flexio_spi_master_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Codes
 ******************************************************************************/

static void FLEXIO_SPI_TransferSendTransaction(FLEXIO_SPI_Type *base, flexio_spi_master_handle_t *handle)
{
    uint16_t tmpData = FLEXIO_SPI_DUMMYDATA;

    if (handle->txData != NULL)
    {
        /* Transmit data and update tx size/buff. */
        if (handle->bytePerFrame == 1U)
        {
            tmpData = *(handle->txData);
            handle->txData++;
        }
        else
        {
            tmpData = (uint32_t)(handle->txData[0]) << 8U;
            tmpData += handle->txData[1];
            handle->txData += 2U;
        }
    }
    else
    {
        tmpData = FLEXIO_SPI_DUMMYDATA;
    }

    handle->txRemainingBytes -= handle->bytePerFrame;

    FLEXIO_SPI_WriteData(base, handle->direction, tmpData);

    if (!handle->txRemainingBytes)
    {
        FLEXIO_SPI_DisableInterrupts(base, kFLEXIO_SPI_TxEmptyInterruptEnable);
    }
}

static void FLEXIO_SPI_TransferReceiveTransaction(FLEXIO_SPI_Type *base, flexio_spi_master_handle_t *handle)
{
    uint16_t tmpData;

    tmpData = FLEXIO_SPI_ReadData(base, handle->direction);

    if (handle->rxData != NULL)
    {
        if (handle->bytePerFrame == 1U)
        {
            *handle->rxData = tmpData;
            handle->rxData++;
        }
        else
        {
            *((uint16_t *)(handle->rxData)) = tmpData;
            handle->rxData += 2U;
        }
    }
    handle->rxRemainingBytes -= handle->bytePerFrame;
}

void FLEXIO_SPI_MasterInit(FLEXIO_SPI_Type *base, flexio_spi_master_config_t *masterConfig, uint32_t srcClock_Hz)
{
    assert(base);
    assert(masterConfig);

    flexio_shifter_config_t shifterConfig;
    flexio_timer_config_t timerConfig;
    uint32_t ctrlReg = 0;
    uint16_t timerDiv = 0;
    uint16_t timerCmp = 0;

    /* Clear the shifterConfig & timerConfig struct. */
    memset(&shifterConfig, 0, sizeof(shifterConfig));
    memset(&timerConfig, 0, sizeof(timerConfig));

    /* Ungate flexio clock. */
    CLOCK_EnableClock(kCLOCK_Flexio0);

    /* Configure FLEXIO SPI Master */
    ctrlReg = base->flexioBase->CTRL;
    ctrlReg &= ~(FLEXIO_CTRL_DOZEN_MASK | FLEXIO_CTRL_DBGE_MASK | FLEXIO_CTRL_FASTACC_MASK | FLEXIO_CTRL_FLEXEN_MASK);
    ctrlReg |= (FLEXIO_CTRL_DOZEN(masterConfig->enableInDoze) | FLEXIO_CTRL_DBGE(masterConfig->enableInDebug) |
                FLEXIO_CTRL_FASTACC(masterConfig->enableFastAccess) | FLEXIO_CTRL_FLEXEN(masterConfig->enableMaster));

    base->flexioBase->CTRL = ctrlReg;

    /* Do hardware configuration. */
    /* 1. Configure the shifter 0 for tx. */
    shifterConfig.timerSelect = base->timerIndex[0];
    shifterConfig.pinConfig = kFLEXIO_PinConfigOutput;
    shifterConfig.pinSelect = base->SDOPinIndex;
    shifterConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode = kFLEXIO_ShifterModeTransmit;
    shifterConfig.inputSource = kFLEXIO_ShifterInputFromPin;
    if (masterConfig->phase == kFLEXIO_SPI_ClockPhaseFirstEdge)
    {
        shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnNegitive;
        shifterConfig.shifterStop = kFLEXIO_ShifterStopBitDisable;
        shifterConfig.shifterStart = kFLEXIO_ShifterStartBitDisabledLoadDataOnEnable;
    }
    else
    {
        shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnPositive;
        shifterConfig.shifterStop = kFLEXIO_ShifterStopBitLow;
        shifterConfig.shifterStart = kFLEXIO_ShifterStartBitDisabledLoadDataOnShift;
    }

    FLEXIO_SetShifterConfig(base->flexioBase, base->shifterIndex[0], &shifterConfig);

    /* 2. Configure the shifter 1 for rx. */
    shifterConfig.timerSelect = base->timerIndex[0];
    shifterConfig.pinConfig = kFLEXIO_PinConfigOutputDisabled;
    shifterConfig.pinSelect = base->SDIPinIndex;
    shifterConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode = kFLEXIO_ShifterModeReceive;
    shifterConfig.inputSource = kFLEXIO_ShifterInputFromPin;
    shifterConfig.shifterStop = kFLEXIO_ShifterStopBitDisable;
    shifterConfig.shifterStart = kFLEXIO_ShifterStartBitDisabledLoadDataOnEnable;
    if (masterConfig->phase == kFLEXIO_SPI_ClockPhaseFirstEdge)
    {
        shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnPositive;
    }
    else
    {
        shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnNegitive;
    }

    FLEXIO_SetShifterConfig(base->flexioBase, base->shifterIndex[1], &shifterConfig);

    /*3. Configure the timer 0 for SCK. */
    timerConfig.triggerSelect = FLEXIO_TIMER_TRIGGER_SEL_SHIFTnSTAT(base->shifterIndex[0]);
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    timerConfig.triggerSource = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.pinConfig = kFLEXIO_PinConfigOutput;
    timerConfig.pinSelect = base->SCKPinIndex;
    timerConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    timerConfig.timerMode = kFLEXIO_TimerModeDual8BitBaudBit;
    timerConfig.timerOutput = kFLEXIO_TimerOutputZeroNotAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerReset = kFLEXIO_TimerResetNever;
    timerConfig.timerDisable = kFLEXIO_TimerDisableOnTimerCompare;
    timerConfig.timerEnable = kFLEXIO_TimerEnableOnTriggerHigh;
    timerConfig.timerStop = kFLEXIO_TimerStopBitEnableOnTimerDisable;
    timerConfig.timerStart = kFLEXIO_TimerStartBitEnabled;

    timerDiv = srcClock_Hz / masterConfig->baudRate_Bps;
    timerDiv = timerDiv / 2 - 1;

    timerCmp = ((uint32_t)(masterConfig->dataMode * 2 - 1U)) << 8U;
    timerCmp |= timerDiv;

    timerConfig.timerCompare = timerCmp;

    FLEXIO_SetTimerConfig(base->flexioBase, base->timerIndex[0], &timerConfig);

    /* 4. Configure the timer 1 for CSn. */
    timerConfig.triggerSelect = FLEXIO_TIMER_TRIGGER_SEL_TIMn(base->timerIndex[0]);
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveHigh;
    timerConfig.triggerSource = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.pinConfig = kFLEXIO_PinConfigOutput;
    timerConfig.pinSelect = base->CSnPinIndex;
    timerConfig.pinPolarity = kFLEXIO_PinActiveLow;
    timerConfig.timerMode = kFLEXIO_TimerModeSingle16Bit;
    timerConfig.timerOutput = kFLEXIO_TimerOutputOneNotAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerReset = kFLEXIO_TimerResetNever;
    timerConfig.timerDisable = kFLEXIO_TimerDisableOnPreTimerDisable;
    timerConfig.timerEnable = kFLEXIO_TimerEnableOnPrevTimerEnable;
    timerConfig.timerStop = kFLEXIO_TimerStopBitDisabled;
    timerConfig.timerStart = kFLEXIO_TimerStartBitDisabled;

    timerConfig.timerCompare = 0xFFFFU;

    FLEXIO_SetTimerConfig(base->flexioBase, base->timerIndex[1], &timerConfig);
}

void FLEXIO_SPI_MasterDeinit(FLEXIO_SPI_Type *base)
{
    /* Disable FLEXIO SPI module. */
    FLEXIO_SPI_Enable(base, false);

    /* Gate flexio clock. */
    CLOCK_DisableClock(kCLOCK_Flexio0);
}

void FLEXIO_SPI_MasterGetDefaultConfig(flexio_spi_master_config_t *masterConfig)
{
    assert(masterConfig);

    masterConfig->enableMaster = true;
    masterConfig->enableInDoze = false;
    masterConfig->enableInDebug = true;
    masterConfig->enableFastAccess = false;
    /* Default baud rate 500kbps. */
    masterConfig->baudRate_Bps = 500000U;
    /* Default CPHA = 0. */
    masterConfig->phase = kFLEXIO_SPI_ClockPhaseFirstEdge;
    /* Default bit count at 8. */
    masterConfig->dataMode = kFLEXIO_SPI_8BitMode;
}

void FLEXIO_SPI_SlaveInit(FLEXIO_SPI_Type *base, flexio_spi_slave_config_t *slaveConfig)
{
    assert(base && slaveConfig);

    flexio_shifter_config_t shifterConfig;
    flexio_timer_config_t timerConfig;
    uint32_t ctrlReg = 0;

    /* Clear the shifterConfig & timerConfig struct. */
    memset(&shifterConfig, 0, sizeof(shifterConfig));
    memset(&timerConfig, 0, sizeof(timerConfig));

    /* Ungate flexio clock. */
    CLOCK_EnableClock(kCLOCK_Flexio0);

    /* Configure FLEXIO SPI Slave */
    ctrlReg = base->flexioBase->CTRL;
    ctrlReg &= ~(FLEXIO_CTRL_DOZEN_MASK | FLEXIO_CTRL_DBGE_MASK | FLEXIO_CTRL_FASTACC_MASK | FLEXIO_CTRL_FLEXEN_MASK);
    ctrlReg |= (FLEXIO_CTRL_DOZEN(slaveConfig->enableInDoze) | FLEXIO_CTRL_DBGE(slaveConfig->enableInDebug) |
                FLEXIO_CTRL_FASTACC(slaveConfig->enableFastAccess) | FLEXIO_CTRL_FLEXEN(slaveConfig->enableSlave));

    base->flexioBase->CTRL = ctrlReg;

    /* Do hardware configuration. */
    /* 1. Configure the shifter 0 for tx. */
    shifterConfig.timerSelect = base->timerIndex[0];
    shifterConfig.pinConfig = kFLEXIO_PinConfigOutput;
    shifterConfig.pinSelect = base->SDOPinIndex;
    shifterConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode = kFLEXIO_ShifterModeTransmit;
    shifterConfig.inputSource = kFLEXIO_ShifterInputFromPin;
    shifterConfig.shifterStop = kFLEXIO_ShifterStopBitDisable;
    if (slaveConfig->phase == kFLEXIO_SPI_ClockPhaseFirstEdge)
    {
        shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnNegitive;
        shifterConfig.shifterStart = kFLEXIO_ShifterStartBitDisabledLoadDataOnEnable;
    }
    else
    {
        shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnPositive;
        shifterConfig.shifterStart = kFLEXIO_ShifterStartBitDisabledLoadDataOnShift;
    }

    FLEXIO_SetShifterConfig(base->flexioBase, base->shifterIndex[0], &shifterConfig);

    /* 2. Configure the shifter 1 for rx. */
    shifterConfig.timerSelect = base->timerIndex[0];
    shifterConfig.pinConfig = kFLEXIO_PinConfigOutputDisabled;
    shifterConfig.pinSelect = base->SDIPinIndex;
    shifterConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode = kFLEXIO_ShifterModeReceive;
    shifterConfig.inputSource = kFLEXIO_ShifterInputFromPin;
    shifterConfig.shifterStop = kFLEXIO_ShifterStopBitDisable;
    shifterConfig.shifterStart = kFLEXIO_ShifterStartBitDisabledLoadDataOnEnable;
    if (slaveConfig->phase == kFLEXIO_SPI_ClockPhaseFirstEdge)
    {
        shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnPositive;
    }
    else
    {
        shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnNegitive;
    }

    FLEXIO_SetShifterConfig(base->flexioBase, base->shifterIndex[1], &shifterConfig);

    /*3. Configure the timer 0 for shift clock. */
    timerConfig.triggerSelect = FLEXIO_TIMER_TRIGGER_SEL_PININPUT(base->CSnPinIndex);
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    timerConfig.triggerSource = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.pinConfig = kFLEXIO_PinConfigOutputDisabled;
    timerConfig.pinSelect = base->SCKPinIndex;
    timerConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    timerConfig.timerMode = kFLEXIO_TimerModeSingle16Bit;
    timerConfig.timerOutput = kFLEXIO_TimerOutputZeroNotAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnPinInputShiftPinInput;
    timerConfig.timerReset = kFLEXIO_TimerResetNever;
    timerConfig.timerEnable = kFLEXIO_TimerEnableOnTriggerRisingEdge;
    timerConfig.timerStop = kFLEXIO_TimerStopBitDisabled;
    if (slaveConfig->phase == kFLEXIO_SPI_ClockPhaseFirstEdge)
    {
        /* The configuration kFLEXIO_TimerDisableOnTimerCompare only support continuous
        PCS access, change to kFLEXIO_TimerDisableNever to enable discontinuous PCS access. */
        timerConfig.timerDisable = kFLEXIO_TimerDisableOnTimerCompare;
        timerConfig.timerStart = kFLEXIO_TimerStartBitDisabled;
    }
    else
    {
        timerConfig.timerDisable = kFLEXIO_TimerDisableOnTriggerFallingEdge;
        timerConfig.timerStart = kFLEXIO_TimerStartBitEnabled;
    }

    timerConfig.timerCompare = slaveConfig->dataMode * 2 - 1U;

    FLEXIO_SetTimerConfig(base->flexioBase, base->timerIndex[0], &timerConfig);
}

void FLEXIO_SPI_SlaveDeinit(FLEXIO_SPI_Type *base)
{
    FLEXIO_SPI_MasterDeinit(base);
}

void FLEXIO_SPI_SlaveGetDefaultConfig(flexio_spi_slave_config_t *slaveConfig)
{
    assert(slaveConfig);

    slaveConfig->enableSlave = true;
    slaveConfig->enableInDoze = false;
    slaveConfig->enableInDebug = true;
    slaveConfig->enableFastAccess = false;
    /* Default CPHA = 0. */
    slaveConfig->phase = kFLEXIO_SPI_ClockPhaseFirstEdge;
    /* Default bit count at 8. */
    slaveConfig->dataMode = kFLEXIO_SPI_8BitMode;
}

void FLEXIO_SPI_EnableInterrupts(FLEXIO_SPI_Type *base, uint32_t mask)
{
    if (mask & kFLEXIO_SPI_TxEmptyInterruptEnable)
    {
        FLEXIO_EnableShifterStatusInterrupts(base->flexioBase, 1 << base->shifterIndex[0]);
    }
    if (mask & kFLEXIO_SPI_RxFullInterruptEnable)
    {
        FLEXIO_EnableShifterStatusInterrupts(base->flexioBase, 1 << base->shifterIndex[1]);
    }
}

void FLEXIO_SPI_DisableInterrupts(FLEXIO_SPI_Type *base, uint32_t mask)
{
    if (mask & kFLEXIO_SPI_TxEmptyInterruptEnable)
    {
        FLEXIO_DisableShifterStatusInterrupts(base->flexioBase, 1 << base->shifterIndex[0]);
    }
    if (mask & kFLEXIO_SPI_RxFullInterruptEnable)
    {
        FLEXIO_DisableShifterStatusInterrupts(base->flexioBase, 1 << base->shifterIndex[1]);
    }
}

void FLEXIO_SPI_EnableDMA(FLEXIO_SPI_Type *base, uint32_t mask, bool enable)
{
    if (mask & kFLEXIO_SPI_TxDmaEnable)
    {
        FLEXIO_EnableShifterStatusDMA(base->flexioBase, 1U << base->shifterIndex[0], enable);
    }

    if (mask & kFLEXIO_SPI_RxDmaEnable)
    {
        FLEXIO_EnableShifterStatusDMA(base->flexioBase, 1U << base->shifterIndex[1], enable);
    }
}

uint32_t FLEXIO_SPI_GetStatusFlags(FLEXIO_SPI_Type *base)
{
    uint32_t shifterStatus = FLEXIO_GetShifterStatusFlags(base->flexioBase);
    uint32_t status = 0;

    status = ((shifterStatus & (1U << base->shifterIndex[0])) >> base->shifterIndex[0]);
    status |= (((shifterStatus & (1U << base->shifterIndex[1])) >> (base->shifterIndex[1])) << 1U);

    return status;
}

void FLEXIO_SPI_ClearStatusFlags(FLEXIO_SPI_Type *base, uint32_t mask)
{
    if (mask & kFLEXIO_SPI_TxBufferEmptyFlag)
    {
        FLEXIO_ClearShifterStatusFlags(base->flexioBase, 1U << base->shifterIndex[0]);
    }
    if (mask & kFLEXIO_SPI_RxBufferFullFlag)
    {
        FLEXIO_ClearShifterStatusFlags(base->flexioBase, 1U << base->shifterIndex[1]);
    }
}

void FLEXIO_SPI_MasterSetBaudRate(FLEXIO_SPI_Type *base, uint32_t baudRate_Bps, uint32_t srcClockHz)
{
    uint16_t timerDiv = 0;
    uint16_t timerCmp = 0;
    FLEXIO_Type *flexioBase = base->flexioBase;

    /* Set TIMCMP[7:0] = (baud rate divider / 2) - 1.*/
    timerDiv = srcClockHz / baudRate_Bps;
    timerDiv = timerDiv / 2 - 1U;

    timerCmp = flexioBase->TIMCMP[base->timerIndex[0]];
    timerCmp &= 0xFF00U;
    timerCmp |= timerDiv;

    flexioBase->TIMCMP[base->timerIndex[0]] = timerCmp;
}

void FLEXIO_SPI_WriteBlocking(FLEXIO_SPI_Type *base,
                              flexio_spi_shift_direction_t direction,
                              const uint8_t *buffer,
                              size_t size)
{
    assert(buffer);
    assert(size);

    while (size--)
    {
        /* Wait until data transfer complete. */
        while (!(FLEXIO_SPI_GetStatusFlags(base) & kFLEXIO_SPI_TxBufferEmptyFlag))
        {
        }
        FLEXIO_SPI_WriteData(base, direction, *buffer++);
    }
}

void FLEXIO_SPI_ReadBlocking(FLEXIO_SPI_Type *base,
                             flexio_spi_shift_direction_t direction,
                             uint8_t *buffer,
                             size_t size)
{
    assert(buffer);
    assert(size);

    while (size--)
    {
        /* Wait until data transfer complete. */
        while (!(FLEXIO_SPI_GetStatusFlags(base) & kFLEXIO_SPI_RxBufferFullFlag))
        {
        }
        *buffer++ = FLEXIO_SPI_ReadData(base, direction);
    }
}

void FLEXIO_SPI_MasterTransferBlocking(FLEXIO_SPI_Type *base, flexio_spi_transfer_t *xfer)
{
    flexio_spi_shift_direction_t direction;
    uint8_t bytesPerFrame;
    uint32_t dataMode = 0;
    uint16_t timerCmp = base->flexioBase->TIMCMP[base->timerIndex[0]];
    uint16_t tmpData = FLEXIO_SPI_DUMMYDATA;

    timerCmp &= 0x00FFU;
    /* Configure the values in handle. */
    switch (xfer->flags)
    {
        case kFLEXIO_SPI_8bitMsb:
            dataMode = (8 * 2 - 1U) << 8U;
            bytesPerFrame = 1;
            direction = kFLEXIO_SPI_MsbFirst;
            break;

        case kFLEXIO_SPI_8bitLsb:
            dataMode = (8 * 2 - 1U) << 8U;
            bytesPerFrame = 1;
            direction = kFLEXIO_SPI_LsbFirst;
            break;

        case kFLEXIO_SPI_16bitMsb:
            dataMode = (16 * 2 - 1U) << 8U;
            bytesPerFrame = 2;
            direction = kFLEXIO_SPI_MsbFirst;
            break;

        case kFLEXIO_SPI_16bitLsb:
            dataMode = (16 * 2 - 1U) << 8U;
            bytesPerFrame = 2;
            direction = kFLEXIO_SPI_LsbFirst;
            break;

        default:
            dataMode = (8 * 2 - 1U) << 8U;
            bytesPerFrame = 1;
            direction = kFLEXIO_SPI_MsbFirst;
            assert(true);
            break;
    }

    dataMode |= timerCmp;

    /* Configure transfer size. */
    base->flexioBase->TIMCMP[base->timerIndex[0]] = dataMode;

    while (xfer->dataSize)
    {
        /* Wait until data transfer complete. */
        while (!(FLEXIO_SPI_GetStatusFlags(base) & kFLEXIO_SPI_TxBufferEmptyFlag))
        {
        }
        if (xfer->txData != NULL)
        {
            /* Transmit data and update tx size/buff. */
            if (bytesPerFrame == 1U)
            {
                tmpData = *(xfer->txData);
                xfer->txData++;
            }
            else
            {
                tmpData = (uint32_t)(xfer->txData[0]) << 8U;
                tmpData += xfer->txData[1];
                xfer->txData += 2U;
            }
        }
        else
        {
            tmpData = FLEXIO_SPI_DUMMYDATA;
        }

        xfer->dataSize -= bytesPerFrame;

        FLEXIO_SPI_WriteData(base, direction, tmpData);

        while (!(FLEXIO_SPI_GetStatusFlags(base) & kFLEXIO_SPI_RxBufferFullFlag))
        {
        }
        tmpData = FLEXIO_SPI_ReadData(base, direction);

        if (xfer->rxData != NULL)
        {
            if (bytesPerFrame == 1U)
            {
                *xfer->rxData = tmpData;
                xfer->rxData++;
            }
            else
            {
                *((uint16_t *)(xfer->rxData)) = tmpData;
                xfer->rxData += 2U;
            }
        }
    }
}

status_t FLEXIO_SPI_MasterTransferCreateHandle(FLEXIO_SPI_Type *base,
                                               flexio_spi_master_handle_t *handle,
                                               flexio_spi_master_transfer_callback_t callback,
                                               void *userData)
{
    assert(handle);

    IRQn_Type flexio_irqs[] = FLEXIO_IRQS;

    /* Zero the handle. */
    memset(handle, 0, sizeof(*handle));

    /* Register callback and userData. */
    handle->callback = callback;
    handle->userData = userData;

    /* Enable interrupt in NVIC. */
    EnableIRQ(flexio_irqs[0]);

    /* Save the context in global variables to support the double weak mechanism. */
    return FLEXIO_RegisterHandleIRQ(base, handle, FLEXIO_SPI_MasterTransferHandleIRQ);
}

status_t FLEXIO_SPI_MasterTransferNonBlocking(FLEXIO_SPI_Type *base,
                                              flexio_spi_master_handle_t *handle,
                                              flexio_spi_transfer_t *xfer)
{
    assert(handle);
    assert(xfer);

    uint32_t dataMode = 0;
    uint16_t timerCmp = base->flexioBase->TIMCMP[base->timerIndex[0]];
    uint16_t tmpData = FLEXIO_SPI_DUMMYDATA;

    timerCmp &= 0x00FFU;

    /* Check if SPI is busy. */
    if (handle->state == kFLEXIO_SPI_Busy)
    {
        return kStatus_FLEXIO_SPI_Busy;
    }

    /* Check if the argument is legal. */
    if ((xfer->txData == NULL) && (xfer->rxData == NULL))
    {
        return kStatus_InvalidArgument;
    }

    /* Configure the values in handle */
    switch (xfer->flags)
    {
        case kFLEXIO_SPI_8bitMsb:
            dataMode = (8 * 2 - 1U) << 8U;
            handle->bytePerFrame = 1U;
            handle->direction = kFLEXIO_SPI_MsbFirst;
            break;
        case kFLEXIO_SPI_8bitLsb:
            dataMode = (8 * 2 - 1U) << 8U;
            handle->bytePerFrame = 1U;
            handle->direction = kFLEXIO_SPI_LsbFirst;
            break;
        case kFLEXIO_SPI_16bitMsb:
            dataMode = (16 * 2 - 1U) << 8U;
            handle->bytePerFrame = 2U;
            handle->direction = kFLEXIO_SPI_MsbFirst;
            break;
        case kFLEXIO_SPI_16bitLsb:
            dataMode = (16 * 2 - 1U) << 8U;
            handle->bytePerFrame = 2U;
            handle->direction = kFLEXIO_SPI_LsbFirst;
            break;
        default:
            dataMode = (8 * 2 - 1U) << 8U;
            handle->bytePerFrame = 1U;
            handle->direction = kFLEXIO_SPI_MsbFirst;
            assert(true);
            break;
    }

    dataMode |= timerCmp;

    /* Configure transfer size. */
    base->flexioBase->TIMCMP[base->timerIndex[0]] = dataMode;

    handle->state = kFLEXIO_SPI_Busy;
    handle->txData = xfer->txData;
    handle->rxData = xfer->rxData;
    handle->rxRemainingBytes = xfer->dataSize;

    /* Save total transfer size. */
    handle->transferSize = xfer->dataSize;

    /* Send first byte of data to trigger the rx interrupt. */
    if (handle->txData != NULL)
    {
        /* Transmit data and update tx size/buff. */
        if (handle->bytePerFrame == 1U)
        {
            tmpData = *(handle->txData);
            handle->txData++;
        }
        else
        {
            tmpData = (uint32_t)(handle->txData[0]) << 8U;
            tmpData += handle->txData[1];
            handle->txData += 2U;
        }
    }
    else
    {
        tmpData = FLEXIO_SPI_DUMMYDATA;
    }

    handle->txRemainingBytes = xfer->dataSize - handle->bytePerFrame;

    FLEXIO_SPI_WriteData(base, handle->direction, tmpData);

    /* Enable transmit and receive interrupt to handle rx. */
    FLEXIO_SPI_EnableInterrupts(base, kFLEXIO_SPI_RxFullInterruptEnable);

    return kStatus_Success;
}

status_t FLEXIO_SPI_MasterTransferGetCount(FLEXIO_SPI_Type *base, flexio_spi_master_handle_t *handle, size_t *count)
{
    assert(handle);

    if (!count)
    {
        return kStatus_InvalidArgument;
    }

    /* Return remaing bytes in different cases. */
    if (handle->rxData)
    {
        *count = handle->transferSize - handle->rxRemainingBytes;
    }
    else
    {
        *count = handle->transferSize - handle->txRemainingBytes;
    }

    return kStatus_Success;
}

void FLEXIO_SPI_MasterTransferAbort(FLEXIO_SPI_Type *base, flexio_spi_master_handle_t *handle)
{
    assert(handle);

    FLEXIO_SPI_DisableInterrupts(base, kFLEXIO_SPI_RxFullInterruptEnable);
    FLEXIO_SPI_DisableInterrupts(base, kFLEXIO_SPI_TxEmptyInterruptEnable);

    /* Transfer finished, set the state to idle. */
    handle->state = kFLEXIO_SPI_Idle;

    /* Clear the internal state. */
    handle->rxRemainingBytes = 0;
    handle->txRemainingBytes = 0;
}

void FLEXIO_SPI_MasterTransferHandleIRQ(void *spiType, void *spiHandle)
{
    assert(spiHandle);

    flexio_spi_master_handle_t *handle = (flexio_spi_master_handle_t *)spiHandle;
    FLEXIO_SPI_Type *base;
    uint32_t status;

    if (handle->state == kFLEXIO_SPI_Idle)
    {
        return;
    }

    base = (FLEXIO_SPI_Type *)spiType;
    status = FLEXIO_SPI_GetStatusFlags(base);

    /* Handle rx. */
    if ((status & kFLEXIO_SPI_RxBufferFullFlag) && (handle->rxRemainingBytes))
    {
        FLEXIO_SPI_TransferReceiveTransaction(base, handle);
    }

    /* Handle tx. */
    if ((status & kFLEXIO_SPI_TxBufferEmptyFlag) && (handle->txRemainingBytes))
    {
        FLEXIO_SPI_TransferSendTransaction(base, handle);
    }

    /* All the transfer finished. */
    if ((handle->txRemainingBytes == 0U) && (handle->rxRemainingBytes == 0U))
    {
        FLEXIO_SPI_MasterTransferAbort(base, handle);
        if (handle->callback)
        {
            (handle->callback)(base, handle, kStatus_FLEXIO_SPI_Idle, handle->userData);
        }
    }
}

status_t FLEXIO_SPI_SlaveTransferCreateHandle(FLEXIO_SPI_Type *base,
                                              flexio_spi_slave_handle_t *handle,
                                              flexio_spi_slave_transfer_callback_t callback,
                                              void *userData)
{
    assert(handle);

    IRQn_Type flexio_irqs[] = FLEXIO_IRQS;

    /* Zero the handle. */
    memset(handle, 0, sizeof(*handle));

    /* Register callback and userData. */
    handle->callback = callback;
    handle->userData = userData;

    /* Enable interrupt in NVIC. */
    EnableIRQ(flexio_irqs[0]);

    /* Save the context in global variables to support the double weak mechanism. */
    return FLEXIO_RegisterHandleIRQ(base, handle, FLEXIO_SPI_SlaveTransferHandleIRQ);
}

status_t FLEXIO_SPI_SlaveTransferNonBlocking(FLEXIO_SPI_Type *base,
                                             flexio_spi_slave_handle_t *handle,
                                             flexio_spi_transfer_t *xfer)
{
    assert(handle);
    assert(xfer);

    uint32_t dataMode = 0;

    /* Check if SPI is busy. */
    if (handle->state == kFLEXIO_SPI_Busy)
    {
        return kStatus_FLEXIO_SPI_Busy;
    }

    /* Check if the argument is legal. */
    if ((xfer->txData == NULL) && (xfer->rxData == NULL))
    {
        return kStatus_InvalidArgument;
    }

    /* Configure the values in handle */
    switch (xfer->flags)
    {
        case kFLEXIO_SPI_8bitMsb:
            dataMode = 8 * 2 - 1U;
            handle->bytePerFrame = 1U;
            handle->direction = kFLEXIO_SPI_MsbFirst;
            break;
        case kFLEXIO_SPI_8bitLsb:
            dataMode = 8 * 2 - 1U;
            handle->bytePerFrame = 1U;
            handle->direction = kFLEXIO_SPI_LsbFirst;
            break;
        case kFLEXIO_SPI_16bitMsb:
            dataMode = 16 * 2 - 1U;
            handle->bytePerFrame = 2U;
            handle->direction = kFLEXIO_SPI_MsbFirst;
            break;
        case kFLEXIO_SPI_16bitLsb:
            dataMode = 16 * 2 - 1U;
            handle->bytePerFrame = 2U;
            handle->direction = kFLEXIO_SPI_LsbFirst;
            break;
        default:
            dataMode = 8 * 2 - 1U;
            handle->bytePerFrame = 1U;
            handle->direction = kFLEXIO_SPI_MsbFirst;
            assert(true);
            break;
    }

    /* Configure transfer size. */
    base->flexioBase->TIMCMP[base->timerIndex[0]] = dataMode;

    handle->state = kFLEXIO_SPI_Busy;
    handle->txData = xfer->txData;
    handle->rxData = xfer->rxData;
    handle->txRemainingBytes = xfer->dataSize;
    handle->rxRemainingBytes = xfer->dataSize;

    /* Save total transfer size. */
    handle->transferSize = xfer->dataSize;

    /* Enable transmit and receive interrupt to handle tx and rx. */
    FLEXIO_SPI_EnableInterrupts(base, kFLEXIO_SPI_TxEmptyInterruptEnable);
    FLEXIO_SPI_EnableInterrupts(base, kFLEXIO_SPI_RxFullInterruptEnable);

    return kStatus_Success;
}

void FLEXIO_SPI_SlaveTransferHandleIRQ(void *spiType, void *spiHandle)
{
    assert(spiHandle);

    flexio_spi_master_handle_t *handle = (flexio_spi_master_handle_t *)spiHandle;
    FLEXIO_SPI_Type *base;
    uint32_t status;

    if (handle->state == kFLEXIO_SPI_Idle)
    {
        return;
    }

    base = (FLEXIO_SPI_Type *)spiType;
    status = FLEXIO_SPI_GetStatusFlags(base);

    /* Handle tx. */
    if ((status & kFLEXIO_SPI_TxBufferEmptyFlag) && (handle->txRemainingBytes))
    {
        FLEXIO_SPI_TransferSendTransaction(base, handle);
    }

    /* Handle rx. */
    if ((status & kFLEXIO_SPI_RxBufferFullFlag) && (handle->rxRemainingBytes))
    {
        FLEXIO_SPI_TransferReceiveTransaction(base, handle);
    }

    /* All the transfer finished. */
    if ((handle->txRemainingBytes == 0U) && (handle->rxRemainingBytes == 0U))
    {
        FLEXIO_SPI_SlaveTransferAbort(base, handle);
        if (handle->callback)
        {
            (handle->callback)(base, handle, kStatus_FLEXIO_SPI_Idle, handle->userData);
        }
    }
}
