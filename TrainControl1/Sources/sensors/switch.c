/*
 * switch.c
 *
 *  Created on: 16.05.2019
 *      Author: Blub
 */

#include "platform.h"
#include "switch.h"

uint8_t switchStateClose(void)
{
  return (GPIOE_PDIR & (1<<0));
}

uint8_t switchStateOpen(void)
{
  uint16_t result = (GPIOC_PDIR & (1<<11))>>11;

  return (uint8_t)result;
}


void switchInit(void)
{
  PORTE_PCR0 = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
  PORTC_PCR11 = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
}


