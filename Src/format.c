/**
  * @file       format.c
  * @author     Filip Klapil (klapil.filip@seznam.cz)
  * @brief      It provides my formating functions. Include convert from hex
  *             char string to 32bit uint number, or lsb to msb reversal func
  * @version    0.1
  * @date       2020-01-18
  * 
  * @copyright  Copyright (c) 2020
  * 
  */

#include "format.h"

/**
  * @brief  This function convert 32bits hexadecimal number (max 8 hex digits)
  *         entered as a text string to 32bits unsigned integer number.
  * @note   https://stackoverflow.com/a/39052987
  * @param  hex: A hexadecimal number entered as a text string. 
  *              It supports both a-f and A-F characters.
  * @return uint32_t The resulting converted value.
  */
uint32_t hex2int(char *hex)
{
    uint32_t val = 0;
    while (*hex)
    {
        // get current character then increment
        uint8_t byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9')
            byte = byte - '0';
        else if (byte >= 'a' && byte <= 'f')
            byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <= 'F')
            byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

/**
  * @brief  Use this function to revers bits in a 32-bit number.
  *         Numbers of bits 3210 to 0123.
  * @note   https://stackoverflow.com/questions/7467997/reversing-the-bits-in-an-integer-x
  * @param  input: 32-bit unsigned integer number for swap
  * @return uint32_t 32-bit swaped number
  */
uint32_t lsb_to_msb_bit_reversal(uint32_t input)
{
    input = (((input & 0xaaaaaaaa) >> 1) | ((input & 0x55555555) << 1));
    input = (((input & 0xcccccccc) >> 2) | ((input & 0x33333333) << 2));
    input = (((input & 0xf0f0f0f0) >> 4) | ((input & 0x0f0f0f0f) << 4));
    input = (((input & 0xff00ff00) >> 8) | ((input & 0x00ff00ff) << 8));
    return ((input >> 16) | (input << 16));
}