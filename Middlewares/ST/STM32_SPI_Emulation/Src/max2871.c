#include "max2871.h"
#include "gpio.h"

volatile plo_new_data_t plo_new_data = PLO_DATA_SENDED;

/*************************************************************************/
uint32_t lsb_to_msb_bit_reversal(uint32_t input)
{
    input = (((input & 0xaaaaaaaa) >> 1) | ((input & 0x55555555) << 1));
    input = (((input & 0xcccccccc) >> 2) | ((input & 0x33333333) << 2));
    input = (((input & 0xf0f0f0f0) >> 4) | ((input & 0x0f0f0f0f) << 4));
    input = (((input & 0xff00ff00) >> 8) | ((input & 0x00ff00ff) << 8));
    return ((input >> 16) | (input << 16));
}

/*************************************************************************/
void plo_spi_emul(uint32_t data)
{
    for (uint8_t j = 0; j < 32; j++) {
        if (data & 0x01)
            HAL_GPIO_WritePin(PLO_DATA_GPIO_Port, PLO_DATA_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(PLO_DATA_GPIO_Port, PLO_DATA_Pin, GPIO_PIN_RESET);
        
        // generate clock pulse and shift next
        HAL_GPIO_WritePin(PLO_CLK_GPIO_Port, PLO_CLK_Pin, GPIO_PIN_SET);
        data >>= 1;
        HAL_GPIO_WritePin(PLO_CLK_GPIO_Port, PLO_CLK_Pin, GPIO_PIN_RESET);
    }
    // generate update pulse
    HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET);
    // set down all wires
    HAL_GPIO_WritePin(PLO_DATA_GPIO_Port, PLO_DATA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PLO_CLK_GPIO_Port, PLO_CLK_Pin, GPIO_PIN_RESET);
}

/*************************************************************************/
void plo_write_register(uint32_t register_data)
{
    register_data = lsb_to_msb_bit_reversal(register_data);

    plo_spi_emul(register_data);
}

/*************************************************************************/
void plo_write_all(uint32_t *max2871, plo_new_data_t plo_write_type)
{
    for (int8_t i = 5; i >= 0; i--) {
        if (plo_write_type == PLO_OUT_ENABLE)
        {
            i=4;
        }

        uint32_t c = max2871[i];
        if ((plo_write_type == PLO_INIT) && (i==4))
        {
            c &= ~((1UL << 5) | (1UL << 9 ));
        }
        c = lsb_to_msb_bit_reversal(c);

        plo_spi_emul(c);

        if ((plo_write_type == PLO_INIT) && (i==5))
            HAL_Delay(20);

        if (plo_write_type == PLO_OUT_ENABLE)
            i=-1;
    }  
}
