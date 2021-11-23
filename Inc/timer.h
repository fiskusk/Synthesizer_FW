/**
  * @file       timer.h
  * @author     Filip Klapil (klapil.filip@seznam.cz)
  * @brief      Header file for timer related functions
  * @version    0.1
  * @date       2020-01-18
  * 
  * @copyright  Copyright (c) 2020
  * 
  */

#ifndef _TIMER_H
#define _TIMER_H

#include "gpio.h"

/**
  * @brief This type handle, if tick occur or not
  * 
  */
typedef enum{
    TICK_OCCUR,         /**< Tick has occurred      */
    TICK_NOT_OCCUR      /**< Tick did not occur     */
}tick_handle_t;

/**< Handle if tick occur or not    */
extern tick_handle_t tick_handle;
extern tick_handle_t three_sec_tick_handle;

extern void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif