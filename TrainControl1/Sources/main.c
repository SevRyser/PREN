/*
 *--------------------------------------------------------------------\n
 *                        Pren Team 29                                \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Test 10
 * \file
 * \author        Severin Ryser
 * \date         10.04.19
 *--------------------------------------------------------------------\n
 */

#include "platform.h"
#include "util.h"

#include "ftm0.h"
#include "ftm1quad.h"
#include "ftm2.h"
#include "ftm3.h"
#include "adc.h"

#include "uart.h"
#include "i2c.h"
#include "term.h"

#include "tof.h"
#include "accel.h"
#include "encoder.h"
#include "beep.h"
#include "switch.h"

#include "servo.h"
#include "motor.h"
#include "spindle.h"

#include "drive.h"
#include "cube.h"

#include "test.h"

// calulates the nr of TOF count for a given number of milliseconds
#define TOFS_MS(x)   ((uint16_t)(((FTM3_CLOCK / 1000) * x) / (FTM3_MODULO + 1)))

void ProcessTest(void)
{
  static uint16_t i;
  if (i++ == TOFS_MS(50))
  {             // run pid worker every 25ms
    i = 0;
    testStateMachine();
  }

}

void ProcessCube(void)
{
  static uint16_t i;
  if (i++ == TOFS_MS(25))
  {             // run pid worker every 25ms
    i = 0;
    cubeStateMachine();
  }
}

void ProcessDrive(void)
{
  static uint16_t i;
  if (i++ == TOFS_MS(20))
  {             // run pid worker every 25ms
    i = 0;

    driveToWork();

//    termWriteNum32s(FTM1_CNT);
//    termWriteLine("  1 ticks");
  }
}

void main(void)
{
  uint8_t test = 0;
  uint8_t work = 1;
  tError result = EC_I2C_NO_ANSWER;
  uint16_t count;

  ftm0Init();
  ftm1quadInit();
  ftm2Init();
  ftm3Init();

  termInit(TERMINAL_BAUDRATE);
  i2cInit();

  tofInit();
  accelInit();
  encoderInit();
  beepInit();
  switchInit();

  servoInit();
  spindleInit();
  motorInit();

  driveInit();
  cubeInit();

  testInit();

  GPIOC_PDDR |= 1 << 2; // Set port direction of the blue Led on tinyK22 as output
  PORTC_PCR2 = PORT_PCR_MUX(1);    // configure port mux of the blue led to GPIO

  GPIOD_PDDR |= 1 << 3;             // Test-Pin
  PORTD_PCR3 = PORT_PCR_MUX(1);    // For Toggle Pin ->   GPIOD_PTOR = (1 << 3);


  while(test)
  {
    ProcessTest();
  }


  while (work)
  {
    termDoWork();                     //listen UART-Com

    if (FTM3_SC & FTM_SC_TOF_MASK)    // check for timer overflow
    {
      FTM3_SC &= ~FTM_SC_TOF_MASK;    // overflow occurred => clear TOF flag
      ProcessDrive();
      ProcessCube();

      if (count++ >= TOFS_MS(200)) // check if number of TOF's is equal or greater 250ms
      {
        count = 0;                    // restart counter to wait again 250ms
        GPIOC_PTOR |= (1 << 2);         // toggle PTC2 (blue led) on the tinyK22

//        tofRange(TOFADD1, &distance);
//        termWriteNum16s(distance);
//        termWriteChar('\n');

//Accelerometer TEST----------------------------------
//        accelRead(&accel_data);
//        termWriteNum16s(accel_data.accel_x);
//        termWriteChar(',');
//        termWriteNum16s(accel_data.accel_y);
//        termWriteChar(',');
//        termWriteNum16s(accel_data.accel_z);
//        termWriteChar('\n');

//Test Switch-------------------------------------------------
//                termWriteNum16s(switchStateOpen());
//                termWriteLine(" Ticks");

//Encoder Counter---------------------------------------------
//        termWriteNum16s(encodergetTicks());
//        termWriteLine(" Ticks");
//        termWriteNum16s(encodergetRPS());
//        termWriteLine(" rps");
//        termWriteNum16s(encodergetRPM());
//        termWriteLine(" rpm");
//        termWriteNum16s(encodergetSpeedmmps());
//        termWriteLine(" mm/s --> 10ms");
//        termWriteNum16s(encodergetSpeedmmps100ms());
//        termWriteLine(" mm/s --> 100ms");
//        termWriteNum16s(encodergetSpeedmmpsavg());
//        termWriteLine(" mm/s --> avg100ms");
//        termWriteChar('\n');

//Accelerometer TEST----------------------------------
//          SensReadAccel(&accel_data);
//
//          termWriteNum16s(accel_data.accel_x);
//          termWriteChar('\n');
//          termWriteNum16s(accel_data.accel_y);
//          termWriteChar('\n');
//          termWriteNum16s(accel_data.accel_z);
//          termWriteChar('\n');
//          termWriteChar('\n');
      }

    }
  }
}

