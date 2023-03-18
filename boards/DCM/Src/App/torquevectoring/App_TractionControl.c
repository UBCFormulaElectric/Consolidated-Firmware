#include "torquevectoring/App_TractionControl.h"
#include "torquevectoring/App_PID.h"
#include "App_CanRx.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include <math.h>

void App_TractionControl_ComputeTorque(TractionControl_Inputs *inputs, TractionControl_Outputs *outputs)
{
    float slip_ratio_left  = ComputeSlip(inputs->motor_speed_left_rpm, inputs->front_wheel_speed_left_rpm);
    float slip_ratio_right = ComputeSlip(inputs->motor_speed_right_rpm, inputs->front_wheel_speed_right_rpm);

    float slip_ratio_max = fmaxf(slip_ratio_left, slip_ratio_right);
    float k              = App_PID_Compute(inputs->pid, SLIP_RATIO_IDEAL, slip_ratio_max);

    outputs->torque_left_final_Nm  = k * inputs->torque_left_Nm;
    outputs->torque_right_final_Nm = k * inputs->torque_right_Nm;
}

float ComputeSlip(float motor_speed_rpm, float front_wheel_speed_rpm)
{
    // TODO(akoen): Should be relative error not absolute
    return (front_wheel_speed_rpm - PLANETARY_GEAR_RATIO * motor_speed_rpm) / (motor_speed_rpm + SMALL_EPSILON);
}
