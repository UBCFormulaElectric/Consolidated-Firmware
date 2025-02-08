#include <stddef.h> 
#include "app_tractionControlState.h"
#include "app_torqueVectoring.h"
#include "app_canRx.h"
#include "app_canTx.h"


static TractionControl_Inputs     traction_control_inputs;
static TractionControl_Outputs    traction_control_outputs;

static float motor_speed_left_rpm; 
static float motor_speed_right_rpm;
static float power_limit; 
static float accelerator_pedal_percent; 
static float wheel_speed_front_left_kph;
static float wheel_speed_front_right_kph;


static void app_tractionControl_run_on_entry(void)
{
    traction_control_inputs.pid->error = 0;
    app_canTx_VC_TVInternalState_set(TRACTION_CONTROL);
    return; 
}

static void app_tractionControl_run_on_exit()
{
    traction_control_inputs.pid->integral = 0; 
    return; 
}

static void app_tractionControl_run_on_tick_100Hz()
{
    motor_speed_left_rpm = app_canTx_VC_

    traction_control_inputs.motor_speed_left_rpm  = motor_speed_left_rpm;
    traction_control_inputs.motor_speed_right_rpm = motor_speed_right_rpm;
    traction_control_inputs.torque_limit =
    app_torqueVectoring_powerToTorque(power_limit, motor_speed_left_rpm, motor_speed_right_rpm, 1, 1);
    traction_control_inputs.torque_left_Nm              = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    traction_control_inputs.torque_right_Nm             = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    traction_control_inputs.wheel_speed_front_left_kph  = wheel_speed_front_left_kph;
    traction_control_inputs.wheel_speed_front_right_kph = wheel_speed_front_right_kph;
    app_tractionControl_computeTorque(&traction_control_inputs, &traction_control_outputs);
}

const State *app_tractionControlOnState_get(void)
{
    static State TractionControlOnState = 
    {
        .name = "TRACTION CONTROL",
        .run_on_entry = app_tractionControl_run_on_entry,
        .run_on_exit = app_tractionControl_run_on_exit,
        .run_on_tick_100Hz = app_tractionControl_run_on_tick_100Hz,
        .run_on_tick_1Hz = NULL,
    };

    return &TractionControlOnState;
} 