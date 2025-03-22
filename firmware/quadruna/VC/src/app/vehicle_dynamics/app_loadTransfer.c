#include <app_loadTransfer.h>
#include <app_vehicleDynamicsConstants.h>
#include <app_utils.h>
#include <app_canTx.h>
#include <io_log.h>

// https://www.zotero.org/groups/5809911/vehicle_controls_2024/items/N4TQBR67/reader used for load transfer equations
#define DIST_FRONT_AXLE_CG (0.837f)                          // a in meters
#define DIST_REAR_AXLE_CG (WHEELBASE_m - DIST_FRONT_AXLE_CG) // b in meters
#define WEIGHT_ACROSS_BODY (CAR_MASS_AT_CG_KG * GRAVITY / WHEELBASE_m)

/************** Macros for finding vertical forces on wheels based on diagram on page 21 ****************/
#define REAR_WEIGHT_DISTRIBUTION (WEIGHT_ACROSS_BODY * DIST_REAR_AXLE_CG)
#define LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel) \
    ((CAR_MASS_AT_CG_KG * (long_accel) * CG_HEIGHT_FROM_GROUND_m / WHEELBASE_m))
#define LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel) \
    ((CAR_MASS_AT_CG_KG * (lat_accel) * CG_HEIGHT_FROM_GROUND_m / (2.0f * (TRACK_WIDTH_m))))

/************** Macros for finding Kmz based on diagram on page 57 ****************/
#define ACCELERATION_TERM_KMZ(long_accel) (DIST_FRONT_AXLE_CG + (long_accel) * CG_HEIGHT_FROM_GROUND_m / GRAVITY)

/************** Macros for moment scaling on page 58 ****************/
#define F ((TRACK_WIDTH_m / (WHEEL_DIAMETER_IN / 2.0f * 2.54f)) * GEAR_RATIO)

void app_wheelVerticalForces_broadcast(const ImuData *imu)
{
    app_canTx_VC_FrontLeftWheelVerticalForce_set(
        ((REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 4.0f) -
        LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
    app_canTx_VC_FrontRightWheelVerticalForce_set(
        ((REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 4.0f) +
        LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
    app_canTx_VC_RearLeftWheelVerticalForce_set(
        ((REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 4.0f) -
        LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
    app_canTx_VC_RearRightWheelVerticalForce_set(
        ((REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 4.0f) +
        LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
}

float app_loadTransferConstant(float long_accel)
{
    // following formula for Kmz on page 57
    float load_transfer_scalar =
        ((CAR_MASS_AT_CG_KG * GRAVITY) - (WEIGHT_ACROSS_BODY * ACCELERATION_TERM_KMZ((long_accel / GRAVITY)))) /
        (WEIGHT_ACROSS_BODY * ACCELERATION_TERM_KMZ((long_accel / GRAVITY)));

    app_canTx_VC_LoadTransferScalar_set(load_transfer_scalar);
    return load_transfer_scalar;
}

void app_torqueAllocation(TorqueAllocationInputs *inputs, float loadTransferConst)
{
    // variable is created for debugging
    // paper divides torque by 2, I assume that has something to do with using 4 motors so I will exclude that
    // rear wheels get same distribution due to weight... yaw rate changes the distribution
    // this should be the final function seen before any torque goes to the car
    // future inputs should include front and rear yaw rate

    inputs->rear_left_motor_torque =
        (inputs->rear_left_motor_torque) -
        (loadTransferConst * inputs->rear_left_motor_torque / (4 * (loadTransferConst + 1)));
    inputs->rear_right_motor_torque = inputs->rear_left_motor_torque - (inputs->rear_yaw_moment / F);

    inputs->rear_left_motor_torque  = CLAMP(inputs->rear_left_motor_torque, 0, MAX_TORQUE_REQUEST_NM);
    inputs->rear_right_motor_torque = CLAMP(inputs->rear_right_motor_torque, 0, MAX_TORQUE_REQUEST_NM);

    // Commented out can messages, there solely for logging purposes

    // app_canTx_VC_LeftLoadBasedTorqueReq_set(inputs->rear_left_motor_torque);
    // app_canTx_VC_RightLoadBasedTorqueReq_set(inputs->rear_right_motor_torque);
    app_canTx_VC_LeftInverterTorqueCommand_set(inputs->rear_left_motor_torque);
    app_canTx_VC_RightInverterTorqueCommand_set(inputs->rear_right_motor_torque);
    app_canTx_VC_RearYawMoment_set(inputs->rear_yaw_moment);
    app_canTx_VC_FrontYawMoment_set(inputs->front_yaw_moment);
}
