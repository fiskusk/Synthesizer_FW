/**
  * @file       flash.h
  * @author     Filip Klapil (klapil.filip@seznam.cz)
  * @brief      Header file flash storage operations
  * @version    0.1
  * @date       2020-01-18
  * 
  * @copyright  Copyright (c) 2020
  * 
  */

#ifndef _FLASH_H
#define _FLASH_H

#include <stdint.h>
#include <max2871.h>

/**< Point to the beginning of the user data area */
#define USER_DATA_ADDRESS_SECTION 0x08007C00

extern __attribute__((__section__(".user_data"))) uint32_t saved_data_1[7];
extern __attribute__((__section__(".user_data"))) uint32_t saved_data_2[7];
extern __attribute__((__section__(".user_data"))) uint32_t saved_data_3[7];
extern __attribute__((__section__(".user_data"))) uint32_t saved_data_4[7];

/** @defgroup   Default_Memories_Values
  * @brief      Default registers values section for memories 1-4
  * @{
  */

/** @defgroup   Default_Memory_1_Values
  * @brief      Default registers values section for memory 1
  * @{
  */
#define DEF_MEMORY1_REG0 "80C80008" /**< Default memory 1 register 0 value  */
#define DEF_MEMORY1_REG1 "800003E9" /**< Default memory 1 register 1 value  */
#define DEF_MEMORY1_REG2 "18004142" /**< Default memory 1 register 2 value  */
#define DEF_MEMORY1_REG3 "00001F23" /**< Default memory 1 register 3 value  */
#define DEF_MEMORY1_REG4 "63BE80FC" /**< Default memory 1 register 4 value  */
#define DEF_MEMORY1_REG5 "00400005" /**< Default memory 1 register 5 value  */
#define DEF_MEMORY1_REG6 "1"        /**< Default memory 1 control settings value  */
/**
  * @}
  */

/** @defgroup   Default_Memory_2_Values
  * @brief      Default registers values section for memory 2
  * @{
  */
#define DEF_MEMORY2_REG0 "00C80008" /**< Default memory 2 register 0 value  */
#define DEF_MEMORY2_REG1 "A00003E9" /**< Default memory 2 register 1 value  */
#define DEF_MEMORY2_REG2 "18005E42" /**< Default memory 2 register 2 value  */
#define DEF_MEMORY2_REG3 "00001F23" /**< Default memory 2 register 3 value  */
#define DEF_MEMORY2_REG4 "63BE80FC" /**< Default memory 2 register 4 value  */
#define DEF_MEMORY2_REG5 "00400005" /**< Default memory 2 register 5 value  */
#define DEF_MEMORY2_REG6 "1"        /**< Default memory 2 control settings value  */
/**
  * @}
  */

/** @defgroup   Default_Memory_3_Values
  * @brief      Default registers values section for memory 3
  * @{
  */
#define DEF_MEMORY3_REG0 "00C80010" /**< Default memory 3 register 0 value  */
#define DEF_MEMORY3_REG1 "A00003E9" /**< Default memory 3 register 1 value  */
#define DEF_MEMORY3_REG2 "18005E42" /**< Default memory 3 register 2 value  */
#define DEF_MEMORY3_REG3 "00001F23" /**< Default memory 3 register 3 value  */
#define DEF_MEMORY3_REG4 "63BE80FC" /**< Default memory 3 register 4 value  */
#define DEF_MEMORY3_REG5 "00400005" /**< Default memory 3 register 5 value  */
#define DEF_MEMORY3_REG6 "1"        /**< Default memory 3 control settings value  */
/**
  * @}
  */

/** @defgroup   Default_Memory_4_Values
  * @brief      Default registers values section for memory 4
  * @{
  */
#define DEF_MEMORY4_REG0 "00C80018" /**< Default memory 4 register 0 value  */
#define DEF_MEMORY4_REG1 "A00003E9" /**< Default memory 4 register 1 value  */
#define DEF_MEMORY4_REG2 "18005E42" /**< Default memory 4 register 2 value  */
#define DEF_MEMORY4_REG3 "00001F23" /**< Default memory 4 register 3 value  */
#define DEF_MEMORY4_REG4 "63BE80FC" /**< Default memory 4 register 4 value  */
#define DEF_MEMORY4_REG5 "00400005" /**< Default memory 4 register 5 value  */
#define DEF_MEMORY4_REG6 "1"        /**< Default memory 4 control settings value  */
/**
  * @}
  */

/**
  * @}
  */

extern void myFLASH_PageErase(uint32_t address);
extern void write_complete_data_to_flash(uint8_t possition, char *val0,
                                         char *val1, char *val2, char *val3,
                                         char *val4, char *val5, char *val6);
extern void flash_send_stored_data(void);
extern void apply_memory_select_changed(plo_new_data_t plo_write_type);
extern void load_default_memory_register_values(void);

#endif