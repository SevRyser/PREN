/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Common settings of the FTM3
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          03.04.20018
 *
 * $Id: ftm3.h 94 2018-04-25 11:07:33Z zajost $
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_FTM3_H_
#define SOURCES_FTM3_H_

#define FTM3_CLOCK              60000000  // 60 MHz
#define FTM3_MODULO             0x0DFF  // 4095

void ftm3Init(void);



#endif /* SOURCES_FTM3_H_ */
