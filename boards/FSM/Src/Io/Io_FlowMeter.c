#include <assert.h>
#include "main.h"
#include "Io_SharedPwmInput.h"
#include "Io_SharedFrequencyOnlyPwmInput.h"
#include "Io_FlowMeter.h"

#define PRIMARY_FLOW_METER_CHANNEL HAL_TIM_ACTIVE_CHANNEL_1
#define SECONDARY_FLOW_METER_CHANNEL HAL_TIM_ACTIVE_CHANNEL_2

extern TIM_HandleTypeDef htim4;

static struct FrequencyOnlyPwmInput *primary_flow_meter_pwm_input,
    *secondary_flow_meter_pwm_input;
static TIM_HandleTypeDef *flow_meters_timer_handle;

void Io_FlowMeter_Init(void)
{
    // Configuring timer handle for both flow meters
    flow_meters_timer_handle = &htim4;

    primary_flow_meter_pwm_input = Io_SharedFrequencyOnlyPwmInput_Create(
        &htim4, TIMx_FREQUENCY / TIMx_PRESCALER, TIM_CHANNEL_1);
    secondary_flow_meter_pwm_input = Io_SharedFrequencyOnlyPwmInput_Create(
        &htim4, TIMx_FREQUENCY / TIMx_PRESCALER, TIM_CHANNEL_2);
}

float Io_FlowMeter_GetPrimaryFlowRate(void)
{
    return Io_SharedFrequencyOnlyPwmInput_GetFrequency(
               primary_flow_meter_pwm_input) /
           7.5f;
}

float Io_FlowMeter_GetSecondaryFlowRate(void)
{
    return Io_SharedFrequencyOnlyPwmInput_GetFrequency(
               secondary_flow_meter_pwm_input) /
           7.5f;
}

void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);
    assert(flow_meters_timer_handle != NULL);

    // First check to see if the interrupt occurs on the timer connected to the
    // flow meter then check to see which channel the rising edge occurs
    if (htim == flow_meters_timer_handle)
    {
        if (htim->Channel == PRIMARY_FLOW_METER_CHANNEL)
        {
            Io_SharedFrequencyOnlyPwmInput_Tick(primary_flow_meter_pwm_input);
        }

        else if (htim->Channel == SECONDARY_FLOW_METER_CHANNEL)
        {
            Io_SharedFrequencyOnlyPwmInput_Tick(secondary_flow_meter_pwm_input);
        }
    }
}
