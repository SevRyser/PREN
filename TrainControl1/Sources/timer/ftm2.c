/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Common settings of the FTM0
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          26.03.20018
 *
 * $Id: ftm0.c 94 2018-04-25 11:07:33Z zajost $
 *
 *--------------------------------------------------------------------
 */

#include "platform.h"
#include "ftm2.h"


/**
 * Default handler is called if there is no handler for the FTM0 channel or tof interrupt
 */
void Default_Handler_FTM2()
{
  __asm("bkpt"); // Still a hacker? ;-)
}


void FTM2CH0_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM2")));
void FTM2CH1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM2")));



#define CHF_CHIE_MASK       (FTM_CnSC_CHF_MASK | FTM_CnSC_CHIE_MASK)
#define TOF_TOIE_MASK       (FTM_SC_TOF_MASK | FTM_SC_TOIE_MASK)

/**
 * Interrupt handler to distribute the different interrupt sources of the FTM2:
 * - channel 0..7
 * - timer overflow
 */
void FTM2_IRQHandler(void)
{
  if ((FTM2_C0SC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM2CH0_IRQHandler();
  if ((FTM2_C1SC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM2CH1_IRQHandler();
}

void ftm2Init(void)
{
  // _todo #7.3-01 set clockgating for FTM0
  SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;

  // _todo #7.3-02 configure the timer with "Fixed frequency clock" as clocksource and with a "Prescaler" of 1 => 250 kHz
  FTM2_SC = FTM_SC_CLKS(1) |  FTM_SC_PS(0);
  FTM2_MOD = FTM2_MODULO ;

  // _todo #7.3-03 Enable FTM0 interrupt on NVIC with Prio: PRIO_FTM0 (defined in platform.h)
  NVIC_SetPriority(FTM2_IRQn, PRIO_FTM2);       // set interrupt priority
  NVIC_EnableIRQ(FTM2_IRQn);                    // enable interrupt
}
