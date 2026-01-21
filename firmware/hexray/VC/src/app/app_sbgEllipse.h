#pragma once

#include <stdint.h>
#include "io_sbgEllipse.h"
#include "app_canTx.h"

// VcEkfStatus enum - should match CAN enum definition
typedef enum
{
    VCEKF_UNINITIALIZED = 0,
    VCEKF_VERTICAL_GYRO = 1,
    VCEKF_AHRS          = 2,
    VCEKF_VELOCITY      = 3,
    VCEKF_POSITION      = 4
} VcEkfStatus;

#define POSITION VCEKF_POSITION

typedef struct
{
    double vehicle_vel_vx;
    double vehicle_vel_vy;
    double vehicle_vel;
} vehicle_velocity_components;

/*
 * Broadcast sensor outputs over CAN.
 */
void app_sbgEllipse_broadcast(void);

/*
 * Calculate vehicle velocity based on motor RPM
 */
void app_sbgEllipse_calculateVelocity(VelocityData *velocity);

/*
 * Get vehicle velocity
 */
vehicle_velocity_components app_sbgEllipse_getVehicleVelocity(void);

/*
 * Get EKF solution mode
 */
VcEkfStatus app_sbgEllipse_getEkfSolutionMode(void);
