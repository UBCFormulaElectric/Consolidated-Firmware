#pragma once

#include "App_SharedExitCode.h"

/**
 * Given three state-of-charge (SoCs), check whether the absolute difference between any two SoCs is less than or equal
 * to the specified maximum absolute difference. If any two SoCs pass the check, return the average value of those two
 * SoCs.
 * @param max_abs_difference The maximum allowable absolute difference between any two given SoCs, must be between 0 and
 * 100 inclusive
 * @param soc_1 The first SoC, must be between 0 and 100 inclusive
 * @param soc_2 The second SoC, must be between 0 and 100 inclusive
 * @param soc_3 The third SoC, must be between 0 and 100 inclusive
 * @param result This will be set to the average value of the two SoCs whose difference is less than or equal to
 * max_difference. Multiple SoC pairs may have their difference less than or equal to max_difference. In this scenario,
 * we prioritize as follows:
 * Highest Priority: soc_1 / soc_2
 *                   soc_2 / soc_3
 * Lowest Priority:  soc_1 / soc_3
 *
 * If no two suitable SoCs can be found, this will be set to NaN.
 * @note An example of a tiebreaker:
 *
 *         max_difference = 5
 *         soc_1 = 0
 *         soc_2 = 50
 *         soc_3 = 100
 *
 * In this scenario, the difference between soc_1 and soc_2 is the same as the difference between soc_2 and soc_3,
 * but we prioritize soc_1 / soc_2 over soc_2 / soc_3. Thus, result is set to  (soc_1 + soc_2) / 2 = 25.
 * @return EXIT_CODE_INVALID_ARGS if max_difference, soc_1, soc_2, or soc_3 is not between 0 and 100 inclusive
 */
ExitCode App_Soc_Vote(float max_abs_difference, float soc_1, float soc_2, float soc_3, float *result);

/**
 * Use the min cell voltage to acquire the state of charge in Ah.
 * Note: This function should be used to determine the state of charge after a charge cycle. Write the SoC to the EEPROM
 * after exiting charge state
 * @param min_cell_voltage The minimum cell voltage of the pack
 * @return The state of charge in percent represented within [0,100]
 */
void App_Soc_GetInitialSoc(float min_cell_voltage);

/**
 * Determine the state of charge by coulomb counting
 * @param output_current The current flowing out of the accumulator in A
 * @param time_interval_s The time interval between current measurements in seconds
 */
void App_Soc_CoulombCount(float *prev_output_current, float curr_output_current, float time_interval_s);

/**
 * Get the state of charge from the EEPROM.
 * Note: This function is used to get the last recorded state of charge. Ideally, this function would be called while
 * entering the drive state
 */
void App_Soc_GetSocFromEeprom(void);

/**
 * A function that is used to get the SoC
 * @return The state of charge in percent represented within [0,1]
 */
float App_Soc_GetSoc(void);

/**
 * Write the state of charge to the EEPROM
 */
void App_Soc_WriteSocToEeprom(void);
