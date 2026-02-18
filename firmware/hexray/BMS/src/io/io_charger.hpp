#pragma once

extern "C"
{
#include "app_canUtils.h"
}

namespace io::charger
{

/**
 * @brief Get the connection status of the EVSE CP PWM.
 *
 * @return ChargerConnectedType indicating whether the charger is
 *         connected to EVSE, wall, or disconnected.
 */
ChargerConnectedType getConnectionStatus();

/**
 * @brief Retrieve the duty cycle of the EVSE CP PWM signal.
 *
 * @return float Duty cycle in the range [0.0, 1.0].
 */
float getCPDutyCycle();

} // namespace io::charger