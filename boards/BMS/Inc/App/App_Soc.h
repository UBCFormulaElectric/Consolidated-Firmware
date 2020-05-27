#pragma once

#include "App_SharedExitCode.h"

/**
 * Given three state-of-charge (SoCs), check whether the difference between any
 * two SoCs is less than or equal to the specified threshold. If any two SoCs
 * pass the check, return the average value of those two SoCs.
 * @param max_difference The maximum tolerable difference between any two given
 *                       state-of-charge values
 * @param soc_1 The first SoC
 * @param soc_2 The second SoC
 * @param soc_3 The third SoC
 * @param result The average value of two SoCs whose difference is less than or
 *               equal to max_value, else NaN. In the event of a tiebreak (i.e.
 *               All three SoCs pass the check), th first two SoCs are used to
 *               calculate the average value.
 * @return EXIT_CODE_INVALID_ARGS if the max difference or any of the SoCs is
 *                                less than 0 or greater than 100
 */
ExitCode App_Soc_Vote(
    float  max_difference,
    float  soc_1,
    float  soc_2,
    float  soc_3,
    float *result);
