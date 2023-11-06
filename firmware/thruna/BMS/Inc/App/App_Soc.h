#pragma once

#include "App_SharedExitCode.h"
#include "App_Accumulator.h"
#include "App_Timer.h"

#define STATE_OF_HEALTH (1.0f)
#define SERIES_ELEMENT_FULL_CHARGE_C (5.9f * 3600.0f * 3.0f * STATE_OF_HEALTH)

/**
 * Function to retrieve soc closest to given open circuit voltage
 * @param voltage open circuit voltage
 * @return soc related to open circuit voltage
 */
float App_Soc_GetSocFromOcv(float voltage);

/**
 * Function to retrieve open circuit voltage closest to given soc
 * @param soc_percent state of charge in percent
 * @return open circuit voltage closest to given state of charge
 */
float App_Soc_GetOcvFromSoc(float soc_percent);

struct SocStats
{
    // Address in EEPROM where SOC is being stored (address changes for wear levelling reasons, address always stored in
    // address 0 of EEPROM)
    uint16_t soc_address;

    // charge in cell in coulombs
    double charge_c;

    // Charge loss at time t-1
    float prev_current_A;

    // Indicates if SOC from EEPROM was corrupt at startup
    bool is_corrupt;

    TimerChannel soc_timer;
};

/**
 * Create the SocStats object
 * @return struct SocStats pointer to object
 */
struct SocStats *App_SocStats_Create(float initial_charge_value, uint16_t soc_address, struct Accumulator *accumulator);

/**
 * Destroy the SocStats object
 */
void App_SocStats_Destroy(struct SocStats *soc_stats);

/**
 * Return the active address on the EEPROM where SOC values are being stored
 * @param soc_stats soc_stats object to retrieve address from
 * @return page being used to store SOC values
 */
uint16_t App_SocStats_GetSocAddress(struct SocStats *soc_stats);

/**
 * Return if the SOC value was corrupt on startup
 * @param soc_stats soc_stats object to retrieve corrupt status from
 * @return corrupt status
 */
bool App_SocStats_GetCorrupt(struct SocStats *soc_stats);

/**
 * Update the state of charge of all series elements using coulomb counting.
 * @param soc_stats The charge stats of the pack
 * @param current The current from the cell to be updated (- is out of the cell, + is into the cell)
 */
void App_SocStats_UpdateSocStats(struct SocStats *soc_stats, float current);

/**
 * return Coulombs of the SE with the lowest SOC
 * @param soc_stats The charge stats of the pack
 * @return cloulombs of lowest SOC SE
 */
float App_SocStats_GetMinSocCoulombs(struct SocStats *soc_stats);

/**
 * return percent SOC of the SE with the lowest SOC
 * @param soc_stats The charge stats of the pack
 * @return soc % of lowest SOC SE
 */
float App_SocStats_GetMinSocPercent(struct SocStats *soc_stats);

/**
 * Get the minimum series element open circuit voltage approximation given current pack SOC status
 * @param soc_stats current SOC of each series element in pack
 * @return float minimum series element open circuit voltage approximation
 */
float App_Soc_GetMinOcvFromSoc(struct SocStats *soc_stats);

/**
 * Reset SOC value based on current minimum cell voltage
 * @param soc_stats object to write SOC stats to
 * @param accumulator accumulator cell information
 */
void App_Soc_ResetSocFromVoltage(struct SocStats *soc_stats, struct Accumulator *accumulator);

/**
 * Reset SOC value with custom input
 * @param soc_stats object to write SOC stats to
 * @param soc_percent desired SOC in percent
 */
void App_Soc_ResetSocCustomValue(struct SocStats *soc_stats, float soc_percent);
