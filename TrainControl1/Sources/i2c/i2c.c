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
 * $Id: i2c.c 102 2018-05-01 14:06:17Z zajost $
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"
#include "i2c.h"


#define I2Cx_F                  (I2C0_F)
#define I2Cx_C1                 (I2C0_C1)
#define I2Cx_S                  (I2C0_S)
#define I2Cx_D                  (I2C0_D)


//---------------------------------------------------------------------------------------------------------------------
// I 2 C   -   B A S E   F U N C T I O N S
//---------------------------------------------------------------------------------------------------------------------

/**
 * Generates a start condition on the I2C-Bus
 *
 * @param [in] adr
 *   the slave address
 * @param [in] read
 *   FALSE => write
 *   TRUE => read
 * @return
 *   EC_SUCCESS       the slave answered with an ACK
 *   C_I2C_NO_ANSWER  no answer from a slave
 */
tError i2cStart(uint8_t adr, bool read)
{
    adr = (uint8_t)adr << 1;                // combine address with r/w bit
    adr |= read;                            // add r/w bit

    while (I2Cx_S & I2C_S_BUSY_MASK);       // wait until i2c bus is idle. Necessary if two transmissions follow one after the other

    I2Cx_S |= I2C_S_IICIF_MASK;             // clear the interrupt flag
    I2Cx_C1 |= I2C_C1_TX_MASK;              // change to transmit mode
    I2Cx_C1 |= I2C_C1_MST_MASK;             // generate the start-condition
    I2Cx_D = adr;                           // send address with r/w bit
    while (!(I2Cx_S & I2C_S_IICIF_MASK));   // wait for the transfer to complete
    I2Cx_S |= I2C_S_IICIF_MASK;             // clear the interrupt flag

    if (I2Cx_S & I2C_S_RXAK_MASK) {         // Check if an ACK has been received
        I2Cx_C1 &= ~I2C_C1_MST_MASK;        // generate Stop-Condition
        return EC_I2C_NO_ANSWER;            // NAK => abort
    }
    return EC_SUCCESS;
}

/**
 * Generates a repeated start condition on the I2C-Bus
 *
 * @param [in] adr
 *   the slave address (7bit)
 * @param [in] read
 *   FALSE => write mode
 *   TRUE =>  read mode
 * @return
 *    EC_SUCCESS        if the slave answered with an ACK
 *    EC_I2C_NO_ANSWER  no answer from a slave
 */
tError i2cRepeatedStart(uint8_t adr, bool read)
{
  adr = (uint8_t)adr << 1;
  adr |= read;

  I2Cx_C1 |= I2C_C1_RSTA_MASK;              // generate a repeated start condition

  I2Cx_D = adr;                             // sends the address with the read/write bit
  while (!(I2Cx_S & I2C_S_IICIF_MASK));     // wait for the transfer to complete
  I2Cx_S |= I2C_S_IICIF_MASK;               // clear the interrupt flag

  if (I2Cx_S & I2C_S_RXAK_MASK) {           // Check if an ACK has been received
    I2Cx_C1 &= ~I2C_C1_MST_MASK;            // generate Stop-Condition
    return EC_I2C_NO_ANSWER;                // NAK => abort
  }
  return EC_SUCCESS;
}

/**
 * Receives the last byte from a slave
 *
 * For the last byte don't send an ACK to generate
 * the stop condition after
 *
 * @return
 *  the received byte
 */
uint8_t i2cReceiveLastByte(void)
{
  uint8_t data;
  I2Cx_C1 &= ~I2C_C1_TX_MASK;               // change to receive mode
  I2Cx_C1 |= I2C_C1_TXAK_MASK;              // Don't send an ACK after the last byte received to generate a stop condition after

  data = I2Cx_D;                            // dummy read to start the last transmission to read the last byte after

  while (!(I2Cx_S & I2C_S_IICIF_MASK));     // wait for the transfer to complete
  I2Cx_S |= I2C_S_IICIF_MASK;               // clear the interrupt flag
  I2Cx_C1 &= ~I2C_C1_MST_MASK;              // generate Stop-Condition
  data = I2Cx_D;                            // read last received byte
  return data;
}

/**
 * Transmitts a buffer. It is assumed that
 * i2cStart() was successful before.
 *
 * @param [in] buf
 *   the data to send
 * @param [in] length
 *   the number of bytes to send
 *
 * @return
 *    EC_SUCCESS if the slave answered with an ACK
 *    EC_I2C_NAK if the slave answered with a NAK
 */
tError i2cSendData(uint8_t *buf, uint8_t length)
{
  uint8_t i;
    for (i=0; i<length; i++)
    {
        I2Cx_D = buf[i];                        // start the transmission of a databyte
        while (!(I2Cx_S & I2C_S_IICIF_MASK));   // wait for the transfer to complete
        I2Cx_S |= I2C_S_IICIF_MASK;             // clear the interrupt flag
        if (I2Cx_S & I2C_S_RXAK_MASK) {         // Check if an ACK has been received
            I2Cx_C1 &= ~I2C_C1_MST_MASK;        // generate Stop-Condition
            return EC_I2C_NAK;                  // NAK => abort
        }
    }
    return EC_SUCCESS;
}

/**
 * Reads data from a I2C-Device. It is assumed that
 * i2cStart() was successful before.
 *
 * This function generates a stop condition at the end!
 *
 * @param [out] buf
 *   the buffer for the receiving data
 * @param [in] length
 *   the number of bytes to receive
 */
void i2cReceiveData(uint8_t *buf, uint8_t length)
{
  uint8_t i;
  I2Cx_C1 &= ~I2C_C1_TX_MASK;               // change to receive mode

  if (length > 1) {
    I2Cx_C1 &= ~I2C_C1_TXAK_MASK;           // generates ACK's from now on
    buf[0] = I2Cx_D;                        // dummy read to start the transmission and to read the first byte
    while (!(I2Cx_S & I2C_S_IICIF_MASK));   // wait for the transfer to complete
    I2Cx_S |= I2C_S_IICIF_MASK;             // clear the interrupt flag

    for (i=0; i<length-2; i++) {            // read the bytes in a loop
      buf[i] = I2Cx_D;                      // read the received byte and start a new transmission...
      while (!(I2Cx_S & I2C_S_IICIF_MASK)); // wait for the transfer to complete
      I2Cx_S |= I2C_S_IICIF_MASK;           // clear the interrupt flag
    }

    I2Cx_C1 |= I2C_C1_TXAK_MASK;            // Don't send an ACK after the last byte received to generate a stop condition after
    buf[length - 2] = I2Cx_D;               // read the received byte and start a new transmission...
    while (!(I2Cx_S & I2C_S_IICIF_MASK));   // wait for the transfer to complete
    I2Cx_S |= I2C_S_IICIF_MASK;             // clear the interrupt flag

    I2Cx_C1 &= ~I2C_C1_MST_MASK;            // generate the stop condition
    buf[length-1] = I2Cx_D;                 // read the last byte
  }
  else {
    I2Cx_C1 |= I2C_C1_TXAK_MASK;            // Don't send an ACK after the last byte received to generate a stop condition after
    buf[0] = I2Cx_D;                        // dummy read to start the last transmission to read the last byte after
    while (!(I2Cx_S & I2C_S_IICIF_MASK));   // wait for the transfer to complete
    I2Cx_S |= I2C_S_IICIF_MASK;             // clear the interrupt flag

    I2Cx_C1 &= ~I2C_C1_MST_MASK;            // generate Stop-Condition
    buf[0] = I2Cx_D;                        // read last received byte
  }
}

/**
 * Generates the stop condition
 */
void i2cStop()
{
  I2Cx_C1 &= ~I2C_C1_MST_MASK;              // Generate Stop-Condition
  I2Cx_S |= I2C_S_IICIF_MASK;               // Clear interrupt flag
}



//---------------------------------------------------------------------------------------------------------------------
// I 2 C   -   C O M P O S I T E   F U N C T I O N S
//---------------------------------------------------------------------------------------------------------------------


/**
 * Checks if a device is on the I2C-Bus and answers with an ACK
 *
 * @param [in] adr
 *   the I2C-Bus address of the device
 *
 * @return
 *   EC_SUCCESS        the slave answered with an ACK
 *   EC_I2C_NO_ANSWER  no answer from a slave
 */
tError i2cTest(uint8_t adr)
{
  tError result;
  result = i2cStart(adr, FALSE);
  if (result != EC_SUCCESS) return result;

  i2cStop();
  return EC_SUCCESS;
}


 /**
 * Reads data from an I2C-Device with an additional command byte
 * which is send before
 *
 * @param [in] adr
 *   the I2C-Bus address of the device
 * @param [in] cmd
 *   the command byte or register byte to send before
 * @param [out] data
 *   the data to send
 * @param [in] length
 *   the number of bytes to send
 *
 * @return
 *   EC_SUCCESS       the slave answered with an ACK
 *   EC_I2C_NAK       if the slave answered with a NAK
 *   EC_I2C_NO_ANSWER no answer from the slave
 */
tError i2cReadCmdData(uint8_t adr, uint8_t cmd, uint8_t *data, uint8_t length)
{
  tError result;
  result = i2cStart(adr, FALSE);            // send I2C-Address
  if (result != EC_SUCCESS) return result;

  result = i2cSendData(&cmd, 1);            // send command byte
  if (result != EC_SUCCESS) return result;

  result = i2cRepeatedStart(adr, TRUE);     // repeated start to change the direction from write to read
  if (result != EC_SUCCESS) return result;

  i2cReceiveData(data, length);             // read the data & generate the stop condition
  return EC_SUCCESS;
}

/**
 * Writes data to an I2C-Device with an additional command byte
 * which is send before
 *
 * @param [in] adr
 *   the I2C-Bus address of the device
 * @param [in] cmd
 *   the command byte or register byte to send before
 * @param [in] data
 *   the buffer for the receiving data
 * @param [in] length
 *   the number of bytes to receive
 * @return
 *   EC_SUCCESS       the slave answered with an ACK
 *   EC_I2C_NAK       if the slave answered with a NAK
 *   EC_I2C_NO_ANSWER no answer from the slave
 */
tError i2cWriteCmdData(uint8_t adr, uint8_t cmd, uint8_t *data, uint8_t length)
{
  tError result;
  result = i2cStart(adr, FALSE);            // send I2C-Address
  if (result != EC_SUCCESS) return result;

  result = i2cSendData(&cmd, 1);            // send command byte
  if (result != EC_SUCCESS) return result;

  result = i2cSendData(data, length);       // send data
  if (result != EC_SUCCESS) return result;

  i2cStop();                                // generate stop condition
  return EC_SUCCESS;
}


/**
 * Initializes the I2C-Bus I2C0 to ~400kBit/s
 */
void i2cInit(void)
{
  // _todo ML#10.01 configure clock gating for i2c bus
  SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;

  // _todo ML#10.02 configure MUX for SCL & SDA
//  PORTE_PCR24 = PORT_PCR_MUX(5) | PORT_PCR_ODE(1);
//  PORTE_PCR25 = PORT_PCR_MUX(5) | PORT_PCR_ODE(1);


  PORTB_PCR0 = PORT_PCR_MUX(2) | PORT_PCR_ODE(1);
  PORTB_PCR1 = PORT_PCR_MUX(2) | PORT_PCR_ODE(1);
  // _todo ML#10.03 configure i2c clock (frequency divider register) to 400 kHz
  I2Cx_F = 0x1D;  // 60 MHz/(1*160) = 375kHz => MULT=0, ICR=1D

  // _todo ML#10.04 enable i2c bus
  I2Cx_C1 |= I2C_C1_IICEN_MASK;
}
