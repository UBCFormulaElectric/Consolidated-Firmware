#pragma once

#include <cstdint>

namespace app::soc
{
/**
 * @brief Initialize the SOC module.
 */
void init();

/**
 * @brief Return whether the SoC estimate is dirty and needs to be updated.
 */
bool getDirty();

/**
 * @brief Get current minimum SoC in percent.
 */
float getMinSocPercent();

/**
 * @brief Reset SoC estimate using a voltage-based method.
 */
void resetSocFromVoltage();

/**
 * @brief Reset SoC estimate using a specific value in percent.
 */
void resetSocCustomValue(float soc_percent);

/**
 * @brief Broadcast SOC related CAN signals.
 */
void broadcast();

} // namespace app::soc
