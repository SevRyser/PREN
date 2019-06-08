/*
 * steper.h
 *
 *  Created on: 12.04.2019
 *      Author: Blub
 */

#ifndef SOURCES_DRIVE_STEPPER_H_
#define SOURCES_DRIVE_STEPPER_H_

#define MAX_STEPS               4096


void steperInit(void);
void steperSet(int8_t value);



#endif /* SOURCES_DRIVE_STEPPER_H_ */
