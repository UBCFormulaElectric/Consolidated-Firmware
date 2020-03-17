#include "SharedAssert.h"
#include "main.h"
#include "Io_SharedPwmInput.h"
#include "Io_FlowMeter.h"

extern TIM_HandleTypeDef htim2;

static struct PwmInput *  flowMeter_pwm_input;
static TIM_HandleTypeDef *flowMeter_htim;

static float Io_FlowMeter_GetFrequency(void)
{
    return Io_SharedPwmInput_GetFrequency(flowMeter_pwm_input);
}

void Io_FlowMeter_Init(void)
{
    flowMeter_pwm_input = Io_SharedPwmInput_Create(
        &htim2, TIM2_FREQUENCY / TIM2_PRESCALER, TIM_CHANNEL_3, TIM_CHANNEL_3);

    flowMeter_htim = &htim2;
}

float Io_FlowMeter_GetFlowRate(void)
{
    // Return flow rate (L/min)
    // Flow rate pulse characteristics: Frequency (Hz) = 7.5 * Flow rate (L/min)
    return Io_FlowMeter_GetFrequency() / 7.5;
}

void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    shared_assert(htim != NULL);

    if (htim == flowMeter_htim)
    {
        Io_SharedPwmInput_Tick(flowMeter_pwm_input);
    }
}
