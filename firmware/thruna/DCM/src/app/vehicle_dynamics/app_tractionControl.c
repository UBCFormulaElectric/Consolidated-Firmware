#include "app_canTx.h"
#include "app_units.h"
#include "app_tractionControl.h"
#include "app_pid.h"
#include "app_canTx.h"
#include "app_vehicleDynamicsConstants.h"
#include <math.h>

void app_tractionControl_computeTorque(TractionControl_Inputs *inputs, TractionControl_Outputs *outputs)
{
    PID *pid = inputs->pid;

    float wheel_speed_front_left_rpm  = app_tractionControl_wheelSpeedKPHToRPM(inputs->wheel_speed_front_left_kph);
    float wheel_speed_front_right_rpm = app_tractionControl_wheelSpeedKPHToRPM(inputs->wheel_speed_front_right_kph);

    float slip_ratio_left = app_tractionControl_computeSlip(inputs->motor_speed_left_rpm, wheel_speed_front_left_rpm);
    float slip_ratio_right =
        app_tractionControl_computeSlip(inputs->motor_speed_right_rpm, wheel_speed_front_right_rpm);

    float slip_ratio_max = fmaxf(slip_ratio_left, slip_ratio_right);
    float k              = app_pid_compute(pid, SLIP_RATIO_IDEAL, slip_ratio_max);

    // Send debug messages over CAN
    app_canTx_DCM_SlipRatioLeft_set(slip_ratio_left);
    app_canTx_DCM_SlipRatioRight_set(slip_ratio_right);
    app_canTx_DCM_PIDSlipRatioOutput_set(k);
    app_canTx_DCM_PIDSlipRatioError_set(pid->error);
    app_canTx_DCM_PIDSlipRatioDerivative_set(pid->derivative);
    app_canTx_DCM_PIDSlipRatioIntegral_set(pid->integral);

    // NOTE: k strictly in range [-1 0] to prevent exceeding power limit
    outputs->torque_left_final_Nm  = (1.0f + k) * inputs->torque_left_Nm;
    outputs->torque_right_final_Nm = (1.0f + k) * inputs->torque_right_Nm;
}

float app_tractionControl_computeSlip(float motor_speed_rpm, float front_wheel_speed_rpm)
{
    return (PLANETARY_GEAR_RATIO * motor_speed_rpm - front_wheel_speed_rpm) / (front_wheel_speed_rpm + SMALL_EPSILON);
}

float app_tractionControl_wheelSpeedKPHToRPM(float speed_kph)
{
    float tire_diameter_m, speed_mpm, speed_rpm;

    tire_diameter_m = TIRE_DIAMETER_in * IN_TO_M;
    speed_mpm       = speed_kph * 1000 / 60;
    speed_rpm       = speed_mpm / ((float)M_PI * tire_diameter_m);

    return speed_rpm;
}
