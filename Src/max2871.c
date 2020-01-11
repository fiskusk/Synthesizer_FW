#include "max2871.h"
#include "gpio.h"
#include "format.h"
#include "usbd_cdc_if.h"

volatile plo_lock_state_t plo_lock_state = PLO_LOCK_STATE_WAIT;
volatile plo_new_data_t plo_new_data = PLO_DATA_SENDED;

/*****************************************************************************/
void plo_spi_emul(uint32_t data)
{
    for (uint8_t j = 0; j < 32; j++) 
    {
        (data & 0x01) ? PLO_DATA_SET : PLO_DATA_RESET;

        // generate clock pulse and shift next
        PLO_CLK_SET;
        data >>= 1;
        PLO_CLK_RESET;
    }
    // generate update pulse
    PLO_LE_SET;
    PLO_LE_RESET;
    // get down data wire
    PLO_DATA_RESET;
}

/*****************************************************************************/
void plo_write_register(uint32_t register_data)
{
    register_data = lsb_to_msb_bit_reversal(register_data);
    plo_spi_emul(register_data);
}

/*****************************************************************************/
void plo_write_all(uint32_t *max2871, plo_new_data_t plo_write_type)
{
    for (int8_t i = 5; i >= 0; i--)
    {
        if (plo_write_type == PLO_OUT_ENABLE)
            i=4;

        uint32_t c = max2871[i];

        if ((plo_write_type == PLO_INIT) && (i==4))
            c &= ~((1UL << 5) | (1UL << 9 ));

        plo_spi_emul(lsb_to_msb_bit_reversal(c));

        if ((plo_write_type == PLO_INIT) && (i==5))
            HAL_Delay(20);

        if (plo_write_type == PLO_OUT_ENABLE)
            i=-1;
    }  
}

void plo_check_lock_status(void)
{
    if ( ( (test_data[2] & ((1<<28) | (1<<27) | (1<<26))) >> 26) == 0b110)
        (HAL_GPIO_ReadPin(PLO_MUXOUT_GPIO_Port, PLO_MUXOUT_Pin) == 1) ?
            (plo_lock_state = PLO_LOCKED) : (plo_lock_state = PLO_UNLOCKED);
    else
        plo_lock_state = PLO_LOCK_STATE_UNKNOWN;
}
