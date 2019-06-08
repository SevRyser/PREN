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
#include "ftm1quad.h"


/**
 * Default handler is called if there is no handler for the FTM0 channel or tof interrupt
 */
void Default_Handler_FTM1()
{
  __asm("bkpt"); // Still a hacker? ;-)
}


void FTM1CH0_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM1")));
void FTM1CH1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM1")));
void FTM1TOF_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM1")));



#define CHF_CHIE_MASK       (FTM_CnSC_CHF_MASK | FTM_CnSC_CHIE_MASK)
#define TOF_TOIE_MASK       (FTM_SC_TOF_MASK | FTM_SC_TOIE_MASK)

/**
 * Interrupt handler to distribute the different interrupt sources of the FTM1:
 * - channel 0..7
 * - timer overflow
 */
void FTM1_IRQHandler(void)
{
  if ((FTM1_C0SC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM1CH0_IRQHandler();
  if ((FTM1_C1SC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM1CH1_IRQHandler();
  if ((FTM1_SC & TOF_TOIE_MASK) == TOF_TOIE_MASK) FTM1TOF_IRQHandler();
}

void ftm1quadInit(void)
{

  SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;

  FTM1_MODE |= FTM_MODE_FTMEN_MASK;
  FTM1_CONF |= FTM_CONF_BDMMODE(3);

  FTM1_CNTIN = 0;
  FTM1_MOD = FTM1_MODULO;
  FTM1_QDCTRL |= FTM_QDCTRL_QUADEN_MASK;

  FTM1_SC |= FTM_SC_CLKS(3) |  FTM_SC_PS(2) | FTM_SC_TOIE_MASK;


  PORTA_PCR12 = PORT_PCR_MUX(7); // FTM1 CH0
  PORTA_PCR13 = PORT_PCR_MUX(7); // FTM1 CH1



  // _todo #7.3-03 Enable FTM0 interrupt on NVIC with Prio: PRIO_FTM0 (defined in platform.h)
  NVIC_SetPriority(FTM1_IRQn, PRIO_FTM1);       // set interrupt priority
  NVIC_EnableIRQ(FTM1_IRQn);                    // enable interrupt
}
