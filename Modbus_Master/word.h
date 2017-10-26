/**
@file
Utility Functions for Manipulating Words

@defgroup util_word "util/word.h": Utility Functions for Manipulating Words
@code#include "util/word.h"@endcode

This header file provides utility functions for manipulating words.

*/
/*

  word.h - Utility Functions for Manipulating Words

  This file is part of ModbusMaster.

  ModbusMaster is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ModbusMaster is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ModbusMaster.  If not, see <http://www.gnu.org/licenses/>.

  Written by Doc Walker (Rx)
  Copyright © 2009-2015 Doc Walker <4-20ma at wvfans dot net>

*/


#ifndef _UTIL_WORD_H_
#define _UTIL_WORD_H_

#include "bsp_board.h"

/** @ingroup util_word
    Return low word of a 32-bit integer.

    @param uint32_t ww (0x00000000..0xFFFFFFFF)
    @return low word of input (0x0000..0xFFFF)
*/
static inline uint16_t lowWord(uint32_t ww)
{
  return (uint16_t) ((ww) & 0xFFFF);
}


/** @ingroup util_word
    Return high word of a 32-bit integer.

    @param uint32_t ww (0x00000000..0xFFFFFFFF)
    @return high word of input (0x0000..0xFFFF)
*/
static inline uint16_t highWord(uint32_t ww)
{
  return (uint16_t) ((ww) >> 16);
}


/*模拟ardunio函数*************************************************/
static inline uint8_t lowByte(uint16_t ww)
{
  return (uint8_t) ((ww) & 0x00FF);
}


static inline uint8_t highByte(uint16_t ww)
{
  return (uint8_t) ((ww) >> 8);
}

static inline uint16_t word(uint8_t H_Byte,uint8_t L_Byte)
{
	uint16_t word;
	word = (uint16_t)(H_Byte<<8);
	word = word + L_Byte;
  return word;
}

#define bitSet(value, bit)  ((value) |= (1UL << (bit))) 
#define bitClear(value, bit)  ((value) &= ~(1UL << (bit)))

#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bitRead(value, bit)  (((value) >> (bit)) & 0x01) 

#endif /* _UTIL_WORD_H_ */
