#pragma once
#include "app_vehicleDynamics.h"
#include "io_imu_config.h"
#include "app_vehicleDynamicsConstants.h"

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
 */
void app_torqueAllocation(TorqueAllocationInputs *inputs, TorqueAllocationOutputs *torqueToMotors);

void app_reset_torqueToMotors(TorqueAllocationOutputs *torqueToMotors);

float app_torqueDistribution_totalTorqueLimit(float total_torque);

TorqueAllocationOutputs *app_get_torqueToMotors(TorqueAllocationOutputs *torqueToMotors);