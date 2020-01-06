#ifndef _TIMER_H
#define _TIMER_H

#include "gpio.h"

typedef enum{
    TICK_OCCUR, 
    TICK_NOT_OCCUR
}tick_handle_t;

extern tick_handle_t tick_handle;

extern void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif