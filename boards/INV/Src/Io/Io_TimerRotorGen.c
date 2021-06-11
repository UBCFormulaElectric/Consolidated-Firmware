#include <math.h>
#include "Io_TimerRotorGen.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;

void Io_TimerRotorGen_SetFundFreq(const double fund_frequency)
{
    uint32_t clk_freq = 2 * HAL_RCC_GetPCLK1Freq();
    uint32_t timer_period      = clk_freq / fund_frequency;
    TIM2->ARR |= timer_period;
}

double Io_TimerRotorGen_GetPosition(void)
{
    uint32_t timer_period      = __HAL_TIM_GET_AUTORELOAD(&htim2);
    uint32_t timer_val         = __HAL_TIM_GET_COUNTER(&htim2);
    double fake_rotor_position = 2 * M_PI * (double)(timer_val / timer_period);
    return fake_rotor_position;
}
