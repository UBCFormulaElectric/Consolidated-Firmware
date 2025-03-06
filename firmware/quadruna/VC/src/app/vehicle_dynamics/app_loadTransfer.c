#include <app_loadTransfer.h>
#include <app_vehicleDynamicsConstants.h>
#include <app_imu.h>
#include <io_imu.h>
#include <app_canTx.h>

// https://www.zotero.org/groups/5809911/vehicle_controls_2024/items/N4TQBR67/reader used for load transfer equations 
#define DIST_FRONT_AXLE_CG (0.837f)  // a in meters 
#define DIST_REAR_AXLE_CG (WHEELBASE_m - DIST_FRONT_AXLE_CG) // b in meters
#define WEIGHT_ACROSS_BODY (CAR_MASS_AT_CG_KG * GRAVITY / WHEELBASE_m)

/************** Macros for finding verticle forces on wheels based on diagram on page 21 ****************/
#define REAR_WEIGHT_DISTRIBUTION (WEIGHT_ACROSS_BODY * DIST_REAR_AXLE_CG)
#define LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel) ((CAR_MASS_AT_CG_KG * (long_accel) * CG_HEIGHT_FROM_GROUND / WHEELBASE_m))
#define LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel) ((CAR_MASS_AT_CG_KG * (lat_accel) * CG_HEIGHT_FROM_GROUND / (2.0f * TRACK_WIDTH_m)))

/************** Macros for finding Kmz based on diagram on page 57 ****************/
#define ACCELERATION_TERM_KMZ(long_accel) (DIST_FRONT_AXLE_CG + (long_accel) * CG_HEIGHT_FROM_GROUND / GRAVITY)


void app_wheelVerticalForce_broadcast(imu_data *imu)
{
    app_canTx_VC_FrontLeftWheelVerticalForce_set(((REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 2.0f) - LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
    app_canTx_VC_FrontRightWheelVerticalForce_set(((REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 2.0f) + LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
    app_canTx_VC_RearLeftWheelVerticalForce_set(((REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 2.0f) - LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));
    app_canTx_VC_RearRightWheelVerticalForce_set(((REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu->long_accel)) / 2.0f) + LAT_ACCEL_TERM_VERTICAL_FORCE(imu->lat_accel));

}

float app_loadTransferConstant(float long_accel)
{
    // following formula for Kmz on page 57
    // Only looks at long shift?? 
   return ((CAR_MASS_AT_CG_KG - (WEIGHT_ACROSS_BODY * ACCELERATION_TERM_KMZ(long_accel)) / (WEIGHT_ACROSS_BODY * ACCELERATION_TERM_KMZ(long_accel))));
}

void torqueAllocation(TorqueAllocationInputs *inputs, float loadTransferConst)
{
    // variable is created for debugging 
    // paper divides torque by 2, I assume that has something to do with using 4 motors so I will exclude that
    // rear wheels get same distribution due to weight... yaw rate changes the distribution 
    float rear_motor = (inputs->rear_left_motor_torque) - (loadTransferConst * inputs->rear_left_motor_torque / (2 * loadTransferConst + 1));
    app_canTx_VC_RightLoadBasedTorqueReq_set(rear_motor);
    app_canTx_VC_LeftLoadBasedTorqueReq_set(rear_motor);
}




