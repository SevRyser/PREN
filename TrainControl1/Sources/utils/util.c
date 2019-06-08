/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         util collection
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          26.03.20018
 *
 * $Id: util.c 102 2018-05-01 14:06:17Z zajost $
 *
 * This file contains as well as modified functions from the utility
 * component of processor expert:
 *
 * Copyright(c) 2014 - 2017, Erich Styger
 * Web : https://mcuoneclipse.com
 * SourceForge: https://sourceforge.net/projects/mcuoneclipse
 * Git : https ://github.com/ErichStyger/McuOnEclipse_PEx
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met :
 *
 * -Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and / or
 * other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *--------------------------------------------------------------------
 */

#include <stdlib.h>
#include "platform.h"
#include "util.h"



/**
 * copy the string src into dst. It performs the same task as strncpy, except
 * - always terminates the result string.
 * - does not zero out the remaining part in dst.
 *
 * Note: dstSize is the size of dst INCLUDING zero byte.
 * Precondition: src, dst != NULL
 *
 * @param[in,out] dst
 *    Start of buffer, where to append the number string
 * @param[in] dstSize
 *    The size of the buffer, including the zero byte
 * @param[in] src
 *    The source string to copy
 */
void utilStrcpy(uint8_t *dst, size_t dstSize, const uint8_t *src)
{
  dstSize--; /* for zero byte */
  while (dstSize > 0 && *src != '\0') {
    *dst++ = *src++;
    dstSize--;
  }
  *dst = '\0';
}



/**
 * Concat the string src into dst. Always terminates the result string.
 *   Note: count is the size of dst INCLUDING zero byte.
 *   Precondition: src, dst != NULL
 *
 * @param[in,out] dst
 *    Start of buffer, where to append the number string.
 * @param[in] dstSize
 *    The size of the buffer, including the zero byte.
 * @param[in] src
 *    The source string to add.
 */
void utilStrcat(uint8_t *dst, size_t dstSize, const uint8_t *src)
{
  dstSize--; /* for zero byte */
  /* point to the end of the source */
  while (dstSize > 0 && *dst != '\0') {
    dst++;
    dstSize--;
  }
  /* copy the src in the destination */
  while (dstSize > 0 && *src != '\0') {
    *dst++ = *src++;
    dstSize--;
  }
  /* terminate the string */
  *dst = '\0';
}


/**
 * Converts a 16bit unsigned number into a string.
 * @param[in,out] dst
 *    String buffer to store the number.
 * @param[in] dstSize
 *    Size of the destination buffer in bytes.
 * @param[in] num
 *    16bit unsigned number to convert.
 */
void utilNum16uToStr(uint8_t *dst, uint16_t dstSize, uint16_t num)
{
  unsigned char *ptr = ((unsigned char *)dst);
  unsigned char i=0, j;
  unsigned char tmp;

  dstSize--; /* for zero byte */
  if (num == 0 && dstSize > 0){
    ptr[i++] = '0';
    dstSize--;
  }
  while (num > 0 && dstSize > 0) {
    ptr[i++] = (unsigned char)((num % 10) + '0');
    dstSize--;
    num /= 10;
  }
  for(j=0; j<(i/2); j++) { /* swap buffer */
    tmp = ptr[j];
    ptr[j] = ptr[(i-j)-1];
    ptr[(i-j)-1] = tmp;
  }
  ptr[i] = '\0';
}


/**
 * Converts a 16bit signed number into a string.
 *
 * @param[in,out] dst String buffer to store the number.
 * @param[in] dstSize Size of the destination buffer in bytes.
 * @param[in] val 16bit signed number to convert.
 */
void utilNum16sToStr(uint8_t *dst, size_t dstSize, int16_t val)
{
  unsigned char *ptr =  ((unsigned char *)dst);
  unsigned char i=0, j;
  unsigned char tmp;
  unsigned char sign = (unsigned char)(val < 0);

  if (val==(int16_t)(0x8000)) { /* special case 0x8000/-32768: prevent overflow below. */
    utilStrcpy(dst, dstSize, (unsigned char*)"-32768");
    return;
  }

  dstSize--; /* for zero byte */
  if (sign) {
    val = (int16_t)(-val);
  }
  if (val == 0 && dstSize > 0){
    ptr[i++] = '0';
    dstSize--;
  }
  while (val > 0 && dstSize > 0) {
    ptr[i++] = (unsigned char)((val % 10) + '0');
    dstSize--;
    val /= 10;
  }
  if (sign && dstSize > 0){
    ptr[i++] = '-';
  }
  for(j=0; j<(i/2); j++) { /* swap buffer */
    tmp = ptr[j];
    ptr[j] = ptr[(i-j)-1];
    ptr[(i-j)-1] = tmp;
  }
  ptr[i] = '\0';
}



/**
 * Converts a 32bit unsigned number into a string.
 * @param[in,out] dst
 *    String buffer to store the number.
 * @param[in] dstSize
 *    Size of the destination buffer in bytes.
 * @param[in] num
 *    16bit unsigned number to convert.
 */
void utilNum32uToStr(uint8_t *dst, size_t dstSize, uint32_t num)
{
  unsigned char *ptr = ((unsigned char *)dst);
  unsigned char i=0, j;
  unsigned char tmp;

  dstSize--; /* for zero byte */
  if (num == 0 && dstSize > 0){
    ptr[i++] = '0';
    dstSize--;
  }
  while (num > 0 && dstSize > 0) {
    ptr[i++] = (unsigned char)((num % 10) + '0');
    dstSize--;
    num /= 10;
  }
  for(j=0; j<(i/2); j++) { /* swap buffer */
    tmp = ptr[j];
    ptr[j] = ptr[(i-j)-1];
    ptr[(i-j)-1] = tmp;
  }
  ptr[i] = '\0';
}


/**
 * Converts a 32bit signed number into a string.
 *
 * @param[in,out] dst String buffer to store the number.
 * @param[in] dstSize Size of the destination buffer in bytes.
 * @param[in] val 32bit signed number to convert.
 */
void utilNum32sToStr(uint8_t *dst, size_t dstSize, int32_t val)
{
  unsigned char *ptr = ((unsigned char *)dst);
  unsigned char i=0, j;
  unsigned char tmp;
  unsigned char sign = (unsigned char)(val < 0);

  if (val==(int32_t)(0x80000000)) { /* special case 0x80000000/-2147483648: prevent overflow below. */
    utilStrcpy(dst, dstSize, (unsigned char*)"-2147483648");
    return;
  }
  dstSize--; /* for zero byte */
  if (sign) {
    val = -val;
  }
  if (val == 0 && dstSize > 0){
    ptr[i++] = '0';
    dstSize--;
  }
  while (val > 0 && dstSize > 0) {
    ptr[i++] = (unsigned char)((val % 10) + '0');
    dstSize--;
    val /= 10;
  }
  if (sign && dstSize > 0){
    ptr[i++] = '-';
  }
  for(j=0; j<(i/2); j++) { /* swap buffer */
    tmp = ptr[j];
    ptr[j] = ptr[(i-j)-1];
    ptr[(i-j)-1] = tmp;
  }
  ptr[i] = '\0';
}



/**
 * appends a 16bit unsigned number to a string. Always terminates the result string.
 *   Note: count is the size of dst INCLUDING zero byte.
 *   Precondition: src, dst != NULL
 *
 * @param[in,out] dst
 *    Start of buffer, where to append the number string.
 * @param[in] dstSize
 *    The size of the buffer, including the zero byte.
 * @param[in] val
 *    The 16bit unsigned number to add.
 */
void utilStrcatNum16u(uint8_t *dst, size_t dstSize, uint16_t num)
{
  unsigned char buf[sizeof("32768")]; /* maximum buffer size we need */

  utilNum16uToStr(buf, sizeof(buf), num);
  utilStrcat(dst, dstSize, buf);
}


/**
 * appends a 16bit signed number to a string. Always terminates the result string.
 *   Note: count is the size of dst INCLUDING zero byte.
 *   Precondition: src, dst != NULL
 *
 * @param[in,out] dst
 *    Start of buffer, where to append the number string.
 * @param[in] dstSize
 *    The size of the buffer, including the zero byte.
 * @param[in] val
 *    The 16bit signed number to add.
 */
void utilStrcatNum16s(uint8_t *dst, size_t dstSize, int16_t num)
{
  unsigned char buf[sizeof("-32768")]; /* maximum buffer size we need */

  utilNum16sToStr(buf, sizeof(buf), num);
  utilStrcat(dst, dstSize, buf);
}


/**
 * appends a 32bit (unsigned long) number to a string. Always terminates the result string.
 * Note: count is the size of dst INCLUDING zero byte.
 * Precondition: src, dst != NULL
 *
 * @param[in,out] dst Start of string buffer, where to append the number string
 * @param[in] dstSize The size of the buffer, including the zero byte
 * @param[in] val The 32bit unsigned number to add
 */
void utilStrcatNum32u(uint8_t *dst, size_t dstSize, uint32_t val)
{
  unsigned char buf[sizeof("4294967295")]; /* maximum buffer size we need */

  utilNum32uToStr(buf, sizeof(buf), val);
  utilStrcat(dst, dstSize, buf);
}

/**
 * appends a 32bit (long) number to a string. Always terminates the result string.
 * Note: count is the size of dst INCLUDING zero byte.
 * Precondition: src, dst != NULL
 *
 * @param[in,out] dst Start of string buffer, where to append the number string
 * @param[in] dstSize The size of the buffer, including the zero byte
 * @param[in] val The 32bit number to add
 */
void utilStrcatNum32s(uint8_t *dst, size_t dstSize, int32_t val)
{
  unsigned char buf[sizeof("-4294967295")]; /* maximum buffer size we need */

  utilNum32sToStr(buf, sizeof(buf), val);
  utilStrcat(dst, dstSize, buf);
}


/**
 * Scans a decimal number, and stops at any non-number.
 * Number can have any preceding zeros or spaces.
 *
 * @param[inout] str
 *   the string to parse
 * @param[in] value
 *   pointer to a variable to store the value
 * @returns
 *   EC_SUCCESS: parsing succeeded
 *   EC_OVERFLOW: to many digits
 *   EC_INVALID_ARG: string doesn't starts with a digit or spaces
 */
tError utilScanDecimal16u(const unsigned char **str, uint16_t *value)
{
  const char *p = *str;
  uint8_t nofDigits = 5+1;

  while (*p == ' ') p++;  // skip leading spaces

  *value = 0;
  while (*p >= '0' && *p <= '9' && nofDigits > 0) {
    *value = (uint16_t)((*value) * 10 + *p-'0');
    nofDigits--;
    p++;
  }
  if (nofDigits == 0) return EC_OVERFLOW;
  if (nofDigits == 5+1) return EC_INVALID_ARG; // no digits at all?
  *str = p;
  return EC_SUCCESS;
}


/**
 * Scans a decimal number, and stops at any non-number.
 * Number can have any preceding zeros or spaces.
 *
 * @param[inout] str
 *   the string to parse
 * @param[in] value
 *   pointer to a variable to store the value
 * @returns
 *   EC_SUCCESS: parsing succeeded
 *   EC_OVERFLOW: to many digits
 *   EC_INVALID_ARG: string doesn't starts with a digit or spaces
 */
tError utilScanDecimal16s(const unsigned char **str, int16_t *value)
{
  const unsigned char *p = *str;
  bool isNeg;
  uint16_t val16u;
  uint8_t res;

  while (*p == ' ') p++;  // skip leading spaces

  *value = 0;
  if (*p=='-') {
    isNeg = TRUE;
    p++; /* skip minus */
  } else {
    isNeg = FALSE;
  }

  res = utilScanDecimal16u(&p, (uint16_t*)&val16u);
  if (res != EC_SUCCESS) return res;

  if (isNeg) {
    *value = (int16_t)(-(int16_t)val16u);
  } else {
    *value = (int16_t)val16u;
  }
  *str = p;
  return EC_SUCCESS;
}


/**
 * This functions waits for the specified time.
 *
 * Tested without Optimization and a CPU clock of 120MHz
 *
 * @param[in] us
 *   the desired time in microseconds.
 */
void utilWaitUs(uint16_t us)
{
  us--;
  while(us--) {
    uint8_t i = 13;
    while (i--) __asm("nop");
  }
}


/**
 * This function checks the size of the used stack
 *
 * @return information about the used and free stack size
 */
tStackData utilCheckStackSize(void)
{
  char* i;
  extern char __StackTop[];
  extern char __StackLimit[];
  int stackSize = __StackTop - __StackLimit;

  tStackData stackData;
  stackData.stackFree = 0;
  stackData.stackUsed = stackSize;
  for (i=__StackLimit; i<__StackTop; i++)
  {
    if (*i == 0x55) stackData.stackFree++;
    else break;
  }
  stackData.stackUsed -= stackData.stackFree;
  return stackData;
}
