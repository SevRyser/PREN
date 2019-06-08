/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         i2c driver
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          24.04.20018
 *
 * $Id: i2c.h 102 2018-05-01 14:06:17Z zajost $
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_I2C_I2C_H_
#define SOURCES_I2C_I2C_H_


// I2C Base Functions
tError i2cStart(uint8_t adr, bool write);
tError i2cRepeatedStart(uint8_t adr, bool write);
uint8_t i2cReceiveLastByte(void);
tError i2cSendData(uint8_t *buf, uint8_t length);
void i2cReceiveData(uint8_t *buf, uint8_t length);
void i2cStop(void);


// I2C Composite Functions
tError i2cTest(uint8_t adr);

tError i2cReadCmdData(uint8_t adr, uint8_t cmd, uint8_t *data, uint8_t length);
tError i2cWriteCmdData(uint8_t adr, uint8_t cmd, uint8_t *data, uint8_t length);

void i2cInit(void);



#endif /* SOURCES_I2C_I2C_H_ */
