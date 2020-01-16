#ifndef _MAX2871_H
#define _MAX2871_H

#include <stdbool.h>
#include <stdint.h>

#define PLO_DATA_SET HAL_GPIO_WritePin(PLO_DATA_GPIO_Port, PLO_DATA_Pin, GPIO_PIN_SET)
#define PLO_DATA_RESET HAL_GPIO_WritePin(PLO_DATA_GPIO_Port, PLO_DATA_Pin, GPIO_PIN_RESET)
#define PLO_CLK_SET HAL_GPIO_WritePin(PLO_CLK_GPIO_Port, PLO_CLK_Pin, GPIO_PIN_SET)
#define PLO_CLK_RESET HAL_GPIO_WritePin(PLO_CLK_GPIO_Port, PLO_CLK_Pin, GPIO_PIN_RESET)
#define PLO_LE_SET HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_SET)
#define PLO_LE_RESET HAL_GPIO_WritePin(PLO_LE_GPIO_Port, PLO_LE_Pin, GPIO_PIN_RESET)

typedef enum{
    PLO_NEW_DATA,
    PLO_INIT,
    PLO_OUT_ENABLE,
    PLO_CHANGED_REGISTER,
    PLO_DATA_SENDED
}plo_new_data_t;

typedef enum{
    PLO_LOCK_STATE_UNKNOWN,
    PLO_LOCK_STATE_WAIT
}plo_lock_state_t;

extern volatile plo_lock_state_t plo_lock_state;
extern volatile plo_new_data_t plo_new_data;

extern void plo_write_all(uint32_t *max2871, plo_new_data_t plo_write_type);
extern void plo_write_register(uint32_t register_data);
extern void plo_check_lock_status(void);
extern bool plo_buff_pop(uint8_t * out_data);
extern void plo_process_lock_status(bool data);
extern void plo_write(uint32_t *data, plo_new_data_t plo_new_data_type);


#endif
