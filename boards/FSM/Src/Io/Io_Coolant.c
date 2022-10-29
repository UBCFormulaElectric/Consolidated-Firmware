#include <assert.h>
#include "main.h"
#include "Io_SharedFreqOnlyPwmInput.h"

static struct FreqOnlyPwmInput *flow_meter;
void                            Io_FlowMeters_Init(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);

    flow_meter = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIM4_PRESCALER, TIM_CHANNEL_2, TIM4_AUTO_RELOAD_REG, HAL_TIM_ACTIVE_CHANNEL_2);
}
float Io_FlowMeters_GetFlowRate(void)
{
    return Io_SharedFreqOnlyPwmInput_GetFrequency(flow_meter) / 7.5f;
}
void Io_FlowMeters_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == Io_SharedFreqOnlyPwmInput_GetTimerHandle(flow_meter) &&
        htim->Channel == Io_SharedFreqOnlyPwmInput_GetTimerActiveChannel(flow_meter))
    {
        Io_SharedFreqOnlyPwmInput_Tick(flow_meter);
    }
}
void Io_FlowMeters_CheckIfFlowMeterIsActive(void)
{
    Io_SharedFreqOnlyPwmInput_CheckIfPwmIsActive(flow_meter);
}

float Io_GetTemperatureA(void)
{
    return 1;
}
float Io_GetTemperatureB(void)
{
    return 1;
}

float Io_GetPressureA(void)
{
    return 1;
}
float Io_GetPressureB(void)
{
    return 1;
}
