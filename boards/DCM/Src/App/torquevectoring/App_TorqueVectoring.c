//
// Created by formulae on 18/02/23.
//
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include "torquevectoring//App_TorqueVectoring.h"
#include "App_CanRx.h"


static PowerLimiting_Inputs power_limiting_inputs;
static float estimated_power_limit;
static float actual_power_limit;
static ActiveDifferential_Inputs active_differential_inputs;
static ActiveDifferential_Outputs active_differential_outputs;
static TractionControl_Inputs traction_control_inputs;
static TractionControl_Outputs traction_control_outputs;

void App_TorqueVectoring_Run(void){
    const float bms_available_power = App_CanRx_BMS_AvailablePower_AvailablePower_Get();
}


