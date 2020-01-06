#include "format.h"

uint32_t hex2int(char *hex) 
{
    uint32_t val = 0;
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++; 
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
        // shift 4 to make space for new digit, and add the 4 bits of the new digit 
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

/*************************************************************************/
uint32_t lsb_to_msb_bit_reversal(uint32_t input)
{
    input = (((input & 0xaaaaaaaa) >> 1) | ((input & 0x55555555) << 1));
    input = (((input & 0xcccccccc) >> 2) | ((input & 0x33333333) << 2));
    input = (((input & 0xf0f0f0f0) >> 4) | ((input & 0x0f0f0f0f) << 4));
    input = (((input & 0xff00ff00) >> 8) | ((input & 0x00ff00ff) << 8));
    return ((input >> 16) | (input << 16));
}