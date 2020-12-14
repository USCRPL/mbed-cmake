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

#ifndef _SAMG55_UHP_INSTANCE_
#define _SAMG55_UHP_INSTANCE_

/* ========== Register definition for UHP peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_UHP_HCREVISION                          (0x4004C000U) /**< \brief (UHP) OHCI Revision Number Register */
#define REG_UHP_HCCONTROL                           (0x4004C004U) /**< \brief (UHP) HC Operating Mode Register */
#define REG_UHP_HCCOMMANDSTATUS                     (0x4004C008U) /**< \brief (UHP) HC Command and Status Register */
#define REG_UHP_HCINTERRUPTSTATUS                   (0x4004C00CU) /**< \brief (UHP) HC Interrupt and Status Register */
#define REG_UHP_HCINTERRUPTENABLE                   (0x4004C010U) /**< \brief (UHP) HC Interrupt Enable Register */
#define REG_UHP_HCINTERRUPTDISABLE                  (0x4004C014U) /**< \brief (UHP) HC Interrupt Disable Register */
#define REG_UHP_HCHCCA                              (0x4004C018U) /**< \brief (UHP) HC HCCA Address Register */
#define REG_UHP_HCPERIODCURRENTED                   (0x4004C01CU) /**< \brief (UHP) HC Current Periodic Register */
#define REG_UHP_HCCONTROLHEADED                     (0x4004C020U) /**< \brief (UHP) HC Head Control Register */
#define REG_UHP_HCCONTROLCURRENTED                  (0x4004C024U) /**< \brief (UHP) HC Current Control Register */
#define REG_UHP_HCBULKHEADED                        (0x4004C028U) /**< \brief (UHP) HC Head Bulk Register */
#define REG_UHP_HCBULKCURRENTED                     (0x4004C02CU) /**< \brief (UHP) HC Current Bulk Register */
#define REG_UHP_HCDONEHEAD                          (0x4004C030U) /**< \brief (UHP) HC Head Done Register */
#define REG_UHP_HCFMINTERVAL                        (0x4004C034U) /**< \brief (UHP) HC Frame Interval Register */
#define REG_UHP_HCFMREMAINING                       (0x4004C038U) /**< \brief (UHP) HC Frame Remaining Register */
#define REG_UHP_HCFMNUMBER                          (0x4004C03CU) /**< \brief (UHP) HC Frame Number Register */
#define REG_UHP_HCPERIODICSTART                     (0x4004C040U) /**< \brief (UHP) HC Periodic Start Register */
#define REG_UHP_HCLSTHRESHOLD                       (0x4004C044U) /**< \brief (UHP) HC Low-Speed Threshold Register */
#define REG_UHP_HCRHDESCRIPTORA                     (0x4004C048U) /**< \brief (UHP) HC Root Hub A Register */
#define REG_UHP_HCRHDESCRIPTORB                     (0x4004C04CU) /**< \brief (UHP) HC Root Hub B Register */
#define REG_UHP_HCRHSTATUS                          (0x4004C050U) /**< \brief (UHP) HC Root Hub Status Register */
#define REG_UHP_HCRHPORTSTATUS                      (0x4004C054U) /**< \brief (UHP) HC Port 1 Status and Control Register */
#else
#define REG_UHP_HCREVISION         (*(__I  uint32_t*)0x4004C000U) /**< \brief (UHP) OHCI Revision Number Register */
#define REG_UHP_HCCONTROL          (*(__IO uint32_t*)0x4004C004U) /**< \brief (UHP) HC Operating Mode Register */
#define REG_UHP_HCCOMMANDSTATUS    (*(__IO uint32_t*)0x4004C008U) /**< \brief (UHP) HC Command and Status Register */
#define REG_UHP_HCINTERRUPTSTATUS  (*(__IO uint32_t*)0x4004C00CU) /**< \brief (UHP) HC Interrupt and Status Register */
#define REG_UHP_HCINTERRUPTENABLE  (*(__IO uint32_t*)0x4004C010U) /**< \brief (UHP) HC Interrupt Enable Register */
#define REG_UHP_HCINTERRUPTDISABLE (*(__IO uint32_t*)0x4004C014U) /**< \brief (UHP) HC Interrupt Disable Register */
#define REG_UHP_HCHCCA             (*(__IO uint32_t*)0x4004C018U) /**< \brief (UHP) HC HCCA Address Register */
#define REG_UHP_HCPERIODCURRENTED  (*(__I  uint32_t*)0x4004C01CU) /**< \brief (UHP) HC Current Periodic Register */
#define REG_UHP_HCCONTROLHEADED    (*(__IO uint32_t*)0x4004C020U) /**< \brief (UHP) HC Head Control Register */
#define REG_UHP_HCCONTROLCURRENTED (*(__IO uint32_t*)0x4004C024U) /**< \brief (UHP) HC Current Control Register */
#define REG_UHP_HCBULKHEADED       (*(__IO uint32_t*)0x4004C028U) /**< \brief (UHP) HC Head Bulk Register */
#define REG_UHP_HCBULKCURRENTED    (*(__IO uint32_t*)0x4004C02CU) /**< \brief (UHP) HC Current Bulk Register */
#define REG_UHP_HCDONEHEAD         (*(__I  uint32_t*)0x4004C030U) /**< \brief (UHP) HC Head Done Register */
#define REG_UHP_HCFMINTERVAL       (*(__IO uint32_t*)0x4004C034U) /**< \brief (UHP) HC Frame Interval Register */
#define REG_UHP_HCFMREMAINING      (*(__I  uint32_t*)0x4004C038U) /**< \brief (UHP) HC Frame Remaining Register */
#define REG_UHP_HCFMNUMBER         (*(__I  uint32_t*)0x4004C03CU) /**< \brief (UHP) HC Frame Number Register */
#define REG_UHP_HCPERIODICSTART    (*(__IO uint32_t*)0x4004C040U) /**< \brief (UHP) HC Periodic Start Register */
#define REG_UHP_HCLSTHRESHOLD      (*(__IO uint32_t*)0x4004C044U) /**< \brief (UHP) HC Low-Speed Threshold Register */
#define REG_UHP_HCRHDESCRIPTORA    (*(__IO uint32_t*)0x4004C048U) /**< \brief (UHP) HC Root Hub A Register */
#define REG_UHP_HCRHDESCRIPTORB    (*(__IO uint32_t*)0x4004C04CU) /**< \brief (UHP) HC Root Hub B Register */
#define REG_UHP_HCRHSTATUS         (*(__IO uint32_t*)0x4004C050U) /**< \brief (UHP) HC Root Hub Status Register */
#define REG_UHP_HCRHPORTSTATUS     (*(__IO uint32_t*)0x4004C054U) /**< \brief (UHP) HC Port 1 Status and Control Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_UHP_INSTANCE_ */
