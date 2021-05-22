
#include "Io_TimerPwmGen.h"
#include "main.h"

TIM_HandleTypeDef *timer_pwm_gen_handle;

void Io_TimerPwmGen_Init(TIM_HandleTypeDef *const handle)
{
    timer_pwm_gen_handle = handle;
}

void Io_TimerPmwGen_Start(void)
{
    //Start PWM Generation
    HAL_TIM_Base_Start_IT(&htim8);

    HAL_TIM_OC_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_OC_Start(&htim8, TIM_CHANNEL_3);
    HAL_TIMEx_OCN_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIMEx_OCN_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIMEx_OCN_Start(&htim8, TIM_CHANNEL_3);
}
