#include "SharedAssert.h"
#include "main.h"
#include "App_FlowMeter.h"
#include "Io_SharedPwmInput.h"
#include "Io_FlowMeter.h"

extern TIM_HandleTypeDef htim2;

static struct PwmInput *  flowMeter_pwm_input;
static TIM_HandleTypeDef *flowMeter_htim;

void Io_FlowMeter_Init(void)
{
    flowMeter_pwm_input = Io_SharedPwmInput_Create(
        &htim2, TIM2_FREQUENCY, TIM_CHANNEL_1, TIM_CHANNEL_2);

    flowMeter_htim = &htim2;
}

float Io_Imd_GetFrequency(void)
{
    return Io_SharedPwmInput_GetFrequency(flowMeter_pwm_input);
}

void Io_flowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    shared_assert(htim != NULL);

    if (htim == flowMeter_htim)
    {
        Io_SharedPwmInput_Tick(flowMeter_pwm_input);
    }
}
