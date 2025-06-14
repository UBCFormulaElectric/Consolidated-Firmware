#include "app_vehicleDynamicsConstants.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "io_log.h"
#include "app_canRx.h"
#include "app_units.h"
#include "io_imu_config.h"
#include "app_vehicleDynamics.h"
#include "app_torqueDistribution.h"
#include "io_imu_config.h"
#include "app_powerLimiting.h"

float app_totalPower(TorqueAllocationOutputs *torques)
{
    return (float)(TORQUE_TO_POWER(torques->front_left_torque, app_canRx_INVFL_ActualVelocity_get()) +
                   TORQUE_TO_POWER(torques->front_right_torque, app_canRx_INVFR_ActualVelocity_get()) +
                   TORQUE_TO_POWER(torques->rear_left_torque, app_canRx_INVRL_ActualVelocity_get()) +
                   TORQUE_TO_POWER(torques->rear_right_torque, app_canRx_INVRR_ActualVelocity_get()));
}

static float app_totalWheelSpeed()
{
    return (float)(app_canRx_INVFL_ActualVelocity_get() + app_canRx_INVFR_ActualVelocity_get() +
                   app_canRx_INVRL_ActualVelocity_get() + app_canRx_INVRR_ActualVelocity_get());
}

void app_wheelVerticalForces_broadcast(const ImuData *imu_data)
{
    app_canTx_VC_FrontLeftWheelVerticalForce_set(
        ((REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu_data->long_accel)) / 4.0f) -
        LAT_ACCEL_TERM_VERTICAL_FORCE(imu_data->lat_accel));
    app_canTx_VC_FrontRightWheelVerticalForce_set(
        ((REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu_data->long_accel)) / 4.0f) +
        LAT_ACCEL_TERM_VERTICAL_FORCE(imu_data->lat_accel));
    app_canTx_VC_RearLeftWheelVerticalForce_set(
        ((REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu_data->long_accel)) / 4.0f) -
        LAT_ACCEL_TERM_VERTICAL_FORCE(imu_data->lat_accel));
    app_canTx_VC_RearRightWheelVerticalForce_set(
        ((REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu_data->long_accel)) / 4.0f) +
        LAT_ACCEL_TERM_VERTICAL_FORCE(imu_data->lat_accel));
}

float app_loadTransferConstant(float long_accel)
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

    torqueToMotors->front_left_torque =
        (inputs->total_torque_request * inputs->load_transfer_const) / (2 * (inputs->load_transfer_const + 1)) -
        (inputs->front_yaw_moment / (2 * F));
    torqueToMotors->front_right_torque = (torqueToMotors->front_left_torque + (inputs->front_yaw_moment / F));

    torqueToMotors->rear_left_torque =
        (inputs->total_torque_request / 2) -
        (inputs->total_torque_request * inputs->load_transfer_const) / (2 * (inputs->load_transfer_const + 1));
    torqueToMotors->rear_right_torque = (torqueToMotors->rear_left_torque + (inputs->rear_yaw_moment / F));

    float total_requestedPower = app_totalPower(torqueToMotors);
    app_canTx_VC_RequestedPower_set(total_requestedPower);

    app_torqueReduction(total_requestedPower, inputs->power_limit_kw, torqueToMotors);

    app_canTx_VC_TotalAllocatedPower_set(app_totalPower(torqueToMotors));

    // Commented out can messages, there solely for logging purposes
    app_canTx_VC_RearYawMoment_set(inputs->rear_yaw_moment);
    app_canTx_VC_FrontYawMoment_set(inputs->front_yaw_moment);

    return;
}

void app_torqueBroadCast(TorqueAllocationOutputs *torqueToMotors)
{
    app_canTx_VC_INVFLTorqueSetpoint_set(PEDAL_REMAPPING(torqueToMotors->front_left_torque));
    app_canTx_VC_INVFRTorqueSetpoint_set(PEDAL_REMAPPING(torqueToMotors->front_right_torque));
    app_canTx_VC_INVRLTorqueSetpoint_set(PEDAL_REMAPPING(torqueToMotors->rear_left_torque));
    app_canTx_VC_INVRRTorqueSetpoint_set(PEDAL_REMAPPING(torqueToMotors->rear_right_torque));
}

// TorqueAllocationOutputs *app_get_torqueToMotors()
// {
//     return &torqueToMotors;
// }

void app_reset_torqueToMotors(TorqueAllocationOutputs *torqueToMotors)
{
    torqueToMotors->front_left_torque  = 0.0f;
    torqueToMotors->front_right_torque = 0.0f;
    torqueToMotors->rear_left_torque   = 0.0f;
    torqueToMotors->rear_right_torque  = 0.0f;
}

void app_torqueReduction(float total_requestedPower, float power_limit, TorqueAllocationOutputs *torqueToMotors)
{
    if (total_requestedPower > power_limit)
    {
        float torque_reduction = POWER_TO_TORQUE((total_requestedPower - power_limit), app_totalWheelSpeed());

        torqueToMotors->front_left_torque -= torque_reduction;
        torqueToMotors->front_right_torque -= torque_reduction;
        torqueToMotors->rear_left_torque -= torque_reduction;
        torqueToMotors->rear_left_torque -= torque_reduction;
    }

    torqueToMotors->front_left_torque  = CLAMP(torqueToMotors->front_left_torque, 0, MAX_TORQUE_REQUEST_NM);
    torqueToMotors->front_right_torque = CLAMP(torqueToMotors->front_right_torque, 0, MAX_TORQUE_REQUEST_NM);
    torqueToMotors->rear_left_torque   = CLAMP(torqueToMotors->rear_left_torque, 0, MAX_TORQUE_REQUEST_NM);
    torqueToMotors->rear_right_torque  = CLAMP(torqueToMotors->rear_right_torque, 0, MAX_TORQUE_REQUEST_NM);
}
