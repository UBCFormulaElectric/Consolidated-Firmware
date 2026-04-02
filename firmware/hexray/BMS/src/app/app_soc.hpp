#pragma once

#include <cstdint>

namespace app::soc
{

inline constexpr uint32_t AH_TO_COULOMBS = 3600.0f;
inline constexpr float STATE_OF_HEALTH             = 1.0f;
// TODO: Replace with cell capacity and cells in parallel constants in app_segments once that's finished
inline constexpr float SERIES_ELEMENT_FULL_CHARGE_C = 3.0f * AH_TO_COULOMBS * 4.0f * STATE_OF_HEALTH;

#ifndef TARGET_EMBEDDED
/**
 * @brief Testing-only helper to set the previously used current sample.
 */
void setPrevCurrent(float current);
#endif

/**
 * @brief Get SoC estimate from open-circuit voltage.
 */
float getSocFromOcv(float voltage);

/**
 * @brief Get open-circuit voltage estimate from SoC.
 */
float getOcvFromSoc(float soc_percent);

/**
 * @brief Initialize the SOC module.
 */
void init();

/**
 * @brief Return whether startup SoC read was corrupt/unavailable.
 */
bool getCorrupt();

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
 * @brief Read SoC value from SD storage.
 */
bool readSocFromSd(float &saved_soc_percent);

/**
 * @brief Write SoC value to SD storage.
 */
bool writeSocToSd(float soc_percent);

/**
 * @brief Get the last SoC value written to SD card in tenths of a percent.
 */
uint32_t getLastWrittenSocTenths();

/**
 * @brief Broadcast SOC related CAN signals.
 */
void broadcast();

/**
 * @brief Save SoC to SD when the value is valid.
 */
void saveToSd();

} // namespace app::soc