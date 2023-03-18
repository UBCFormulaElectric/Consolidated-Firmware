#include "torquevectoring/App_TractionControl.h"
#include "torquevectoring/App_PID.h"
#include "App_CanRx.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include <math.h>

void App_TractionControl_ComputeTorque(TractionControl_Inputs *inputs, TractionControl_Outputs *outputs)
{
    float wheel_speed_front_left_rpm  = App_TractionControl_WheelSpeedKPHToRPM(inputs->wheel_speed_front_left_kph);
    float wheel_speed_front_right_rpm = App_TractionControl_WheelSpeedKPHToRPM(inputs->wheel_speed_front_right_kph);

    float slip_ratio_left = App_TractionControl_ComputeSlip(inputs->motor_speed_left_rpm, wheel_speed_front_left_rpm);
    float slip_ratio_right =
        App_TractionControl_ComputeSlip(inputs->motor_speed_right_rpm, wheel_speed_front_right_rpm);

    float slip_ratio_max = fmaxf(slip_ratio_left, slip_ratio_right);
    float k              = App_PID_Compute(inputs->pid, SLIP_RATIO_IDEAL, slip_ratio_max);

    // NOTE: k strictly in range [-1 0] to prevent exceeding power limit
    outputs->torque_left_final_Nm  = (1.0f + k) * inputs->torque_left_Nm;
    outputs->torque_right_final_Nm = (1.0f + k) * inputs->torque_right_Nm;
}

float App_TractionControl_ComputeSlip(float motor_speed_rpm, float front_wheel_speed_rpm)
{
    return (PLANETARY_GEAR_RATIO * motor_speed_rpm - front_wheel_speed_rpm) / (front_wheel_speed_rpm + SMALL_EPSILON);
}

float App_TractionControl_WheelSpeedKPHToRPM(float speed_kph)
{
    float tire_diameter_m, speed_mpm, speed_rpm;

    tire_diameter_m = TIRE_DIAMETER_in * IN_TO_M;
    speed_mpm       = speed_kph * 1000 / 60;
    speed_rpm       = speed_mpm / ((float)M_PI * tire_diameter_m);

    return speed_rpm;
}
