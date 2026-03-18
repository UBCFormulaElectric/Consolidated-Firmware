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
 * Get vehicle velocity
 */
float getVehicleVelocity(void);

/*
 * Get vehicle velocity
 */
app::can_utils::VcEkfStatus getEkfSolutionMode(void);
} // namespace app::sbgEllipse
