#include "io_imd.h"
#include "Io_SharedPwmInput.h"
#include "main.h"
#include <assert.h>
#include <task.h>

extern TIM_HandleTypeDef htim1;

static struct PwmInput*   imd_pwm_input;
static TIM_HandleTypeDef* imd_htim;

void io_imd_init(void)
{
    imd_pwm_input = Io_SharedPwmInput_Create(&htim1, TIM1_FREQUENCY / TIM1_PRESCALER, TIM_CHANNEL_2, TIM_CHANNEL_1);
    imd_htim      = &htim1;
}

float io_imd_getFrequency(void)
{
    return Io_SharedPwmInput_GetFrequency(imd_pwm_input);
}

float io_imd_getDutyCycle(void)
{
    return Io_SharedPwmInput_GetDutyCycle(imd_pwm_input);
}

void io_imd_inputCaptureCallback(TIM_HandleTypeDef* htim)
{
    if (htim == imd_htim)
    {
        Io_SharedPwmInput_Tick(imd_pwm_input);
    }
}

uint16_t io_imd_getTimeSincePowerOn(void)
{
    // The IMD shares the same power rail as the BMS, so we assume that the IMD
    // and BMS boot up at the same time. In other words, the IMD has been ON
    // for just as long as the BMS.
    return (uint16_t)(xTaskGetTickCount() * portTICK_RATE_MS) / 1000U;
}
