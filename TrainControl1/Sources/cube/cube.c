/*
 * cube.c
 *
 *  Created on: 16.05.2019
 *      Author: Blub
 */

#include "platform.h"
#include "term.h"
#include "util.h"
#include "string.h"
#include "cube.h"
#include "tof.h"
#include "drive.h"
#include "encoder.h"
#include "spindle.h"
#include "servo.h"
#include "accel.h"
#include "switch.h"

#define TOFS_MS(x)   ((uint16_t)(x/25 ))

static tCommandLineHandler clh;       // terminal command line handler

typedef enum
{
  Start,
  DriveToCube,
  DriveDistance,
  MeassureCube,
  LoadDown,
  ControlGyro,
  DriveBack,
  CloseSpindle,
  OpenSpindle,
  LoadUp,
  SendStatusOk,
  SendStatusFail

} state_t;

static state_t state = Start;
static uint8_t startPi = 0;
uint8_t distance = 0;
uint8_t distance1 = 0;
uint8_t accelXStart = 0;
uint8_t fail = 0;
uint16_t closeTime = 15000;

tSensAccel accel_data;

void cubeStateMachine(void)
{
  static uint16_t i, j, k;

  if (fail > 1)
  {
    state = SendStatusFail;
  }

  switch (state) {

  case Start:
    if (startPi)
    {
      state = DriveToCube;
    }
  break;

  case DriveToCube:
    driveSetDistance(2000);
    if (driveGetDistanceInfo() == 0)
    {
      state = SendStatusFail;
    }
    tofRange(TOFADD1, &distance);
    if (distance < 80)
    {
      driveSetDistance(194);
      state = DriveDistance;
    }

  break;

  case DriveDistance:
    if (driveGetDistanceInfo() == 0)
    {
      state = LoadDown;
      i = 0;
    }
  break;

  case LoadDown:
    //Wait 500ms
    tofRange(TOFADD2, &distance);

      if (i++ == TOFS_MS(400))
      {
        i = 0;
        if (distance < 100)
        {
          state = ControlGyro;
          //accelResetData();
          k = SERVO_LOAD_MIDDLE;
          //accelResetData();
          accelRead(&accel_data);
          accelXStart = accel_data.accel_x;
        }
        else
        {
          state = DriveBack;
          driveSetDistance(-300);
        }


    }
  break;



  case ControlGyro:

    servoSet(k);
    if (j++ == TOFS_MS(500))
    {
      j = 0;

      accelRead(&accel_data);
      if ((accel_data.accel_x - accelXStart) > 40)
      {
        fail++;
        state = DriveBack;
        servoSet(SERVO_LOAD_UP);
        driveSetDistance(-300);
        break;
      }
      else if (k == SERVO_LOAD_DOWN)
      {
        servoSet(SERVO_LOAD_DOWN);
        i = 0;
        state = CloseSpindle;
        break;
      }
      k -= 10;
      if (k <= SERVO_LOAD_DOWN)
      {
        k = SERVO_LOAD_DOWN;
      }

    }

  break;

  case CloseSpindle:
    spindleSet(50);
    if (switchStateClose())
    {
      j = 0;
      spindleSet(0);
      state = LoadUp;
    }

    if (j++ == TOFS_MS(closeTime))
    {
      j = 0;
      spindleSet(0);
      state = SendStatusFail;
    }
  break;

  case LoadUp:

    //Wait 500ms

    if (j++ == TOFS_MS(500))
    {
      j = 0;

      if (distance > 70)
      {
        servoSet(SERVO_LOAD_MIDDLE);
        state = OpenSpindle;
      }
      else
      {
        servoSet(SERVO_LOAD_UP);
        state = SendStatusOk;
      }
    }
  break;

  case OpenSpindle:
    spindleSet(-50);
    if (i++ == TOFS_MS(3600)||switchStateOpen())
    {
      i = 0;
      spindleSet(0);
      closeTime= 4500;
      state = LoadDown;
    }
  break;

  case SendStatusOk:
    tofRange(TOFADD2, &distance);
    if (j++ == TOFS_MS(1000))
    {
      j = 0;
      if (distance > 30)
      {
        state = LoadDown;
      }
      else
      {
        termWriteLine("cube:on");
        //servoOff(); //--> Optional, falls FTM0 anders noch gebraucht wird
        startPi = 0;
        state = Start;
      }

    }
  break;

  case SendStatusFail:

    if (j++ == TOFS_MS(500))
    {
      j = 0;

      spindleSet(-50);

      if (i++ == 8)
      {
        i = 0;
        servoSet(SERVO_LOAD_UP);
        spindleSet(0);
        termWriteLine("cube:fail");

        startPi = 0;
        state = Start;
      }
    }
  break;

  case DriveBack:


    if (driveGetDistanceInfo() == 0)
    {
      if (i++ == TOFS_MS(500))
      {
        i = 0;
        state = DriveToCube;
      }
    }

  break;
  }
}

tError cubeParseCommand(const unsigned char *cmd)
{
  tError result = EC_INVALID_ARG;
  if (strcmp(cmd, "help") == 0)
  {
    termWriteLine("motor (motor) commands:");
    termWriteLine("  help");
    termWriteLine("  set [-100..120]");
    termWriteLine("  status");
    result = EC_SUCCESS;
  }
  else if (strcmp(cmd, "start") == 0)
  {
    startPi = 1;
    result = EC_SUCCESS;
  }

  else if (strncmp(cmd, "kp", sizeof("kp") - 1) == 0)
  {
    cmd += sizeof("kp");
    int16_t v;
    result = utilScanDecimal16s(&cmd, &v);
    if (result != EC_SUCCESS) return result;
  }

  else if (strncmp(cmd, "set", sizeof("set") - 1) == 0)
  {
    cmd += sizeof("set");
    int16_t v;
    result = utilScanDecimal16s(&cmd, &v);
    if (result != EC_SUCCESS) return result;
  }
  return result;
}

void cubeInit(void)
{

// register terminal command line handler
  termRegisterCommandLineHandler(&clh, "cube", "help, set", cubeParseCommand);
}

