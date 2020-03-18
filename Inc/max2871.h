/**
  * @file       max2871.h
  * @author     Filip Klapil (klapil.filip@seznam.cz)
  * @brief      Header file for PLO MAX2871.
  * @version    0.1
  * @date       2020-01-18
  * 
  * @copyright  Copyright (c) 2020
  * 
  */

#ifndef _MAX2871_H
#define _MAX2871_H

#include <stdbool.h>
#include <stdint.h>

/**
  * @brief Type of PLO new data structure definition 
  * 
  */
typedef enum{
    PLO_NEW_DATA,   /**< Common new register value */
    PLO_INIT,       /**< Carries the requested initialization of the PLO    */
    PLO_OUT_ENABLE  /**< Carries that PLO need enable outputs (after init)  */
}plo_new_data_t;


/**
  * @brief Carry if PLO lock state is available or not
  * 
  */
typedef enum{
    PLO_LOCK_STATE_UNKNOWN, /**< PLO lock-state isn't known */
    PLO_LOCK_STATE_WAIT     /**< PLO lock-state wait handle */
}plo_lock_state_t;

/**
  * @brief Carry if PLO lock state is available or not 
  * 
  */
extern volatile plo_lock_state_t plo_lock_state;

/**
  * @brief  A global variable that carries information about the state of data 
  *         processing for the PLO.
  * 
  */
extern volatile plo_new_data_t plo_new_data;

extern uint32_t plo_read(uint32_t reg5);
extern void plo_write_register(uint32_t register_data);
extern void plo_check_lock_status(void);
extern bool plo_buff_pop(uint8_t * out_data);
extern void plo_process_lock_status(bool data);
extern void plo_write(uint32_t *data, plo_new_data_t plo_new_data_type);


#endif
