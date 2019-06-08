/*
 * tof.c
 *
 *  Created on: 13.03.2019
 *      Author: Blub
 */

#include "platform.h"
#include "ftm0.h"
#include "ftm1quad.h"
#include "i2c.h"
#include "util.h"
#include "term.h"
#include "string.h"
#include "encoder.h"

#define CheckCommand(buf, command)   (strncmp(buf, command, sizeof(command)-1) == 0)
#define TOFS_MS(x)   ((uint16_t)(((FTM0_CLOCK / 1000) * x) / (FTM0_MODULO + 1)))

static tCommandLineHandler clh;

int32_t ticks10ms, ticks100ms;

static int32_t ticks = 0;
static int32_t oldticks = 0;
static int32_t avgticks = 0;
static int32_t oldavgticks = 0;


void FTM1TOF_IRQHandler(void)       //Overflow Handler
{
  FTM1_SC &= ~FTM_SC_TOF_MASK;
  if (FTM1_QDCTRL & FTM_QDCTRL_TOFDIR_MASK)
  {
    oldticks = oldticks - FTM1_MODULO;
  }
  else
  {
    oldticks = oldticks + FTM1_MODULO;
  }

}

void FTM0CH0_IRQHandler(void)
{
  static uint8_t i;
  static int16_t sumticks;
  static uint8_t getTicks = 0;

  FTM0_C0SC &= ~FTM_CnSC_CHF_MASK;
  //clear the channel flag

  ticks = FTM1_CNT;
  ticks10ms = (-1)*(ticks - oldticks);
  oldticks = ticks;


  if (i++ == 10)
  {
    i = 0;
   avgticks = FTM1_CNT;
    ticks100ms = (-1)*(avgticks - oldavgticks);
    oldavgticks =avgticks;
  }

//set next compare time (value is stored in tmrFrequencyTicks)
  if (FTM0_CNT == FTM0_MODULO)
  {
    FTM0_C0V = FTM0CH0_VALUE;
  }
  else
  {
    FTM0_C0V = FTM0_MODULO;
  }

}

int16_t encodergetTicks()
{
  return (int16_t) ticks;
}
int16_t encodergetTicks10ms()
{
  return (int16_t) ticks10ms;
}
int16_t encodergetRPS()
{
  float result = (((ticks10ms * 100) / 512));
  return (int16_t) result;
}
int16_t encodergetRPM()
{
  float result = ((ticks10ms * 100) / 512) * (60);
  return (int16_t) result;
}
int16_t encodergetSpeedmmps()
{
  float result = (((ticks10ms * 100) / 512)) * (15 * 1000 / 35) * 70;
  float result2 = result / 1000;
  return (int16_t) (result / 1000);
}

int16_t encodergetSpeedmmps100ms()
{
  float result = (((ticks100ms * 1) / 512)) * (15 * 1000 / 35) * 70;
  float result2 = result / 1000;
  return (int16_t) (result / 1000);
}


tError encoderParseCommand(const unsigned char *cmd)
{
  tError result = EC_INVALID_ARG;

  if (strcmp(cmd, "rpm") == 0)
  {
    int16_t rpm[2];
    rpm[1] = encodergetRPS() / 1000;
    rpm[0] = encodergetRPS() - (rpm[1] * 1000);

    termWriteNum16s(rpm[1]);
    termWriteChar('.');
    termWriteNum16s(rpm[0]);
    termWriteLine("");

    result = EC_SUCCESS;
  }

  return result;
}

tError encoderInit()
{

  FTM0_C0SC = FTM_CnSC_MSx(1) | FTM_CnSC_ELSx(0)| FTM_CnSC_CHIE(1);
  //FTM0_C0SC |= FTM_CnSC_CHF(1) | FTM_CnSC_CHIE(1);
  FTM0_C0V = FTM0CH0_VALUE;

  // register terminal command line handler
  termRegisterCommandLineHandler(&clh, "speed", "", encoderParseCommand);

  return EC_SUCCESS;
}

