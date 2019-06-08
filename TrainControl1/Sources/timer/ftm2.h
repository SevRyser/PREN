/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Common settings of the FTM0
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          26.03.20018
 *
 * $Id: ftm0.h 94 2018-04-25 11:07:33Z zajost $
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_UTILS_FTM2_H_
#define SOURCES_UTILS_FTM2_H_

#define FTM2_CLOCK              60000000  // 60 MHz
#define FTM2_MODULO             0x0FFF

void ftm2Init(void);

#endif /* SOURCES_UTILS_FTM2_H_ */
