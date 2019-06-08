/*
 * drive.h
 *
 *  Created on: 02.05.2019
 *      Author: Blub
 */

#ifndef SOURCES_DRIVE_DRIVE_H_
#define SOURCES_DRIVE_DRIVE_H_

#define DELTA_T_MS 25


void driveInit(void);
void driveToWork(void);
void driveSetSpeed(int32_t speed);
void driveSetParameterKd(uint8_t pKd);
void driveSetParameterKi(uint8_t pKi);
void driveSetParameterKp(uint8_t pKp);
void driveSetDistance(int32_t distance);
uint32_t driveGetDistanceInfo(void);


#endif /* SOURCES_DRIVE_DRIVE_H_ */
