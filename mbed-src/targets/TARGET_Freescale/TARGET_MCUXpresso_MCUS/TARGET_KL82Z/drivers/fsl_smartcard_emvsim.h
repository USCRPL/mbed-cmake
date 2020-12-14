/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
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

#ifndef _FSL_SMARTCARD_EMVSIM_H_
#define _FSL_SMARTCARD_EMVSIM_H_

#include "fsl_smartcard.h"

/*!
 * @addtogroup smartcard_emvsim_driver
 * @{
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief EMV RX NACK interrupt generation threshold */
#define SMARTCARD_EMV_RX_NACK_THRESHOLD (5u)

/*! @brief EMV TX NACK interrupt generation threshold */
#define SMARTCARD_EMV_TX_NACK_THRESHOLD (5u)

/*! @brief Smart card Word Wait Timer adjustment value */
#define SMARTCARD_WWT_ADJUSTMENT (160u)

/*! @brief Smart card Character Wait Timer adjustment value */
#define SMARTCARD_CWT_ADJUSTMENT (3u)

/*! @brief General Purpose Counter clock selections */
typedef enum _emvsim_gpc_clock_select
{
    kEMVSIM_GPCClockDisable = 0u, /*!< disabled */
    kEMVSIM_GPCCardClock = 1u,    /*!< card clock */
    kEMVSIM_GPCRxClock = 2u,      /*!< receive clock */
    kEMVSIM_GPCTxClock = 3u,      /*!< transmit ETU clock */
} emvsim_gpc_clock_select_t;

/*! @brief EMVSIM card presence detection edge control */
typedef enum _presence_detect_edge
{
    kEMVSIM_DetectOnFallingEdge = 0u, /*!< presence detect on falling edge */
    kEMVSIM_DetectOnRisingEdge = 1u,  /*!< presence detect on rising edge */
} emvsim_presence_detect_edge_t;

/*! @brief EMVSIM card presence detection status */
typedef enum _presence_detect_status
{
    kEMVSIM_DetectPinIsLow = 0u,  /*!< presence detect pin is logic low */
    kEMVSIM_DetectPinIsHigh = 1u, /*!< presence detect pin is logic high */
} emvsim_presence_detect_status_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Smart card EMVSIM Driver
 * @{
 */

/*!
 * @brief Fill in smartcard_card_params structure with default values according EMV 4.3 specification.
 *
 * @param cardParams The configuration structure of type smartcard_interface_config_t.
 * Function fill in members:
 *        Fi = 372;
 *        Di = 1;
 *        currentD = 1;
 *        WI = 0x0A;
 *        GTN = 0x00;
 * with default values.
 */
void SMARTCARD_EMVSIM_GetDefaultConfig(smartcard_card_params_t *cardParams);

/*!
 * @brief Initializes an EMVSIM peripheral for smart card/ISO-7816 operation.
 *
 * This function Un-gate EMVSIM clock, initializes the module to EMV default settings,
 * configures the IRQ, enables the module-level interrupt to the core and initialize driver context.
 *
 * @param base The EMVSIM peripheral base address.
 * @param context A pointer to a smart card driver context structure.
 * @param srcClock_Hz Smart card clock generation module source clock.
 *
 * @return An error code or kStatus_SMARTCARD_Success.
 */
status_t SMARTCARD_EMVSIM_Init(EMVSIM_Type *base, smartcard_context_t *context, uint32_t srcClock_Hz);

/*!
 * @brief This function disables the EMVSIM interrupts, disables the transmitter and receiver,
 * flushes the FIFOs and gates EMVSIM clock in SIM.
 *
 * @param base The EMVSIM module base address.
 */
void SMARTCARD_EMVSIM_Deinit(EMVSIM_Type *base);

/*!
 * @brief Returns whether the previous EMVSIM transfer has finished.
 *
 * When performing an async transfer, call this function to ascertain the context of the
 * current transfer: in progress (or busy) or complete (success). If the
 * transfer is still in progress, the user can obtain the number of words that have not been
 * transferred.
 *
 * @param base The EMVSIM module base address.
 * @param context A pointer to a smart card driver context structure.
 *
 * @return The number of bytes not transferred.
 */
int32_t SMARTCARD_EMVSIM_GetTransferRemainingBytes(EMVSIM_Type *base, smartcard_context_t *context);

/*!
 * @brief Terminates an asynchronous EMVSIM transfer early.
 *
 * During an async EMVSIM transfer, the user can terminate the transfer early
 * if the transfer is still in progress.
 *
 * @param base The EMVSIM peripheral address.
 * @param context A pointer to a smart card driver context structure.
 * @retval kStatus_SMARTCARD_Success The transmit abort was successful.
 * @retval kStatus_SMARTCARD_NoTransmitInProgress No transmission is currently in progress.
 */
status_t SMARTCARD_EMVSIM_AbortTransfer(EMVSIM_Type *base, smartcard_context_t *context);

/*!
 * @brief Transfer data using interrupts.
 *
 * A non-blocking (also known as asynchronous) function means that the function returns
 * immediately after initiating the transfer function. The application has to get the
 * transfer status to see when the transfer is complete. In other words, after calling non-blocking
 * (asynchronous) transfer function, the application must get the transfer status to check if transmit
 * is completed or not.
 *
 * @param base The EMVSIM peripheral base address.
 * @param context A pointer to a smart card driver context structure.
 * @param xfer A pointer to smart card transfer structure where are linked buffers and sizes.
 *
 * @return An error code or kStatus_SMARTCARD_Success.
 */
status_t SMARTCARD_EMVSIM_TransferNonBlocking(EMVSIM_Type *base, smartcard_context_t *context, smartcard_xfer_t *xfer);

/*!
 * @brief Controls EMVSIM module as per different user request.
 *
 * @param base The EMVSIM peripheral base address.
 * @param context A pointer to a smart card driver context structure.
 * @param control Control type
 * @param param Integer value of specific to control command.
 *
 * return kStatus_SMARTCARD_Success in success.
 * return kStatus_SMARTCARD_OtherError in case of error.
 */
status_t SMARTCARD_EMVSIM_Control(EMVSIM_Type *base,
                                  smartcard_context_t *context,
                                  smartcard_control_t control,
                                  uint32_t param);

/*!
 * @brief Handles EMVSIM module interrupts.
 *
 * @param base The EMVSIM peripheral base address.
 * @param context A pointer to a smart card driver context structure.
 */
void SMARTCARD_EMVSIM_IRQHandler(EMVSIM_Type *base, smartcard_context_t *context);
/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_SMARTCARD_EMVSIM_H_*/
