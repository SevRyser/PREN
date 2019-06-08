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

#ifndef SOURCES_FTM0_H_
#define SOURCES_FTM0_H_

#define FTM0_CLOCK              250000  // 250 kHz
#define FTM0_MODULO             5000 // dec=4095 -> 50Hz

void ftm0Init(void);



#endif /* SOURCES_FTM0_H_ */
