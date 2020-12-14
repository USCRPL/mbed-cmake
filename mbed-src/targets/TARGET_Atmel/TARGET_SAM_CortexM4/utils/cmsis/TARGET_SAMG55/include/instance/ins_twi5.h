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

#ifndef _SAMG55_TWI5_INSTANCE_
#define _SAMG55_TWI5_INSTANCE_

/* ========== Register definition for TWI5 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWI5_CR                     (0x40008600U) /**< \brief (TWI5) TWI Control Register */
#define REG_TWI5_MMR                    (0x40008604U) /**< \brief (TWI5) TWI Master Mode Register */
#define REG_TWI5_SMR                    (0x40008608U) /**< \brief (TWI5) TWI Slave Mode Register */
#define REG_TWI5_IADR                   (0x4000860CU) /**< \brief (TWI5) TWI Internal Address Register */
#define REG_TWI5_CWGR                   (0x40008610U) /**< \brief (TWI5) TWI Clock Waveform Generator Register */
#define REG_TWI5_SR                     (0x40008620U) /**< \brief (TWI5) TWI Status Register */
#define REG_TWI5_IER                    (0x40008624U) /**< \brief (TWI5) TWI Interrupt Enable Register */
#define REG_TWI5_IDR                    (0x40008628U) /**< \brief (TWI5) TWI Interrupt Disable Register */
#define REG_TWI5_IMR                    (0x4000862CU) /**< \brief (TWI5) TWI Interrupt Mask Register */
#define REG_TWI5_RHR                    (0x40008630U) /**< \brief (TWI5) TWI Receive Holding Register */
#define REG_TWI5_THR                    (0x40008634U) /**< \brief (TWI5) TWI Transmit Holding Register */
#define REG_TWI5_SMBTR                  (0x40008638U) /**< \brief (TWI5) TWI SMBus Timing Register */
#define REG_TWI5_ACR                    (0x40008640U) /**< \brief (TWI5) TWI Alternative Command Register */
#define REG_TWI5_FILTR                  (0x40008644U) /**< \brief (TWI5) TWI Filter Register */
#define REG_TWI5_SWMR                   (0x4000864CU) /**< \brief (TWI5) TWI SleepWalking Matching Register */
#define REG_TWI5_WPMR                   (0x400086E4U) /**< \brief (TWI5) TWI Write Protection Mode Register */
#define REG_TWI5_WPSR                   (0x400086E8U) /**< \brief (TWI5) TWI Write Protection Status Register */
#define REG_TWI5_RPR                    (0x40008700U) /**< \brief (TWI5) Receive Pointer Register */
#define REG_TWI5_RCR                    (0x40008704U) /**< \brief (TWI5) Receive Counter Register */
#define REG_TWI5_TPR                    (0x40008708U) /**< \brief (TWI5) Transmit Pointer Register */
#define REG_TWI5_TCR                    (0x4000870CU) /**< \brief (TWI5) Transmit Counter Register */
#define REG_TWI5_RNPR                   (0x40008710U) /**< \brief (TWI5) Receive Next Pointer Register */
#define REG_TWI5_RNCR                   (0x40008714U) /**< \brief (TWI5) Receive Next Counter Register */
#define REG_TWI5_TNPR                   (0x40008718U) /**< \brief (TWI5) Transmit Next Pointer Register */
#define REG_TWI5_TNCR                   (0x4000871CU) /**< \brief (TWI5) Transmit Next Counter Register */
#define REG_TWI5_PTCR                   (0x40008720U) /**< \brief (TWI5) Transfer Control Register */
#define REG_TWI5_PTSR                   (0x40008724U) /**< \brief (TWI5) Transfer Status Register */
#else
#define REG_TWI5_CR    (*(__O  uint32_t*)0x40008600U) /**< \brief (TWI5) TWI Control Register */
#define REG_TWI5_MMR   (*(__IO uint32_t*)0x40008604U) /**< \brief (TWI5) TWI Master Mode Register */
#define REG_TWI5_SMR   (*(__IO uint32_t*)0x40008608U) /**< \brief (TWI5) TWI Slave Mode Register */
#define REG_TWI5_IADR  (*(__IO uint32_t*)0x4000860CU) /**< \brief (TWI5) TWI Internal Address Register */
#define REG_TWI5_CWGR  (*(__IO uint32_t*)0x40008610U) /**< \brief (TWI5) TWI Clock Waveform Generator Register */
#define REG_TWI5_SR    (*(__I  uint32_t*)0x40008620U) /**< \brief (TWI5) TWI Status Register */
#define REG_TWI5_IER   (*(__O  uint32_t*)0x40008624U) /**< \brief (TWI5) TWI Interrupt Enable Register */
#define REG_TWI5_IDR   (*(__O  uint32_t*)0x40008628U) /**< \brief (TWI5) TWI Interrupt Disable Register */
#define REG_TWI5_IMR   (*(__I  uint32_t*)0x4000862CU) /**< \brief (TWI5) TWI Interrupt Mask Register */
#define REG_TWI5_RHR   (*(__I  uint32_t*)0x40008630U) /**< \brief (TWI5) TWI Receive Holding Register */
#define REG_TWI5_THR   (*(__O  uint32_t*)0x40008634U) /**< \brief (TWI5) TWI Transmit Holding Register */
#define REG_TWI5_SMBTR (*(__IO uint32_t*)0x40008638U) /**< \brief (TWI5) TWI SMBus Timing Register */
#define REG_TWI5_ACR   (*(__IO uint32_t*)0x40008640U) /**< \brief (TWI5) TWI Alternative Command Register */
#define REG_TWI5_FILTR (*(__IO uint32_t*)0x40008644U) /**< \brief (TWI5) TWI Filter Register */
#define REG_TWI5_SWMR  (*(__IO uint32_t*)0x4000864CU) /**< \brief (TWI5) TWI SleepWalking Matching Register */
#define REG_TWI5_WPMR  (*(__IO uint32_t*)0x400086E4U) /**< \brief (TWI5) TWI Write Protection Mode Register */
#define REG_TWI5_WPSR  (*(__I  uint32_t*)0x400086E8U) /**< \brief (TWI5) TWI Write Protection Status Register */
#define REG_TWI5_RPR   (*(__IO uint32_t*)0x40008700U) /**< \brief (TWI5) Receive Pointer Register */
#define REG_TWI5_RCR   (*(__IO uint32_t*)0x40008704U) /**< \brief (TWI5) Receive Counter Register */
#define REG_TWI5_TPR   (*(__IO uint32_t*)0x40008708U) /**< \brief (TWI5) Transmit Pointer Register */
#define REG_TWI5_TCR   (*(__IO uint32_t*)0x4000870CU) /**< \brief (TWI5) Transmit Counter Register */
#define REG_TWI5_RNPR  (*(__IO uint32_t*)0x40008710U) /**< \brief (TWI5) Receive Next Pointer Register */
#define REG_TWI5_RNCR  (*(__IO uint32_t*)0x40008714U) /**< \brief (TWI5) Receive Next Counter Register */
#define REG_TWI5_TNPR  (*(__IO uint32_t*)0x40008718U) /**< \brief (TWI5) Transmit Next Pointer Register */
#define REG_TWI5_TNCR  (*(__IO uint32_t*)0x4000871CU) /**< \brief (TWI5) Transmit Next Counter Register */
#define REG_TWI5_PTCR  (*(__O  uint32_t*)0x40008720U) /**< \brief (TWI5) Transfer Control Register */
#define REG_TWI5_PTSR  (*(__I  uint32_t*)0x40008724U) /**< \brief (TWI5) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_TWI5_INSTANCE_ */
