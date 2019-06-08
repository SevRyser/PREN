/*
 * tof.c
 *
 *  Created on: 13.03.2019
 *      Author: Blub
 */


#include "platform.h"
#include "tof.h"
#include "i2c.h"
#include "util.h"
#include "term.h"
#include "string.h"

static tCommandLineHandler clh;



  tError tofWriteReg(uint16_t tofAddress, uint16_t reg, uint8_t data) {
    tError result = EC_FAILED;

    // send I2C-Address
    result = i2cStart(tofAddress, FALSE);

    // send index high byte
    if (result == EC_SUCCESS) {
      result = i2cSendData((uint8_t*) (&reg) + 1, 1);
    }

    // send index low byte
    if (result == EC_SUCCESS) {
      //uint8_t regs = (uint8_t)(reg >> 8);
      result = i2cSendData((uint8_t*) (&reg), 1);
    }

    // send data
    if (result == EC_SUCCESS) {
      result = i2cSendData(&data, 1);
    }

    // generate stop condition
    i2cStop();

    return result;
  }

  tError tofReadReg(uint16_t tofAddress, uint16_t reg, uint8_t* data) {
    tError result = EC_FAILED;

    // send I2C-Address
    result = i2cStart(tofAddress, FALSE);

    // send index high byte
    if (result == EC_SUCCESS) {
      result = i2cSendData((uint8_t*) (&reg) + 1, 1);
    }

    // send index low byte
    if (result == EC_SUCCESS) {
      result = i2cSendData((uint8_t*) (&reg), 1);
    }

    // repeated start to change the direction from write to read
    if (result == EC_SUCCESS) {
      result = i2cRepeatedStart(tofAddress, TRUE);
    }

    // read the data & generate the stop condition
    if (result == EC_SUCCESS) {
      i2cReceiveData(data, 1);
    }

    return result;
  }

  tError tofRange(uint16_t tofAddress,uint8_t* data) {
    tError result = EC_FAILED;
    uint8_t status[1];
    status[0] = 0;

    // start single range measurement
    result = tofWriteReg(tofAddress, 0x018, 0x01);

    // poll the VL6180 till new sample ready
    if (EC_FAILED == result) {
      // wait for new measurement ready status
      while (*status != 0x04 && EC_SUCCESS == result) {
        // check the status
        result = tofReadReg(tofAddress, 0x04f, status);
        status[0] = status[0] & 0x07;
        //wait_ms(1); // (can be removed)
      }
    }

    // read range result
    if (EC_SUCCESS == result) {
      result = tofReadReg(tofAddress, 0x062, data);
    }

    // clear the interrupt on VL6180
    if (EC_SUCCESS == result) {
      result = tofWriteReg(tofAddress, 0x015, 0x07);
    }

    return result;

  }

  tError tofInitSequence(uint16_t tofAddress) {
      tError result = EC_FAILED;
      uint8_t readBuffer[1];

      result = tofReadReg(tofAddress, 0x016, readBuffer);
      // check to see has it be Initialised already
      if (1 != *readBuffer && EC_SUCCESS != result) {
        result = EC_FAILED;
      }

      // Mandatory : private registers
      //TODO comment configurations
      result = tofWriteReg(tofAddress, 0x0207, 0x01);

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x0208, 0x01);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x0096, 0x00);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x0097, 0xfd);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00e3, 0x01);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00e4, 0x03);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00e5, 0x02);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00e6, 0x01);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00e7, 0x03);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00f5, 0x02);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00d9, 0x05);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00db, 0xce);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00dc, 0x03);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00dd, 0xf8);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x009f, 0x00);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00a3, 0x3c);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00b7, 0x00);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00bb, 0x3c);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00b2, 0x09);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00ca, 0x09);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x0198, 0x01);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x01b0, 0x17);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x01ad, 0x00);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x00ff, 0x05);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x0100, 0x05);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x0199, 0x05);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x01a6, 0x1b);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x01ac, 0x3e);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x01a7, 0x1f);
      }

      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x0030, 0x00);
      }

      // Recommended : Public registers - See data sheet for more detail

      // Enables polling for ‘New Sample ready when measurement completes
      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x0011, 0x10);
      }
      // Set the averaging sample period (compromise between lower noise and increased execution time)
      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x010a, 0x30);
      }
      // Sets the light and dark gain (upper nibble). Dark gain should not be changed
      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x003f, 0x46);
      }
      // sets the # of range measurements after which auto calibration of system is performed
      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x0031, 0xFF);
      }
      // Set ALS integration time to 100ms
      if (result == EC_SUCCESS) {
        result = tofWriteReg(tofAddress, 0x0041, 0x63);
      }

      return result;
    }


#define CheckCommand(buf, command)   (strncmp(buf, command, sizeof(command)-1) == 0)

  tError tofParseCommand(const unsigned char *cmd)
  {
    tError result = EC_INVALID_ARG;
    if (strcmp(cmd, "help") == 0)
    {
      termWriteLine(" tof commands: tof 1, tof 2");
      result = EC_SUCCESS;
    }
    else if (CheckCommand(cmd, "1"))
      {
        uint8_t range_data;
        tofRange(TOFADD1, &range_data);
        termWrite("TOF 1: ");
        termWriteNum16s((int16_t)range_data);
        termWrite(" mm");
        termWriteChar('\n');
        result = EC_SUCCESS;
      }
    else if (CheckCommand(cmd, "2"))
    {
      uint8_t range_data;
      tofRange(TOFADD2, &range_data);
      termWrite("TOF 2: ");
      termWriteNum16s((int16_t)range_data);
      termWrite(" mm");
      termWriteChar('\n');

      result = EC_SUCCESS;
    }

    return result;
  }


  tError tofInit() {
    GPIOB_PDDR |= 1<<16 | 1<<17;       // configure PT & PT as output
    PORTB_PCR16 = PORT_PCR_MUX(1);
    PORTB_PCR17 = PORT_PCR_MUX(1);

    GPIOB_PSOR |= 1<<17;               // set PTD0 & PTD1 = 1
    GPIOB_PCOR |= 1<<16;               // set PTD0 & PTD1 = 1
    utilWaitUs(400);
    tError result = tofWriteReg(TOFADD1, 0x0212, TOFADD2);
    GPIOB_PSOR |= 1<<16;               // set PTD0 & PTD1 = 1
    utilWaitUs(400);
    tofInitSequence(TOFADD1);
    tofInitSequence(TOFADD2);
    uint8_t range_data;
    tofRange(TOFADD1, &range_data);
    if(range_data<0){
      result = EC_FAILED;
    }
    tofRange(TOFADD2, &range_data);
    if(range_data<0){
      result = EC_FAILED;
    }

    // register terminal command line handler
      termRegisterCommandLineHandler(&clh, "tof", "1, 2", tofParseCommand);

    return result;
  }

