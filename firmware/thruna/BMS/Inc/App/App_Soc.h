#pragma once

#include "App_SharedExitCode.h"
#include "App_Accumulator.h"
#include "App_Timer.h"

/**
 * Function to retrieve soc closest to given open circuit voltage
 * @param voc open circuit voltage
 * @return soc related to open circuit voltage
 */
float App_Soc_GetSocFromVoc(float voltage);

/**
 * Function to retrieve open circuit voltage closest to given soc
 * @param soc_percent state of charge in percent
 * @return open circuit voltage closest to given state of charge
 */
float App_Soc_GetVocFromSoc(float soc_percent);

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
 * Given three state-of-charge (SoCs), check whether the absolute difference
 * between any two SoCs is less than or equal to the specified maximum absolute
 * difference. If any two SoCs pass the check, return the average value of those
 * two SoCs.
 * @param max_abs_difference The maximum allowable absolute difference between
 *                           any two given SoCs, must be between 0 and 100
 *                           inclusive
 * @param soc_1 The first SoC, must be between 0 and 100 inclusive
 * @param soc_2 The second SoC, must be between 0 and 100 inclusive
 * @param soc_3 The third SoC, must be between 0 and 100 inclusive
 * @param result This will be set to the average value of the two SoCs whose
 *               difference is less than or equal to max_difference. Multiple
 *               SoC pairs may have their difference less than or equal to
 *               max_difference. In this scenario, we prioritize as follows:
 *
 *               Highest Priority: soc_1 / soc_2
 *                                 soc_2 / soc_3
 *               Lowest Priority:  soc_1 / soc_3
 *
 *               If no two suitable SoCs can be found, this will be set to NaN.
 * @note An example of a tiebreaker:
 *
 *         max_difference = 5
 *         soc_1 = 0
 *         soc_2 = 50
 *         soc_3 = 100
 *
 *       In this scenario, the difference between soc_1 and soc_2 is the same as
 *       the difference between soc_2 and soc_3, but we prioritize soc_1 / soc_2
 *       over soc_2 / soc_3. Thus, result is set to  (soc_1 + soc_2) / 2 = 25.
 * @return EXIT_CODE_INVALID_ARGS if max_difference, soc_1, soc_2, or soc_3 is
 *                                not between 0 and 100 inclusive
 */
ExitCode App_Soc_Vote(float max_abs_difference, float soc_1, float soc_2, float soc_3, float *result);

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
float App_SOC_GetMinVocFromSoc(struct SocStats *soc_stats);

/**
 * Reset SOC value based on current minimum cell voltage
 * @param soc_stats object to write SOC stats to
 * @param accumulator accumulator cell information
 */
void App_SOC_ResetSocFromVoltage(struct SocStats *soc_stats, struct Accumulator *accumulator);

/**
 * Reset SOC value with custom input
 * @param soc_stats object to write SOC stats to
 * @param soc_percent desired SOC in percent
 */
void App_SOC_ResetSocCustomValue(struct SocStats *soc_stats, float soc_percent);
