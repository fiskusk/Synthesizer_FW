#ifndef _FLASH_H
#define _FLASH_H

#include <stdint.h>

extern __attribute__((__section__(".user_data"))) uint32_t saved_data_1[7];
extern __attribute__((__section__(".user_data"))) uint32_t saved_data_2[7];
extern __attribute__((__section__(".user_data"))) uint32_t saved_data_3[7];
extern __attribute__((__section__(".user_data"))) uint32_t saved_data_4[7];

extern void myFLASH_PageErase(uint32_t address);
extern void write_complete_data_to_flash(uint8_t possition, char *val0, char *val1, char *val2, char *val3, char *val4, char *val5, char *val6);
extern void flash_send_stored_data(void);
extern void memory_select_check(void);



#endif