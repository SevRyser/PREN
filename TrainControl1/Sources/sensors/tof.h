/**
 *--------------------------------------------------------------------\n
 *                          Pren Team 29                              \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Tof
 * \file
 * \author        Severin Ryser
 * \date          13.03.2019
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_UTILS_TOF_H_
#define SOURCES_UTILS_TOF_H_

#define TOFADD1                 0x29
#define TOFADD2                 0x39
  tError tofInit();
  tError tofRange(uint16_t tofAddress,uint8_t* data);
  tError tofWriteReg(uint16_t tofAddress, uint16_t reg, uint8_t data);
  tError tofReadReg(uint16_t tofAddress, uint16_t reg, uint8_t* data);

#endif /* SOURCES_UTILS_TOF_H_ */
