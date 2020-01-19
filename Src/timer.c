/**
  * @file       timer.c
  * @author     Filip Klapil (klapil.filip@seznam.cz)
  * @brief      This file contains timer related functions
  * @version    0.1
  * @date       2020-01-18
  * 
  * @copyright  Copyright (c) 2020
  * 
  */

#include "timer.h"
#include "gpio.h"

tick_handle_t tick_handle = TICK_NOT_OCCUR;

// Override weak function for handle period occur
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        /**< Counting the number of times an interrupt from the timer occurred */
        static uint8_t cnt = 0;
        // this is period interupt, which occur each 500ms
        if (cnt > 1)
        {   // so this occur every 1 second
            tick_handle = TICK_OCCUR;
            cnt = 0;
        }
        else
        {
            cnt++;
            tick_handle = TICK_NOT_OCCUR;
        }
    }
}