#pragma once

#include <expected>
#include <util_errorCodes.hpp>

namespace io::coolant
{
/*
 * Initialize the flow meter by configuring PWM input
 */
void init();

/*
 * Check if the flow meter is active. If not, set frequency to 0.
 */
std::expected<void, ErrorCode> checkIfFlowMeterActive();

/*
 * return the flow rate in liters per minute.
 * NOTE: flow rate is calculated based on frequency of the PWM signal
 */
float getFlowRate();
} // namespace io::coolant
