#include <assert.h>
#include "main.h"
#include "Io_SharedPwmInput.h"
#include "Io_FlowMeter.h"

static struct PwmInput *  primary_flow_meter_pwm_input;
static TIM_HandleTypeDef *primary_flow_meter_htim;

float Io_FlowMeter_GetPrimaryFlowRate(void)
{
    return Io_SharedPwmInput_GetFrequency(primary_flow_meter_pwm_input) / 7.5f;
}

void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);

    if (htim == primary_flow_meter_htim)
    {
        Io_SharedPwmInput_Tick(primary_flow_meter_pwm_input);
    }
}
