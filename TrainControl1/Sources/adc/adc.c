/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         ADC-Driver for the MC-Car
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          23.04.2018
 *
 * $Id: adcML.c 113 2018-05-07 08:27:36Z zajost $
 *
 *--------------------------------------------------------------------
 */

#include "platform.h"
#include "adc.h"
#include "util.h"
#if !SOLUTION


#define REFERENCE_VOLTAGE    1195000  // 1'195'000uV = 1.195V


/**
 * Performs one A/D conversion for the specified channel with 16 bit resolution.
 * The function blocks until the conversion has been finished.
 *
 * @param[in]
 *   the channel number to convert
 * @return
 *   a 16bit value
 */
uint16_t adcGet16BitValue(uint8_t channel)
{
  // _todo ML#11.06 implement the function code as follows:
  // 1. start a conversion
  // 2. wait until conversion has been finished
  // 3. return the result.
  ADC1_SC1A = channel;
  while (!(ADC1_SC1A & ADC_SC1_COCO_MASK)){}  // wait until conversion has been finished
  return ADC1_RA;
}


/**
 * Performs a A/D conversation and returns the measured voltage of this channel.
 * The maximum voltage is the REFERENCE_VOLTAGE defined above.
 *
 * @param[in]
 *   the channel number to convert
 * @return
 *   the measured voltage in uV (0..REFERENCE_VOLTAGE)
 */
uint32_t adcGetVoltage(uint8_t channel)
{
  // _todo ML#11.07 calculate the measured voltage
  // 1. perform a conversion of the desired channel
  // 2. calculate and return the voltage in uV.
  uint16_t value = adcGet16BitValue(channel);
  uint64_t uV = ((uint64_t)REFERENCE_VOLTAGE * value) / 65535;
  return (uint32_t)uV;
}


/**
 * Reads and returns the temperature of the microcontroller
 *
 * @returns
 *   the temperature in 0.1°C => 237 = 23.7°C
 */
int16_t adcGetTemperature(void)
{
  // _todo ML#11.08 determine the temperature of the microcontroller
  // 1. perform a adc conversion
  // 2. calculate the temperatur. KRM804 & MK22 Datasheet p42
  int32_t vTemp = adcGetVoltage(ADC_CH_TEMPERATURE);
  int16_t temp = 250 - ((vTemp - 716000) / 162);
  return temp;
}


/**
 * Initializes the ADC1.
 */
void adcInit(void)
{
  uint16_t calib;

  // init voltage reference module
  SIM_SCGC4 |= SIM_SCGC4_VREF_MASK;           // clock gating
  VREF_TRM |= VREF_TRM_CHOPEN(1);             // Chop oscillator enable
  VREF_SC = VREF_SC_VREFEN(1)                 // internal voltage reference enable
      | VREF_SC_ICOMPEN(1)                    // second order curvature compensation enable
      | VREF_SC_MODE_LV(1);                   // buffer mode: high power
  utilWaitUs(1);                              // Wait >300ns
  VREF_SC |= VREF_SC_REGEN(1);                // 1.75V regulator enable
  utilWaitUs(35000);                          // Wait 35ms

  // init adc

  // _todo ML#11.02 configure clock gating
  SIM_SCGC6 |= SIM_SCGC6_ADC1_MASK;

  // _todo ML#11.03 configure adc as follows:
  // set the ADCK to 7.5 MHz using the busClock
  // and configure 16 bit conversion with long sample time
  ADC1_CFG1 = ADC_CFG1_ADIV(3)                // Div 8 => ADCK = 60MHz/8 = 7.5MHz
      | ADC_CFG1_ADLSMP(1)                    // Long sample time
      | ADC_CFG1_MODE(3)                      // 16 bit conversion
      | ADC_CFG1_ADICLK(0);                   // clock = Busclock
  ADC1_CFG2 = 0;

  // _todo ML#11.04 select the alternate reference source (VREF)
  ADC1_SC2 = ADC_SC2_REFSEL(0x01);                            // alternate reference source (VREF)

  // _todo ML#11.05 configure hardware average of 32 samples
  ADC1_SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(0x03));        // hardware average enable, 32 samples averaged

  // adc calibration
  ADC1_SC3 |= ADC_SC3_CAL(1);                 // Start calibration
  while (!(ADC1_SC1A & ADC_SC1_COCO_MASK)){}  // wait until calibration has been finished

  // set calib data
  calib = ADC1_CLP0 + ADC1_CLP1 + ADC1_CLP2 + ADC1_CLP3 + ADC1_CLP4 + ADC1_CLPS;
  calib = calib / 2;
  calib = calib | 0x8000;
  ADC1_PG = calib;

  calib = ADC1_CLM0 + ADC1_CLM1 + ADC1_CLM2 + ADC1_CLM3 + ADC1_CLM4 + ADC1_CLMS;
  calib = calib / 2;
  calib = calib | 0x8000;
  ADC1_MG = calib;
}
#endif
