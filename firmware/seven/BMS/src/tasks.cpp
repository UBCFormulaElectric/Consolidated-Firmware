#include "tasks.h"

#include "stm32h7xx_hal_tim.h"

void tasks_handle_arr_rollover_callback(TIM_HandleTypeDef *htim);
{
    if (htim == &imd_pwm_input.get_timer_handle())
    {
        imd_pwm_input.increment_arrRolloverCount();
    }
    else if (htim == &evse_pwm_input.get_timer_handle())
    {
        evse_pwm_input.increment_arrRolloverCount();
    }
}