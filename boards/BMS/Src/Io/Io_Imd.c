#include <FreeRTOS.h>
#include <task.h>
#include <assert.h>

#include "main.h"
#include "Io_Imd.h"
#include "Io_SharedPwmInput.h"

extern TIM_HandleTypeDef htim2;

static struct PwmInput *  imd_pwm_input;
static TIM_HandleTypeDef *imd_htim;

void Io_Imd_Init(void)
{
    imd_pwm_input = Io_SharedPwmInput_Create(&htim2, TIM2_FREQUENCY / TIM2_PRESCALER, TIM_CHANNEL_2, TIM_CHANNEL_1);
    imd_htim      = &htim2;
}

float Io_Imd_GetFrequency(void)
{
    return Io_SharedPwmInput_GetFrequency(imd_pwm_input);
}

float Io_Imd_GetDutyCycle(void)
{
    return Io_SharedPwmInput_GetDutyCycle(imd_pwm_input);
}

void Io_Imd_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == imd_htim)
    {
        Io_SharedPwmInput_Tick(imd_pwm_input);
    }
}

uint16_t Io_Imd_GetTimeSincePowerOn(void)
{
    // The IMD shares the same power rail as the BMS, so we assume that the IMD
    // and BMS boot up at the same time. In other words, the IMD has been ON
    // for just as long as the BMS.
    return (uint16_t)(xTaskGetTickCount() * portTICK_RATE_MS) / 1000U;
}
