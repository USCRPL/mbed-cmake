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
 * @addtogroup nrf_timer Timer HAL and driver
 * @ingroup    nrf_drivers
 * @brief      Timer APIs.
 * @details    The timer HAL provides basic APIs for accessing the registers
 *             of the timer. The timer driver provides APIs on a higher level.
 *
 * @defgroup   lib_driver_timer Timer driver
 * @{
 * @ingroup    nrf_timer
 * @brief      Multi-instance timer driver.
 */

#ifndef NRF_DRV_TIMER_H__
#define NRF_DRV_TIMER_H__

#include "nordic_common.h"
#include "nrf_drv_config.h"
#include "nrf_timer.h"
#include "sdk_errors.h"
#include "nrf_assert.h"

/**
 * @brief Timer driver instance data structure.
 */
typedef struct
{
    NRF_TIMER_Type * p_reg;            ///< Pointer to the structure with TIMER peripheral instance registers.
    uint8_t          instance_id;      ///< Driver instance index.
    uint8_t          cc_channel_count; ///< Number of capture/compare channels.
} nrf_drv_timer_t;

/**
 * @brief Macro for creating a timer driver instance.
 */
#define NRF_DRV_TIMER_INSTANCE(id) \
{                                                             \
    .p_reg            = CONCAT_2(NRF_TIMER, id),              \
    .instance_id      = CONCAT_3(TIMER, id, _INSTANCE_INDEX), \
    .cc_channel_count = NRF_TIMER_CC_CHANNEL_COUNT(id),       \
}

/**
 * @brief Timer driver instance configuration structure.
 */
typedef struct
{
    nrf_timer_frequency_t frequency;          ///< Frequency.
    nrf_timer_mode_t      mode;               ///< Mode of operation.
    nrf_timer_bit_width_t bit_width;          ///< Bit width.
    uint8_t               interrupt_priority; ///< Interrupt priority.
    void *                p_context;          ///< Context passed to interrupt handler.
} nrf_drv_timer_config_t;

#define TIMER_CONFIG_FREQUENCY(id)    CONCAT_3(TIMER, id, _CONFIG_FREQUENCY)
#define TIMER_CONFIG_MODE(id)         CONCAT_3(TIMER, id, _CONFIG_MODE)
#define TIMER_CONFIG_BIT_WIDTH(id)    CONCAT_3(TIMER, id, _CONFIG_BIT_WIDTH)
#define TIMER_CONFIG_IRQ_PRIORITY(id) CONCAT_3(TIMER, id, _CONFIG_IRQ_PRIORITY)

/**
 * @brief Timer driver instance default configuration.
 */
#define NRF_DRV_TIMER_DEFAULT_CONFIG(id) \
{                                                                            \
    .frequency          = TIMER_CONFIG_FREQUENCY(id),                        \
    .mode               = (nrf_timer_mode_t)TIMER_CONFIG_MODE(id),           \
    .bit_width          = (nrf_timer_bit_width_t)TIMER_CONFIG_BIT_WIDTH(id), \
    .interrupt_priority = TIMER_CONFIG_IRQ_PRIORITY(id),                     \
    .p_context          = NULL                                               \
}

/**
 * @brief Timer driver event handler type.
 *
 * @param[in] event_type Timer event.
 * @param[in] p_context  General purpose parameter set during initialization of
 *                       the timer. This parameter can be used to pass 
 *                       additional information to the handler function, for 
 *                       example, the timer ID.
 */
typedef void (* nrf_timer_event_handler_t)(nrf_timer_event_t event_type,
                                           void * p_context);

/**
 * @brief Function for initializing the timer.
 *
 * @param[in] p_instance          Timer instance.
 * @param[in] p_config            Initial configuration.
 *                                If NULL, the default configuration is used.
 * @param[in] timer_event_handler Event handler provided by the user.
 *                                Must not be NULL.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 * @retval NRF_ERROR_INVALID_STATE If the instance is already initialized.
 * @retval NRF_ERROR_INVALID_PARAM If no handler was provided.
 */
ret_code_t nrf_drv_timer_init(nrf_drv_timer_t const * const p_instance,
                              nrf_drv_timer_config_t const * p_config,
                              nrf_timer_event_handler_t timer_event_handler);

/**
 * @brief Function for uninitializing the timer.
 *
 * @param[in] p_instance Timer instance.
 */
void nrf_drv_timer_uninit(nrf_drv_timer_t const * const p_instance);

/**
 * @brief Function for turning on the timer.
 *
 * @param[in] p_instance Timer instance.
 */
void nrf_drv_timer_enable(nrf_drv_timer_t const * const p_instance);

/**
 * @brief Function for turning off the timer.
 *
 * Note that the timer will allow to enter the lowest possible SYSTEM_ON state 
 * only after this function is called.
 *
 * @param[in] p_instance Timer instance.
 */
void nrf_drv_timer_disable(nrf_drv_timer_t const * const p_instance);

/**
 * @brief Function for pausing the timer.
 *
 * @param[in] p_instance Timer instance.
 */
void nrf_drv_timer_pause(nrf_drv_timer_t const * const p_instance);

/**
 * @brief Function for resuming the timer.
 *
 * @param[in] p_instance Timer instance.
 */
void nrf_drv_timer_resume(nrf_drv_timer_t const * const p_instance);

/**
 * @brief Function for clearing the timer.
 *
 * @param[in] p_instance Timer instance.
 */
void nrf_drv_timer_clear(nrf_drv_timer_t const * const p_instance);

/**
 * @brief Function for incrementing the timer.
 *
 * @param[in] p_instance Timer instance.
 */
void nrf_drv_timer_increment(nrf_drv_timer_t const * const p_instance);

/**
 * @brief Function for returning the address of a specific timer task.
 *
 * @param[in] p_instance Timer instance.
 * @param[in] timer_task Timer task.
 *
 * @return Task address.
 */
__STATIC_INLINE uint32_t nrf_drv_timer_task_address_get(
                                       nrf_drv_timer_t const * const p_instance,
                                       nrf_timer_task_t timer_task);

/**
 * @brief Function for returning the address of a specific timer capture task.
 *
 * @param[in] p_instance Timer instance.
 * @param[in] channel    Capture channel number.
 *
 * @return Task address.
 */
__STATIC_INLINE uint32_t nrf_drv_timer_capture_task_address_get(
                                       nrf_drv_timer_t const * const p_instance,
                                       uint32_t channel);

/**
 * @brief Function for returning the address of a specific timer event.
 *
 * @param[in] p_instance  Timer instance.
 * @param[in] timer_event Timer event.
 *
 * @return Event address.
 */
__STATIC_INLINE uint32_t nrf_drv_timer_event_address_get(
                                       nrf_drv_timer_t const * const p_instance,
                                       nrf_timer_event_t timer_event);

/**
 * @brief Function for returning the address of a specific timer compare event.
 *
 * @param[in] p_instance Timer instance.
 * @param[in] channel    Compare channel number.
 *
 * @return Event address.
 */
__STATIC_INLINE uint32_t nrf_drv_timer_compare_event_address_get(
                                       nrf_drv_timer_t const * const p_instance,
                                       uint32_t channel);

/**
 * @brief Function for capturing the timer value.
 *
 * @param[in] p_instance Timer instance.
 * @param[in] cc_channel Capture channel number.
 *
 * @return Captured value.
 */
uint32_t nrf_drv_timer_capture(nrf_drv_timer_t const * const p_instance,
                               nrf_timer_cc_channel_t cc_channel);

/**
 * @brief Function for returning the capture value from a specific channel.
 *
 * Use this function to read channel values when PPI is used for capturing.
 *
 * @param[in] p_instance Timer instance.
 * @param[in] cc_channel Capture channel number.
 *
 * @return Captured value.
 */
__STATIC_INLINE uint32_t nrf_drv_timer_capture_get(
                                       nrf_drv_timer_t const * const p_instance,
                                       nrf_timer_cc_channel_t cc_channel);

/**
 * @brief Function for setting the timer channel in compare mode.
 *
 * @param[in] p_instance Timer instance.
 * @param[in] cc_channel Compare channel number.
 * @param[in] cc_value   Compare value.
 * @param[in] enable_int Enable or disable the interrupt for the compare channel.
 */
void nrf_drv_timer_compare(nrf_drv_timer_t const * const p_instance,
                           nrf_timer_cc_channel_t cc_channel,
                           uint32_t               cc_value,
                           bool                   enable_int);

/**
 * @brief Function for setting the timer channel in extended compare mode.
 *
 * @param[in] p_instance       Timer instance.
 * @param[in] cc_channel       Compare channel number.
 * @param[in] cc_value         Compare value.
 * @param[in] timer_short_mask Shortcut between the compare event on the channel
 *                             and the timer task (STOP or CLEAR).
 * @param[in] enable_int       Enable or disable the interrupt for the compare
 *                             channel.
 */
void nrf_drv_timer_extended_compare(nrf_drv_timer_t const * const p_instance,
                                    nrf_timer_cc_channel_t cc_channel,
                                    uint32_t               cc_value,
                                    nrf_timer_short_mask_t timer_short_mask,
                                    bool                   enable_int);

/**
 * @brief Function for converting time in microseconds to timer ticks.
 *
 * @param[in] p_instance Timer instance.
 * @param[in] time_us    Time in microseconds.
 *
 * @return Number of ticks.
 */
__STATIC_INLINE uint32_t nrf_drv_timer_us_to_ticks(
                                       nrf_drv_timer_t const * const p_instance,
                                       uint32_t time_us);

/**
 * @brief Function for converting time in milliseconds to timer ticks.
 *
 * @param[in] p_instance Timer instance.
 * @param[in] time_ms    Time in milliseconds.
 *
 * @return Number of ticks.
 */
__STATIC_INLINE uint32_t nrf_drv_timer_ms_to_ticks(
                                       nrf_drv_timer_t const * const p_instance,
                                       uint32_t time_ms);

/**
 * @brief Function for enabling timer compare interrupt.
 *
 * @param[in] p_instance Timer instance.
 * @param[in] channel    Compare channel.
 */
void nrf_drv_timer_compare_int_enable(nrf_drv_timer_t const * const p_instance,
                                      uint32_t channel);

/**
 * @brief Function for disabling timer compare interrupt.
 *
 * @param[in] p_instance Timer instance.
 * @param[in] channel    Compare channel.
 */
void nrf_drv_timer_compare_int_disable(nrf_drv_timer_t const * const p_instance,
                                       uint32_t channel);


#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE uint32_t nrf_drv_timer_task_address_get(
                                       nrf_drv_timer_t const * const p_instance,
                                       nrf_timer_task_t timer_task)
{
    return (uint32_t)nrf_timer_task_address_get(p_instance->p_reg, timer_task);
}

__STATIC_INLINE uint32_t nrf_drv_timer_capture_task_address_get(
                                       nrf_drv_timer_t const * const p_instance,
                                       uint32_t channel)
{
    ASSERT(channel < p_instance->cc_channel_count);
    return (uint32_t)nrf_timer_task_address_get(p_instance->p_reg,
                         nrf_timer_capture_task_get(channel));
}

__STATIC_INLINE uint32_t nrf_drv_timer_event_address_get(
                                       nrf_drv_timer_t const * const p_instance,
                                       nrf_timer_event_t timer_event)
{
    return (uint32_t)nrf_timer_event_address_get(p_instance->p_reg, timer_event);
}

__STATIC_INLINE uint32_t nrf_drv_timer_compare_event_address_get(
                                       nrf_drv_timer_t const * const p_instance,
                                       uint32_t channel)
{
    ASSERT(channel < p_instance->cc_channel_count);
    return (uint32_t)nrf_timer_event_address_get(p_instance->p_reg,
                         nrf_timer_compare_event_get(channel));
}

__STATIC_INLINE uint32_t nrf_drv_timer_capture_get(
                                       nrf_drv_timer_t const * const p_instance,
                                       nrf_timer_cc_channel_t cc_channel)
{
    return nrf_timer_cc_read(p_instance->p_reg, cc_channel);
}

__STATIC_INLINE uint32_t nrf_drv_timer_us_to_ticks(
                                       nrf_drv_timer_t const * const p_instance,
                                       uint32_t timer_us)
{
    return nrf_timer_us_to_ticks(timer_us,
        nrf_timer_frequency_get(p_instance->p_reg));
}

__STATIC_INLINE uint32_t nrf_drv_timer_ms_to_ticks(
                                       nrf_drv_timer_t const * const p_instance,
                                       uint32_t timer_ms)
{
    return nrf_timer_ms_to_ticks(timer_ms,
        nrf_timer_frequency_get(p_instance->p_reg));
}

#endif // SUPPRESS_INLINE_IMPLEMENTATION

#endif // NRF_DRV_TIMER_H__

/** @} */
