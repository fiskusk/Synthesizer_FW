/**
  * @file       max2871.c
  * @author     Filip Klapil (klapil.filip@seznam.cz)
  * @brief      This file contains functions related PLO MAX2871.
  * @version    0.1
  * @date       2020-01-18
  * 
  * @copyright  Copyright (c) 2020
  * 
  */

#include "max2871.h"
#include "gpio.h"
#include "format.h"
#include "usb.h"

volatile plo_lock_state_t plo_lock_state = PLO_LOCK_STATE_WAIT;
volatile plo_new_data_t plo_new_data;

/** @defgroup   PLO_Synthesizer_SPI_controls
  * @brief      PLO Synthesizer SPI pins Controls Private Macros
  * @{
  */

/**< Sets the data wire to the high logic level     */
#define PLO_DATA_SET HAL_GPIO_WritePin(PLO_DATA_GPIO_Port, PLO_DATA_Pin, GPIO_PIN_SET)

/**< Sets the data wire to the low logic level      */
#define PLO_DATA_RESET HAL_GPIO_WritePin(PLO_DATA_GPIO_Port, PLO_DATA_Pin, GPIO_PIN_RESET)

/**< Sets the clock wire to the high logic level    */
#define PLO_CLK_SET HAL_GPIO_WritePin(PLO_CLK_GPIO_Port, PLO_CLK_Pin, GPIO_PIN_SET)

/**< Sets the clock wire to the low logic level     */
#define PLO_CLK_RESET HAL_GPIO_WritePin(PLO_CLK_GPIO_Port, PLO_CLK_Pin, GPIO_PIN_RESET)

/**< Sets the load enable wire to the high logic level   */
#define PLO_LE_SET HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET)

/**< Sets the load enable wire to the low logic level   */
#define PLO_LE_RESET HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET)

/**
 * @}
 */

typedef struct
{
    uint8_t incnt;
    uint8_t outcnt;
    uint32_t buffer;
} plo_state_t;

plo_state_t plo_state = {0};

/*****************************************************************************/
void plo_buff_push(bool in_data)
{
    if ((plo_state.incnt + 1) % 32 == plo_state.outcnt)
        return; // Buffer is full. Cannot store more data

    if (in_data)
        plo_state.buffer |= (1UL << plo_state.incnt); // Save logic 1 to buffer
    else
        plo_state.buffer &= ~(1UL << plo_state.incnt); // Save logic 0 to buffer

    plo_state.incnt = (plo_state.incnt + 1) % 32; // Move "in" pointer
}

/*****************************************************************************/
bool plo_buff_pop(uint8_t *out_data)
{
    if (plo_state.incnt == plo_state.outcnt)
        return false; // No data in buffer, nothing to do

    *out_data = (plo_state.buffer >> plo_state.outcnt) & 1;
    plo_state.outcnt = (plo_state.outcnt + 1) % 32; // Move "out" pointer
    return true;
}

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
            i = 4;

        uint32_t c = max2871[i];

        if ((plo_write_type == PLO_INIT) && (i == 4))
            c &= ~((1UL << 5) | (1UL << 9));

        plo_spi_emul(lsb_to_msb_bit_reversal(c));

        if ((plo_write_type == PLO_INIT) && (i == 5))
            HAL_Delay(20);

        if (plo_write_type == PLO_OUT_ENABLE)
            i = -1;
    }
}

void plo_check_lock_status(void)
{
    if (((test_data[2] & ((1 << 28) | (1 << 27) | (1 << 26))) >> 26) == 0b110)
        plo_buff_push(HAL_GPIO_ReadPin(PLO_MUXOUT_GPIO_Port, PLO_MUXOUT_Pin));
}

void plo_write(uint32_t *data, plo_new_data_t plo_new_data_type)
{
    if (plo_new_data_type == PLO_INIT)
    {
        plo_write_all(data, PLO_INIT);
        plo_write_all(data, PLO_INIT);
        plo_write_all(data, PLO_OUT_ENABLE);
    }
    else if (plo_new_data_type == PLO_NEW_DATA)
    {
        plo_write_all(data, PLO_NEW_DATA);
    }
}

void plo_process_lock_status(bool data)
{
    if (data)
    {
        printf("plo locked\r");
    }
    else
    {
        printf("plo isn't locked\r");
    }
}
