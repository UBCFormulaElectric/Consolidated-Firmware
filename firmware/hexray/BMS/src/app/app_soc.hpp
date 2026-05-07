#pragma once

#include <cstdint>

namespace app::soc
{

inline constexpr uint32_t AH_TO_COULOMBS  = 3600.0f;
inline constexpr float    STATE_OF_HEALTH = 1.0f;
// TODO: Replace with cell capacity and cells in parallel constants in app_segments once that's finished
inline constexpr float SERIES_ELEMENT_FULL_CHARGE_C = 3.0f * AH_TO_COULOMBS * 4.0f * STATE_OF_HEALTH;

/**
 * @brief Initialize the SOC module.
 */
void init();

/**
 * @brief Return whether the SoC estimate is dirty and needs to be updated.
 */
bool getDirty();

/**
 * @brief Integrate current over time to update SoC charge estimate.
 */
void updateSocStats();

/**
 * @brief Get current minimum SoC in coulombs.
 */
float getMinSocCoulombs();

/**
 * @brief Get current minimum SoC in percent.
 */
float getMinSocPercent();

/**
 * @brief Get OCV estimate corresponding to current minimum SoC.
 */
float getMinOcvFromSoc();

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
