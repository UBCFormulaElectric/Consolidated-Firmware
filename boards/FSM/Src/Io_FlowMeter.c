#include "SharedAssert.h"
#include "main.h"
#include "App_FlowMeter.h"

extern TIM_HandleTypeDef htim2;

static struct PwmInput * flowMeter_pwm_input;
static TIM_HandleTypeDef *flowMeter_htim;

#define FREQ_CAPTURE
#define FREQ_DUTYCYCLE_CAPTURE

#ifdef FREQ_CAPTURE
void Io_FlowMeter_Init (){

    flowMeter_pwm_input = Io_SharedPwmInput_Create (&htim2, TIM2_FREQUENCY , TIM_CHANNEL_2, TIM_CHANNEL_2);
    flowMeter_htim = &htim2;
}
#else

void Io_FlowMeter_Init () {
    flowMeter_pwm_input = Io_SharedPwmInput_Create (&htim, TIM2_FREQUENCY, TIM_CHANNEL_2, TIM_CHANNEL_1);
    flowMeter_htim = &htim2;
}
#endif

float Io_Imd_GetFrequency (void) {

    return Io_SharedPwmInput_GetFrequency(flowMeter_pwm_input);

}

void Io_flowMeter_InputCaptureCallback (TIM_HandleTypeDef *htim){

    shared_assert(htim != NULL);

    if(htim == flowMeter_htim){
        Io_Shared_PwmInput_Tick(flowMeter_pwm_input);
    }

}
