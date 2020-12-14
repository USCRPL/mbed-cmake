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

#ifndef _SAMG55_I2SC_COMPONENT_
#define _SAMG55_I2SC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Inter-IC Sound Controller */
/* ============================================================================= */
/** \addtogroup SAMG55_I2SC Inter-IC Sound Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief I2sc hardware registers */
typedef struct {
    __O  uint32_t I2SC_CR;       /**< \brief (I2sc Offset: 0x00) Control Register */
    __IO uint32_t I2SC_MR;       /**< \brief (I2sc Offset: 0x04) Mode Register */
    __I  uint32_t I2SC_SR;       /**< \brief (I2sc Offset: 0x08) Status Register */
    __O  uint32_t I2SC_SCR;      /**< \brief (I2sc Offset: 0x0C) Status Clear Register */
    __O  uint32_t I2SC_SSR;      /**< \brief (I2sc Offset: 0x10) Status Set Register */
    __O  uint32_t I2SC_IER;      /**< \brief (I2sc Offset: 0x14) Interrupt Enable Register */
    __O  uint32_t I2SC_IDR;      /**< \brief (I2sc Offset: 0x18) Interrupt Disable Register */
    __I  uint32_t I2SC_IMR;      /**< \brief (I2sc Offset: 0x1C) Interrupt Mask Register */
    __I  uint32_t I2SC_RHR;      /**< \brief (I2sc Offset: 0x20) Receiver Holding Register */
    __O  uint32_t I2SC_THR;      /**< \brief (I2sc Offset: 0x24) Transmitter Holding Register */
    __I  uint32_t Reserved1[54];
    __IO uint32_t I2SC_RPR;      /**< \brief (I2sc Offset: 0x100) Receive Pointer Register */
    __IO uint32_t I2SC_RCR;      /**< \brief (I2sc Offset: 0x104) Receive Counter Register */
    __IO uint32_t I2SC_TPR;      /**< \brief (I2sc Offset: 0x108) Transmit Pointer Register */
    __IO uint32_t I2SC_TCR;      /**< \brief (I2sc Offset: 0x10C) Transmit Counter Register */
    __IO uint32_t I2SC_RNPR;     /**< \brief (I2sc Offset: 0x110) Receive Next Pointer Register */
    __IO uint32_t I2SC_RNCR;     /**< \brief (I2sc Offset: 0x114) Receive Next Counter Register */
    __IO uint32_t I2SC_TNPR;     /**< \brief (I2sc Offset: 0x118) Transmit Next Pointer Register */
    __IO uint32_t I2SC_TNCR;     /**< \brief (I2sc Offset: 0x11C) Transmit Next Counter Register */
    __O  uint32_t I2SC_PTCR;     /**< \brief (I2sc Offset: 0x120) Transfer Control Register */
    __I  uint32_t I2SC_PTSR;     /**< \brief (I2sc Offset: 0x124) Transfer Status Register */
} I2sc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- I2SC_CR : (I2SC Offset: 0x00) Control Register -------- */
#define I2SC_CR_RXEN (0x1u << 0) /**< \brief (I2SC_CR) Receiver Enable */
#define I2SC_CR_RXDIS (0x1u << 1) /**< \brief (I2SC_CR) Receiver Disable */
#define I2SC_CR_CKEN (0x1u << 2) /**< \brief (I2SC_CR) Clocks Enable */
#define I2SC_CR_CKDIS (0x1u << 3) /**< \brief (I2SC_CR) Clocks Disable */
#define I2SC_CR_TXEN (0x1u << 4) /**< \brief (I2SC_CR) Transmitter Enable */
#define I2SC_CR_TXDIS (0x1u << 5) /**< \brief (I2SC_CR) Transmitter Disable */
#define I2SC_CR_SWRST (0x1u << 7) /**< \brief (I2SC_CR) Software Reset */
/* -------- I2SC_MR : (I2SC Offset: 0x04) Mode Register -------- */
#define I2SC_MR_MODE (0x1u << 0) /**< \brief (I2SC_MR) Inter-IC Sound Controller Mode */
#define   I2SC_MR_MODE_SLAVE (0x0u << 0) /**< \brief (I2SC_MR) I2SCK and i2SWS pin inputs used as bit clock and word select/frame synchronization. */
#define   I2SC_MR_MODE_MASTER (0x1u << 0) /**< \brief (I2SC_MR) Bit clock and word select/frame synchronization generated by I2SC from MCK and output to I2SCK and I2SWS pins. MCK is output as master clock on I2SMCK if IMCKMODE bit of I2SC_MR is set. */
#define I2SC_MR_DATALENGTH_Pos 2
#define I2SC_MR_DATALENGTH_Msk (0x7u << I2SC_MR_DATALENGTH_Pos) /**< \brief (I2SC_MR) Data Word Length */
#define   I2SC_MR_DATALENGTH_32_BITS (0x0u << 2) /**< \brief (I2SC_MR) Data length is set to 32 bits */
#define   I2SC_MR_DATALENGTH_24_BITS (0x1u << 2) /**< \brief (I2SC_MR) Data length is set to 24 bits */
#define   I2SC_MR_DATALENGTH_20_BITS (0x2u << 2) /**< \brief (I2SC_MR) Data length is set to 20 bits */
#define   I2SC_MR_DATALENGTH_18_BITS (0x3u << 2) /**< \brief (I2SC_MR) Data length is set to 18 bits */
#define   I2SC_MR_DATALENGTH_16_BITS (0x4u << 2) /**< \brief (I2SC_MR) Data length is set to 16 bits */
#define   I2SC_MR_DATALENGTH_16_BITS_COMPACT (0x5u << 2) /**< \brief (I2SC_MR) Data length is set to 16-bit compact stereo. Left sample in bits 15:0 and right sample in bits 31:16 of same word. */
#define   I2SC_MR_DATALENGTH_8_BITS (0x6u << 2) /**< \brief (I2SC_MR) Data length is set to 8 bits */
#define   I2SC_MR_DATALENGTH_8_BITS_COMPACT (0x7u << 2) /**< \brief (I2SC_MR) Data length is set to 8-bit compact stereo. Left sample in bits 7:0 and right sample in bits 15:8 of the same word. */
#define I2SC_MR_RXMONO (0x1u << 8) /**< \brief (I2SC_MR) Receive Mono */
#define I2SC_MR_RXDMA (0x1u << 9) /**< \brief (I2SC_MR) Single or Multiple PDC Channels for Receiver */
#define I2SC_MR_RXLOOP (0x1u << 10) /**< \brief (I2SC_MR) Loop-back Test Mode */
#define I2SC_MR_TXMONO (0x1u << 12) /**< \brief (I2SC_MR) Transmit Mono */
#define I2SC_MR_TXDMA (0x1u << 13) /**< \brief (I2SC_MR) Single or Multiple PDC Channels for Transmitter */
#define I2SC_MR_TXSAME (0x1u << 14) /**< \brief (I2SC_MR) Transmit Data when Underrun */
#define I2SC_MR_IMCKDIV_Pos 16
#define I2SC_MR_IMCKDIV_Msk (0x3fu << I2SC_MR_IMCKDIV_Pos) /**< \brief (I2SC_MR) Peripheral Clock to I2SC Master Clock Ratio */
#define I2SC_MR_IMCKDIV(value) ((I2SC_MR_IMCKDIV_Msk & ((value) << I2SC_MR_IMCKDIV_Pos)))
#define I2SC_MR_IMCKFS_Pos 24
#define I2SC_MR_IMCKFS_Msk (0x3fu << I2SC_MR_IMCKFS_Pos) /**< \brief (I2SC_MR) Master Clock to fs Ratio */
#define   I2SC_MR_IMCKFS_M2SF16 (0x0u << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 16 */
#define   I2SC_MR_IMCKFS_M2SF32 (0x1u << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 32 */
#define   I2SC_MR_IMCKFS_M2SF48 (0x2u << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 48 */
#define   I2SC_MR_IMCKFS_M2SF64 (0x3u << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 64 */
#define   I2SC_MR_IMCKFS_M2SF96 (0x5u << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 96 */
#define   I2SC_MR_IMCKFS_M2SF128 (0x7u << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 128 */
#define   I2SC_MR_IMCKFS_M2SF192 (0xBu << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 192 */
#define   I2SC_MR_IMCKFS_M2SF256 (0xFu << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 256 */
#define   I2SC_MR_IMCKFS_M2SF384 (0x17u << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 384 */
#define   I2SC_MR_IMCKFS_M2SF512 (0x1Fu << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 512 */
#define   I2SC_MR_IMCKFS_M2SF768 (0x2Fu << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 768 */
#define   I2SC_MR_IMCKFS_M2SF1024 (0x3Fu << 24) /**< \brief (I2SC_MR) Sample frequency ratio set to 1024 */
#define I2SC_MR_IMCKMODE (0x1u << 30) /**< \brief (I2SC_MR) Master Clock Mode */
#define I2SC_MR_IWS (0x1u << 31) /**< \brief (I2SC_MR) I2SWS TDM Slot Width */
/* -------- I2SC_SR : (I2SC Offset: 0x08) Status Register -------- */
#define I2SC_SR_RXEN (0x1u << 0) /**< \brief (I2SC_SR) Receiver Enabled */
#define I2SC_SR_RXRDY (0x1u << 1) /**< \brief (I2SC_SR) Receive Ready */
#define I2SC_SR_RXOR (0x1u << 2) /**< \brief (I2SC_SR) Receive Overrun */
#define I2SC_SR_ENDRX (0x1u << 3) /**< \brief (I2SC_SR) End of Receiver Transfer */
#define I2SC_SR_TXEN (0x1u << 4) /**< \brief (I2SC_SR) Transmitter Enabled */
#define I2SC_SR_TXRDY (0x1u << 5) /**< \brief (I2SC_SR) Transmit Ready */
#define I2SC_SR_TXUR (0x1u << 6) /**< \brief (I2SC_SR) Transmit Underrun */
#define I2SC_SR_ENDTX (0x1u << 7) /**< \brief (I2SC_SR) End of Transmitter Transfer */
#define I2SC_SR_RXORCH_Pos 8
#define I2SC_SR_RXORCH_Msk (0x3u << I2SC_SR_RXORCH_Pos) /**< \brief (I2SC_SR) Receive Overrun Channel */
#define I2SC_SR_RXBUFF (0x1u << 19) /**< \brief (I2SC_SR) Receive Buffer Full */
#define I2SC_SR_TXURCH_Pos 20
#define I2SC_SR_TXURCH_Msk (0x3u << I2SC_SR_TXURCH_Pos) /**< \brief (I2SC_SR) Transmit Underrun Channel */
#define I2SC_SR_TXBUFE (0x1u << 31) /**< \brief (I2SC_SR) Transmit Buffer Empty */
/* -------- I2SC_SCR : (I2SC Offset: 0x0C) Status Clear Register -------- */
#define I2SC_SCR_RXOR (0x1u << 2) /**< \brief (I2SC_SCR) Receive Overrun Status Clear */
#define I2SC_SCR_TXUR (0x1u << 6) /**< \brief (I2SC_SCR) Transmit Underrun Status Clear */
#define I2SC_SCR_RXORCH_Pos 8
#define I2SC_SCR_RXORCH_Msk (0x3u << I2SC_SCR_RXORCH_Pos) /**< \brief (I2SC_SCR) Receive Overrun Per Channel Status Clear */
#define I2SC_SCR_RXORCH(value) ((I2SC_SCR_RXORCH_Msk & ((value) << I2SC_SCR_RXORCH_Pos)))
#define I2SC_SCR_TXURCH_Pos 20
#define I2SC_SCR_TXURCH_Msk (0x3u << I2SC_SCR_TXURCH_Pos) /**< \brief (I2SC_SCR) Transmit Underrun Per Channel Status Clear */
#define I2SC_SCR_TXURCH(value) ((I2SC_SCR_TXURCH_Msk & ((value) << I2SC_SCR_TXURCH_Pos)))
/* -------- I2SC_SSR : (I2SC Offset: 0x10) Status Set Register -------- */
#define I2SC_SSR_RXOR (0x1u << 2) /**< \brief (I2SC_SSR) Receive Overrun Status Set */
#define I2SC_SSR_TXUR (0x1u << 6) /**< \brief (I2SC_SSR) Transmit Underrun Status Set */
#define I2SC_SSR_RXORCH_Pos 8
#define I2SC_SSR_RXORCH_Msk (0x3u << I2SC_SSR_RXORCH_Pos) /**< \brief (I2SC_SSR) Receive Overrun Per Channel Status Set */
#define I2SC_SSR_RXORCH(value) ((I2SC_SSR_RXORCH_Msk & ((value) << I2SC_SSR_RXORCH_Pos)))
#define I2SC_SSR_TXURCH_Pos 20
#define I2SC_SSR_TXURCH_Msk (0x3u << I2SC_SSR_TXURCH_Pos) /**< \brief (I2SC_SSR) Transmit Underrun Per Channel Status Set */
#define I2SC_SSR_TXURCH(value) ((I2SC_SSR_TXURCH_Msk & ((value) << I2SC_SSR_TXURCH_Pos)))
/* -------- I2SC_IER : (I2SC Offset: 0x14) Interrupt Enable Register -------- */
#define I2SC_IER_RXRDY (0x1u << 1) /**< \brief (I2SC_IER) Receiver Ready Interrupt Enable */
#define I2SC_IER_RXOR (0x1u << 2) /**< \brief (I2SC_IER) Receiver Overrun Interrupt Enable */
#define I2SC_IER_ENDRX (0x1u << 3) /**< \brief (I2SC_IER) End of Reception Interrupt Enable */
#define I2SC_IER_TXRDY (0x1u << 5) /**< \brief (I2SC_IER) Transmit Ready Interrupt Enable */
#define I2SC_IER_TXUR (0x1u << 6) /**< \brief (I2SC_IER) Transmit Underflow Interrupt Enable */
#define I2SC_IER_ENDTX (0x1u << 7) /**< \brief (I2SC_IER) End of Transmission Interrupt Enable */
#define I2SC_IER_RXFULL (0x1u << 19) /**< \brief (I2SC_IER) Receive Buffer Full Interrupt Enable */
#define I2SC_IER_TXEMPTY (0x1u << 31) /**< \brief (I2SC_IER) Transmit Buffer Empty Interrupt Enable */
/* -------- I2SC_IDR : (I2SC Offset: 0x18) Interrupt Disable Register -------- */
#define I2SC_IDR_RXRDY (0x1u << 1) /**< \brief (I2SC_IDR) Receiver Ready Interrupt Disable */
#define I2SC_IDR_RXOR (0x1u << 2) /**< \brief (I2SC_IDR) Receiver Overrun Interrupt Disable */
#define I2SC_IDR_ENDRX (0x1u << 3) /**< \brief (I2SC_IDR) End of Reception Interrupt Disable */
#define I2SC_IDR_TXRDY (0x1u << 5) /**< \brief (I2SC_IDR) Transmit Ready Interrupt Disable */
#define I2SC_IDR_TXUR (0x1u << 6) /**< \brief (I2SC_IDR) Transmit Underflow Interrupt Disable */
#define I2SC_IDR_ENDTX (0x1u << 7) /**< \brief (I2SC_IDR) End of Transmission Interrupt Disable */
#define I2SC_IDR_RXFULL (0x1u << 19) /**< \brief (I2SC_IDR) Receive Buffer Full Interrupt Disable */
#define I2SC_IDR_TXEMPTY (0x1u << 31) /**< \brief (I2SC_IDR) Transmit Buffer Empty Interrupt Disable */
/* -------- I2SC_IMR : (I2SC Offset: 0x1C) Interrupt Mask Register -------- */
#define I2SC_IMR_RXRDY (0x1u << 1) /**< \brief (I2SC_IMR) Receiver Ready Interrupt Disable */
#define I2SC_IMR_RXOR (0x1u << 2) /**< \brief (I2SC_IMR) Receiver Overrun Interrupt Disable */
#define I2SC_IMR_ENDRX (0x1u << 3) /**< \brief (I2SC_IMR) End of Reception Interrupt Disable */
#define I2SC_IMR_TXRDY (0x1u << 5) /**< \brief (I2SC_IMR) Transmit Ready Interrupt Disable */
#define I2SC_IMR_TXUR (0x1u << 6) /**< \brief (I2SC_IMR) Transmit Underflow Interrupt Disable */
#define I2SC_IMR_ENDTX (0x1u << 7) /**< \brief (I2SC_IMR) End of Transmission Interrupt Disable */
#define I2SC_IMR_RXFULL (0x1u << 19) /**< \brief (I2SC_IMR) Receive Buffer Full Interrupt Disable */
#define I2SC_IMR_TXEMPTY (0x1u << 31) /**< \brief (I2SC_IMR) Transmit Buffer Empty Interrupt Disable */
/* -------- I2SC_RHR : (I2SC Offset: 0x20) Receiver Holding Register -------- */
#define I2SC_RHR_RHR_Pos 0
#define I2SC_RHR_RHR_Msk (0xffffffffu << I2SC_RHR_RHR_Pos) /**< \brief (I2SC_RHR) Receiver Holding Register */
/* -------- I2SC_THR : (I2SC Offset: 0x24) Transmitter Holding Register -------- */
#define I2SC_THR_THR_Pos 0
#define I2SC_THR_THR_Msk (0xffffffffu << I2SC_THR_THR_Pos) /**< \brief (I2SC_THR) Transmitter Holding Register */
#define I2SC_THR_THR(value) ((I2SC_THR_THR_Msk & ((value) << I2SC_THR_THR_Pos)))
/* -------- I2SC_RPR : (I2SC Offset: 0x100) Receive Pointer Register -------- */
#define I2SC_RPR_RXPTR_Pos 0
#define I2SC_RPR_RXPTR_Msk (0xffffffffu << I2SC_RPR_RXPTR_Pos) /**< \brief (I2SC_RPR) Receive Pointer Register */
#define I2SC_RPR_RXPTR(value) ((I2SC_RPR_RXPTR_Msk & ((value) << I2SC_RPR_RXPTR_Pos)))
/* -------- I2SC_RCR : (I2SC Offset: 0x104) Receive Counter Register -------- */
#define I2SC_RCR_RXCTR_Pos 0
#define I2SC_RCR_RXCTR_Msk (0xffffu << I2SC_RCR_RXCTR_Pos) /**< \brief (I2SC_RCR) Receive Counter Register */
#define I2SC_RCR_RXCTR(value) ((I2SC_RCR_RXCTR_Msk & ((value) << I2SC_RCR_RXCTR_Pos)))
/* -------- I2SC_TPR : (I2SC Offset: 0x108) Transmit Pointer Register -------- */
#define I2SC_TPR_TXPTR_Pos 0
#define I2SC_TPR_TXPTR_Msk (0xffffffffu << I2SC_TPR_TXPTR_Pos) /**< \brief (I2SC_TPR) Transmit Counter Register */
#define I2SC_TPR_TXPTR(value) ((I2SC_TPR_TXPTR_Msk & ((value) << I2SC_TPR_TXPTR_Pos)))
/* -------- I2SC_TCR : (I2SC Offset: 0x10C) Transmit Counter Register -------- */
#define I2SC_TCR_TXCTR_Pos 0
#define I2SC_TCR_TXCTR_Msk (0xffffu << I2SC_TCR_TXCTR_Pos) /**< \brief (I2SC_TCR) Transmit Counter Register */
#define I2SC_TCR_TXCTR(value) ((I2SC_TCR_TXCTR_Msk & ((value) << I2SC_TCR_TXCTR_Pos)))
/* -------- I2SC_RNPR : (I2SC Offset: 0x110) Receive Next Pointer Register -------- */
#define I2SC_RNPR_RXNPTR_Pos 0
#define I2SC_RNPR_RXNPTR_Msk (0xffffffffu << I2SC_RNPR_RXNPTR_Pos) /**< \brief (I2SC_RNPR) Receive Next Pointer */
#define I2SC_RNPR_RXNPTR(value) ((I2SC_RNPR_RXNPTR_Msk & ((value) << I2SC_RNPR_RXNPTR_Pos)))
/* -------- I2SC_RNCR : (I2SC Offset: 0x114) Receive Next Counter Register -------- */
#define I2SC_RNCR_RXNCTR_Pos 0
#define I2SC_RNCR_RXNCTR_Msk (0xffffu << I2SC_RNCR_RXNCTR_Pos) /**< \brief (I2SC_RNCR) Receive Next Counter */
#define I2SC_RNCR_RXNCTR(value) ((I2SC_RNCR_RXNCTR_Msk & ((value) << I2SC_RNCR_RXNCTR_Pos)))
/* -------- I2SC_TNPR : (I2SC Offset: 0x118) Transmit Next Pointer Register -------- */
#define I2SC_TNPR_TXNPTR_Pos 0
#define I2SC_TNPR_TXNPTR_Msk (0xffffffffu << I2SC_TNPR_TXNPTR_Pos) /**< \brief (I2SC_TNPR) Transmit Next Pointer */
#define I2SC_TNPR_TXNPTR(value) ((I2SC_TNPR_TXNPTR_Msk & ((value) << I2SC_TNPR_TXNPTR_Pos)))
/* -------- I2SC_TNCR : (I2SC Offset: 0x11C) Transmit Next Counter Register -------- */
#define I2SC_TNCR_TXNCTR_Pos 0
#define I2SC_TNCR_TXNCTR_Msk (0xffffu << I2SC_TNCR_TXNCTR_Pos) /**< \brief (I2SC_TNCR) Transmit Counter Next */
#define I2SC_TNCR_TXNCTR(value) ((I2SC_TNCR_TXNCTR_Msk & ((value) << I2SC_TNCR_TXNCTR_Pos)))
/* -------- I2SC_PTCR : (I2SC Offset: 0x120) Transfer Control Register -------- */
#define I2SC_PTCR_RXTEN (0x1u << 0) /**< \brief (I2SC_PTCR) Receiver Transfer Enable */
#define I2SC_PTCR_RXTDIS (0x1u << 1) /**< \brief (I2SC_PTCR) Receiver Transfer Disable */
#define I2SC_PTCR_TXTEN (0x1u << 8) /**< \brief (I2SC_PTCR) Transmitter Transfer Enable */
#define I2SC_PTCR_TXTDIS (0x1u << 9) /**< \brief (I2SC_PTCR) Transmitter Transfer Disable */
#define I2SC_PTCR_RXCBEN (0x1u << 16) /**< \brief (I2SC_PTCR) Receiver Circular Buffer Enable */
#define I2SC_PTCR_RXCBDIS (0x1u << 17) /**< \brief (I2SC_PTCR) Receiver Circular Buffer Disable */
#define I2SC_PTCR_TXCBEN (0x1u << 18) /**< \brief (I2SC_PTCR) Transmitter Circular Buffer Enable */
#define I2SC_PTCR_TXCBDIS (0x1u << 19) /**< \brief (I2SC_PTCR) Transmitter Circular Buffer Disable */
#define I2SC_PTCR_ERRCLR (0x1u << 24) /**< \brief (I2SC_PTCR) Transfer Bus Error Clear */
/* -------- I2SC_PTSR : (I2SC Offset: 0x124) Transfer Status Register -------- */
#define I2SC_PTSR_RXTEN (0x1u << 0) /**< \brief (I2SC_PTSR) Receiver Transfer Enable */
#define I2SC_PTSR_TXTEN (0x1u << 8) /**< \brief (I2SC_PTSR) Transmitter Transfer Enable */
#define I2SC_PTSR_RXCBEN (0x1u << 16) /**< \brief (I2SC_PTSR) Receiver Transfer Enable */
#define I2SC_PTSR_TXCBEN (0x1u << 18) /**< \brief (I2SC_PTSR) Transmitter Transfer Enable */
#define I2SC_PTSR_ERR (0x1u << 24) /**< \brief (I2SC_PTSR) Transfer Bus Error (clear on read) */

/*@}*/


#endif /* _SAMG55_I2SC_COMPONENT_ */
