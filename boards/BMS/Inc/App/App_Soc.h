#pragma once

#include "App_SharedExitCode.h"

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
 * Initialize the value of the cell energy for the given cell
 * @param segment The current segment where the cell is located
 * @param cell The current cell to initialize the energy for
 * @param cell_v The current cell voltage used to look up the initial cell energy
 */
void App_Soc_InitCellEnergy(uint8_t segment, uint8_t cell, float cell_v);

/**
 * Update the energy of a given cell
 * @param segment The current segment where the cell is located
 * @param cell The current cell
 * @param current The tractive system current in A
 * @param cell_voltage The cell voltage of the given cell
 * @param time_step_s Period for cell energy measurements
 */
void App_Soc_UpdateCellEnergy(uint8_t segment, uint8_t cell, float current, float cell_voltage, float time_step_s);

/**
 * Get the min cell energy in percent. The pack when unbalanced is limited by this cell energy
 * @param segment The segment where the cell with the min cell energy is located
 * @param cell The cell containing the min cell energy
 * @return The cell energy in percent relative to the max cell energy
 */
float App_Soc_GetMinCellEnergyInPercent(uint8_t *segment, uint8_t *cell);

float App_Soc_GetMaxCellEnergyInPercent(uint8_t *segment, uint8_t *cell);
