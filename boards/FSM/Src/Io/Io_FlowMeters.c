#include "main.h"
#include "Io_SharedFreqOnlyPwmInput.h"
#include "Io_FlowMeters.h"
#include <assert.h>

struct FlowMeters_Context
{
    struct FreqOnlyPwmInput *pwm_input;
    TIM_HandleTypeDef *      htim;
    HAL_TIM_ActiveChannel    active_channel;
};

static struct FlowMeters_Context primary_flow_meter, secondary_flow_meter;

void Io_FlowMeters_Init(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);

    primary_flow_meter.htim   = htim;
    secondary_flow_meter.htim = htim;

    primary_flow_meter.active_channel   = HAL_TIM_ACTIVE_CHANNEL_1;
    secondary_flow_meter.active_channel = HAL_TIM_ACTIVE_CHANNEL_2;

    primary_flow_meter.pwm_input = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIMx_PRESCALER, TIM_CHANNEL_1,
        TIMx_AUTO_RELOAD_REG);
    secondary_flow_meter.pwm_input = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIMx_PRESCALER, TIM_CHANNEL_2,
        TIMx_AUTO_RELOAD_REG);
}

float Io_FlowMeters_GetPrimaryFlowRate(void)
{
    return Io_SharedFreqOnlyPwmInput_GetFrequency(
               primary_flow_meter.pwm_input) /
           7.5f;
}

float Io_FlowMeters_GetSecondaryFlowRate(void)
{
    return Io_SharedFreqOnlyPwmInput_GetFrequency(
               secondary_flow_meter.pwm_input) /
           7.5f;
}

void Io_FlowMeters_TickPrimary(void)
{
    Io_SharedFreqOnlyPwmInput_Tick(primary_flow_meter.pwm_input);
}

void Io_FlowMeters_TickSecondary(void)
{
    Io_SharedFreqOnlyPwmInput_Tick(secondary_flow_meter.pwm_input);
}

void Io_FlowMeters_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == primary_flow_meter.htim &&
        htim->Channel == primary_flow_meter.active_channel)
    {
        Io_FlowMeters_TickPrimary();
    }

    else if (
        htim == secondary_flow_meter.htim &&
        htim->Channel == secondary_flow_meter.active_channel)
    {
        Io_FlowMeters_TickSecondary();
    }
}

void Io_FlowMeters_Elapsed_TickPrimary(void)
{
    Io_SharedFreqOnlyPwmInput_Elapsed_Tick(primary_flow_meter.pwm_input);
}

void Io_FlowMeters_Elapsed_TickSecondary(void)
{
    Io_SharedFreqOnlyPwmInput_Elapsed_Tick(secondary_flow_meter.pwm_input);
}
