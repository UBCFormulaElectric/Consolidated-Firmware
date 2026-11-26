#pragma once
#include <cstring>
#include <concepts>

#include "io_batteryMonitoring_datatypes.hpp"
#include "util_errorCodes.hpp"

namespace io::batteryMonitoring
{
/**
 * @brief Gets the cell voltage
 * @param request The voltage to read (ie: CELL1_V or PACK_V)
 * @return The voltage on success, errorcode if messed up
 */
template <typename T>
    requires(std::same_as<T, cellReading> || std::same_as<T, systemReading>)
result<float> getVoltage(const T request);
/**
 * @brief Gets the current through pack
 * @param void
 * @return float value of current
 */
result<float> getCurrent();
/**
 * @brief Gets the temperature of the die 
 * @param void
 * @return float value of temperature
 */
result<float> getTemperature();
/**
 * @brief initialize configs on chip
 * @param void
 */
result<void> init();
/**
 * @brief Reads a safety status register
 * @param T SafetyStatusA (cell over/undervoltage, overcurrent, short circuit) or SafetyStatusB (temperature faults)
 * @return the status bits on success, errorcode if messed up
 */
template <typename T>
    requires(std::same_as<T, SafetyStatusA> || std::same_as<T, SafetyStatusB>)
result<T> getSafetyStatus();

} // namespace io::batteryMonitoring    