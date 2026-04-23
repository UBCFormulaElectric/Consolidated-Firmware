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
 * Check if the Sbg Ellipsed Initialized properly 
 */
bool sbgInitOk();

/*
 * Get Ekf Solution mode
 *
 * POSITION represents the highest accuracy mode
 */
app::can_utils::VcEkfStatus getEkfSolutionMode(void);
} // namespace app::sbgEllipse
