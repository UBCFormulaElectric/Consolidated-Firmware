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
 * @brief Retrieve the duty cycle of the EVSE CP PWM signal.
 *
 * @return float Duty cycle in the range [0.0, 1.0].
 */
float getCPDutyCycle();

/**
 * @brief Retrieve the frequency of the EVSE CP PWM signal.
 *
 * @return float Frequency in Hz.
 */
float getCPFrequency();

} // namespace io::charger