/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Driver for the color sensor TCS34725
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          24.04.20018
 *
 * $Id: colSens.c 116 2018-05-11 14:30:59Z zajost $
 *
 *--------------------------------------------------------------------
 */

#include "platform.h"
#include "string.h"
#include "i2c.h"
#include "util.h"
#include "accel.h"
#include "term.h"

static tCommandLineHandler clh;

#define I2C_ACCELSENS_ADR                 0x6B
// LSM6DS3 register definitions
#define FIFO_CTRL1                     0x06
#define FIFO_CTRL2                     0x07
#define FIFO_CTRL3                     0x08
#define FIFO_CTRL4                     0x09
#define FIFO_CTRL5                     0x0A

#define CTRL1_XL                       0x10
#define CTRL2_G                        0x11
#define CTRL3_C                        0x12
#define CTRL4_C                        0x13
#define CTRL5_C                        0x14
#define CTRL6_C                        0x15
#define CTRL7_G                        0x16
#define CTRL8_XL                       0x17
#define CTRL9_XL                       0x18
#define CTRL10_C                       0x19

#define WHO_AM_I                       0x0F

tSensAccel Offset = {0,0,0};


void accelResetData(void)
{
  tSensAccel accel_data;
  accelRead(&accel_data);
  Offset.accel_x = 0 - accel_data.accel_x;
  Offset.accel_y = 0 - accel_data.accel_y;
  Offset.accel_z = 1000 - accel_data.accel_z;
}

/**
 * Reads the raw color data from the color sensor.
 *
 * @param[out] col
 *   a pointer to a tRegColor struct which will contains the color read out of the sensor
 * @return
 *   the error code or EC_SUCCESS if no error has occurred
 */
tError accelReadRaw(tSensAccel *data)
{
  tError result;

  // _todo ML#10.09 Implement the function colSensReadRawColor(...)
  uint8_t cmd = 0x28;
  result = i2cReadCmdData(I2C_ACCELSENS_ADR, cmd, data->data,
      sizeof(tSensAccel));

  return result;
}

tError accelRead(tSensAccel *data)
{
  tError result;
  tSensAccel accel_raw_data;
  accelReadRaw(&accel_raw_data);
  data->accel_x = (ACCEL_RANGE * accel_raw_data.accel_x / REGISTER_RANGE)
      + Offset.accel_x;
  data->accel_y = (ACCEL_RANGE * accel_raw_data.accel_y / REGISTER_RANGE)
      + Offset.accel_y;
  data->accel_z = (ACCEL_RANGE * accel_raw_data.accel_z / REGISTER_RANGE)
      + Offset.accel_z;

  return EC_SUCCESS;
}

tError gyroReadRaw(tSensGyro *data)
{
  tError result;

  // _todo ML#10.09 Implement the function colSensReadRawColor(...)
  uint8_t cmd = 0x22;
  result = i2cReadCmdData(I2C_ACCELSENS_ADR, cmd, data->data,
      sizeof(tSensGyro));

  return result;
}
tError gyroRead(tSensGyro *data)
{
  tError result;
  tSensGyro gyro_raw_data;
  gyroReadRaw(&gyro_raw_data);
  data->gyro_x = GYRO_RANGE * gyro_raw_data.gyro_x / REGISTER_RANGE;
  data->gyro_y = GYRO_RANGE * gyro_raw_data.gyro_y / REGISTER_RANGE;
  data->gyro_z = GYRO_RANGE * gyro_raw_data.gyro_z / REGISTER_RANGE;

  return EC_SUCCESS;
}

tError accelParseCommand(const unsigned char *cmd)
{
  tError result = EC_INVALID_ARG;
  if (strcmp(cmd, "all") == 0)
  {
    tSensAccel accel_data;
    accelRead(&accel_data);
    termWriteNum16s(accel_data.accel_x);
    termWriteChar(',');
    termWriteNum16s(accel_data.accel_y);
    termWriteChar(',');
    termWriteNum16s(accel_data.accel_z);
    termWriteChar('\n');
    result = EC_SUCCESS;
  }
  return result;
}

tError accelInit(void)
{
  tError result;
  //Who am I
  uint8_t cmd = WHO_AM_I;
  uint8_t data;
  result = i2cReadCmdData(I2C_ACCELSENS_ADR, cmd, &data, sizeof(data));
  // register terminal command line handler
  termRegisterCommandLineHandler(&clh, "accel", "", accelParseCommand);
  if (data == 0x69)
  {
    //Accelerometer ODR (output data rate) configuration setting
    cmd = CTRL1_XL;
    data = 0x40;
    result = i2cWriteCmdData(I2C_ACCELSENS_ADR, cmd, &data, sizeof(data));

    //Gyroscope ODR (output data rate) configuration setting
    cmd = CTRL2_G;
    data = 0x40;
    result = i2cWriteCmdData(I2C_ACCELSENS_ADR, cmd, &data, sizeof(data));

    accelResetData();


    return EC_SUCCESS;
  }

  return EC_FAILED;

}
