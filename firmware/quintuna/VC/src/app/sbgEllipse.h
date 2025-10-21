#pragma once

#include <stdint.h>
#include "io/sbgEllipse.h"
#include "app_canTx.h"

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
float app_sbgEllipse_getVehicleVelocity(void);

/*
 * Get vehicle velocity
 */
VcEkfStatus app_sbgEllipse_getEkfSolutionMode(void);