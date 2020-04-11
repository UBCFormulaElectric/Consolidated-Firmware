#include "main.h"
#include "Io_SharedFrequencyOnlyPwmInput.h"
#include "Io_FlowMeters.h"
#include <assert.h>

#define PRIMARY_FLOW_METER_CHANNEL HAL_TIM_ACTIVE_CHANNEL_1
#define SECONDARY_FLOW_METER_CHANNEL HAL_TIM_ACTIVE_CHANNEL_2

struct FlowMeters_Context
{
    struct FreqOnlyPwmInput *pwm_input;
    size_t                   elapsed_tick;
};

static TIM_HandleTypeDef *       flow_meters_htim;
static struct FlowMeters_Context primary_flow_meter, secondary_flow_meter;

void Io_FlowMeters_Init(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);

    flow_meters_htim                = htim;
    primary_flow_meter.elapsed_tick = secondary_flow_meter.elapsed_tick = 0U;

    primary_flow_meter.pwm_input = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIMx_PRESCALER, TIM_CHANNEL_1);
    secondary_flow_meter.pwm_input = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIMx_PRESCALER, TIM_CHANNEL_2);
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
    primary_flow_meter.elapsed_tick = 0U;
    Io_SharedFreqOnlyPwmInput_Tick(primary_flow_meter.pwm_input);
}

void Io_FlowMeters_TickSecondary(void)
{
    secondary_flow_meter.elapsed_tick = 0U;
    Io_SharedFreqOnlyPwmInput_Tick(secondary_flow_meter.pwm_input);
}

void Io_FlowMeters_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    // Primary and secondary flow meter timer handler are the same
    if (htim == flow_meters_htim)
    {
        if (htim->Channel == PRIMARY_FLOW_METER_CHANNEL)
        {
            Io_FlowMeters_TickPrimary();
        }

        else if (htim->Channel == SECONDARY_FLOW_METER_CHANNEL)
        {
            Io_FlowMeters_TickSecondary();
        }
    }
}

void Io_FlowMeters_Elapsed_TickPrimary(void)
{
    primary_flow_meter.elapsed_tick = Io_SharedFreqOnlyPwmInput_Elapsed_Tick(
        primary_flow_meter.pwm_input, primary_flow_meter.elapsed_tick);
}

void Io_FlowMeters_Elapsed_TickSecondary(void)
{
    secondary_flow_meter.elapsed_tick = Io_SharedFreqOnlyPwmInput_Elapsed_Tick(
        secondary_flow_meter.pwm_input, secondary_flow_meter.elapsed_tick);
}
