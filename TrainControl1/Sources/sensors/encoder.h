/*
 * encoder.h
 *
 *  Created on: 28.03.2019
 *      Author: Blub
 */

#ifndef SOURCES_SENSORS_ENCODER_H_
#define SOURCES_SENSORS_ENCODER_H_


#define FTM0CH0_VALUE   2500 // 10ms

tError encoderInit();
int16_t encodergetTicks();
int16_t encodergetTicks10ms();
int16_t encodergetRPS();
int16_t encodergetRPM();
int16_t encodergetSpeedmmps();


#endif /* SOURCES_SENSORS_ENCODER_H_ */
