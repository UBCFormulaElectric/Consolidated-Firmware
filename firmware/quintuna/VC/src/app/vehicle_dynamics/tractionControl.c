#include "app_canTx.h"
#include "app/units.h"
#include "app/vehicle_dynamics/tractionControl.h"
#include "app/vehicle_dynamics/pid.h"
#include "app_canTx.h"
#include "app/vehicle_dynamics/vehicleDynamicsConstants.h"
#include <math.h>

// consider velocity different on different points of the car
/**
 * Ex. Driving in a left hand circle
 *
 * Outer side of the car travels faster than the inside of the car?
 * Idk if this is correct as the car is a single body not independent
 * like the wheels
 */
void app_tractionControl_computeTorque(TractionControl_Inputs *inputs, TractionControl_Outputs *outputs)
{
    PID *pid = inputs->pid;

    float slip_ratio_fl = app_tractionControl_computeSlip(inputs->motor_speed_fl_rpm, inputs->vehicle_velocity_kmh);
    float slip_ratio_fr = app_tractionControl_computeSlip(inputs->motor_speed_fr_rpm, inputs->vehicle_velocity_kmh);
    float slip_ratio_rl = app_tractionControl_computeSlip(inputs->motor_speed_rl_rpm, inputs->vehicle_velocity_kmh);
    float slip_ratio_rr = app_tractionControl_computeSlip(inputs->motor_speed_rr_rpm, inputs->vehicle_velocity_kmh);

    float slip_ratio_max = fmaxf(fmaxf(slip_ratio_fl, slip_ratio_fr), fmaxf(slip_ratio_rl, slip_ratio_rr));
    float k              = app_pid_compute(pid, SLIP_RATIO_IDEAL, slip_ratio_max);

    // Send debug messages over CAN
    app_canTx_VC_SlipRatioFL_set(slip_ratio_fl);
    app_canTx_VC_SlipRatioFR_set(slip_ratio_fr);
    app_canTx_VC_SlipRatioRL_set(slip_ratio_rl);
    app_canTx_VC_SlipRatioRR_set(slip_ratio_rr);
    app_canTx_VC_PIDSlipRatioOutput_set(k);
    app_canTx_VC_PIDSlipRatioError_set(pid->error);
    app_canTx_VC_PIDSlipRatioDerivative_set(pid->derivative);
    app_canTx_VC_PIDSlipRatioIntegral_set(pid->integral);

    // NOTE: k strictly in range [-1 0] to prevent exceeding power limit
    outputs->torque_fl_final_Nm = (1.0f + k) * inputs->torque_fl_Nm;
    outputs->torque_fr_final_Nm = (1.0f + k) * inputs->torque_fr_Nm;
    outputs->torque_rl_final_Nm = (1.0f + k) * inputs->torque_rl_Nm;
    outputs->torque_rr_final_Nm = (1.0f + k) * inputs->torque_rr_Nm;
}

float app_tractionControl_computeSlip(const float motor_speed_rpm, const float vehicle_velocity)
{
    return (MOTOR_RPM_TO_KMH(motor_speed_rpm) / GEAR_RATIO - vehicle_velocity) / (vehicle_velocity + SMALL_EPSILON);
}
