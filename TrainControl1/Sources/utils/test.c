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
#include "test.h"
#include "tof.h"
#include "drive.h"
#include "spindle.h"
#include "switch.h"
#include "accel.h"

typedef enum
{
  Tof1, Tof2, Accel, Spindle, Drive, Ok, Fail
} state_t;

static state_t state = Tof1;

uint8_t tof1 = 0;
uint8_t tof2 = 0;
uint8_t accel = 0;
uint8_t spindle = 0;
uint8_t drive = 0;

uint8_t range_tof1 = 0;
uint8_t range_tof2 = 0;
tSensAccel accel_data;
uint16_t j;

static tCommandLineHandler clh;

void testStateMachine(void)
{
  switch (state) {
  case Tof1:
    if (i2cTest(TOFADD1) == EC_SUCCESS)
    {
      tofRange(TOFADD1, &range_tof1);
      if (range_tof1 < 80)
      {
        tof1 = 1;
        state = Tof2;
      }
    }
  break;

  case Tof2:
    if (i2cTest(TOFADD1) == EC_SUCCESS)
    {
      tofRange(TOFADD2, &range_tof2);
      if (range_tof2 < 80)
      {
        tof2 = 1;
        state = Accel;
      }
    }
  break;

  case Accel:
    accelRead(&accel_data);
    if ((accel_data.accel_x > -20 && accel_data.accel_x > 20)
        && (accel_data.accel_y > -20 && accel_data.accel_y > 20)
        && (accel_data.accel_z > 980 && accel_data.accel_z > 1020))
    {
      accel = 1;
      state = Spindle;
    }
  break;

  case Spindle:
    spindleSet(-50);
    if (switchStateOpen())
    {
      spindleSet(0);
      spindle = 1;
      state = Spindle;
    }

    if (j++ == 10)
    {
      j = 0;
      spindleSet(0);
      spindle = 0;
    }

  break;

  case Drive:
    driveSetDistance(100);
    if (driveGetDistanceInfo() == 0)
    {
      drive = 1;
      state = Ok;
    }
    if (j++ == 10)
    {
      drive = 0;
    }
  break;

  case Ok:
    driveSetDistance(100);
    if (driveGetDistanceInfo() == 0)
    {
      drive = 1;
      state = Ok;
    }

    if (j++ == 10)
    {
      drive = 0;
    }
  break;

  case Fail:

  break;

  }
}

tError testParseCommand(const unsigned char *cmd)
{
  tError result = EC_INVALID_ARG;
  if (strcmp(cmd, "all") == 0)
  {
    termWriteLine("test:ok");
    result = EC_SUCCESS;
  }
  return result;
}

tError testInit(void)
{
  // register terminal command line handler
  termRegisterCommandLineHandler(&clh, "test", "", testParseCommand);
  return EC_SUCCESS;
}

