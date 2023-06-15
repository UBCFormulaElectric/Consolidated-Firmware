#include <FreeRTOS.h>
#include <task.h>
#include <assert.h>

#include "main.h"
#include "Io_Imd.h"
#include "Io_SharedPwmInput.h"

extern TIM_HandleTypeDef htim1;

static struct PwmInput *  imd_pwm_input;
static TIM_HandleTypeDef *imd_htim;

void Io_Imd_Init(void)
{
    imd_pwm_input = Io_SharedPwmInput_Create(&htim1, TIM1_FREQUENCY / TIM1_PRESCALER, TIM_CHANNEL_2, TIM_CHANNEL_1);
    imd_htim      = &htim1;
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

bool Io_Imd_CheckLatchedFault(void)
{
    return HAL_GPIO_ReadPin(IMD_LATCH_GPIO_Port, IMD_LATCH_Pin) == GPIO_PIN_SET;
}

bool Io_Bspd_CheckLatchedFault(void)
{
    return HAL_GPIO_ReadPin(BSPD_LATCH_GPIO_Port, BSPD_LATCH_Pin) == GPIO_PIN_SET;
}

bool Io_Bms_CheckLatchedFault(void)
{
    return HAL_GPIO_ReadPin(BMS_LATCH_GPIO_Port, BMS_LATCH_Pin) == GPIO_PIN_SET;
}
