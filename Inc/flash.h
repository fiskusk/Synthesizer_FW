#ifndef _FLASH_H
#define _FLASH_H

#include <stdint.h>
#include <max2871.h>

#define USER_DATA_ADDRESS_SECTION 0x08007C00

extern __attribute__((__section__(".user_data"))) uint32_t saved_data_1[7];
extern __attribute__((__section__(".user_data"))) uint32_t saved_data_2[7];
extern __attribute__((__section__(".user_data"))) uint32_t saved_data_3[7];
extern __attribute__((__section__(".user_data"))) uint32_t saved_data_4[7];

#define DEF_MEMORY1_REG0 "80C80008"
#define DEF_MEMORY1_REG1 "800003E9"
#define DEF_MEMORY1_REG2 "18004142"
#define DEF_MEMORY1_REG3 "00001F23"
#define DEF_MEMORY1_REG4 "63BE80FC"
#define DEF_MEMORY1_REG5 "00400005"
#define DEF_MEMORY1_REG6 "1"

#define DEF_MEMORY2_REG0 "00C80008"
#define DEF_MEMORY2_REG1 "A00003E9"
#define DEF_MEMORY2_REG2 "18005E42"
#define DEF_MEMORY2_REG3 "00001F23"
#define DEF_MEMORY2_REG4 "63BE80FC"
#define DEF_MEMORY2_REG5 "00400005"
#define DEF_MEMORY2_REG6 "1"

#define DEF_MEMORY3_REG0 "00C80010"
#define DEF_MEMORY3_REG1 "A00003E9"
#define DEF_MEMORY3_REG2 "18005E42"
#define DEF_MEMORY3_REG3 "00001F23"
#define DEF_MEMORY3_REG4 "63BE80FC"
#define DEF_MEMORY3_REG5 "00400005"
#define DEF_MEMORY3_REG6 "1"

#define DEF_MEMORY4_REG0 "00C80018"
#define DEF_MEMORY4_REG1 "A00003E9"
#define DEF_MEMORY4_REG2 "18005E42"
#define DEF_MEMORY4_REG3 "00001F23"
#define DEF_MEMORY4_REG4 "63BE80FC"
#define DEF_MEMORY4_REG5 "00400005"
#define DEF_MEMORY4_REG6 "1"

extern void myFLASH_PageErase(uint32_t address);
extern void write_complete_data_to_flash(uint8_t possition, char *val0,
                                         char *val1, char *val2, char *val3,
                                         char *val4, char *val5, char *val6);
extern void flash_send_stored_data(void);
extern void apply_memory_select_changed(plo_new_data_t plo_write_type);
extern void load_default_memory_register_values(void);

#endif