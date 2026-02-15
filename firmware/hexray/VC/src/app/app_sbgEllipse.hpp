#pragma once

#include <cstdint>
#include "io_sbgEllipse.hpp"
#include "app_math.hpp"
#include "io_log.hpp"

namespace app::sbgEllipse
{
/*
 * Broadcast sensor outputs over CAN.
 */
void broadcast(void);

/*
 * Calculate vehicle velocity based on motor RPM
 */
void calculateVelocity(VelocityData *velocity);

/*
 * Get vehicle velocity
 */
float getVehicleVelocity(void);

/*
 * Get vehicle velocity
 */
VcEkfStatus getEkfSolutionMode(void);
} // namespace app::sbgEllipse
