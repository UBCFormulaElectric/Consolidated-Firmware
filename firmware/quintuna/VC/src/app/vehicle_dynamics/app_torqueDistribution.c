#include "app_vehicleDynamicsConstants.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "io_log.h"
#include "app_canRx.h"
#include "app_units.h"
#include "app_vehicleDynamics.h"
#include "app_torqueDistribution.h"
#include "io_imu_config.h"
#include "app_powerLimiting.h"

#define TORQUE_LIMITER_EN 1

static PowerLimitingInputs powerLimitingInputs;

static float app_torqueDistribution_powerToTorque(const float power_kW)
{
    float total_wheel_speed = (float)(app_canRx_INVRL_ActualVelocity_get() + app_canRx_INVRR_ActualVelocity_get() +
                                app_canRx_INVFL_ActualVelocity_get() + app_canRx_INVFR_ActualVelocity_get());

    return (POWER_TO_TORQUE_CONVERSION_FACTOR * power_kW) / (total_wheel_speed + SMALL_EPSILON);
}

void app_wheelVerticalForces_broadcast(const ImuData *imu_data)
{
    app_canTx_VC_FrontLeftWheelVerticalForce_set(
        (uint32_t)(((REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu_data->long_accel)) / 4.0f) -
                   LAT_ACCEL_TERM_VERTICAL_FORCE(imu_data->lat_accel)));
    app_canTx_VC_FrontRightWheelVerticalForce_set(
        (uint32_t)(((REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu_data->long_accel)) / 4.0f) +
                   LAT_ACCEL_TERM_VERTICAL_FORCE(imu_data->lat_accel)));
    app_canTx_VC_RearLeftWheelVerticalForce_set(
        (uint32_t)(((REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu_data->long_accel)) / 4.0f) -
                   LAT_ACCEL_TERM_VERTICAL_FORCE(imu_data->lat_accel)));
    app_canTx_VC_RearRightWheelVerticalForce_set(
        (uint32_t)(((REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu_data->long_accel)) / 4.0f) +
                   LAT_ACCEL_TERM_VERTICAL_FORCE(imu_data->lat_accel)));
}

float app_loadTransferConstant(const float long_accel)
{
    /************************************** following formula for Kmz on page 57*********************************/
    float load_transfer_scalar =
        ((CAR_MASS_AT_CG_KG * GRAVITY) - (WEIGHT_ACROSS_BODY * ACCELERATION_TERM_KMZ((long_accel / GRAVITY)))) /
        (WEIGHT_ACROSS_BODY * ACCELERATION_TERM_KMZ((long_accel / GRAVITY)));

    app_canTx_VC_LoadTransferScalar_set(load_transfer_scalar);
    return load_transfer_scalar;
}

void app_torqueAllocation(TorqueAllocationInputs *inputs, TorqueAllocationOutputs *torqueToMotors)
{
    /************************************** following torque distribution on page 57 *********************************/

    // below the formulas are written with respect to left hand turns.... this is opposite of what active differential
    // previously did I believe this will work better with the imu as a left hand turn induces a counter clockwise yaw
    // moment

    // Total toruqe is to be our torque request from pedal * 4. We will assume the pedal percentage applies to
    // percentage of max torque of motor not of the car

    // potential divisino by 0 if load_transfer_const = -1, not sure what values are expected here but should add safety
    float front_yaw_moment   = inputs->yaw_moment / (1 + inputs->load_transfer_const);
    float rear_yaw_moment   = inputs->yaw_moment - front_yaw_moment;

    float total_torque_limited = app_torqueDistribution_totalTorqueLimit(inputs->total_torque_request);

    torqueToMotors->front_left_torque =
        (total_torque_limited * inputs->load_transfer_const) / (2 * (inputs->load_transfer_const + 1)) -
        (front_yaw_moment / (2 * F));
    torqueToMotors->front_right_torque = (torqueToMotors->front_left_torque + (front_yaw_moment / F));

    torqueToMotors->rear_left_torque =
        (total_torque_limited / 2) -
        (total_torque_limited * inputs->load_transfer_const) / (2 * (inputs->load_transfer_const + 1)) -
        (front_yaw_moment / (2 * F));
    torqueToMotors->rear_right_torque = (torqueToMotors->rear_left_torque + (rear_yaw_moment / F));

    powerLimitingInputs.total_requestedPower = app_totalPower(torqueToMotors);
    app_canTx_VC_RequestedPower_set(powerLimitingInputs.total_requestedPower);
    powerLimitingInputs.is_regen_mode  = false;
    powerLimitingInputs.torqueToMotors = torqueToMotors;
    powerLimitingInputs.power_limit    = inputs->power_limit_kw;

    app_powerLimiting_torqueReduction(&powerLimitingInputs);

    app_canTx_VC_TotalAllocatedPower_set(app_totalPower(torqueToMotors));

    // Commented out can messages, there solely for logging purposes
    app_canTx_VC_RearYawMoment_set(rear_yaw_moment);
    app_canTx_VC_FrontYawMoment_set(front_yaw_moment);
}

// void app_torqueBroadCast(TorqueAllocationOutputs *torqueToMotors)
// {
//     //     app_canTx_VC_INVFLTorqueSetpoint_set(
//     //         CLAMP(PEDAL_REMAPPING(torqueToMotors->front_left_torque), PEDAL_REMAPPING(-MAX_TORQUE_REQUEST_NM),
//     //         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
//     //     app_canTx_VC_INVFRTorqueSetpoint_set(
//     //         CLAMP(PEDAL_REMAPPING(torqueToMotors->front_right_torque), PEDAL_REMAPPING(-MAX_TORQUE_REQUEST_NM),
//     //         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
//     //     app_canTx_VC_INVRLTorqueSetpoint_set(
//     //         CLAMP(PEDAL_REMAPPING(torqueToMotors->rear_left_torque), PEDAL_REMAPPING(-MAX_TORQUE_REQUEST_NM),
//     //         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
//     //     app_canTx_VC_INVRRTorqueSetpoint_set(
//     //         CLAMP(PEDAL_REMAPPING(torqueToMotors->rear_right_torque), PEDAL_REMAPPING(-MAX_TORQUE_REQUEST_NM),
//     //         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
//     app_canTx_VC_INVFLTorqueSetpoint_set(CLAMP(
//         PEDAL_REMAPPING(torqueToMotors->front_left_torque), PEDAL_REMAPPING(MAX_REGEN_Nm),
//         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
//     app_canTx_VC_INVFRTorqueSetpoint_set(CLAMP(
//         PEDAL_REMAPPING(torqueToMotors->front_right_torque), PEDAL_REMAPPING(MAX_REGEN_Nm),
//         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
//     app_canTx_VC_INVRLTorqueSetpoint_set(CLAMP(
//         PEDAL_REMAPPING(torqueToMotors->rear_left_torque), PEDAL_REMAPPING(MAX_REGEN_Nm),
//         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
//     app_canTx_VC_INVRRTorqueSetpoint_set(CLAMP(
//         PEDAL_REMAPPING(torqueToMotors->rear_right_torque), PEDAL_REMAPPING(MAX_REGEN_Nm),
//         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
// }

// TorqueAllocationOutputs *app_get_torqueToMotors()
// {
//     return &torqueToMotors;
// }

float app_torqueDistribution_totalTorqueLimit(float total_torque)
{
    /**
     * Feed forward torque limiting:
     * should encompass traction and power limiting torque limits, but additional control systems should still be used
     * basically like feedforward control
     * 
     * This does not account for:
     * downforce
     * drag
     * rolling resistance
     * changing friction coefficients
     * changing tire diameter
     * 
     * it is idealized but provides a theoretical upper bound for the torque of the car
     */
#if (TORQUE_LIMITER_EN)
    float power_limited_torque      = app_torqueDistribution_powerToTorque(POWER_LIMIT_CAR_kW);
    return fminf(STATIC_VEHICLE_TORQUE_LIMIT, power_limited_torque);
#else
    return total_torque;
#endif
}

void app_reset_torqueToMotors(TorqueAllocationOutputs *torqueToMotors)
{
    torqueToMotors->front_left_torque  = 0.0f;
    torqueToMotors->front_right_torque = 0.0f;
    torqueToMotors->rear_left_torque   = 0.0f;
    torqueToMotors->rear_right_torque  = 0.0f;
}