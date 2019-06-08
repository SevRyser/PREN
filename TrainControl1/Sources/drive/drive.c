/*
 * drive.c
 *
 *  Created on: 02.05.2019
 *      Author: Blub
 */

#include "platform.h"
#include "term.h"
#include "util.h"
#include "string.h"
#include "drive.h"
#include "motor.h"
#include "encoder.h"

static tCommandLineHandler clh;       // terminal command line handler

static int32_t targetSpeed;
static int32_t targetDistance;
static uint8_t kp, ki, kd;
static uint8_t Ta;
static int32_t val;
static uint8_t motorBrake = 0;
static int16_t startTicks = 0;
static int16_t targetTicks = 0;


void driveSetParameters(uint8_t pKp, uint8_t pKi)
{
  kp = pKp;
  ki = pKi;
}
void driveSetParameterKp(uint8_t pKp)
{
  kp = pKp;
}
void driveSetParameterKi(uint8_t pKi)
{
  ki = pKi;
}
void driveSetParameterKd(uint8_t pKd)
{
  kd = pKd;
}
void driveSetParameterTa(uint8_t pTa)
{
  Ta = pTa;
}

void driveSetSpeed(int32_t speed)
{
  targetSpeed = speed;
}

void driveSetDistance(int32_t distance)
{
  FTM1_C0SC = (FTM_CnSC_MSB(0)|FTM_CnSC_MSA(1)) | (FTM_CnSC_ELSB(1)|FTM_CnSC_ELSA(0)) | (FTM_CnSC_CHIE_MASK);

  targetDistance = distance;
  startTicks = encodergetTicks();
  targetTicks = ((float) (1194 * distance) / 70);    //+ 40; ->nur fürs anfahren
  if (encodergetTicks10ms()==0)
  {
    if (distance > 0)
    {
      targetTicks += 40; //Offset wegen schlupf von anfang an
    }
    else
    {
      targetTicks -= 40; //Offset wegen schlupf von anfang an
    }

  }

  FTM1_C0V = FTM1_CNT - targetTicks;

  if (distance > 0)
  {
    driveSetSpeed(50);
  }
  else
  {
    driveSetSpeed(-50);
  }
}

uint32_t driveGetDistanceInfo(void)
{
  return targetDistance;
}

void FTM1CH0_IRQHandler()  //driveDistance
{
  FTM1_C0SC &= ~FTM_CnSC_CHF_MASK;
  FTM1_C0SC =  FTM_CnSC_CHIE(0);
  motorBrake = 1;
  driveSetSpeed(0);

  targetDistance = 0;
  targetTicks = 0;
}


/**
 * This function contains the PID closed loop controller
 */
void driveToWork(void)
{
  static int16_t trueSpeed = 0;
  static int16_t setSpeed = 0;
  static int oldError;
  static int intError;
  int ctrl_p;
  int ctrl_i = 0;
  int ctrl_d;
  int currentError;

  trueSpeed = encodergetSpeedmmps();

  if (targetSpeed > setSpeed)
  {                                        // accelerate
    setSpeed += 30;
    if (targetSpeed < setSpeed) setSpeed = targetSpeed;
  }
  if (targetSpeed < setSpeed)
  {                                        // decelerate
    setSpeed -= 30;
    if (targetSpeed > setSpeed) setSpeed = targetSpeed;
  }

//   //dynamic parameters
//   if (abs(setSpeed) < 200)
//   {
//     kp = 40; //80;
//     ki = 4; //30;
//   }
//   else
//   {
//     kp = 5; //80;
//     ki = 40; //30;
//   }

  currentError = (setSpeed - trueSpeed);
  // calculate Controller
  ctrl_p = kp * currentError;
  intError += currentError;
  ctrl_i = ki * intError* Ta;
  ctrl_d = kd * (currentError - oldError)*1/Ta;
  oldError = currentError;


  val = ctrl_p + ctrl_i/1000 + ctrl_d/1000;

  val /= 10;                                        //scaling

  if(setSpeed<200)
  {
    if(setSpeed>0)
    {
      val += 1000;
    }
    else
    {
      val -= 1000;
    }
    kp =90;
    ki=60;
  }

  if (val > MOTOR_MAX_SPEED)
  {
    val = MOTOR_MAX_SPEED;
    intError -= currentError;
  }
  else if (val < -MOTOR_MAX_SPEED)
  {
    val = -MOTOR_MAX_SPEED;
    intError += currentError;
  }

  else if (targetSpeed == 0 && trueSpeed == 0)
  {
    val = 0;
    intError = 0;
  }
//  else if (targetSpeed != 0 && trueSpeed == 0)
//  {
//    static uint16_t i;
//    if (i++ == 20)
//    {
//      i=0;
//      val = 0;
//      intError = 0;
//    }
//
//  }
  else if (motorBrake == 1)
  {
    val = 0;
    motorBrake=0;
    intError = 0;

  }

  motorSetPwm(val);


//  termWriteNum32s(val);
//  termWriteChar('\n');

}

tError driveParseCommand(const unsigned char *cmd)
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

  else if (strncmp(cmd, "kp", sizeof("kp") - 1) == 0)
  {
    cmd += sizeof("kp");
    int16_t v;
    result = utilScanDecimal16s(&cmd, &v);
    if (result != EC_SUCCESS) return result;
    driveSetParameterKp(v);

  }
  else if (strncmp(cmd, "ki", sizeof("ki") - 1) == 0)
  {
    cmd += sizeof("ki");
    int16_t v;
    result = utilScanDecimal16s(&cmd, &v);
    if (result != EC_SUCCESS) return result;
    driveSetParameterKi(v);

  }
  else if (strncmp(cmd, "kd", sizeof("kd") - 1) == 0)
  {
    cmd += sizeof("kd");
    int16_t v;
    result = utilScanDecimal16s(&cmd, &v);
    if (result != EC_SUCCESS) return result;
    driveSetParameterKd(v);

  }
  else if (strncmp(cmd, "set", sizeof("set") - 1) == 0)
  {
    cmd += sizeof("set");
    int16_t v;
    result = utilScanDecimal16s(&cmd, &v);
    if (result != EC_SUCCESS) return result;
    driveSetSpeed(v);

  }
  else if (strncmp(cmd, "distance", sizeof("distance") - 1) == 0)
  {
    cmd += sizeof("distance");
    int16_t v;
    result = utilScanDecimal16s(&cmd, &v);
    if (result != EC_SUCCESS) return result;
    driveSetDistance(v);

  }
  return result;
}

void driveInit(void)
{
  kp = 20; //80;
  ki = 40; //30;
  kd = 0;
  Ta = 25;
  targetSpeed = 0;

// register terminal command line handler
  termRegisterCommandLineHandler(&clh, "drive", "help, set", driveParseCommand);
}

