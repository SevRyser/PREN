/**
 *--------------------------------------------------------------------\n
 *                     \n
 *--------------------------------------------------------------------\n
 *
 * \brief         motor driver
 * \file
 * \author        Severin Ryser
 * \date          21.03.2019
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_MOTOR_H_
#define SOURCES_MOTOR_H_

#define MOTOR_MAX_VALUE               10000
#define MOTOR_MAX_SPEED               8000

void motorIncrementPwm(int16_t value);


void motorSetPwm(int16_t value);

void motorInit(void);



#endif /* SOURCES_MOTOR_H_ */
