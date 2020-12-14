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

#ifndef _SAMG55_USART5_INSTANCE_
#define _SAMG55_USART5_INSTANCE_

/* ========== Register definition for USART5 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_USART5_CR                      (0x40008200U) /**< \brief (USART5) USART Control Register */
#define REG_USART5_MR                      (0x40008204U) /**< \brief (USART5) USART Mode Register */
#define REG_USART5_IER                     (0x40008208U) /**< \brief (USART5) USART Interrupt Enable Register */
#define REG_USART5_IDR                     (0x4000820CU) /**< \brief (USART5) USART Interrupt Disable Register */
#define REG_USART5_IMR                     (0x40008210U) /**< \brief (USART5) USART Interrupt Mask Register */
#define REG_USART5_CSR                     (0x40008214U) /**< \brief (USART5) USART Channel Status Register */
#define REG_USART5_RHR                     (0x40008218U) /**< \brief (USART5) USART Receive Holding Register */
#define REG_USART5_THR                     (0x4000821CU) /**< \brief (USART5) USART Transmit Holding Register */
#define REG_USART5_BRGR                    (0x40008220U) /**< \brief (USART5) USART Baud Rate Generator Register */
#define REG_USART5_RTOR                    (0x40008224U) /**< \brief (USART5) USART Receiver Time-out Register */
#define REG_USART5_TTGR                    (0x40008228U) /**< \brief (USART5) USART Transmitter Timeguard Register */
#define REG_USART5_FIDI                    (0x40008240U) /**< \brief (USART5) USART FI DI Ratio Register */
#define REG_USART5_NER                     (0x40008244U) /**< \brief (USART5) USART Number of Errors Register */
#define REG_USART5_LINMR                   (0x40008254U) /**< \brief (USART5) USART LIN Mode Register */
#define REG_USART5_LINIR                   (0x40008258U) /**< \brief (USART5) USART LIN Identifier Register */
#define REG_USART5_LINBRR                  (0x4000825CU) /**< \brief (USART5) USART LIN Baud Rate Register */
#define REG_USART5_CMPR                    (0x40008290U) /**< \brief (USART5) USART Comparison Register */
#define REG_USART5_WPMR                    (0x400082E4U) /**< \brief (USART5) USART Write Protection Mode Register */
#define REG_USART5_WPSR                    (0x400082E8U) /**< \brief (USART5) USART Write Protection Status Register */
#define REG_USART5_RPR                     (0x40008300U) /**< \brief (USART5) Receive Pointer Register */
#define REG_USART5_RCR                     (0x40008304U) /**< \brief (USART5) Receive Counter Register */
#define REG_USART5_TPR                     (0x40008308U) /**< \brief (USART5) Transmit Pointer Register */
#define REG_USART5_TCR                     (0x4000830CU) /**< \brief (USART5) Transmit Counter Register */
#define REG_USART5_RNPR                    (0x40008310U) /**< \brief (USART5) Receive Next Pointer Register */
#define REG_USART5_RNCR                    (0x40008314U) /**< \brief (USART5) Receive Next Counter Register */
#define REG_USART5_TNPR                    (0x40008318U) /**< \brief (USART5) Transmit Next Pointer Register */
#define REG_USART5_TNCR                    (0x4000831CU) /**< \brief (USART5) Transmit Next Counter Register */
#define REG_USART5_PTCR                    (0x40008320U) /**< \brief (USART5) Transfer Control Register */
#define REG_USART5_PTSR                    (0x40008324U) /**< \brief (USART5) Transfer Status Register */
#else
#define REG_USART5_CR     (*(__O  uint32_t*)0x40008200U) /**< \brief (USART5) USART Control Register */
#define REG_USART5_MR     (*(__IO uint32_t*)0x40008204U) /**< \brief (USART5) USART Mode Register */
#define REG_USART5_IER    (*(__O  uint32_t*)0x40008208U) /**< \brief (USART5) USART Interrupt Enable Register */
#define REG_USART5_IDR    (*(__O  uint32_t*)0x4000820CU) /**< \brief (USART5) USART Interrupt Disable Register */
#define REG_USART5_IMR    (*(__I  uint32_t*)0x40008210U) /**< \brief (USART5) USART Interrupt Mask Register */
#define REG_USART5_CSR    (*(__I  uint32_t*)0x40008214U) /**< \brief (USART5) USART Channel Status Register */
#define REG_USART5_RHR    (*(__I  uint32_t*)0x40008218U) /**< \brief (USART5) USART Receive Holding Register */
#define REG_USART5_THR    (*(__O  uint32_t*)0x4000821CU) /**< \brief (USART5) USART Transmit Holding Register */
#define REG_USART5_BRGR   (*(__IO uint32_t*)0x40008220U) /**< \brief (USART5) USART Baud Rate Generator Register */
#define REG_USART5_RTOR   (*(__IO uint32_t*)0x40008224U) /**< \brief (USART5) USART Receiver Time-out Register */
#define REG_USART5_TTGR   (*(__IO uint32_t*)0x40008228U) /**< \brief (USART5) USART Transmitter Timeguard Register */
#define REG_USART5_FIDI   (*(__IO uint32_t*)0x40008240U) /**< \brief (USART5) USART FI DI Ratio Register */
#define REG_USART5_NER    (*(__I  uint32_t*)0x40008244U) /**< \brief (USART5) USART Number of Errors Register */
#define REG_USART5_LINMR  (*(__IO uint32_t*)0x40008254U) /**< \brief (USART5) USART LIN Mode Register */
#define REG_USART5_LINIR  (*(__IO uint32_t*)0x40008258U) /**< \brief (USART5) USART LIN Identifier Register */
#define REG_USART5_LINBRR (*(__I  uint32_t*)0x4000825CU) /**< \brief (USART5) USART LIN Baud Rate Register */
#define REG_USART5_CMPR   (*(__IO uint32_t*)0x40008290U) /**< \brief (USART5) USART Comparison Register */
#define REG_USART5_WPMR   (*(__IO uint32_t*)0x400082E4U) /**< \brief (USART5) USART Write Protection Mode Register */
#define REG_USART5_WPSR   (*(__I  uint32_t*)0x400082E8U) /**< \brief (USART5) USART Write Protection Status Register */
#define REG_USART5_RPR    (*(__IO uint32_t*)0x40008300U) /**< \brief (USART5) Receive Pointer Register */
#define REG_USART5_RCR    (*(__IO uint32_t*)0x40008304U) /**< \brief (USART5) Receive Counter Register */
#define REG_USART5_TPR    (*(__IO uint32_t*)0x40008308U) /**< \brief (USART5) Transmit Pointer Register */
#define REG_USART5_TCR    (*(__IO uint32_t*)0x4000830CU) /**< \brief (USART5) Transmit Counter Register */
#define REG_USART5_RNPR   (*(__IO uint32_t*)0x40008310U) /**< \brief (USART5) Receive Next Pointer Register */
#define REG_USART5_RNCR   (*(__IO uint32_t*)0x40008314U) /**< \brief (USART5) Receive Next Counter Register */
#define REG_USART5_TNPR   (*(__IO uint32_t*)0x40008318U) /**< \brief (USART5) Transmit Next Pointer Register */
#define REG_USART5_TNCR   (*(__IO uint32_t*)0x4000831CU) /**< \brief (USART5) Transmit Next Counter Register */
#define REG_USART5_PTCR   (*(__O  uint32_t*)0x40008320U) /**< \brief (USART5) Transfer Control Register */
#define REG_USART5_PTSR   (*(__I  uint32_t*)0x40008324U) /**< \brief (USART5) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_USART5_INSTANCE_ */
