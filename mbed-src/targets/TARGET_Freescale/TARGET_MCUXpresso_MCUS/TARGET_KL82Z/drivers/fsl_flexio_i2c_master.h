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
#ifndef _FSL_FLEXIO_I2C_MASTER_H_
#define _FSL_FLEXIO_I2C_MASTER_H_

#include "fsl_common.h"
#include "fsl_flexio.h"

/*!
 * @addtogroup flexio_i2c_master
 * @{
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief FlexIO I2C master driver version 2.1.2. */
#define FSL_FLEXIO_I2C_MASTER_DRIVER_VERSION (MAKE_VERSION(2, 1, 2))
/*@}*/

/*! @brief  FlexIO I2C transfer status*/
enum _flexio_i2c_status
{
    kStatus_FLEXIO_I2C_Busy = MAKE_STATUS(kStatusGroup_FLEXIO_I2C, 0), /*!< I2C is busy doing transfer. */
    kStatus_FLEXIO_I2C_Idle = MAKE_STATUS(kStatusGroup_FLEXIO_I2C, 1), /*!< I2C is busy doing transfer. */
    kStatus_FLEXIO_I2C_Nak = MAKE_STATUS(kStatusGroup_FLEXIO_I2C, 2),  /*!< NAK received during transfer. */
};

/*! @brief Define FlexIO I2C master interrupt mask. */
enum _flexio_i2c_master_interrupt
{
    kFLEXIO_I2C_TxEmptyInterruptEnable = 0x1U, /*!< Tx buffer empty interrupt enable. */
    kFLEXIO_I2C_RxFullInterruptEnable = 0x2U,  /*!< Rx buffer full interrupt enable. */
};

/*! @brief Define FlexIO I2C master status mask. */
enum _flexio_i2c_master_status_flags
{
    kFLEXIO_I2C_TxEmptyFlag = 0x1U,    /*!< Tx shifter empty flag. */
    kFLEXIO_I2C_RxFullFlag = 0x2U,     /*!< Rx shifter full/Transfer complete flag. */
    kFLEXIO_I2C_ReceiveNakFlag = 0x4U, /*!< Receive NAK flag. */
};

/*! @brief Direction of master transfer.*/
typedef enum _flexio_i2c_direction
{
    kFLEXIO_I2C_Write = 0x0U, /*!< Master send to slave. */
    kFLEXIO_I2C_Read = 0x1U,  /*!< Master receive from slave. */
} flexio_i2c_direction_t;

/*! @brief Define FlexIO I2C master access structure typedef. */
typedef struct _flexio_i2c_type
{
    FLEXIO_Type *flexioBase; /*!< FlexIO base pointer. */
    uint8_t SDAPinIndex;     /*!< Pin select for I2C SDA. */
    uint8_t SCLPinIndex;     /*!< Pin select for I2C SCL. */
    uint8_t shifterIndex[2]; /*!< Shifter index used in FlexIO I2C. */
    uint8_t timerIndex[2];   /*!< Timer index used in FlexIO I2C. */
} FLEXIO_I2C_Type;

/*! @brief Define FlexIO I2C master user configuration structure. */
typedef struct _flexio_i2c_master_config
{
    bool enableMaster;     /*!< Enables the FLEXIO I2C peripheral at initialization time. */
    bool enableInDoze;     /*!< Enable/disable FlexIO operation in doze mode. */
    bool enableInDebug;    /*!< Enable/disable FlexIO operation in debug mode. */
    bool enableFastAccess; /*!< Enable/disable fast access to FlexIO registers, fast access requires
                           the FlexIO clock to be at least twice the frequency of the bus clock. */
    uint32_t baudRate_Bps; /*!< Baud rate in Bps. */
} flexio_i2c_master_config_t;

/*! @brief Define FlexIO I2C master transfer structure. */
typedef struct _flexio_i2c_master_transfer
{
    uint32_t flags;                   /*!< Transfer flag which controls the transfer, reserved for FlexIO I2C. */
    uint8_t slaveAddress;             /*!< 7-bit slave address. */
    flexio_i2c_direction_t direction; /*!< Transfer direction, read or write. */
    uint32_t subaddress;              /*!< Sub address. Transferred MSB first. */
    uint8_t subaddressSize;           /*!< Size of command buffer. */
    uint8_t volatile *data;           /*!< Transfer buffer. */
    volatile size_t dataSize;         /*!< Transfer size. */
} flexio_i2c_master_transfer_t;

/*! @brief FlexIO I2C master handle typedef. */
typedef struct _flexio_i2c_master_handle flexio_i2c_master_handle_t;

/*! @brief FlexIO I2C master transfer callback typedef. */
typedef void (*flexio_i2c_master_transfer_callback_t)(FLEXIO_I2C_Type *base,
                                                      flexio_i2c_master_handle_t *handle,
                                                      status_t status,
                                                      void *userData);

/*! @brief Define FlexIO I2C master handle structure. */
struct _flexio_i2c_master_handle
{
    flexio_i2c_master_transfer_t transfer;                    /*!< FlexIO I2C master transfer copy. */
    size_t transferSize;                                      /*!< Total bytes to be transferred. */
    uint8_t state;                                            /*!< Transfer state maintained during transfer. */
    flexio_i2c_master_transfer_callback_t completionCallback; /*!< Callback function called at transfer event. */
                                                              /*!< Callback function called at transfer event. */
    void *userData;                                           /*!< Callback parameter passed to callback function. */
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/*!
 * @name Initialization and deinitialization
 * @{
 */

/*!
 * @brief Ungates the FlexIO clock, resets the FlexIO module, and configures the FlexIO I2C
 * hardware configuration.
 *
 * Example
   @code
   FLEXIO_I2C_Type base = {
   .flexioBase = FLEXIO,
   .SDAPinIndex = 0,
   .SCLPinIndex = 1,
   .shifterIndex = {0,1},
   .timerIndex = {0,1}
   };
   flexio_i2c_master_config_t config = {
   .enableInDoze = false,
   .enableInDebug = true,
   .enableFastAccess = false,
   .baudRate_Bps = 100000
   };
   FLEXIO_I2C_MasterInit(base, &config, srcClock_Hz);
   @endcode
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param masterConfig pointer to flexio_i2c_master_config_t structure.
 * @param srcClock_Hz FlexIO source clock in Hz.
*/
void FLEXIO_I2C_MasterInit(FLEXIO_I2C_Type *base, flexio_i2c_master_config_t *masterConfig, uint32_t srcClock_Hz);

/*!
 * @brief De-initializes the FlexIO I2C master peripheral. Calling this API gates the FlexIO clock,
 * so the FlexIO I2C master module can't work unless call FLEXIO_I2C_MasterInit.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 */
void FLEXIO_I2C_MasterDeinit(FLEXIO_I2C_Type *base);

/*!
 * @brief Gets the default configuration to configure the FlexIO module. The configuration
 * can be used directly for calling FLEXIO_I2C_MasterInit().
 *
 * Example:
   @code
   flexio_i2c_master_config_t config;
   FLEXIO_I2C_MasterGetDefaultConfig(&config);
   @endcode
 * @param masterConfig pointer to flexio_i2c_master_config_t structure.
*/
void FLEXIO_I2C_MasterGetDefaultConfig(flexio_i2c_master_config_t *masterConfig);

/*!
 * @brief Enables/disables the FlexIO module operation.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param enable pass true to enable module, false to disable module.
*/
static inline void FLEXIO_I2C_MasterEnable(FLEXIO_I2C_Type *base, bool enable)
{
    if (enable)
    {
        base->flexioBase->CTRL |= FLEXIO_CTRL_FLEXEN_MASK;
    }
    else
    {
        base->flexioBase->CTRL &= ~FLEXIO_CTRL_FLEXEN_MASK;
    }
}

/* @} */

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Gets the FlexIO I2C master status flags.
 *
 * @param base pointer to FLEXIO_I2C_Type structure
 * @return status flag, use status flag to AND #_flexio_i2c_master_status_flags could get the related status.
*/

uint32_t FLEXIO_I2C_MasterGetStatusFlags(FLEXIO_I2C_Type *base);

/*!
 * @brief Clears the FlexIO I2C master status flags.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param mask status flag.
 *      The parameter could be any combination of the following values:
 *          @arg kFLEXIO_I2C_RxFullFlag
 *          @arg kFLEXIO_I2C_ReceiveNakFlag
*/

void FLEXIO_I2C_MasterClearStatusFlags(FLEXIO_I2C_Type *base, uint32_t mask);

/*@}*/

/*!
 * @name Interrupts
 * @{
 */

/*!
 * @brief Enables the FlexIO i2c master interrupt requests.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param mask interrupt source.
 *     Currently only one interrupt request source:
 *     @arg kFLEXIO_I2C_TransferCompleteInterruptEnable
 */
void FLEXIO_I2C_MasterEnableInterrupts(FLEXIO_I2C_Type *base, uint32_t mask);

/*!
 * @brief Disables the FlexIO I2C master interrupt requests.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param mask interrupt source.
 */
void FLEXIO_I2C_MasterDisableInterrupts(FLEXIO_I2C_Type *base, uint32_t mask);

/*@}*/

/*!
 * @name Bus Operations
 * @{
 */

/*!
 * @brief Sets the FlexIO I2C master transfer baudrate.
 *
 * @param base pointer to FLEXIO_I2C_Type structure
 * @param baudRate_Bps the baud rate value in HZ
 * @param srcClock_Hz source clock in HZ
 */
void FLEXIO_I2C_MasterSetBaudRate(FLEXIO_I2C_Type *base, uint32_t baudRate_Bps, uint32_t srcClock_Hz);

/*!
 * @brief Sends START + 7-bit address to the bus.
 *
 * @note This is API should be called when transfer configuration is ready to send a START signal
 * and 7-bit address to the bus. This is a non-blocking API, which returns directly after the address
 * is put into the data register but not address transfer finished on the bus. Ensure that
 * the kFLEXIO_I2C_RxFullFlag status is asserted before calling this API.
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param address 7-bit address.
 * @param direction transfer direction.
 *     This parameter is one of the values in flexio_i2c_direction_t:
 *        @arg kFLEXIO_I2C_Write: Transmit
 *        @arg kFLEXIO_I2C_Read:  Receive
 */

void FLEXIO_I2C_MasterStart(FLEXIO_I2C_Type *base, uint8_t address, flexio_i2c_direction_t direction);

/*!
 * @brief Sends the stop signal on the bus.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 */
void FLEXIO_I2C_MasterStop(FLEXIO_I2C_Type *base);

/*!
 * @brief Sends the repeated start signal on the bus.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 */
void FLEXIO_I2C_MasterRepeatedStart(FLEXIO_I2C_Type *base);

/*!
 * @brief Sends the stop signal when transfer is still on-going.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 */
void FLEXIO_I2C_MasterAbortStop(FLEXIO_I2C_Type *base);

/*!
 * @brief Configures the sent ACK/NAK for the following byte.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param enable true to configure send ACK, false configure to send NAK.
 */
void FLEXIO_I2C_MasterEnableAck(FLEXIO_I2C_Type *base, bool enable);

/*!
 * @brief Sets the number of bytes to be transferred from a start signal to a stop signal.
 *
 * @note Call this API before a transfer begins because the timer generates a number of clocks according
 * to the number of bytes that need to be transferred.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param count number of bytes need to be transferred from a start signal to a re-start/stop signal
 * @retval kStatus_Success Successfully configured the count.
 * @retval kStatus_InvalidArgument Input argument is invalid.
*/
status_t FLEXIO_I2C_MasterSetTransferCount(FLEXIO_I2C_Type *base, uint8_t count);

/*!
 * @brief Writes one byte of data to the I2C bus.
 *
 * @note This is a non-blocking API, which returns directly after the data is put into the
 * data register but not data transfer finished on the bus. Ensure that
 * the TxEmptyFlag is asserted before calling this API.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param data a byte of data.
 */
static inline void FLEXIO_I2C_MasterWriteByte(FLEXIO_I2C_Type *base, uint32_t data)
{
    base->flexioBase->SHIFTBUFBBS[base->shifterIndex[0]] = data;
}

/*!
 * @brief Reads one byte of data from the I2C bus.
 *
 * @note This is a non-blocking API, which returns directly after the data is read from the
 * data register. Ensure that the data is ready in the register.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @return data byte read.
 */
static inline uint8_t FLEXIO_I2C_MasterReadByte(FLEXIO_I2C_Type *base)
{
    return base->flexioBase->SHIFTBUFBIS[base->shifterIndex[1]];
}

/*!
 * @brief Sends a buffer of data in bytes.
 *
 * @note This function blocks via polling until all bytes have been sent.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param txBuff The data bytes to send.
 * @param txSize The number of data bytes to send.
 * @retval kStatus_Success Successfully write data.
 * @retval kStatus_FLEXIO_I2C_Nak Receive NAK during writing data.
 */
status_t FLEXIO_I2C_MasterWriteBlocking(FLEXIO_I2C_Type *base, const uint8_t *txBuff, uint8_t txSize);

/*!
 * @brief Receives a buffer of bytes.
 *
 * @note This function blocks via polling until all bytes have been received.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param rxBuff The buffer to store the received bytes.
 * @param rxSize The number of data bytes to be received.
 */
void FLEXIO_I2C_MasterReadBlocking(FLEXIO_I2C_Type *base, uint8_t *rxBuff, uint8_t rxSize);

/*!
 * @brief Performs a master polling transfer on the I2C bus.
 *
 * @note The API does not return until the transfer succeeds or fails due
 * to receiving NAK.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param xfer pointer to flexio_i2c_master_transfer_t structure.
 * @return status of status_t.
 */
status_t FLEXIO_I2C_MasterTransferBlocking(FLEXIO_I2C_Type *base, flexio_i2c_master_transfer_t *xfer);
/*@}*/

/*Transactional APIs*/

/*!
 * @name Transactional
 * @{
 */

/*!
 * @brief Initializes the I2C handle which is used in transactional functions.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param handle pointer to flexio_i2c_master_handle_t structure to store the transfer state.
 * @param callback pointer to user callback function.
 * @param userData user param passed to the callback function.
 * @retval kStatus_Success Successfully create the handle.
 * @retval kStatus_OutOfRange The FlexIO type/handle/isr table out of range.
 */
status_t FLEXIO_I2C_MasterTransferCreateHandle(FLEXIO_I2C_Type *base,
                                               flexio_i2c_master_handle_t *handle,
                                               flexio_i2c_master_transfer_callback_t callback,
                                               void *userData);

/*!
 * @brief Performs a master interrupt non-blocking transfer on the I2C bus.
 *
 * @note The API returns immediately after the transfer initiates.
 * Call FLEXIO_I2C_MasterGetTransferCount to poll the transfer status to check whether
 * the transfer is finished. If the return status is not kStatus_FLEXIO_I2C_Busy, the transfer
 * is finished.
 *
 * @param base pointer to FLEXIO_I2C_Type structure
 * @param handle pointer to flexio_i2c_master_handle_t structure which stores the transfer state
 * @param xfer pointer to flexio_i2c_master_transfer_t structure
 * @retval kStatus_Success Successfully start a transfer.
 * @retval kStatus_FLEXIO_I2C_Busy FlexIO I2C is not idle, is running another transfer.
 */
status_t FLEXIO_I2C_MasterTransferNonBlocking(FLEXIO_I2C_Type *base,
                                              flexio_i2c_master_handle_t *handle,
                                              flexio_i2c_master_transfer_t *xfer);

/*!
 * @brief Gets the master transfer status during a interrupt non-blocking transfer.
 *
 * @param base pointer to FLEXIO_I2C_Type structure.
 * @param handle pointer to flexio_i2c_master_handle_t structure which stores the transfer state.
 * @param count Number of bytes transferred so far by the non-blocking transaction.
 * @retval kStatus_InvalidArgument count is Invalid.
 * @retval kStatus_Success Successfully return the count.
 */
status_t FLEXIO_I2C_MasterTransferGetCount(FLEXIO_I2C_Type *base, flexio_i2c_master_handle_t *handle, size_t *count);

/*!
 * @brief Aborts an interrupt non-blocking transfer early.
 *
 * @note This API can be called at any time when an interrupt non-blocking transfer initiates
 * to abort the transfer early.
 *
 * @param base pointer to FLEXIO_I2C_Type structure
 * @param handle pointer to flexio_i2c_master_handle_t structure which stores the transfer state
 */
void FLEXIO_I2C_MasterTransferAbort(FLEXIO_I2C_Type *base, flexio_i2c_master_handle_t *handle);

/*!
 * @brief Master interrupt handler.
 *
 * @param i2cType pointer to FLEXIO_I2C_Type structure
 * @param i2cHandle pointer to flexio_i2c_master_transfer_t structure
 */
void FLEXIO_I2C_MasterTransferHandleIRQ(void *i2cType, void *i2cHandle);

/*@}*/

#if defined(__cplusplus)
}
#endif /*_cplusplus*/
/*@}*/

#endif /*_FSL_FLEXIO_I2C_MASTER_H_*/
