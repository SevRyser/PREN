/**
 *--------------------------------------------------------------------\n
 *                                                                    \n
 *--------------------------------------------------------------------\n
 *
 * \brief         maxon motor driver
 * \file
 * \author        Severin Ryser
 * \date          21.03.2019
 *
 *--------------------------------------------------------------------
 */

#include "platform.h"
#include "ftm3.h"
#include "term.h"
#include "util.h"
#include "string.h"
#include "motor.h"

static int8_t newvalue;


#define TOFS_MS(x)   ((uint16_t)(((FTM3_CLOCK / 1000) * x) / (FTM3_MODULO + 1)))
//switch pin configuration from GPIO to FTM-Mode
#define MOTOR_IN1_PWM()           (PORTD_PCR0 = PORT_PCR_MUX(4))  // PTD0[4]: FTM3_CH3
#define MOTOR_IN1_GPIO()          (PORTD_PCR0 = PORT_PCR_MUX(1))  // PTD0[1]: GPIO
#define MOTOR_IN2_PWM()           (PORTD_PCR1 = PORT_PCR_MUX(4))  // PTE5[6]: FTM3_CH3
#define MOTOR_IN2_GPIO()          (PORTD_PCR1 = PORT_PCR_MUX(1))  // PTE5[1]: GPIO

static tCommandLineHandler clh;       // terminal command line handler
int16_t valueMotor;


/**
 * Increments or decrements the PWM value oft the right wheel
 * @param[in] value
 *   a positive or negative value to add
 */
void motorIncrementPwm(int16_t value)
{
  int32_t v = valueMotor + value;
  if (v > MOTOR_MAX_VALUE) v = MOTOR_MAX_VALUE;
  if (v < -MOTOR_MAX_VALUE) v = -MOTOR_MAX_VALUE;
  motorSetPwm((int16_t)v);
}

void FTM3CH2_IRQHandler(void)
{
  FTM3_C2SC &= ~FTM_CnSC_CHF_MASK;  // clear timer channel flag

  static uint16_t cnt_timer; // count the number of TOF's
  static int16_t cnt_value;
  cnt_timer++;

        if (cnt_timer >= TOFS_MS(50)) // check if number of TOF's is equal or greater 250ms
        {

          if(cnt_value<newvalue)
          {
            cnt_value++;
            motorIncrementPwm(10);
            cnt_timer = 0;
          }
          else if(cnt_value>newvalue)
          {
            cnt_value--;
            motorIncrementPwm(-10);
            cnt_timer = 0;

          }
          else{
            FTM3_C2SC = 0;
            //cnt_value = 0;
            cnt_timer = 0;
          }

        }

}
void motorRamp(int16_t value)
{
  newvalue = value;
  FTM3_C2SC = (FTM_CnSC_MSB(1)|FTM_CnSC_MSA(0)) | (FTM_CnSC_ELSB(0)|FTM_CnSC_ELSA(1)) | (FTM_CnSC_CHIE_MASK);

  FTM3_C2V = FTM3_MODULO;// FTM3_CNT + tmrRampTicks;

}


/**
 * Sets the PWM value of the right wheel
 *
 * @param[in] value
 *   the value between -MOTOR_MAX_VALUE..0..+MOTOR_MAX_VALUE
 *   A value of '0' stops the wheel.
 */
void motorSetPwm(int16_t value)
{
  if (value > MOTOR_MAX_VALUE) value = MOTOR_MAX_VALUE;
  if (value < -MOTOR_MAX_VALUE) value = -MOTOR_MAX_VALUE;
  valueMotor = value;

  if (value < 0)
  {
    // drive backward
    value = -value;         // value has to be a positive channel value!
    MOTOR_IN1_GPIO();
    MOTOR_IN2_PWM();


  }
  else if (value > 0)
  {
    // drive forward

    MOTOR_IN1_PWM();        // set IN1 as timer Pin (pwm signal)
    MOTOR_IN2_GPIO();       // set IN2 as GPIO Pin (high-level)

  }
  else
  {
    // stop
    MOTOR_IN1_GPIO();
    MOTOR_IN2_GPIO();
  }
  int16_t v = (uint16_t)(((FTM3_MODULO + 1) * (MOTOR_MAX_VALUE-(uint32_t)value)) / MOTOR_MAX_VALUE);
  //int16_t v = (uint16_t)(((FTM3_MODULO + 1) * ((uint32_t)value)) / MOTOR_MAX_VALUE);
  FTM3_C0V = v;
  FTM3_C1V = v;
}



/**
 * Command line parser for this file.
 * This code is complete and works.
 *
 * @param[in] cmd
 *   the command to parse
 */
tError motorParseCommand(const unsigned char *cmd)
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

  else if (strncmp(cmd, "set", sizeof("set")-1) == 0)
  {
    cmd += sizeof("set");
    int16_t v;
    result = utilScanDecimal16s(&cmd, &v);
    if (result != EC_SUCCESS) return result;

    motorSetPwm((int16_t)((MOTOR_MAX_VALUE * v) / 120));
  }
  return result;
}



/**
 * Initializes the motor driver:
 * - Motor: PTD0, FTM3_CH0, Mux:4
 * - Motor: PTD1, FTM3_CH1, Mux:4
 */
void motorInit(void)
{
  // _todo ML#9.05 Configure the pin direction of the 4 pins as output.
  GPIOD_PDDR |= 1<<0 | 1<<1;               // configure PTD0 & PTD1 as output

  // _todo ML#9.06 set the pin value of all of the 4 pins to '1'
  GPIOD_PSOR |= 1<<0 | 1<<1 ;               // set PTD0 & PTD1 = 1


  // configures the pin muxing of all of the 4 pins as GPIO-Pin.
  // the output level will be '1' because of the configuration above.
  MOTOR_IN1_GPIO();
  MOTOR_IN2_GPIO();

  // _todo ML#9.07 configure both channels as edge aligned PWM with low-true pulses
  FTM3_C0SC = FTM_CnSC_MSx(2) | FTM_CnSC_ELSx(2);
  FTM3_C1SC = FTM_CnSC_MSx(2) | FTM_CnSC_ELSx(2);


  // register terminal command line handler
  termRegisterCommandLineHandler(&clh, "motor", "help, set", motorParseCommand);
}



