#include "flash.h"
#include "gpio.h"
#include "format.h"
#include "usbd_cdc_if.h"


__attribute__((__section__(".user_data"))) uint32_t saved_data_1[6];
__attribute__((__section__(".user_data"))) uint32_t saved_data_2[6];
__attribute__((__section__(".user_data"))) uint32_t saved_data_3[6];
__attribute__((__section__(".user_data"))) uint32_t saved_data_4[6];

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
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, &saved_data_1[index], data);
            break;
        case 2:
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, &saved_data_2[index], data);
            break;
        case 3:
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, &saved_data_3[index], data);
            break;
        case 4:
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, &saved_data_4[index], data);
            break;
    }
    HAL_FLASH_Lock();
}

void write_complete_data_to_flash(uint8_t possition, char *val0, char *val1, char *val2, char *val3, char *val4, char *val5)
{
    write_data_to_flash(possition, 0, hex2int(val0));
    write_data_to_flash(possition, 1, hex2int(val1));
    write_data_to_flash(possition, 2, hex2int(val2));
    write_data_to_flash(possition, 3, hex2int(val3));
    write_data_to_flash(possition, 4, hex2int(val4));
    write_data_to_flash(possition, 5, hex2int(val5));
}