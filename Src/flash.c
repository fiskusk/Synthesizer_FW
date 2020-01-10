#include "flash.h"
#include "gpio.h"
#include "format.h"
#include "usbd_cdc_if.h"
#include "stm32f0xx_it.h"
#include "max2871.h"
#include "timer.h"

/*****************************************************************************/
/* saved_data_x[0] ... saved_data_x[5] are appropriate registers for MAX2871 */
/* x is possition of one of four memories.                                   */
/* saved_data_x[6] contains other setting for module as a whole              */
/* saved_data_x[6] 0. bit is enable output 1 (1 - en, 0 - dis.)              */
/* saved_data_x[6] 1. bit is enable output 2 (1 - en, 0 - dis.)              */
/* saved_data_x[6] 2. bit external (1) or internal (0) freq. reference       */
/*****************************************************************************/
__attribute__((__section__(".user_data"))) uint32_t saved_data_1[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_2[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_3[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_4[7];

void myFLASH_PageErase(uint32_t address)
{
    HAL_FLASH_Unlock();
    FLASH->CR |= FLASH_CR_PER; /* (1) */
    FLASH->AR = address; /* (2) */
    FLASH->CR |= FLASH_CR_STRT; /* (3) */
    while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (4) */
    if ((FLASH->SR & FLASH_SR_EOP) != 0) /* (5) */
    {
        FLASH->SR |= FLASH_SR_EOP; /* (6)*/
    }
    FLASH->CR &= ~FLASH_CR_PER; /* (7) */
    HAL_FLASH_Lock();
}

void write_data_to_flash(uint8_t position, uint32_t index, uint32_t data)
{
    HAL_FLASH_Unlock();

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
    HAL_FLASH_Lock();
}

void write_complete_data_to_flash(uint8_t possition, char *val0, char *val1, char *val2, char *val3, char *val4, char *val5, char *val6)
{
    write_data_to_flash(possition, 0, hex2int(val0));
    write_data_to_flash(possition, 1, hex2int(val1));
    write_data_to_flash(possition, 2, hex2int(val2));
    write_data_to_flash(possition, 3, hex2int(val3));
    write_data_to_flash(possition, 4, hex2int(val4));
    write_data_to_flash(possition, 5, hex2int(val5));
    write_data_to_flash(possition, 6, hex2int(val6));
}

void memory_select_check(void)
{
    if (tick_handle == TICK_OCCUR)
    {
        volatile uint8_t bit0 = HAL_GPIO_ReadPin(JP2_GPIO_Port, JP2_Pin);
        volatile uint8_t bit1 = HAL_GPIO_ReadPin(JP1_GPIO_Port, JP1_Pin);
        bit0 = (1<<0) & ~bit0;
        bit1 = (1<<0) & ~bit1;
        volatile uint8_t jp_select_bits = bit0 | (bit1 << 1);
        switch (jp_select_bits)
        {
            case 0:
                memory_select_state = MEMORY_1_SELECTED;
                plo_write_all(saved_data_1, PLO_INIT);
                plo_write_all(saved_data_1, PLO_INIT);
                plo_write_all(saved_data_1, PLO_OUT_ENABLE);
                break;
            case 1:
                memory_select_state = MEMORY_2_SELECTED;
                plo_write_all(saved_data_2, PLO_INIT);
                plo_write_all(saved_data_2, PLO_INIT);
                plo_write_all(saved_data_2, PLO_OUT_ENABLE);
                break;
            case 2:
                memory_select_state = MEMORY_3_SELECTED;
                plo_write_all(saved_data_3, PLO_INIT);
                plo_write_all(saved_data_3, PLO_INIT);
                plo_write_all(saved_data_3, PLO_OUT_ENABLE);
                break;
            case 3:
                memory_select_state = MEMORY_4_SELECTED;
                plo_write_all(saved_data_4, PLO_INIT);
                plo_write_all(saved_data_4, PLO_INIT);
                plo_write_all(saved_data_4, PLO_OUT_ENABLE);
                break;
            default:
                memory_select_state = MEMORY_1_SELECTED;
                plo_write_all(saved_data_1, PLO_INIT);
                plo_write_all(saved_data_1, PLO_INIT);
                plo_write_all(saved_data_1, PLO_OUT_ENABLE);
                break;
        }
        memory_select_event = MEMORY_SELECT_WAIT;
    }
}