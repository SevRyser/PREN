/*
 * servo.c
 *
 *  Created on: 13.03.2019
 *      Author: Blub
 */
#include <ftm0.h>
#include <string.h>
#include "platform.h"
#include "servo.h"
#include "term.h"
#include "util.h"

#define SERVO_PWM()           (PORTD_PCR7 = PORT_PCR_MUX(4))
#define SERVO_GPIO()          (PORTD_PCR7 = PORT_PCR_MUX(1))

static tCommandLineHandler clh;

void servoOff(void)
{
  SERVO_GPIO();
  GPIOD_PCOR |= 1<<7;
}

void servoSet(int16_t value)
{
  SERVO_PWM();
  if (value > SERVO_MAX_VALUE) value = SERVO_MAX_VALUE;

  int16_t v = (uint16_t)(((FTM0_MODULO + 1) * ((uint32_t)value)) / SERVO_MAX_VALUE);
  FTM0_C7V = v;
}

#define CheckCommand(buf, command)   (strncmp(buf, command, sizeof(command)-1) == 0)
/**
 * This function parses one command line, executes the command and returns the status
 *
 * @param[in] cmd
 *   the null terminated string to parse
 * @returns
 *   EC_SUCCESS if there was a valid command
 *   EC_INVALID_CMD if the command was unknown or invalid
 */
tError servoParseCommand(const unsigned char *cmd)
{
  tError result = EC_INVALID_ARG;
  if (strcmp(cmd, "help") == 0)
  {
    termWriteLine(" servo commands:");
    result = EC_SUCCESS;
  }
  else if (strncmp(cmd, "set", sizeof("set")-1) == 0)
    {
      cmd += sizeof("set");
      int16_t v;
      result = utilScanDecimal16s(&cmd, &v);
  if (result != EC_SUCCESS) return result;

      servoSet((int16_t)(v));
    }
  else if (CheckCommand(cmd, "0"))
    {
      SERVO_GPIO();
      GPIOD_PCOR |= 1<<7;
      result = EC_SUCCESS;
    }


  return result;
}


void servoInit(void)
{
  // _todo ML#9.05 Configure the pin direction of the 4 pins as output.
  GPIOD_PDDR |= 1<<7;               // configure PTA12

//-------------------------------Achtung noch anpassen
  SERVO_GPIO();
  //SERVO_PWM();
  GPIOD_PCOR |= 1<<7;

  // _todo ML#9.07 configure both channels as edge aligned PWM with low-true pulses
  FTM0_C7SC = FTM_CnSC_MSx(2) | FTM_CnSC_ELSx(2);
  servoSet(SERVO_START);


  // register terminal command line handler
    termRegisterCommandLineHandler(&clh, "servo", "1, 2, 3, 4, 5, 6, 7, 8, 9", servoParseCommand);
}
