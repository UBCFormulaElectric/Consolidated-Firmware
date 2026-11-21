// to be written 
#pragma once
#include "app_canUtils.hpp"

namespace io::charger
{

/**
 * @brief Get the connection status of the EVSE CP PWM.
 *
 * @return ChargerConnectedType indicating whether the charger is
 *         connected to EVSE, wall, or disconnected.
 */
app::can_utils::ChargerConnectedType getConnectionStatus();

/**
 * @brief Get the duty cycle of the Control Pilot (CP) signal.
 * 
 * @return float Duty cycle in the range [0.0, 1.0].
 */
float getCPDutyCycle();

/**
 * @brief Get the frequency of the Control Pilot (CP) signal.
 * 
 * @return float Frequency in Hz.
 */
float getCPFrequency();

} // namespace io::charger