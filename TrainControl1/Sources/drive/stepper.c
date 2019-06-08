/*
 * steper.c
 *
 *  Created on: 12.04.2019
 *      Author: Blub
 */


#include <ftm1quad.h>
#include <string.h>
#include "platform.h"
#include "term.h"
#include "util.h"
#include <stepper.h>




#define IN1_GPIO()          (PORTC_PCR8 = PORT_PCR_MUX(1))
#define IN2_GPIO()          (PORTC_PCR9 = PORT_PCR_MUX(1))
#define IN3_GPIO()          (PORTC_PCR10 = PORT_PCR_MUX(1))
#define IN4_GPIO()          (PORTC_PCR11 = PORT_PCR_MUX(1))

#define MASK()            (0xF0FF)

static tCommandLineHandler clh;



void steperSet(int8_t value)
{
  uint16_t reg =0;
  switch (value) {
    case 0:

      GPIOC_PDOR = (GPIOC_PSOR & MASK()) |0x0100;

      break;
    case 1:

      GPIOC_PDOR = (GPIOC_PSOR & MASK()) |0x0300;

      break;
    case 2:
      GPIOC_PDOR = (GPIOC_PSOR & MASK()) |0x0200;
      break;
    case 3:
      GPIOC_PDOR = (GPIOC_PSOR & MASK()) |0x0600;
      break;
    case 4:
      GPIOC_PDOR = (GPIOC_PSOR & MASK()) |0x0400;
      break;
    case 5:
      GPIOC_PDOR = (GPIOC_PSOR & MASK()) |0x0C00;
      break;
    case 6:
      GPIOC_PDOR = (GPIOC_PSOR & MASK()) |0x0800;
      break;
    case 7:
      GPIOC_PDOR = (GPIOC_PSOR & MASK()) |0x0900;
      break;
    default:
      GPIOC_PDOR = (GPIOC_PSOR & MASK()) |0x0000;
      break;
  }
  // Richtung definieren, Links/Rechts Drehung

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
tError steperParseCommand(const unsigned char *cmd)
{
  tError result = EC_INVALID_ARG;
  if (strcmp(cmd, "help") == 0)
  {
    termWriteLine(" servo commands:");

    result = EC_SUCCESS;
  }
  else if (CheckCommand(cmd, "0"))
    {


      result = EC_SUCCESS;
    }
  else if (CheckCommand(cmd, "1"))
  {

    result = EC_SUCCESS;
  }


  return result;
}


void steperInit(void)
{
  // _todo ML#9.05 Configure the pin direction of the 4 pins as output.
  GPIOC_PDDR |= 1<<8 | 1<<9 |1<<10 |1<<11;               // configure PTA12

  IN1_GPIO();
  IN2_GPIO();
  IN3_GPIO();
  IN4_GPIO();

  GPIOC_PCOR |=  1<<8 | 1<<9 |1<<10 |1<<11;



  // register terminal command line handler
    termRegisterCommandLineHandler(&clh, "steper", "", steperParseCommand);
}
