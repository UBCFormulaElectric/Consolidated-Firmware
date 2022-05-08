#include <assert.h>
#include "main.h"
#include "Io_SharedFreqOnlyPwmInput.h"
#include "Io_FlowMeters.h"

static struct FreqOnlyPwmInput *primary_flow_meter, *secondary_flow_meter;

void Io_FlowMeters_Init(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);

    primary_flow_meter = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIM4_PRESCALER, TIM_CHANNEL_1, TIM4_AUTO_RELOAD_REG, HAL_TIM_ACTIVE_CHANNEL_1);
    secondary_flow_meter = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIM4_PRESCALER, TIM_CHANNEL_2, TIM4_AUTO_RELOAD_REG, HAL_TIM_ACTIVE_CHANNEL_2);
}

float Io_FlowMeters_GetPrimaryFlowRate(void)
{
    return Io_SharedFreqOnlyPwmInput_GetFrequency(primary_flow_meter) / 7.5f;
}

float Io_FlowMeters_GetSecondaryFlowRate(void)
{
    return Io_SharedFreqOnlyPwmInput_GetFrequency(secondary_flow_meter) / 7.5f;
}

void Io_FlowMeters_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == Io_SharedFreqOnlyPwmInput_GetTimerHandle(primary_flow_meter) &&
        htim->Channel == Io_SharedFreqOnlyPwmInput_GetTimerActiveChannel(primary_flow_meter))
    {
        Io_SharedFreqOnlyPwmInput_Tick(primary_flow_meter);
    }

    else if (
        htim == Io_SharedFreqOnlyPwmInput_GetTimerHandle(secondary_flow_meter) &&
        htim->Channel == Io_SharedFreqOnlyPwmInput_GetTimerActiveChannel(secondary_flow_meter))
    {
        Io_SharedFreqOnlyPwmInput_Tick(secondary_flow_meter);
    }
}

void Io_FlowMeters_CheckIfPrimaryIsActive(void)
{
    Io_SharedFreqOnlyPwmInput_CheckIfPwmIsActive(primary_flow_meter);
}

void Io_FlowMeters_CheckIfSecondaryIsActive(void)
{
    Io_SharedFreqOnlyPwmInput_CheckIfPwmIsActive(secondary_flow_meter);
}
