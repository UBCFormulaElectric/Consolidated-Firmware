#include "SharedAssert.h"
#include "main.h"
#include "Io_SharedPwmInput.h"
#include "Io_FlowMeter.h"

extern TIM_HandleTypeDef htim2;

static struct PwmInput *  flow_meter_pwm_input;
static TIM_HandleTypeDef *flow_meter_htim;

void Io_FlowMeter_Init(void)
{
    flow_meter_pwm_input = Io_SharedPwmInput_Create(
        &htim2, TIMx_FREQUENCY / TIMx_PRESCALER, TIM_CHANNEL_1, TIM_CHANNEL_1);

    flow_meter_htim = &htim2;
}

float Io_FlowMeter_GetFlowRate(void)
{
    return Io_SharedPwmInput_GetFrequency(flow_meter_pwm_input) / 7.5;
}

void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    shared_assert(htim != NULL);

    if (htim == flow_meter_htim)
    {
        Io_SharedPwmInput_Tick(flow_meter_pwm_input);
    }
}
