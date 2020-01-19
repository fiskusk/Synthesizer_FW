/**
  * @file       flash.c
  * @author     Filip Klapil (klapil.filip@seznam.cz)
  * @brief      This file contains functions related user data storage into flash.
  * @version    0.1
  * @date       2020-01-18
  * 
  * @copyright  Copyright (c) 2020
  * 
  */

#include "flash.h"
#include "gpio.h"
#include "format.h"
#include "usbd_cdc_if.h"
#include "stm32f0xx_it.h"
#include "max2871.h"
#include "timer.h"

/** @defgroup Stored_Data_Variables
  * @brief  stored settings for PLO
  * @note   saved_data_1[0] ... saved_data_1[5] are appropriate registers 
  *         for MAX2871 x is possition of one of four memories.
  *         saved_data_1[6] contains other setting for module as a whole
  *         saved_data_1[6] 0. bit is enable output 1 (1 - en, 0 - dis.)
  *         saved_data_1[6] 1. bit is enable output 2 (1 - en, 0 - dis.)
  *         saved_data_1[6] 2. bit external (1) or internal (0) freq. reference
  * @{
  */
__attribute__((__section__(".user_data"))) uint32_t saved_data_1[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_2[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_3[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_4[7];
/**
  * @}
  */

/**
  * @brief Use this function to control PLO module periferials (outputs, ref switch)
  * 
  * @param control_register: input control register value
  */
void change_plo_module_states(uint32_t control_register)
{
    (control_register & (1 << 0)) ? PLO_MODULE_OUT1_ON : PLO_MODULE_OUT1_OFF;
    ((control_register & (1 << 1)) >> 1) ? PLO_MODULE_OUT2_ON : PLO_MODULE_OUT2_OFF;
    ((control_register & (1 << 2)) >> 2) ? PLO_MODULE_EXT_REF : PLO_MODULE_INT_REF;
}

/**
  * @brief  Use for page memory erase
  * @note   https://www.st.com/content/ccc/resource/technical/document/reference_manual/c2/f8/8a/f2/18/e6/43/96/DM00031936.pdf/files/DM00031936.pdf/jcr:content/translations/en.DM00031936.pdf
  *         page 55
  *         https://community.st.com/s/question/0D50X00009XkfIO/stm32f0-help-with-flash-to-read-and-write-hal-libraries
  * @param address The starting address of the page to delete
  */
void myFLASH_PageErase(uint32_t address)
{
    HAL_FLASH_Unlock();     // first of all, you must unlock access to flash 
    FLASH->CR |= FLASH_CR_PER;                  /* (1) */
    FLASH->AR = address;                        /* (2) */
    FLASH->CR |= FLASH_CR_STRT;                 /* (3) */
    while ((FLASH->SR & FLASH_SR_BSY) != 0)     /* (4) */
    if ((FLASH->SR & FLASH_SR_EOP) != 0)        /* (5) */
    {
        FLASH->SR |= FLASH_SR_EOP;              /* (6)*/
    }
    FLASH->CR &= ~FLASH_CR_PER;                 /* (7) */
    HAL_FLASH_Lock();       // finnaly lock the flash memory
}

/**
  * @brief Function write input data into data flash part
  * 
  * @param position Specifies which of the four memories the data is intended for. (1-4)
  * @param index Specifies which registry the data applies to. (0-6)
  * @param data 32-bit unsigned integer data value to write
  */
void write_data_to_flash(uint8_t position, uint8_t index, uint32_t data)
{
    HAL_FLASH_Unlock();     // First of all unlock flash partition access

    switch (position)
    {
    case 1:
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(&saved_data_1[index]), data);
        break;
    case 2:
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(&saved_data_2[index]), data);
        break;
    case 3:
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(&saved_data_3[index]), data);
        break;
    case 4:
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(&saved_data_4[index]), data);
        break;
    }
    HAL_FLASH_Lock();       // lock flash partition
}

/**
  * @brief  The function writes the complete set of registers (0-6) 
  *         that enter the function to the memory location (1-4).
  * 
  * @param possition:    One of 4 possible memory positions
  * @param val0:         MAX2871 Register 0
  * @param val1:         MAX2871 Register 1
  * @param val2:         MAX2871 Register 2
  * @param val3:         MAX2871 Register 3
  * @param val4:         MAX2871 Register 4
  * @param val5:         MAX2871 Register 5
  * @param val6:         PLO module control register value
  */
void write_complete_data_to_flash(uint8_t possition, char *val0,
                                  char *val1, char *val2, char *val3,
                                  char *val4, char *val5, char *val6)
{
    write_data_to_flash(possition, 0, hex2int(val0));
    write_data_to_flash(possition, 1, hex2int(val1));
    write_data_to_flash(possition, 2, hex2int(val2));
    write_data_to_flash(possition, 3, hex2int(val3));
    write_data_to_flash(possition, 4, hex2int(val4));
    write_data_to_flash(possition, 5, hex2int(val5));
    write_data_to_flash(possition, 6, hex2int(val6));
}

/**
  * @brief This function apply memory select according to jumper possition settings
  * 
  * @param plo_write_type Type of write to MAX2871. PLO_NEW_DATA or PLO_INIT
  */
void apply_memory_select_changed(plo_new_data_t plo_write_type)
{
    if ((tick_handle == TICK_OCCUR) || (plo_write_type == PLO_INIT))
    {
        // Get jumper settings and switch-case apply changes
        switch (((1 << 0) & ~HAL_GPIO_ReadPin(JP2_GPIO_Port, JP2_Pin)) | \
            (((1 << 0) & ~HAL_GPIO_ReadPin(JP1_GPIO_Port, JP1_Pin)) << 1))
        {
        case 0:
            plo_write(saved_data_1, plo_write_type);
            change_plo_module_states(saved_data_1[6]);
            break;
        case 1:
            plo_write(saved_data_2, plo_write_type);
            change_plo_module_states(saved_data_2[6]);
            break;
        case 2:
            plo_write(saved_data_3, plo_write_type);
            change_plo_module_states(saved_data_3[6]);
            break;
        case 3:
            plo_write(saved_data_4, plo_write_type);
            change_plo_module_states(saved_data_4[6]);
            break;
        default:
            plo_write(saved_data_1, plo_write_type);
            change_plo_module_states(saved_data_1[6]);
            break;
        }
        memory_select_event = MEMORY_SELECT_WAIT;
    }
}

/**
  * @brief  Use this function to load the default register values, if necessary.
  *         For example, after programming, the user data partition is empty.
  *         Alternatively, if the user cleared the memory settings by USB
  *         command, the default values will be loaded after startup.
  * 
  */
void load_default_memory_register_values(void)
{
    myFLASH_PageErase(USER_DATA_ADDRESS_SECTION);
    write_complete_data_to_flash(1, DEF_MEMORY1_REG0, DEF_MEMORY1_REG1,
                                 DEF_MEMORY1_REG2, DEF_MEMORY1_REG3,
                                 DEF_MEMORY1_REG4, DEF_MEMORY1_REG5,
                                 DEF_MEMORY1_REG6);
    write_complete_data_to_flash(2, DEF_MEMORY2_REG0, DEF_MEMORY2_REG1,
                                 DEF_MEMORY2_REG2, DEF_MEMORY2_REG3,
                                 DEF_MEMORY2_REG4, DEF_MEMORY2_REG5,
                                 DEF_MEMORY2_REG6);
    write_complete_data_to_flash(3, DEF_MEMORY3_REG0, DEF_MEMORY3_REG1,
                                 DEF_MEMORY3_REG2, DEF_MEMORY3_REG3,
                                 DEF_MEMORY3_REG4, DEF_MEMORY3_REG5,
                                 DEF_MEMORY3_REG6);
    write_complete_data_to_flash(4, DEF_MEMORY4_REG0, DEF_MEMORY4_REG1,
                                 DEF_MEMORY4_REG2, DEF_MEMORY4_REG3,
                                 DEF_MEMORY4_REG4, DEF_MEMORY4_REG5,
                                 DEF_MEMORY4_REG6);
}

/**
  * @brief This function sends back the stored data in the user memory to the serial link.
  * 
  */
void flash_send_stored_data(void)
{
    printf("stored_data_1 %08x %08x %08x %08x %08x %08x %08x\r",
           (unsigned int)(saved_data_1[0]), (unsigned int)(saved_data_1[1]),
           (unsigned int)(saved_data_1[2]), (unsigned int)(saved_data_1[3]),
           (unsigned int)(saved_data_1[4]), (unsigned int)(saved_data_1[5]),
           (unsigned int)(saved_data_1[6]));
    printf("stored_data_2 %08x %08x %08x %08x %08x %08x %08x\r",
           (unsigned int)(saved_data_2[0]), (unsigned int)(saved_data_2[1]),
           (unsigned int)(saved_data_2[2]), (unsigned int)(saved_data_2[3]),
           (unsigned int)(saved_data_2[4]), (unsigned int)(saved_data_2[5]),
           (unsigned int)(saved_data_2[6]));
    printf("stored_data_3 %08x %08x %08x %08x %08x %08x %08x\r",
           (unsigned int)(saved_data_3[0]), (unsigned int)(saved_data_3[1]),
           (unsigned int)(saved_data_3[2]), (unsigned int)(saved_data_3[3]),
           (unsigned int)(saved_data_3[4]), (unsigned int)(saved_data_3[5]),
           (unsigned int)(saved_data_3[6]));
    printf("stored_data_4 %08x %08x %08x %08x %08x %08x %08x\r",
           (unsigned int)(saved_data_4[0]), (unsigned int)(saved_data_4[1]),
           (unsigned int)(saved_data_4[2]), (unsigned int)(saved_data_4[3]),
           (unsigned int)(saved_data_4[4]), (unsigned int)(saved_data_4[5]),
           (unsigned int)(saved_data_4[6]));
}