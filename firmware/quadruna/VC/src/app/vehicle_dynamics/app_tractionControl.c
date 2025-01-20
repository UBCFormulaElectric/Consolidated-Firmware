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

    // float wheel_speed_front_left_rpm  = app_tractionControl_wheelSpeedKPHToRPM(inputs->wheel_speed_front_left_kph);
    // float wheel_speed_front_right_rpm = app_tractionControl_wheelSpeedKPHToRPM(inputs->wheel_speed_front_right_kph);
    float wheel_speed_front_left_rpm  = WHEEL_KMH_TO_RPM(inputs->wheel_speed_front_left_kph);
    float wheel_speed_front_right_rpm = WHEEL_KMH_TO_RPM(inputs->wheel_speed_front_right_kph);

    float slip_ratio_left = app_tractionControl_computeSlip(inputs->motor_speed_left_rpm, wheel_speed_front_left_rpm);
    float slip_ratio_right =
        app_tractionControl_computeSlip(inputs->motor_speed_right_rpm, wheel_speed_front_right_rpm);
        

        // corrected as we should correct the wheel that deviates most from the ideal slip ratio rather than just the one with the highest slip ratio
    // ex. left wheel = 0.04 slip ratio and right wheel = 0.03, our old logic would correct left wheel due to max(left wheel, right wheel)
    // however we should correct the left wheel first to get it in ideal operating range

    float slip_ratio_error_left = (float)fabs(slip_ratio_left - SLIP_RATIO_IDEAL); 
    float slip_ratio_error_right = (float)fabs(slip_ratio_right - SLIP_RATIO_IDEAL); 

    float k; 

    if( slip_ratio_error_left <= slip_ratio_error_right){
        k              = app_pid_compute(pid, SLIP_RATIO_IDEAL, slip_ratio_right);

    }
    else{
        k              = app_pid_compute(pid, SLIP_RATIO_IDEAL, slip_ratio_left);
    }
    // Send debug messages over CAN
    app_canTx_VC_SlipRatioLeft_set(slip_ratio_left);
    app_canTx_VC_SlipRatioRight_set(slip_ratio_right);
    app_canTx_VC_PIDSlipRatioOutput_set(k);
    app_canTx_VC_PIDSlipRatioError_set(pid->error);
    app_canTx_VC_PIDSlipRatioDerivative_set(pid->derivative);
    app_canTx_VC_PIDSlipRatioIntegral_set(pid->integral);

    // NOTE: k strictly in range [-1 0] to prevent exceeding power limit
    outputs->torque_left_final_Nm  = (1.0f + k) * inputs->torque_left_Nm;
    outputs->torque_right_final_Nm = (1.0f + k) * inputs->torque_right_Nm;
}

float app_tractionControl_computeSlip(float motor_speed_rpm, float front_wheel_speed_rpm)
{
    return (motor_speed_rpm / GEAR_RATIO - front_wheel_speed_rpm) / (front_wheel_speed_rpm + SMALL_EPSILON);
}
