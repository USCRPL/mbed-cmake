/* 
 * Copyright (c) 2015 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
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
 * 
 */


/**@file
 * @addtogroup nrf_spi Serial peripheral interface (SPI)
 * @ingroup    nrf_drivers
 * @brief      Serial peripheral interface (SPI) APIs.
 *
 *
 * @addtogroup nrf_spi_master SPI master HAL and driver
 * @ingroup    nrf_spi
 * @brief      SPI master APIs.
 */

#ifndef NRF_DRV_SPI_H__
#define NRF_DRV_SPI_H__

#include "nordic_common.h"
#include "nrf_drv_config.h"
#include "nrf_spi.h"
#include "nrf_spim.h"
#include "sdk_errors.h"

#if defined(NRF52)
    #define NRF_DRV_SPI_PERIPHERAL(id)           \
        (CONCAT_3(SPI, id, _USE_EASY_DMA) == 1 ? \
            (void *)CONCAT_2(NRF_SPIM, id)       \
          : (void *)CONCAT_2(NRF_SPI, id))
    #define SPI2_IRQ            SPIM2_SPIS2_SPI2_IRQn
    #define SPI2_IRQ_HANDLER    SPIM2_SPIS2_SPI2_IRQHandler
#else
    #define NRF_DRV_SPI_PERIPHERAL(id)  (void *)CONCAT_2(NRF_SPI, id)
#endif
#define SPI0_IRQ            SPI0_TWI0_IRQn
#define SPI0_IRQ_HANDLER    SPI0_TWI0_IRQHandler
#define SPI1_IRQ            SPI1_TWI1_IRQn
#define SPI1_IRQ_HANDLER    SPI1_TWI1_IRQHandler


/**
 * @defgroup nrf_drv_spi_master SPI master driver
 * @{
 * @ingroup  nrf_spi_master
 *
 * @brief    Multi-instance SPI master driver.
 */

/**
 * @brief SPI master driver instance data structure.
 */
typedef struct
{
    void *    p_registers;  ///< Pointer to the structure with SPI/SPIM peripheral instance registers.
    IRQn_Type irq;          ///< SPI/SPIM peripheral instance IRQ number.
    uint8_t   drv_inst_idx; ///< Driver instance index.
    bool      use_easy_dma; ///< True if the peripheral with EasyDMA (SPIM) shall be used.
} nrf_drv_spi_t;

/**
 * @brief Macro for creating an SPI master driver instance.
 */
#define NRF_DRV_SPI_INSTANCE(id)                        \
{                                                       \
    .p_registers  = NRF_DRV_SPI_PERIPHERAL(id),         \
    .irq          = CONCAT_3(SPI, id, _IRQ),            \
    .drv_inst_idx = CONCAT_3(SPI, id, _INSTANCE_INDEX), \
    .use_easy_dma = CONCAT_3(SPI, id, _USE_EASY_DMA)    \
}

/**
 * @brief This value can be provided instead of a pin number for signals MOSI,
 *        MISO, and Slave Select to specify that the given signal is not used and
 *        therefore does not need to be connected to a pin.
 */
#define NRF_DRV_SPI_PIN_NOT_USED  0xFF

/**
 * @brief SPI data rates.
 */
typedef enum
{
    NRF_DRV_SPI_FREQ_125K = NRF_SPI_FREQ_125K, ///< 125 kbps.
    NRF_DRV_SPI_FREQ_250K = NRF_SPI_FREQ_250K, ///< 250 kbps.
    NRF_DRV_SPI_FREQ_500K = NRF_SPI_FREQ_500K, ///< 500 kbps.
    NRF_DRV_SPI_FREQ_1M   = NRF_SPI_FREQ_1M,   ///< 1 Mbps.
    NRF_DRV_SPI_FREQ_2M   = NRF_SPI_FREQ_2M,   ///< 2 Mbps.
    NRF_DRV_SPI_FREQ_4M   = NRF_SPI_FREQ_4M,   ///< 4 Mbps.
    NRF_DRV_SPI_FREQ_8M   = NRF_SPI_FREQ_8M    ///< 8 Mbps.
} nrf_drv_spi_frequency_t;

/**
 * @brief SPI modes.
 */
typedef enum
{
    NRF_DRV_SPI_MODE_0 = NRF_SPI_MODE_0, ///< SCK active high, sample on leading edge of clock.
    NRF_DRV_SPI_MODE_1 = NRF_SPI_MODE_1, ///< SCK active high, sample on trailing edge of clock.
    NRF_DRV_SPI_MODE_2 = NRF_SPI_MODE_2, ///< SCK active low, sample on leading edge of clock.
    NRF_DRV_SPI_MODE_3 = NRF_SPI_MODE_3  ///< SCK active low, sample on trailing edge of clock.
} nrf_drv_spi_mode_t;

/**
 * @brief SPI bit orders.
 */
typedef enum
{
    NRF_DRV_SPI_BIT_ORDER_MSB_FIRST = NRF_SPI_BIT_ORDER_MSB_FIRST, ///< Most significant bit shifted out first.
    NRF_DRV_SPI_BIT_ORDER_LSB_FIRST = NRF_SPI_BIT_ORDER_LSB_FIRST  ///< Least significant bit shifted out first.
} nrf_drv_spi_bit_order_t;

/**
 * @brief SPI master driver instance configuration structure.
 */
typedef struct
{
    uint8_t sck_pin;      ///< SCK pin number.
    uint8_t mosi_pin;     ///< MOSI pin number (optional).
                          /**< Set to @ref NRF_DRV_SPI_PIN_NOT_USED
                           *   if this signal is not needed. */
    uint8_t miso_pin;     ///< MISO pin number (optional).
                          /**< Set to @ref NRF_DRV_SPI_PIN_NOT_USED
                           *   if this signal is not needed. */
    uint8_t ss_pin;       ///< Slave Select pin number (optional).
                          /**< Set to @ref NRF_DRV_SPI_PIN_NOT_USED
                           *   if this signal is not needed. The driver 
                           *   supports only active low for this signal. 
                           *   If the signal should be active high,
                           *   it must be controlled externally. */
    uint8_t irq_priority; ///< Interrupt priority.
    uint8_t orc;          ///< Over-run character.
                          /**< This character is used when all bytes from the TX buffer are sent,
                               but the transfer continues due to RX. */
    nrf_drv_spi_frequency_t frequency; ///< SPI frequency.
    nrf_drv_spi_mode_t      mode;      ///< SPI mode.
    nrf_drv_spi_bit_order_t bit_order; ///< SPI bit order.
} nrf_drv_spi_config_t;

/**
 * @brief SPI master instance default configuration.
 */
#define NRF_DRV_SPI_DEFAULT_CONFIG(id)                       \
{                                                            \
    .sck_pin      = CONCAT_3(SPI, id, _CONFIG_SCK_PIN),      \
    .mosi_pin     = CONCAT_3(SPI, id, _CONFIG_MOSI_PIN),     \
    .miso_pin     = CONCAT_3(SPI, id, _CONFIG_MISO_PIN),     \
    .ss_pin       = NRF_DRV_SPI_PIN_NOT_USED,                \
    .irq_priority = CONCAT_3(SPI, id, _CONFIG_IRQ_PRIORITY), \
    .orc          = 0xFF,                                    \
    .frequency    = NRF_DRV_SPI_FREQ_4M,                     \
    .mode         = NRF_DRV_SPI_MODE_0,                      \
    .bit_order    = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST,         \
}

#define NRF_DRV_SPI_FLAG_TX_POSTINC          (1UL << 0) /**< TX buffer address incremented after transfer. */
#define NRF_DRV_SPI_FLAG_RX_POSTINC          (1UL << 1) /**< RX buffer address incremented after transfer. */
#define NRF_DRV_SPI_FLAG_NO_XFER_EVT_HANDLER (1UL << 2) /**< Interrupt after each transfer is suppressed, and the event handler is not called. */
#define NRF_DRV_SPI_FLAG_HOLD_XFER           (1UL << 3) /**< Set up the transfer but do not start it. */
#define NRF_DRV_SPI_FLAG_REPEATED_XFER       (1UL << 4) /**< Flag indicating that the transfer will be executed multiple times. */

/**
 * @brief Single transfer descriptor structure.
 */
typedef struct
{
    uint8_t const * p_tx_buffer; ///< Pointer to TX buffer.
    uint8_t         tx_length;   ///< TX buffer length.
    uint8_t       * p_rx_buffer; ///< Pointer to RX buffer.
    uint8_t         rx_length;   ///< RX buffer length.
}nrf_drv_spi_xfer_desc_t;


/**
 * @brief Macro for setting up single transfer descriptor.
 *
 * This macro is for internal use only.
 */
#define NRF_DRV_SPI_SINGLE_XFER(p_tx, tx_len, p_rx, rx_len)  \
    {                                                        \
    .p_tx_buffer = (uint8_t const *)(p_tx),                  \
    .tx_length = (tx_len),                                   \
    .p_rx_buffer = (p_rx),                                   \
    .rx_length = (rx_len),                                   \
    }

/**
 * @brief Macro for setting duplex TX RX transfer.
 */
#define NRF_DRV_SPI_XFER_TRX(p_tx_buf, tx_length, p_rx_buf, rx_length)                    \
        NRF_DRV_SPI_SINGLE_XFER(p_tx_buf, tx_length, p_rx_buf, rx_length)

/**
 * @brief Macro for setting TX transfer.
 */
#define NRF_DRV_SPI_XFER_TX(p_buf, length) \
        NRF_DRV_SPI_SINGLE_XFER(p_buf, length, NULL, 0)

/**
 * @brief Macro for setting RX transfer.
 */
#define NRF_DRV_SPI_XFER_RX(p_buf, length) \
        NRF_DRV_SPI_SINGLE_XFER(NULL, 0, p_buf, length)

/**
 * @brief SPI master driver event types, passed to the handler routine provided
 *        during initialization.
 */
typedef enum
{
    NRF_DRV_SPI_EVENT_DONE, ///< Transfer done.
} nrf_drv_spi_evt_type_t;

typedef struct
{
    nrf_drv_spi_evt_type_t  type;      ///< Event type.
    union
    {
        nrf_drv_spi_xfer_desc_t done;  ///< Event data for DONE event.
    } data;
} nrf_drv_spi_evt_t;

/**
 * @brief SPI master driver event handler type.
 */
typedef void (*nrf_drv_spi_handler_t)(nrf_drv_spi_evt_t const * p_event);


/**
 * @brief Function for initializing the SPI master driver instance.
 *
 * This function configures and enables the specified peripheral.
 *
 * @param[in] p_instance Pointer to the instance structure.
 * @param[in] p_config   Pointer to the structure with the initial configuration.
 *                       If NULL, the default configuration is used.
 * @param     handler    Event handler provided by the user. If NULL, transfers
 *                       will be performed in blocking mode.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 * @retval NRF_ERROR_INVALID_STATE If the driver was already initialized.
 * @retval NRF_ERROR_BUSY          If some other peripheral with the same
 *                                 instance ID is already in use. This is 
 *                                 possible only if PERIPHERAL_RESOURCE_SHARING_ENABLED 
 *                                 is set to a value other than zero.
 */
ret_code_t nrf_drv_spi_init(nrf_drv_spi_t const * const p_instance,
                            nrf_drv_spi_config_t const * p_config,
                            nrf_drv_spi_handler_t handler);

/**
 * @brief Function for uninitializing the SPI master driver instance.
 *
 * @param[in] p_instance Pointer to the instance structure.
 */
void       nrf_drv_spi_uninit(nrf_drv_spi_t const * const p_instance);

/**
 * @brief Function for starting the SPI data transfer.
 *
 * If an event handler was provided in the @ref nrf_drv_spi_init call, this function
 * returns immediately and the handler is called when the transfer is done.
 * Otherwise, the transfer is performed in blocking mode, which means that this function
 * returns when the transfer is finished.
 *
 * @note Peripherals using EasyDMA (for example, SPIM) require the transfer buffers
 *       to be placed in the Data RAM region. If they are not and an SPIM instance is
 *       used, this function will fail with the error code NRF_ERROR_INVALID_ADDR.
 *
 * @param[in] p_instance       Pointer to the instance structure.
 * @param[in] p_tx_buffer      Pointer to the transmit buffer. Can be NULL
 *                             if there is nothing to send.
 * @param     tx_buffer_length Length of the transmit buffer.
 * @param[in] p_rx_buffer      Pointer to the receive buffer. Can be NULL
 *                             if there is nothing to receive.
 * @param     rx_buffer_length Length of the receive buffer.
 *
 * @retval NRF_SUCCESS            If the operation was successful.
 * @retval NRF_ERROR_BUSY         If a previously started transfer has not finished
 *                                yet.
 * @retval NRF_ERROR_INVALID_ADDR If the provided buffers are not placed in the Data
 *                                RAM region.
 */
ret_code_t nrf_drv_spi_transfer(nrf_drv_spi_t const * const p_instance,
                                uint8_t const * p_tx_buffer,
                                uint8_t         tx_buffer_length,
                                uint8_t       * p_rx_buffer,
                                uint8_t         rx_buffer_length);


/**
 * @brief Function for starting the SPI data transfer with additional option flags.
 *
 * Function enables customizing the transfer by using option flags.
 *
 * Additional options are provided using the flags parameter:
 *
 * - @ref NRF_DRV_SPI_FLAG_TX_POSTINC and @ref NRF_DRV_SPI_FLAG_RX_POSTINC<span></span>:
 *   Post-incrementation of buffer addresses. Supported only by SPIM.
 * - @ref NRF_DRV_SPI_FLAG_HOLD_XFER<span></span>: Driver is not starting the transfer. Use this
 *   flag if the transfer is triggered externally by PPI. Supported only by SPIM. Use
 *   @ref nrf_drv_twi_start_task_get to get the address of the start task.
 * - @ref NRF_DRV_SPI_FLAG_NO_XFER_EVT_HANDLER<span></span>: No user event handler after transfer
 *   completion. This also means no interrupt at the end of the transfer. Supported only by SPIM.
 *   If @ref NRF_DRV_SPI_FLAG_NO_XFER_EVT_HANDLER is used, the driver does not set the instance into
 *   busy state, so you must ensure that the next transfers are set up when SPIM is not active.
 *   @ref nrf_drv_spi_end_event_get function can be used to detect end of transfer. Option can be used
 *   together with @ref NRF_DRV_SPI_FLAG_REPEATED_XFER to prepare a sequence of SPI transfers
 *   without interruptions.
 * - @ref NRF_DRV_SPI_FLAG_REPEATED_XFER<span></span>: Prepare for repeated transfers. You can set
 *   up a number of transfers that will be triggered externally (for example by PPI). An example is
 *   a TXRX transfer with the options @ref NRF_DRV_SPI_FLAG_RX_POSTINC,
 *   @ref NRF_DRV_SPI_FLAG_NO_XFER_EVT_HANDLER, and @ref NRF_DRV_SPI_FLAG_REPEATED_XFER. After the
 *   transfer is set up, a set of transfers can be triggered by PPI that will read, for example,
 *   the same register of an external component and put it into a RAM buffer without any interrupts.
 *   @ref nrf_drv_spi_end_event_get can be used to get the address of the END event, which can be
 *   used to count the number of transfers. If @ref NRF_DRV_SPI_FLAG_REPEATED_XFER is used,
 *   the driver does not set the instance into busy state, so you must ensure that the next
 *   transfers are set up when SPIM is not active. Supported only by SPIM.
 * @note Function is intended to be used only in non-blocking mode.
 *
 * @param p_instance  SPI instance.
 * @param p_xfer_desc Pointer to the transfer descriptor.
 * @param flags       Transfer options (0 for default settings).
 *
 * @retval NRF_SUCCESS             If the procedure was successful.
 * @retval NRF_ERROR_BUSY          If the driver is not ready for a new transfer.
 * @retval NRF_ERROR_NOT_SUPPORTED If the provided parameters are not supported.
 * @retval NRF_ERROR_INVALID_ADDR  If the provided buffers are not placed in the Data
 *                                 RAM region.
 */
ret_code_t nrf_drv_spi_xfer(nrf_drv_spi_t     const * const p_instance,
                            nrf_drv_spi_xfer_desc_t const * p_xfer_desc,
                            uint32_t                        flags);

/**
 * @brief Function for returning the address of a SPIM start task.
 *
 * This function should be used if @ref nrf_drv_spi_xfer was called with the flag @ref NRF_DRV_SPI_FLAG_HOLD_XFER.
 * In that case, the transfer is not started by the driver, but it must be started externally by PPI.
 *
 * @param[in]  p_instance SPI instance.
 *
 * @return     Start task address.
 */
uint32_t nrf_drv_spi_start_task_get(nrf_drv_spi_t const * p_instance);

/**
 * @brief Function for returning the address of a END SPIM event.
 *
 * A END event can be used to detect the end of a transfer if the @ref NRF_DRV_SPI_FLAG_NO_XFER_EVT_HANDLER
 * option is used.
 *
 * @param[in]  p_instance  SPI instance.
 *
 * @return     END event address.
 */
uint32_t nrf_drv_spi_end_event_get(nrf_drv_spi_t const * p_instance);

void nrf_drv_spi_abort(nrf_drv_spi_t const * p_instance);

#endif // NRF_DRV_SPI_H__

/** @} */
