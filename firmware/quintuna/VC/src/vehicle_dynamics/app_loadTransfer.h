#pragma once
#include <app_vehicleDynamics.h>
#include "io_imu_config.h"

/*
 * Broadcasts the vertical force of each wheel
 * calculated based on IMU accelerations
 */
void app_wheelVerticalForces_broadcast(const ImuData *imu);

/*
 * Calculates the load transfer constant needed to distribute torque
 * to all the wheels
 */
float app_loadTransferConstant(float long_accel);

/*
 * Calculates torque to allocate to each wheel
 *
 * Notes:
 * Currently adapted to 2 wheel drive
 * Sends torque requests if TV is on
 */
void app_torqueAllocation(TorqueAllocationInputs *inputs, float loadTransferConst);
