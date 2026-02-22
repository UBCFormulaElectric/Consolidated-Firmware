#pragma once

namespace io::brake
{
/*
 * Get pressure in psi from the ___BrakePressureSensorName___
 * return the pressure of primary brake pressure sensor in psi
 */
float getRearPressurePsi();

/*
 * return the hardware overcurrent/short-circuit status for the brake system
 */
bool OCSC();
} // namespace io::brake
