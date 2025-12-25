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

static PowerLimitingInputs powerLimitingInputs;

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
    torqueToMotors->front_left_torque =
        (inputs->total_torque_request * inputs->load_transfer_const) / (2 * (inputs->load_transfer_const + 1)) -
        (inputs->front_yaw_moment / (2 * F));
    torqueToMotors->front_right_torque = (torqueToMotors->front_left_torque + (inputs->front_yaw_moment / F));

    torqueToMotors->rear_left_torque =
        (inputs->total_torque_request / 2) -
        (inputs->total_torque_request * inputs->load_transfer_const) / (2 * (inputs->load_transfer_const + 1));
    torqueToMotors->rear_right_torque = (torqueToMotors->rear_left_torque + (inputs->rear_yaw_moment / F));

    powerLimitingInputs.total_requestedPower = app_totalPower(torqueToMotors);
    app_canTx_VC_RequestedPower_set(powerLimitingInputs.total_requestedPower);
    powerLimitingInputs.is_regen_mode  = false;
    powerLimitingInputs.torqueToMotors = torqueToMotors;
    powerLimitingInputs.power_limit    = inputs->power_limit_kw;

    app_powerLimiting_torqueReduction(&powerLimitingInputs);

    app_canTx_VC_TotalAllocatedPower_set(app_totalPower(torqueToMotors));

    // Commented out can messages, there solely for logging purposes
    app_canTx_VC_RearYawMoment_set(inputs->rear_yaw_moment);
    app_canTx_VC_FrontYawMoment_set(inputs->front_yaw_moment);

    return;
}

void app_torqueBroadCast(TorqueAllocationOutputs *torqueToMotors)
{
    //     app_canTx_VC_INVFLTorqueSetpoint_set(
    //         CLAMP(PEDAL_REMAPPING(torqueToMotors->front_left_torque), PEDAL_REMAPPING(-MAX_TORQUE_REQUEST_NM),
    //         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
    //     app_canTx_VC_INVFRTorqueSetpoint_set(
    //         CLAMP(PEDAL_REMAPPING(torqueToMotors->front_right_torque), PEDAL_REMAPPING(-MAX_TORQUE_REQUEST_NM),
    //         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
    //     app_canTx_VC_INVRLTorqueSetpoint_set(
    //         CLAMP(PEDAL_REMAPPING(torqueToMotors->rear_left_torque), PEDAL_REMAPPING(-MAX_TORQUE_REQUEST_NM),
    //         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
    //     app_canTx_VC_INVRRTorqueSetpoint_set(
    //         CLAMP(PEDAL_REMAPPING(torqueToMotors->rear_right_torque), PEDAL_REMAPPING(-MAX_TORQUE_REQUEST_NM),
    //         PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM)));
    if(app_canRx_Debug_EnableBreakinMode_get()){
        app_canTx_VC_INVFLTorqueSetpoint_set((int32_t)app_canRx_Debug_VelocitySetpoint_get());
        app_canTx_VC_INVFRTorqueSetpoint_set((int32_t)app_canRx_Debug_VelocitySetpoint_get());
        app_canTx_VC_INVRLTorqueSetpoint_set((int32_t)app_canRx_Debug_VelocitySetpoint_get());
        app_canTx_VC_INVRRTorqueSetpoint_set((int32_t)app_canRx_Debug_VelocitySetpoint_get()); 
    }
    else{
        app_canTx_VC_INVFLTorqueSetpoint_set(0);
        app_canTx_VC_INVFRTorqueSetpoint_set(0);
        app_canTx_VC_INVRLTorqueSetpoint_set(0);
        app_canTx_VC_INVRRTorqueSetpoint_set(0); 
    }
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