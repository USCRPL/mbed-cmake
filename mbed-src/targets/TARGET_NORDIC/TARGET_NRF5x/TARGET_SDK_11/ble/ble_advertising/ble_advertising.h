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
 *
 * @defgroup ble_sdk_lib_advertising Advertising Module
 * @{
 * @ingroup  ble_sdk_lib
 * @brief    Module for handling connectable BLE advertising.
 *
 * @details  The Advertising Module handles connectable advertising for your application. It can
 *           be configured with advertising modes to suit most typical use cases.
 *           Your main application can react to changes in advertising modes
 *           if an event handler is provided.
 *
 * @note     The Advertising Module supports only applications with a single peripheral link.
 *
 * The application must propagate BLE stack events to this module by calling
 * @ref ble_advertising_on_ble_evt() and system events by calling
 * @ref ble_advertising_on_sys_evt().
 *
 */

#ifndef BLE_ADVERTISING_H__
#define BLE_ADVERTISING_H__

#include <stdint.h>
#include "nrf_ble_gattc.h"
#include "nrf_ble.h"
#include "nrf_error.h"
#include "ble_advdata.h"

/**@brief Advertising modes.
*/
typedef enum
{
    BLE_ADV_MODE_IDLE,          /**< Idle; no connectable advertising is ongoing.*/
    BLE_ADV_MODE_DIRECTED,      /**< Directed advertising attempts to connect to the most recently disconnected peer. */
    BLE_ADV_MODE_DIRECTED_SLOW, /**< Directed advertising (low duty cycle) attempts to connect to the most recently disconnected peer. */
    BLE_ADV_MODE_FAST,          /**< Fast advertising will connect to any peer device, or filter with a whitelist if one exists. */
    BLE_ADV_MODE_SLOW,          /**< Slow advertising is similar to fast advertising. By default, it uses a longer advertising interval and time-out than fast advertising. However, these options are defined by the user. */
} ble_adv_mode_t;

/**@brief Advertising events.
 *
 * @details These events are propagated to the main application if a handler was provided during
 *          initialization of the Advertising Module. Events for modes that are not used can be
 *          ignored. Similarly, BLE_ADV_EVT_WHITELIST_REQUEST and BLE_ADV_EVT_PEER_ADDR_REQUEST
 *          can be ignored if whitelist and direct advertising is not used.
 */
typedef enum
{
    BLE_ADV_EVT_IDLE,                /**< Idle; no connectable advertising is ongoing.*/
    BLE_ADV_EVT_DIRECTED,            /**< Direct advertising mode has started. */
    BLE_ADV_EVT_DIRECTED_SLOW,       /**< Directed advertising (low duty cycle) has started. */
    BLE_ADV_EVT_FAST,                /**< Fast advertising mode has started. */
    BLE_ADV_EVT_SLOW,                /**< Slow advertising mode has started.*/
    BLE_ADV_EVT_FAST_WHITELIST,      /**< Fast advertising mode using the whitelist has started. */
    BLE_ADV_EVT_SLOW_WHITELIST,      /**< Slow advertising mode using the whitelist has started.*/
    BLE_ADV_EVT_WHITELIST_REQUEST,   /**< Request a whitelist from the main application. For whitelist advertising to work, the whitelist must be set when this event occurs. */
    BLE_ADV_EVT_PEER_ADDR_REQUEST    /**< Request a peer address from the main application. For directed advertising to work, the peer address must be set when this event occurs. */
} ble_adv_evt_t;

/**@brief Options for the different advertisement modes.
 *
 * @details This structure is used to enable or disable advertising modes and to configure time-out
 *          periods and advertising intervals.
 */
typedef struct
{
    bool     ble_adv_whitelist_enabled;       /**< Enable or disable use of the whitelist. */
    bool     ble_adv_directed_enabled;        /**< Enable or disable direct advertising mode. */
    bool     ble_adv_directed_slow_enabled;   /**< Enable or disable direct advertising mode. */
    uint32_t ble_adv_directed_slow_interval;  /**< Advertising interval for directed advertising. */
    uint32_t ble_adv_directed_slow_timeout;   /**< Time-out (number of tries) for direct advertising. */
    bool     ble_adv_fast_enabled;            /**< Enable or disable fast advertising mode. */
    uint32_t ble_adv_fast_interval;           /**< Advertising interval for fast advertising. */
    uint32_t ble_adv_fast_timeout;            /**< Time-out (in seconds) for fast advertising. */
    bool     ble_adv_slow_enabled;            /**< Enable or disable slow advertising mode. */
    uint32_t ble_adv_slow_interval;           /**< Advertising interval for slow advertising. */
    uint32_t ble_adv_slow_timeout;            /**< Time-out (in seconds) for slow advertising. */
}ble_adv_modes_config_t;


/**@brief BLE advertising event handler type. */
typedef void (*ble_advertising_evt_handler_t) (ble_adv_evt_t const adv_evt);

/**@brief BLE advertising error handler type. */
typedef void (*ble_advertising_error_handler_t) (uint32_t nrf_error);

/**@brief Initialization parameters for the Advertising Module.
 * @details This structure is used to pass advertising options, advertising data, and an event handler to the Advertising Module during initialization. */
typedef struct
{
    ble_adv_modes_config_t        options;     /**< Parameters for advertising modes.*/
    ble_advdata_t                 advdata;     /**< Advertising data. */
    ble_advertising_evt_handler_t evt_handler; /**< Event handler. */
}ble_adv_init_t;


/* Defines to make the mode options easier to set during advertising init.*/
#define BLE_ADV_DIRECTED_ENABLED       true
#define BLE_ADV_DIRECTED_DISABLED      false

#define BLE_ADV_DIRECTED_SLOW_ENABLED  true
#define BLE_ADV_DIRECTED_SLOW_DISABLED false

#define BLE_ADV_FAST_ENABLED           true
#define BLE_ADV_FAST_DISABLED          false

#define BLE_ADV_SLOW_ENABLED           true
#define BLE_ADV_SLOW_DISABLED          false

#define BLE_ADV_WHITELIST_ENABLED      true
#define BLE_ADV_WHITELIST_DISABLED     false


/**@brief Function for handling BLE events.
 *
 * @details This function must be called from the BLE stack event dispatcher for
 *          the module to handle BLE events that are relevant for the Advertising Module.
 *
 * @param[in] p_ble_evt BLE stack event.
 */
void ble_advertising_on_ble_evt(const ble_evt_t * const p_ble_evt);


/**@brief Function for handling system events.
 *
 * @details This function must be called to handle system events that are relevant
 *          for the Advertising Module. Specifically, the advertising module can not use the
 *          softdevice as long as there are pending writes to the flash memory. This
 *          event handler is designed to delay advertising until there is no flash operation.
 *
 * @param[in] sys_evt  System event.
 */
void ble_advertising_on_sys_evt(uint32_t sys_evt);


/**@brief Function for initializing the Advertising Module.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 *          The supplied advertising data is copied to a local structure and is manipulated
 *          depending on what advertising modes are started in @ref ble_advertising_start.
 *
 * @param[in] p_advdata     Advertising data: name, appearance, discovery flags, and more.
 * @param[in] p_srdata      Scan response data: Supplement to advertising data.
 * @param[in] p_config      Select which advertising modes and intervals will be utilized.
 * @param[in] evt_handler   Event handler that will be called upon advertising events.
 * @param[in] error_handler Error handler that will propogate internal errors to the main applications.
 *
 * @retval NRF_SUCCESS If initialization was successful. Otherwise, an error code is returned.
 */
uint32_t ble_advertising_init(ble_advdata_t const                 * p_advdata,
                              ble_advdata_t const                 * p_srdata,
                              ble_adv_modes_config_t const        * p_config,
                              ble_advertising_evt_handler_t const   evt_handler,
                              ble_advertising_error_handler_t const error_handler);


/**@brief Function for starting advertising.
 *
 * @details You can start advertising in any of the advertising modes that you enabled
 *          during initialization.
 *
 * @param[in] advertising_mode  Advertising mode.
 *
 * @retval @ref NRF_SUCCESS On success, else an error code indicating reason for failure.
 * @retval @ref NRF_ERROR_INVALID_STATE
 */
uint32_t ble_advertising_start(ble_adv_mode_t advertising_mode);

/**@brief Function for setting the peer address.
 *
 * @details The peer address must be set by the application upon receiving a
 *          @ref BLE_ADV_EVT_PEER_ADDR_REQUEST event. Without the peer address, the directed
 *          advertising mode will not be run.
 *
 * @param[in] p_peer_addr  Pointer to a peer address.
 *
 * @retval @ref NRF_SUCCESS Successfully stored the peer address pointer in the advertising module.
 * @retval @ref NRF_ERROR_INVALID_STATE If a reply was not expected.
 */
uint32_t ble_advertising_peer_addr_reply(ble_gap_addr_t * p_peer_addr);


/**@brief Function for setting a whitelist.
 *
 * @details The whitelist must be set by the application upon receiving a
 *          @ref BLE_ADV_EVT_WHITELIST_REQUEST event. Without the whitelist, the whitelist
 *          advertising for fast and slow modes will not be run.
 *
 * @param[in] p_whitelist  Pointer to a whitelist.
 *
 * @retval @ref NRF_SUCCESS Successfully stored pointers to the whitelist into the advertising module.
 * @retval @ref NRF_ERROR_INVALID_STATE If a reply was not expected.
 */
uint32_t ble_advertising_whitelist_reply(ble_gap_whitelist_t * p_whitelist);


/**@brief Function for disabling whitelist advertising.
 *
 * @details This function temporarily disables whitelist advertising.
 *          Calling this function resets the current time-out countdown.
 *
 * @retval @ref NRF_SUCCESS On success, else an error message propogated from the Softdevice.
 */
uint32_t ble_advertising_restart_without_whitelist(void);
/** @} */

#endif // BLE_ADVERTISING_H__

/** @} */
