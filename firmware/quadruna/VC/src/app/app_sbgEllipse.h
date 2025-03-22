#pragma once

#include <stdint.h>
#include "io_sbgEllipse.h"

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