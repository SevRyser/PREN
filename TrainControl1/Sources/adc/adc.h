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
 * $Id: adc.h 113 2018-05-07 08:27:36Z zajost $
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_UTILS_ADC_H_
#define SOURCES_UTILS_ADC_H_

#define ADC_CH_DP0                 0
#define ADC_CH_DM0                19
#define ADC_CH_TEMPERATURE        26


uint16_t adcGet16BitValue(uint8_t channel);
uint32_t adcGetVoltage(uint8_t channel);
int16_t adcGetTemperature(void);
uint16_t adcGetCurrent(void);
uint16_t adcGetBatVoltage(void);
void adcInit(void);

#endif /* SOURCES_UTILS_ADC_H_ */
