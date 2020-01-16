#ifndef _USB_H
#define _USB_H

#include "stdio.h"

#define DEF_TEST_DATA_REG0 0x80C80008
#define DEF_TEST_DATA_REG1 0x800003E9
#define DEF_TEST_DATA_REG2 0x18004142
#define DEF_TEST_DATA_REG3 0x00001F23
#define DEF_TEST_DATA_REG4 0x63BE80FC
#define DEF_TEST_DATA_REG5 0x00400005

extern uint32_t test_data[6];

extern void usb_data_available(uint8_t c);
extern void usb_procesing_command_data(void);

uint32_t usb_process_command(char *command_data);

#endif