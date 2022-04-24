#include "App_SharedTorqueRequest.h"

float App_SharedTorqueRequest_CalculateTorqueRequest(
    float right_wheel_speed_kph,
    float left_wheel_speed_kph,
    bool  air_negative_status,
    bool  air_positive_status,
    float regen_paddle_percentage,
    float accel_pedal_percentage)
{
    const bool are_both_airs_closed =
        air_negative_status && air_positive_status;
    const bool is_vehicle_over_regen_threshold =
        right_wheel_speed_kph > REGEN_WHEEL_SPEED_THRESHOLD_KPH &&
        left_wheel_speed_kph > REGEN_WHEEL_SPEED_THRESHOLD_KPH;

    // Regen allowed when wheel speed > REGEN_WHEEL_SPEED_THRESHOLD_KPH and AIRs
    // closed
    const bool is_regen_allowed =
        are_both_airs_closed && is_vehicle_over_regen_threshold;

    float torque_request;

    // Calculating the torque request
    // 1) If regen is allowed and the regen paddle is actuated, use the regen
    // paddle percentage 2) If regen is not allowed, use the accelerator pedal
    // percentage
    //
    // Constants:  MAX_TORQUE_REQUEST_NM = 132 Nm,
    //              - the max torque the motor can provide
    //
    // 1) If regen is allowed and the regen paddle is actuated,
    //         the torque request (in Nm) is negative and given by:
    //              (-) Regen Paddle Percentage
    //  Torque  =  -------------------------------  * MAX_TORQUE_REQUEST_NM
    //                         100%
    // 2) Otherwise, the torque request (in Nm) is positive and given by:
    //              Accelerator Pedal Percentage
    //  Torque =  -------------------------------  * MAX_TORQUE_REQUEST_NM
    //                        100%

    if (regen_paddle_percentage > 0.0f && is_regen_allowed)
    {
        torque_request =
            -0.01f * regen_paddle_percentage * MAX_TORQUE_REQUEST_NM;
    }
    else
    {
        torque_request = 0.01f * accel_pedal_percentage * MAX_TORQUE_REQUEST_NM;
    }

    return torque_request;
}
