#pragma once

#include <stdint.h>
#include "io_sbgEllipse.h"
#include "app_canTx.h"

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
 * Get vehicle velocity
 */
VcEkfStatus app_sbgEllipse_getEkfSolutionMode(void);