#include "app_vehicleDynamicsConstants.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "io_log.h"
#include "app_canRx.h"
#include "app_units.h"
#include "io_imu_config.h"
#include "app_vehicleDynamics.h"

/**
 * Reference used: https://www.zotero.org/groups/5809911/vehicle_controls_2024/items/N4TQBR67/reader
 */

// #define DIST_FRONT_AXLE_CG (0.837f)                          // a in meters
// #define DIST_REAR_AXLE_CG (WHEELBASE_m - DIST_FRONT_AXLE_CG) // b in meters
// #define WEIGHT_ACROSS_BODY (CAR_MASS_AT_CG_KG * GRAVITY / WHEELBASE_m)

/************** Macros for finding vertical forces on wheels based on diagram on page 21 ****************/
// #define REAR_WEIGHT_DISTRIBUTION (WEIGHT_ACROSS_BODY * DIST_REAR_AXLE_CG)
// #define LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel)
//     ((CAR_MASS_AT_CG_KG * (long_accel) * CG_HEIGHT_FROM_GROUND_m / WHEELBASE_m))
// #define LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel)
//     ((CAR_MASS_AT_CG_KG * (lat_accel) * CG_HEIGHT_FROM_GROUND_m / (2.0f * (TRACK_WIDTH_m))))

/************** Macros for finding Kmz based on diagram on page 57 ****************/
// #define ACCELERATION_TERM_KMZ(long_accel) (DIST_FRONT_AXLE_CG + (long_accel) * CG_HEIGHT_FROM_GROUND_m / GRAVITY)

/************** Macros for moment scaling on page 58 ****************/
// #define F ((TRACK_WIDTH_m / (WHEEL_DIAMETER_IN / 2.0f * 2.54f)) * GEAR_RATIO)

// Our motors no longer take in a torque command, instead they take in a percentage value. The percentage for the
// DD5-14-10-POW motor is relative to their nominal torque (9.8 Nm) where 100% torque is 9.8. The motors are able to
// output up to 21 Nm, this however cannot be a sustained behaviour #define PEDAL_REMAPPING(torque) ((torque /
// NOMINAL_TORQUE_NM) * 100.0f ) #define TORQUE_TO_POWER(torque, rpm) (((torque) * ((rpm) / GEAR_RATIO) /
// POWER_TO_TORQUE_CONVERSION_FACTOR)) #define POWER_TO_TORQUE(power, rpm) ((power) * POWER_TO_TORQUE_CONVERSION_FACTOR
// / ((rpm) / GEAR_RATIO))

// void app_wheelVerticalForces_broadcast(const ImuData *imu)
// {
//     app_canTx_VC_FrontLeftWheelVerticalForce_set(
//         ((REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 4.0f) -
//         LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
//     app_canTx_VC_FrontRightWheelVerticalForce_set(
//         ((REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 4.0f) +
//         LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
//     app_canTx_VC_RearLeftWheelVerticalForce_set(
//         ((REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 4.0f) -
//         LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
//     app_canTx_VC_RearRightWheelVerticalForce_set(
//         ((REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 4.0f) +
//         LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
// }

// float app_loadTransferConstant(float long_accel)
// {
/************************************** following formula for Kmz on page 57*********************************/
// float load_transfer_scalar =
//     ((CAR_MASS_AT_CG_KG * GRAVITY) - (WEIGHT_ACROSS_BODY * ACCELERATION_TERM_KMZ((long_accel / GRAVITY)))) /
//     (WEIGHT_ACROSS_BODY * ACCELERATION_TERM_KMZ((long_accel / GRAVITY)));

//     app_canTx_VC_LoadTransferScalar_set(load_transfer_scalar);
//     return load_transfer_scalar;
// }

// static float app_totalPower(TorqueAllocationOutputs *torques)
// {
//     return  (TORQUE_TO_POWER(torqueToMotors.front_left_torque, app_canRx_INVFL_AMK_ActualVelocity_get()) +
//             TORQUE_TO_POWER(torqueToMotors.front_right_torque, app_canRx_INVFR_ActualVelocity_get()) +
//             TORQUE_TO_POWER(torqueToMotors.rear_left_torque, app_canRx_INVRL_ActualVelocity_get()) +
//             TORQUE_TO_POWER(torqueToMotors.rear_right_torque, app_canRx_INVRR_ActualVelocity_get()));
// }
// TorqueAllocationOutputs* app_torqueAllocation(TorqueAllocationInputs *inputs)
// {
/************************************** following torque distribution on page 57 *********************************/

// below the formulas are written with respect to left hand turns.... this is opposite of what active differential
// previously did I believe this will work better with the imu as a left hand turn induces a counter clockwise yaw
// moment

// Total toruqe is to be our torque request from pedal * 4. We will assume the pedal percentage applies to percentage of
// max torque of motor not of the car

// torqueToMotors.front_left_torque = (inputs->total_torque_request * inputs->load_transfer_const) /
//                                     (2 * (inputs->load_transfer_const + 1)) - (inputs->front_yaw_moment/ (2 * F));
// torqueToMotors.front_right_torque = (torqueToMotors.front_left_torque + (inputs->front_yaw_moment / F));

// torqueToMotors.rear_left_torque = (inputs->total_torque_request / 2) -
//                                   (inputs->total_torque_request * inputs->load_transfer_const) / (2 *
//                                   (inputs->load_transfer_const + 1));
// torqueToMotors.rear_right_torque = (torqueToMotors.rear_left_torque + (inputs->rear_yaw_moment / F));

// float total_requestedPower = app_totalPower(torqueToMotors);
// app_canTx_VC_TotalRequestedPower_set(total_requestedPower);

// if(total_requestedPower > inputs-> power_limit_kw)
// {
// the idea is to reduce all torque requests by the same out of torque to get our power based on the torque request
// to be <= to the power_limit. The way to do this is to convert the differenc in the power to a torque using the lowest
// wheel speed, this is because it will provide the largest torque reduction and thus the largest decrease in power
// consumption

//         float torque_reduction = POWER_TO_TORQUE((total_requestedPower - inputs-> power_limit_kw),
//         fminf(fminf(app_canRx_INVFL_AMK_ActualVelocity_get(), app_canRx_INVFR_ActualVelocity_get()),
//                                                                                 fminf(app_canRx_INVRL_ActualVelocity_get,
//                                                                                 app_canRx_INVRR_ActualVelocity_get)));

//         torqueToMotors.front_left_torque -= torque_reduction;
//         torqueToMotors.front_right_torque -= torque_reduction;
//         torqueToMotors.rear_left_torque -= torque_reduction;
//         torqueToMotors.rear_left_torque -= torque_reduction;
//     }

//     torqueToMotors.front_left_torque = CLAMP(torqueToMotors.front_left_torque, 0, MAX_TORQUE_REQUEST_NM);
//     torqueToMotors.front_right_torque = CLAMP(torqueToMotors.front_right_torque, 0, MAX_TORQUE_REQUEST_NM);
//     torqueToMotors.rear_left_torque = CLAMP(torqueToMotors.rear_left_torque, 0, MAX_TORQUE_REQUEST_NM);
//     torqueToMotors.rear_right_torque = CLAMP(torqueToMotors.rear_right_torque, 0, MAX_TORQUE_REQUEST_NM);

//     app_canTx_VC_TotalAllocatedPower_set(app_totalPower(inputs));

//     // Commented out can messages, there solely for logging purposes
//     app_canTx_VC_RearYawMoment_set(inputs->rear_yaw_moment);
//     app_canTx_VC_FrontYawMoment_set(inputs->front_yaw_moment);

//     return &torqueToMotors;
// }

// void app_torqueBroadCast(TorqueAllocationOutputs *motor_torques)
// {
//     // TODO: ONCE INVERTER COMMUNUCATION IS DONE BROADCAST MOTOR TORQUES HERE
// }
