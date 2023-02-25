#include "torquevectoring/App_TorqueVectoring.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include "App_CanRx.h"

static PowerLimiting_Inputs       power_limiting_inputs;
static float                      estimated_power_limit;
static float                      actual_power_limit;
static ActiveDifferential_Inputs  active_differential_inputs;
static ActiveDifferential_Outputs active_differential_outputs;
static TractionControl_Inputs     traction_control_inputs;
static TractionControl_Outputs    traction_control_outputs;

void App_TorqueVectoring_Run(void)
{
    // To Do : change this to can msg for BMS Current const float bms_available_power = App_CanRx_BMS_AvailablePower_AvailablePower_Get();
    // To Do : get can msgs for steering angle and pedal percentage transmitted via can by fsm


}
