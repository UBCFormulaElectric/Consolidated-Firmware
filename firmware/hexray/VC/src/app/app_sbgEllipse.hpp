#pragma once

#include <cstdint>
#include "io_sbgEllipse.hpp"
#include "app_canUtils.hpp"

namespace app::sbgEllipse
{
/*
 * Broadcast sensor outputs over CAN.
 */
void broadcast(void);

/*
 * Get vehicle velocity
 */
float getVehicleVelocity(io::sbgEllipse::VelocityData *VelData);

/*
 * Get vehicle velocity
 */
app::can_utils::VcEkfStatus getEkfSolutionMode(void);
} // namespace app::sbgEllipse
