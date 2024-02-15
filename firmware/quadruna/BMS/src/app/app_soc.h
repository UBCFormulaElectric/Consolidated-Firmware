#pragma once

#include "app_utils.h"
#include "app_accumulator.h"
#include "app_timer.h"

#define STATE_OF_HEALTH (0.94f)
#define SERIES_ELEMENT_FULL_CHARGE_C (5.9f * 3600.0f * 3.0f * STATE_OF_HEALTH)

#ifndef TARGET_EMBEDDED
/**
 * @brief Allows specifying the prev_current member of the SocStats struct, for off-target testing only.
 * @param current current to set
 */
void app_soc_setPrevCurrent(float current);
#endif

/**
 * @brief Function to retrieve soc closest to given open circuit voltage
 * @param voltage open circuit voltage
 * @return soc related to open circuit voltage
 */
float app_soc_getSocFromOcv(float voltage);

/**
 * @brief Function to retrieve open circuit voltage closest to given soc
 * @param soc_percent state of charge in percent
 * @return open circuit voltage closest to given state of charge
 */
float app_soc_getOcvFromSoc(float soc_percent);

/**
 * @brief Initialize the SOC module.
 */
void app_soc_init(void);

/**
 * @brief Return the active address on the EEPROM where SOC values are being stored
 * @return page being used to store SOC values
 */
uint16_t app_soc_getSocAddress(void);

/**
 * @brief Return if the SOC value was corrupt on startup
 * @return corrupt status
 */
bool app_soc_getCorrupt(void);

/**
 * @brief Update the state of charge of all series elements using coulomb counting.
 */
void app_soc_updateSocStats(void);

/**
 * @brief return Coulombs of the SE with the lowest SOC
 * @return cloulombs of lowest SOC SE
 */
float app_soc_getMinSocCoulombs(void);

/**
 * @brief return percent SOC of the SE with the lowest SOC
 * @return soc % of lowest SOC SE
 */
float app_soc_getMinSocPercent(void);

/**
 * @brief Get the minimum series element open circuit voltage approximation given current pack SOC status
 * @return float minimum series element open circuit voltage approximation
 */
float app_soc_getMinOcvFromSoc(void);

/**
 * @brief Reset SOC value based on current minimum cell voltage
 */
void app_soc_resetSocFromVoltage(void);

/**
 * @brief Reset SOC value with custom input
 * @param soc_percent desired SOC in percent
 */
void app_soc_resetSocCustomValue(float soc_percent);

/**
 * @brief Write updated SOC value to non-voltile storage
 */
void app_soc_writeValue(void);
