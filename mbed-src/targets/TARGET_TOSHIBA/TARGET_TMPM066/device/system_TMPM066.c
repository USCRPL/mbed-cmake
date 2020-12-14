/**
 *******************************************************************************
 * @file    system_TMPM066.c
 * @brief   CMSIS Cortex-M0 Device Peripheral Access Layer Source File for the
 *          TOSHIBA 'TMPM066' Device Series
 * @version V2.0.2.1
 * @date    2015/10/22
 *
 * (C)Copyright TOSHIBA CORPORATION 2015 All rights reserved
 *******************************************************************************
 */

#include "TMPM066.h"

/*-------- <<< Start of configuration section >>> ----------------------------*/

/* Watchdog Timer (WD) Configuration */
#define WD_SETUP                (1U)
#define WDMOD_Val               (0x00000000UL)
#define WDCR_Val                (0x000000B1UL)

/* Clock Generator (CG) Configuration */
#define CLOCK_SETUP             (1U)
#define SYSCR_Val               (0x00000000UL)
#define OSCCR_Val               (0x00000102UL)  /* OSCCR<OSCSEL> = 1, OSCCR<EOSCEN> = 01 */

#define STBYCR_Val              (0x00000000UL)

#define CG_8M_MUL_12_FPLL        (0x00C60B00UL<<8U)

#define CG_10M_MUL_8_FPLL        (0x00C60700UL<<8U)

#define CG_12M_MUL_8_FPLL        (0x00C60700UL<<8U)

#define CG_16M_MUL_6_FPLL        (0x00C60500UL<<8U)


#define CG_PLL0SEL_PLL0ON_SET                  ((uint32_t)0x00000001)
#define CG_PLL0SEL_PLL0ON_CLEAR                ((uint32_t)0xFFFFFFFE)
#define CG_PLL0SEL_PLL0SEL_SET                 ((uint32_t)0x00000002)
#define CG_PLL0SEL_PLL0ST_MASK                 ((uint32_t)0x00000004)

#define CG_OSCCR_IOSCEN_CLEAR                  ((uint32_t)0xFFFFFFFE)
#define CG_OSCCR_EOSCEN_SET                    ((uint32_t)0x00000002)
#define CG_OSCCR_OSCSEL_SET                    ((uint32_t)0x00000100)
#define CG_OSCCR_OSCF_MASK                     ((uint32_t)0x00000200)

#define CG_WUON_START_SET                     ((uint32_t)0x00000001)
#define CG_WUEF_VALUE_MASK                    ((uint32_t)0x00000002)
#define CG_WUPHCR_WUCLK_SET                   ((uint32_t)0x00000100)

#define WD_MOD_WDTE_SET                      ((uint32_t)0x00000080)

#define PLLSEL_Ready            CG_12M_MUL_8_FPLL

#define PLLSEL_Val              (PLLSEL_Ready|0x00000003UL)
#define PLLSEL_MASK             (0xFFFFFF00UL)

/*-------- <<< End of configuration section >>> ------------------------------*/

/*-------- DEFINES -----------------------------------------------------------*/
/* Define clocks */
#define OSC_8M                  ( 8000000UL)
#define OSC_10M                 (10000000UL)
#define OSC_12M                 (12000000UL)
#define OSC_16M                 (16000000UL)
#define EXTALH                  OSC_12M /* External high-speed oscillator freq */
#define XTALH                   OSC_10M /* Internal high-speed oscillator freq */

/* Configure Warm-up time */
#define HZ_1M                  (1000000UL)
#define WU_TIME_EXT            (5000UL) /* warm-up time for EXT is 5ms   */
#define WU_TIME_PLL            (100UL)  /* warm-up time for PLL is 100us */
#define WUPHCR_WUPT_MASK        (0x000FFFFFUL)
#define WUPHCR_WUPT_EXT         ((uint32_t)(((uint64_t)WU_TIME_EXT * EXTALH / HZ_1M / 16UL) << 20U))    /* WUPHCR<WUPT11:0> = warm-up time(us) * EXTALH / 16 */
#define WUPHCR_WUPT_PLL         ((WU_TIME_PLL * EXTALH / HZ_1M /16UL) << 20U)

#if (CLOCK_SETUP)               /* Clock(external) Setup */
/* Determine core clock frequency according to settings */
/* System clock is high-speed clock*/
#if (OSCCR_Val & (1U<<8U))
#define CORE_TALH (EXTALH)
#else
#define CORE_TALH (XTALH)
#endif

#if ((PLLSEL_Val & (1U<<1U)) && (PLLSEL_Val & (1U<<0U)))      /* If PLL selected and enabled */
#if (CORE_TALH == OSC_8M)       /* If input is 8MHz */
#if ((PLLSEL_Val & PLLSEL_MASK) == (CG_8M_MUL_12_FPLL))
#define __CORE_CLK   ((CORE_TALH * 12U) / 4U) /* output clock is 24MHz */
#else                           /* fc -> reserved */
#define __CORE_CLK   (0U)
#endif                          /* End input is 8MHz */
#elif (CORE_TALH == OSC_10M)    /* If input is 10MHz */
#if ((PLLSEL_Val & PLLSEL_MASK) == (CG_10M_MUL_8_FPLL))
#define __CORE_CLK   ((CORE_TALH * 8U) / 4U)  /* output clock is 20MHz */
#else                           /* fc -> reserved */
#define __CORE_CLK   (0U)
#endif                          /* End input is 10MHz */
#elif (CORE_TALH == OSC_12M)    /* If input is 12MHz */
#if ((PLLSEL_Val & PLLSEL_MASK) == CG_12M_MUL_8_FPLL)
#define __CORE_CLK   ((CORE_TALH * 8U) / 4U)  /* output clock is 24MHz */
#else                           /* fc -> reserved */
#define __CORE_CLK   (0U)
#endif                          /* End input is 12MHz */
#elif (CORE_TALH == OSC_16M)    /* If input is 16MHz */
#if ((PLLSEL_Val & PLLSEL_MASK) == CG_16M_MUL_6_FPLL)
#define __CORE_CLK   ((CORE_TALH * 6U) / 4U)  /* output clock is 24MHz */
#else                           /* fc -> reserved        */
#define __CORE_CLK   (0U)
#endif                          /* End input is 16MHz    */
#else                           /* input clock not known */
#define __CORE_CLK   (0U)
#error "Core Oscillator Frequency invalid!"
#endif                          /* End switch input clock */
#else
#define __CORE_CLK   (CORE_TALH)
#endif

#if   ((SYSCR_Val & 7U) == 0U)  /* Gear -> fc             */
#define __CORE_SYS   (__CORE_CLK)
#elif ((SYSCR_Val & 7U) == 1U)  /* Gear -> fc/2           */
#define __CORE_SYS   (__CORE_CLK / 2U)
#elif ((SYSCR_Val & 7U) == 2U)  /* Gear -> fc/4           */
#define __CORE_SYS   (__CORE_CLK / 4U)
#elif ((SYSCR_Val & 7U) == 3U)  /* Gear -> fc/8           */
#define __CORE_SYS   (__CORE_CLK / 8U)
#elif ((SYSCR_Val & 7U) == 4U)  /* Gear -> fc/16          */
#define __CORE_SYS   (__CORE_CLK / 16U)
#else                           /* Gear -> reserved       */
#define __CORE_SYS   (0U)
#endif

#else
#define __CORE_SYS   (XTALH)

#endif                          /* clock Setup */

/* Clock Variable definitions */
uint32_t SystemCoreClock = __CORE_SYS;  /*!< System Clock Frequency (Core Clock) */


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Update SystemCoreClock according register values.
 */
void SystemCoreClockUpdate(void)
{                               /* Get Core Clock Frequency */
    uint32_t CoreClock = 0U;
    uint32_t CoreClockInput = 0U;
    uint32_t regval = 0U;
    uint32_t oscsel = 0U;
    uint32_t pllsel = 0U;
    uint32_t pllon = 0U;
    /* Determine clock frequency according to clock register values */
    /* System clock is high-speed clock */
    regval = TSB_CG->OSCCR;
    oscsel = regval & CG_OSCCR_OSCSEL_SET;
    if (oscsel) {               /* If system clock is External high-speed oscillator freq */
        CoreClock = EXTALH;
    } else {                    /* If system clock is Internal high-speed oscillator freq */
        CoreClock = XTALH;
    }
    regval = TSB_CG->PLL0SEL;
    pllsel = regval & CG_PLL0SEL_PLL0SEL_SET;
    pllon = regval & CG_PLL0SEL_PLL0ON_SET;
    if (pllsel && pllon) {      /* If PLL enabled  */
        if (CoreClock == OSC_8M) {      /* If input is 8MHz */
            if ((TSB_CG->PLL0SEL & PLLSEL_MASK) == CG_8M_MUL_12_FPLL) {
                CoreClockInput = (CoreClock * 12U) / 4U;       /* output clock is 24MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved            */
            }
        } else if (CoreClock == OSC_10M) {      /* If input is 10MHz */
            if ((TSB_CG->PLL0SEL & PLLSEL_MASK) == CG_10M_MUL_8_FPLL) {
                CoreClockInput = (CoreClock * 8U) / 4U;        /* output clock is 20MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved */
            }
        } else if (CoreClock == OSC_12M) {      /* If input is 12MHz */
            if ((TSB_CG->PLL0SEL & PLLSEL_MASK) == CG_12M_MUL_8_FPLL) {
                CoreClockInput = (CoreClock * 8U) / 4U;        /* output clock is 24MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved */
            }
        } else if (CoreClock == OSC_16M) {      /* If input is 16MHz */
            if ((TSB_CG->PLL0SEL & PLLSEL_MASK) == CG_16M_MUL_6_FPLL) {
                CoreClockInput = (CoreClock * 6U) / 4U;        /* output clock is 24MHz */
            } else {
                CoreClockInput = 0U;    /* fc -> reserved        */
            }
        } else {
            CoreClockInput = 0U;
        }
    } else {
        CoreClockInput = CoreClock;
    }
    switch (TSB_CG->SYSCR & 7U) {
    case 0U:
        SystemCoreClock = CoreClockInput;       /* Gear -> fc      */
        break;
    case 1U:                   /* Gear -> fc/2     */
        SystemCoreClock = CoreClockInput / 2U;
        break;
    case 2U:                   /* Gear -> fc/4     */
        SystemCoreClock = CoreClockInput / 4U;
        break;
    case 3U:                   /* Gear -> fc/8     */
        SystemCoreClock = CoreClockInput / 8U;
        break;
    case 4U:                   /* Gear -> fc/16     */
        if (CoreClockInput >= OSC_16M) {
            SystemCoreClock = CoreClockInput / 16U;
        } else {
            SystemCoreClock = 0U;
        }
        break;
    default:
        SystemCoreClock = 0U;
        break;
    }
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit(void)
{
    uint32_t regval = 0U;
    volatile uint32_t pllst = 0U;
    volatile uint32_t wuef = 0U;  
    volatile uint32_t oscf = 0U;
    uint32_t wdte = 0U;

#if (WD_SETUP)                  /* Watchdog Setup */
    while (TSB_WD->FLG != 0U) {
    }                /* When writing to WDMOD or WDCR, confirm "0" of WDFLG<FLG>. */
    TSB_WD->MOD = WDMOD_Val;
    regval = TSB_WD->MOD;
    wdte = regval & WD_MOD_WDTE_SET;
    if (!wdte) {     /* If watchdog is to be disabled */
        TSB_WD->CR = WDCR_Val;
    } else {
        /*Do nothing*/
    }
#endif

#if (CLOCK_SETUP)               /* Clock(external) Setup */
    TSB_CG->SYSCR = SYSCR_Val;
    TSB_CG->WUPHCR &= WUPHCR_WUPT_MASK;
    TSB_CG->WUPHCR |= WUPHCR_WUPT_EXT;    
    TSB_CG->OSCCR |= CG_OSCCR_EOSCEN_SET;    
    TSB_CG->WUPHCR |= CG_WUPHCR_WUCLK_SET;   
    TSB_CG->WUPHCR |= CG_WUON_START_SET;
    wuef = TSB_CG->WUPHCR & CG_WUEF_VALUE_MASK;
    while (wuef) {
        wuef = TSB_CG->WUPHCR & CG_WUEF_VALUE_MASK;
    }                           /* Warm-up */
    TSB_CG->OSCCR |= CG_OSCCR_OSCSEL_SET;
    oscf = TSB_CG->OSCCR & CG_OSCCR_OSCF_MASK;
    while (oscf != CG_OSCCR_OSCF_MASK) {
        oscf = TSB_CG->OSCCR & CG_OSCCR_OSCF_MASK;
    }                           /* Confirm CGOSCCR<OSCF>="1" */

    TSB_CG->WUPHCR &= WUPHCR_WUPT_MASK;
    TSB_CG->WUPHCR |= WUPHCR_WUPT_PLL;
    TSB_CG->PLL0SEL &= CG_PLL0SEL_PLL0ON_CLEAR;
    TSB_CG->PLL0SEL = PLLSEL_Ready;
    TSB_CG->WUPHCR |= CG_WUON_START_SET;
    wuef = TSB_CG->WUPHCR & CG_WUEF_VALUE_MASK;
    while (wuef) {
        wuef = TSB_CG->WUPHCR & CG_WUEF_VALUE_MASK;
    }                           /* Warm-up */

    TSB_CG->WUPHCR &= WUPHCR_WUPT_MASK;
    TSB_CG->WUPHCR |= WUPHCR_WUPT_PLL;
    TSB_CG->PLL0SEL |= CG_PLL0SEL_PLL0ON_SET;   /* PLL enabled */
    TSB_CG->STBYCR = STBYCR_Val;
    TSB_CG->WUPHCR |= CG_WUON_START_SET;
    wuef = TSB_CG->WUPHCR & CG_WUEF_VALUE_MASK;
   while (wuef) {
        wuef = TSB_CG->WUPHCR & CG_WUEF_VALUE_MASK;
   }                           /* Warm-up */
    TSB_CG->PLL0SEL |= CG_PLL0SEL_PLL0SEL_SET;
   pllst = TSB_CG->PLL0SEL & CG_PLL0SEL_PLL0ST_MASK;
   while (pllst != CG_PLL0SEL_PLL0ST_MASK) {
        pllst = TSB_CG->PLL0SEL & CG_PLL0SEL_PLL0ST_MASK;
    }                           /*Confirm CGPLLSEL<PLLST> = "1" */

#endif
}
