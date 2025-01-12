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
VelocityData app_sbgEllipse_calculateVelocity(void);