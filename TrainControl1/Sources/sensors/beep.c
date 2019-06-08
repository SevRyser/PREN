/*
 * beep.c
 *
 *  Created on: 09.05.2019
 *      Author: Blub
 */

#include "platform.h"
#include "string.h"
#include "term.h"
#include "util.h"
#include "ftm0.h"
#include "beep.h"

#define TOFS_MS(x)   ((uint16_t)(((FTM0_CLOCK / 1000) * x) / (FTM0_MODULO + 1)))

static tCommandLineHandler clh;
static int16_t number;

void FTM0CH1_IRQHandler(void)
{
  FTM0_C1SC &= ~FTM_CnSC_CHF_MASK;              //clear the channel flag

  static uint16_t i = 0, j = 0;
  if (i++ == TOFS_MS(150))                      //run pid worker every 25ms
  {
    i = 0;
    if (j < (2 * number))
    {
      j++;
      GPIOE_PTOR |= (1 << 1);                   // Toggle Buzzer
//    GPIOD_PTOR |= (1<<2);                     // Toggle LED
    }
    else
    {
      GPIOE_PCOR |= (1 << 1);
      FTM0_C1SC &= ~FTM_CnSC_CHIE_MASK;
      j = 0;
    }
  }
}

tError beepParseCommand(const unsigned char *cmd)
{
  tError result = EC_INVALID_ARG;
  if (strcmp(cmd, "help") == 0)
  {
    termWriteLine(" beep commands: beep:[nr]");
    result = EC_SUCCESS;
  }
  else                        //if (strncmp(cmd, "set", sizeof("set") - 1) == 0)
  {
    FTM0_C1SC |= FTM_CnSC_CHIE(1);
    //cmd += sizeof("set");
    result = utilScanDecimal16s(&cmd, &number);
    if (result != EC_SUCCESS) return result;
  }

  return result;
}

tError beepInit(void)
{
  GPIOE_PDDR |= 1 << 1; // Buzzer
  PORTE_PCR1 = PORT_PCR_MUX(1);

//  GPIOD_PDDR |= 1 << 2; // LED
//  PORTD_PCR2 = PORT_PCR_MUX(1);

  FTM0_C1SC = FTM_CnSC_MSx(1) | FTM_CnSC_ELSx(0);
  FTM0_C1SC |= FTM_CnSC_CHF(1) | FTM_CnSC_CHIE(0);

  // register terminal command line handler
  termRegisterCommandLineHandler(&clh, "beeps", "", beepParseCommand);
  return EC_SUCCESS;
}

