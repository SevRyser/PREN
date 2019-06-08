/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         terminal program
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          19.03.2018
 *
 * $Id: term.h 94 2018-04-25 11:07:33Z zajost $
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_TERM_H_
#define SOURCES_TERM_H_

typedef tError (*cmdHandler)(const unsigned char *cmd);

typedef struct commandLineHandler
{
  char cmd[16];
  char cmdDesc[32];
  cmdHandler cmdHandler;
  struct commandLineHandler *next;
} tCommandLineHandler;

void termRegisterCommandLineHandler(tCommandLineHandler *clh, unsigned char* cmd, unsigned char *cmdDesc, cmdHandler h);

void termWrite(const char *str);
void termWriteChar(char ch);
void termWriteNum32s(int32_t value);
void termWriteNum16s(int16_t value);

void termWriteLine(const char *str);
void termDoWork(void);
void termInit(uint16_t baudrate);


#endif /* SOURCES_TERM_H_ */
