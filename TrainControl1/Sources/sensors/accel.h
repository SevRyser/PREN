/**
 *--------------------------------------------------------------------\n
 *                          Pren Team 29                              \n
 *--------------------------------------------------------------------\n
 *
 * \brief         ACCEL
 * \file
 * \author        Severin Ryser
 * \date          15.03.2019
 *
 *--------------------------------------------------------------------
 */
#ifndef SOURCES_UTILS_ACCEL_H_
#define SOURCES_UTILS_ACCEL_H_

#define ACCEL_RANGE     2000    //Accel-Range +/- 2000mg
#define REGISTER_RANGE  0x7FFF  //Register-Range +/- 32767 = 1/-2000mg
#define GYRO_RANGE      250000     //GYRO-Range 25000mdps

typedef union {
  uint8_t data[6];
  struct {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
  };
} tSensAccel;

typedef union {
  uint8_t data[6];
  struct {
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
  };
} tSensGyro;

tError accelRead(tSensAccel *data);
tError accelInit(void);
tError gyroRead(tSensGyro *data);
tError gyroInit(void);
void accelResetData(void);

#endif /* SOURCES_UTILS_ACCEL_H_ */
