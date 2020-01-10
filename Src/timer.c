#include "timer.h"
#include "gpio.h"

tick_handle_t tick_handle = TICK_NOT_OCCUR;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == TIM3)
    {
        //HAL_GPIO_TogglePin(RF_OUT2_GPIO_Port, RF_OUT2_Pin);
        //tick_handle = TICK_OCCUR;

        static uint8_t cnt = 0;
        if (cnt > 1)
        {
            //check_lock_status();
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