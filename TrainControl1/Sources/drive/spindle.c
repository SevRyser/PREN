/*
 * spindle.c
 *
 *  Created on: 22.03.2019
 *      Author: Blub
 */

#include <string.h>
#include "platform.h"
#include "spindle.h"
#include "ftm2.h"
#include "term.h"
#include "util.h"
#include "servo.h"

//switch pin configuration from GPIO to FTM-Mode
#define SPINDLE_IN1_PWM()           (PORTB_PCR18 = PORT_PCR_MUX(3))  // PTD0[4]: FTM2_CH3
#define SPINDLE_IN1_GPIO()          (PORTB_PCR18 = PORT_PCR_MUX(1))  // PTD0[1]: GPIO
#define SPINDLE_IN2_PWM()           (PORTB_PCR19 = PORT_PCR_MUX(3))  // PTE5[6]: FTM2_CH3
#define SPINDLE_IN2_GPIO()          (PORTB_PCR19 = PORT_PCR_MUX(1))  // PTE5[1]: GPIO

static tCommandLineHandler clh;       // terminal command line handler
static int8_t valueSpindle;

/**
 * Increments or decrements the PWM value oft the right wheel
 * @param[in] value
 *   a positive or negative value to add
 */
void spindleIncrementPwm(int8_t value)
{
  int32_t v = valueSpindle + value;
  if (v > SPINDLE_MAX_VALUE) v = SPINDLE_MAX_VALUE;
  if (v < -SPINDLE_MAX_VALUE) v = -SPINDLE_MAX_VALUE;
  spindleSet((int8_t) v);
}

/**
 * Sets the PWM value of the right wheel
 *
 * @param[in] value
 *   the value between -SPINDLE_MAX_VALUE..0..+SPINDLE_MAX_VALUE
 *   A value of '0' stops the wheel.
 */
void spindleSet(int8_t value)
{
  if (value > SPINDLE_MAX_VALUE) value = SPINDLE_MAX_VALUE;
  if (value < -SPINDLE_MAX_VALUE) value = -SPINDLE_MAX_VALUE;
  valueSpindle = value;

  if (value < 0)
  {
    // drive backward
    value = -value;         // value has to be a positive channel value!
    SPINDLE_IN1_PWM();        // set IN1 as timer Pin (pwm signal)
    SPINDLE_IN2_GPIO();       // set IN2 as GPIO Pin (high-level)

  }
  else if (value > 0)
  {
    // drive forward
    SPINDLE_IN1_GPIO();
    SPINDLE_IN2_PWM();
  }
  else
  {
    // stop
    SPINDLE_IN1_GPIO();
    SPINDLE_IN2_GPIO();
  }
  int16_t v = (uint16_t) (((FTM2_MODULO + 1)
      * (SPINDLE_MAX_VALUE - (uint32_t) value)) / SPINDLE_MAX_VALUE);

  //int16_t v = (uint16_t)(((FTM2_MODULO + 1) * ((uint32_t)value)) / SPINDLE_MAX_VALUE);
  FTM2_C0V = v;
  FTM2_C1V = v;
}

/**
 * Command line parser for this file.
 * This code is complete and works.
 *
 * @param[in] cmd
 *   the command to parse
 */
tError spindleParseCommand(const unsigned char *cmd)
{
  tError result = EC_INVALID_ARG;
  if (strcmp(cmd, "help") == 0)
  {
    termWriteLine("spindle commands:");
    termWriteLine("  help");
    termWriteLine("  [value] (Range: -100..100)");
    result = EC_SUCCESS;
  }

  else if (strncmp(cmd, "set", sizeof("set") - 1) == 0)
  {
    cmd += sizeof("set");
    int16_t v;
    result = utilScanDecimal16s(&cmd, &v);
    if (result != EC_SUCCESS) return result;
    spindleSet((int16_t) ((SPINDLE_MAX_VALUE * v) / 120));
  }
  return result;
}

/**
 * Initializes the motor driver:
 * - Motor: PTD0, FTM2_CH0, Mux:4
 * - Motor: PTD1, FTM2_CH1, Mux:4
 */
void spindleInit(void)
{
  // _todo ML#9.05 Configure the pin direction of the 4 pins as output.
  GPIOB_PDDR |= 1 << 18 | 1 << 19;            // configure PTD0 & PTD1 as output

  // _todo ML#9.06 set the pin value of all of the 4 pins to '1'
  GPIOB_PSOR |= 1 << 18 | 1 << 19;               // set PTD0 & PTD1 = 1

//  GPIOB_PCOR |= 1<<19;               // set PTD0 & PTD1 = 1
//  GPIOB_PSOR |=  1<<18;               // set PTD0 & PTD1 = 1

  // configures the pin muxing of all of the 4 pins as GPIO-Pin.
  // the output level will be '1' because of the configuration above.
  SPINDLE_IN1_GPIO();
  SPINDLE_IN2_GPIO();

  // _todo ML#9.07 configure both channels as edge aligned PWM with low-true pulses
  FTM2_C0SC = FTM_CnSC_MSx(2) | FTM_CnSC_ELSx(2);
  FTM2_C1SC = FTM_CnSC_MSx(2) | FTM_CnSC_ELSx(2);


  while (switchStateOpen() == 0)
  {
    spindleSet(-40);
  }
  spindleSet(0);




// register terminal command line handler
termRegisterCommandLineHandler(&clh, "spindle", "help, set", spindleParseCommand);
}

