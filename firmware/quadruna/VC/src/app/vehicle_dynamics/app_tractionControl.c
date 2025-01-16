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

    float slip_ratio_max = fmaxf(slip_ratio_left, slip_ratio_right);
    float k              = app_pid_compute(pid, SLIP_RATIO_IDEAL, slip_ratio_max);
    static float correction_torque; 

    // NOTE: we want to map K to a vlaue that we can multiple with input torque. K is based on the slip error 
    // we want to map it to torque error, we also want to  make sure we use the torque associated to the wheel from which slip max was taken

   if (slip_ratio_max == slip_ratio_left) {
    correction_torque = k * inputs->torque_left_Nm;    
    outputs->torque_left_final_Nm  = inputs->torque_left_Nm + correction_torque;
    outputs->torque_right_final_Nm = inputs->torque_right_Nm;  
    }
    else {
    correction_torque = k * inputs->torque_right_Nm; 
    outputs->torque_right_final_Nm = inputs->torque_right_Nm + correction_torque;
    outputs->torque_left_final_Nm  = inputs->torque_left_Nm; 
    }
    // Send debug messages over CAN
    app_canTx_VC_SlipRatioLeft_set(slip_ratio_left);
    app_canTx_VC_SlipRatioRight_set(slip_ratio_right);
    app_canTx_VC_PIDSlipRatioOutput_set(k);
    app_canTx_VC_CorrectionTorque_set(correction_torque);
    app_canTx_VC_PIDSlipRatioError_set(pid->error);
    app_canTx_VC_PIDSlipRatioDerivative_set(pid->derivative);
    app_canTx_VC_PIDSlipRatioIntegral_set(pid->integral);

}

float app_tractionControl_computeSlip(float motor_speed_rpm, float front_wheel_speed_rpm)
{
    return (motor_speed_rpm / GEAR_RATIO - front_wheel_speed_rpm) / (front_wheel_speed_rpm + SMALL_EPSILON);
}
