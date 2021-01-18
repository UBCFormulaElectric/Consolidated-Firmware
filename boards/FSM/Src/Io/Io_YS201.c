#include <assert.h>
#include "main.h"
#include "Io_SharedFreqOnlyPwmInput.h"
#include "Io_YS201.h"

static struct FreqOnlyPwmInput *primary_ys201, *secondary_ys201;

void Io_YS201_Init(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);

    primary_ys201 = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIM4_PRESCALER, TIM_CHANNEL_1,
        TIM4_AUTO_RELOAD_REG, HAL_TIM_ACTIVE_CHANNEL_1);
    secondary_ys201 = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIM4_PRESCALER, TIM_CHANNEL_2,
        TIM4_AUTO_RELOAD_REG, HAL_TIM_ACTIVE_CHANNEL_2);
}

float Io_YS201_GetPrimaryFlowRate(void)
{
    return Io_SharedFreqOnlyPwmInput_GetFrequency(primary_ys201) / 7.5f;
}

float Io_YS201_GetSecondaryFlowRate(void)
{
    return Io_SharedFreqOnlyPwmInput_GetFrequency(secondary_ys201) / 7.5f;
}

void Io_YS201_InputCaptureCallback(TIM_HandleTypeDef *const htim)
{
    if (htim == Io_SharedFreqOnlyPwmInput_GetTimerHandle(primary_ys201) &&
        htim->Channel ==
            Io_SharedFreqOnlyPwmInput_GetTimerActiveChannel(primary_ys201))
    {
        Io_SharedFreqOnlyPwmInput_Tick(primary_ys201);
    }

    else if (
        htim == Io_SharedFreqOnlyPwmInput_GetTimerHandle(secondary_ys201) &&
        htim->Channel ==
            Io_SharedFreqOnlyPwmInput_GetTimerActiveChannel(secondary_ys201))
    {
        Io_SharedFreqOnlyPwmInput_Tick(secondary_ys201);
    }
}

void Io_YS201_CheckIfPrimaryIsActive(void)
{
    Io_SharedFreqOnlyPwmInput_CheckIfPwmIsActive(primary_ys201);
}

void Io_YS201_CheckIfSecondaryIsActive(void)
{
    Io_SharedFreqOnlyPwmInput_CheckIfPwmIsActive(secondary_ys201);
}
