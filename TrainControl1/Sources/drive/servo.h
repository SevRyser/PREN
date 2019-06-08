/*
 * servo.h
 *
 *  Created on: 16.03.2019
 *      Author: Blub
 */



#ifndef SOURCES_SERVO_H_
#define SOURCES_SERVO_H_

#define SERVO_MAX_VALUE               2000

#define SERVO_START                   185
#define SERVO_LOAD_UP                 180       //200 Wert vor Endschalter Ausen
#define SERVO_LOAD_MIDDLE             120
#define SERVO_LOAD_DOWN               80        //90 Wert vor Endschalter Ausen


void servoInit(void);
void servoSet(int16_t value);
void servoOff(void);



#endif /* SOURCES_UTILS_SERVO_H_ */
