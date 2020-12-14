/**
 * \file
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _SAMG55_RTT_COMPONENT_
#define _SAMG55_RTT_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Real-time Timer */
/* ============================================================================= */
/** \addtogroup SAMG55_RTT Real-time Timer */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Rtt hardware registers */
typedef struct {
    __IO uint32_t RTT_MR;   /**< \brief (Rtt Offset: 0x00) Mode Register */
    __IO uint32_t RTT_AR;   /**< \brief (Rtt Offset: 0x04) Alarm Register */
    __I  uint32_t RTT_VR;   /**< \brief (Rtt Offset: 0x08) Value Register */
    __I  uint32_t RTT_SR;   /**< \brief (Rtt Offset: 0x0C) Status Register */
    __IO uint32_t RTT_MODR; /**< \brief (Rtt Offset: 0x10) Modulo Selection Register */
} Rtt;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- RTT_MR : (RTT Offset: 0x00) Mode Register -------- */
#define RTT_MR_RTPRES_Pos 0
#define RTT_MR_RTPRES_Msk (0xffffu << RTT_MR_RTPRES_Pos) /**< \brief (RTT_MR) Real-time Timer Prescaler Value */
#define RTT_MR_RTPRES(value) ((RTT_MR_RTPRES_Msk & ((value) << RTT_MR_RTPRES_Pos)))
#define RTT_MR_ALMIEN (0x1u << 16) /**< \brief (RTT_MR) Alarm Interrupt Enable */
#define RTT_MR_RTTINCIEN (0x1u << 17) /**< \brief (RTT_MR) Real-time Timer Increment Interrupt Enable */
#define RTT_MR_RTTRST (0x1u << 18) /**< \brief (RTT_MR) Real-time Timer Restart */
#define RTT_MR_RTTDIS (0x1u << 20) /**< \brief (RTT_MR) Real-time Timer Disable */
#define RTT_MR_INC2AEN (0x1u << 21) /**< \brief (RTT_MR) RTTINC2 Alarm Enable */
#define RTT_MR_EVAEN (0x1u << 22) /**< \brief (RTT_MR) Trigger Event Alarm Enable */
#define RTT_MR_RTC1HZ (0x1u << 24) /**< \brief (RTT_MR) Real-Time Clock 1Hz Clock Selection */
/* -------- RTT_AR : (RTT Offset: 0x04) Alarm Register -------- */
#define RTT_AR_ALMV_Pos 0
#define RTT_AR_ALMV_Msk (0xffffffffu << RTT_AR_ALMV_Pos) /**< \brief (RTT_AR) Alarm Value */
#define RTT_AR_ALMV(value) ((RTT_AR_ALMV_Msk & ((value) << RTT_AR_ALMV_Pos)))
/* -------- RTT_VR : (RTT Offset: 0x08) Value Register -------- */
#define RTT_VR_CRTV_Pos 0
#define RTT_VR_CRTV_Msk (0xffffffffu << RTT_VR_CRTV_Pos) /**< \brief (RTT_VR) Current Real-time Value */
/* -------- RTT_SR : (RTT Offset: 0x0C) Status Register -------- */
#define RTT_SR_ALMS (0x1u << 0) /**< \brief (RTT_SR) Real-time Alarm Status */
#define RTT_SR_RTTINC (0x1u << 1) /**< \brief (RTT_SR) Prescaler Roll-over Status */
#define RTT_SR_RTTINC2 (0x1u << 2) /**< \brief (RTT_SR) Predefined Number of Prescaler Roll-over Status */
/* -------- RTT_MODR : (RTT Offset: 0x10) Modulo Selection Register -------- */
#define RTT_MODR_SELINC2_Pos 0
#define RTT_MODR_SELINC2_Msk (0x7u << RTT_MODR_SELINC2_Pos) /**< \brief (RTT_MODR) Selection of the 32-bit Counter Modulo to generate RTTINC2 flag */
#define   RTT_MODR_SELINC2_NO_RTTINC2 (0x0u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag never rises */
#define   RTT_MODR_SELINC2_MOD64 (0x1u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 64 equals 0 */
#define   RTT_MODR_SELINC2_MOD128 (0x2u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 128 equals 0 */
#define   RTT_MODR_SELINC2_MOD256 (0x3u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 256 equals 0 */
#define   RTT_MODR_SELINC2_MOD512 (0x4u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 512 equals 0 */
#define   RTT_MODR_SELINC2_MOD1024 (0x5u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 1024 equals 0.Example: If RTPRES=32 then RTTINC2 flag rises once per second if the slow clock is 32.768 kHz. */
#define   RTT_MODR_SELINC2_MOD2048 (0x6u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 2048 equals 0 */
#define   RTT_MODR_SELINC2_MOD4096 (0x7u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 4096 equals 0 */
#define RTT_MODR_SELTRGEV_Pos 8
#define RTT_MODR_SELTRGEV_Msk (0x7u << RTT_MODR_SELTRGEV_Pos) /**< \brief (RTT_MODR) Selection of the 32-bit Counter Modulo to generate the trigger event */
#define   RTT_MODR_SELTRGEV_NO_EVENT (0x0u << 8) /**< \brief (RTT_MODR) No event generated */
#define   RTT_MODR_SELTRGEV_MOD2 (0x1u << 8) /**< \brief (RTT_MODR) Event occurs when CRTV modulo 2 equals 0 */
#define   RTT_MODR_SELTRGEV_MOD4 (0x2u << 8) /**< \brief (RTT_MODR) Event occurs when CRTV modulo 4 equals 0 */
#define   RTT_MODR_SELTRGEV_MOD8 (0x3u << 8) /**< \brief (RTT_MODR) Event occurs when CRTV modulo 8 equals 0 */
#define   RTT_MODR_SELTRGEV_MOD16 (0x4u << 8) /**< \brief (RTT_MODR) Event occurs when CRTV modulo 16 equals 0 */
#define   RTT_MODR_SELTRGEV_MOD32 (0x5u << 8) /**< \brief (RTT_MODR) Event occurs when CRTV modulo 32 equals 0 */
#define   RTT_MODR_SELTRGEV_MOD64 (0x6u << 8) /**< \brief (RTT_MODR) Event occurs when CRTV modulo 64 equals 0 */
#define   RTT_MODR_SELTRGEV_MOD128 (0x7u << 8) /**< \brief (RTT_MODR) Event occurs when CRTV modulo 128 equals 0 */

/*@}*/


#endif /* _SAMG55_RTT_COMPONENT_ */
