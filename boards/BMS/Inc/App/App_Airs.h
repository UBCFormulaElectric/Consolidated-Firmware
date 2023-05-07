#pragma once

#include <stdbool.h>

struct Airs;

/**
 * Allocate and initialize a pair of AIRs (AIR+ and AIR-)
 * @param is_air_positive_closed A function to check if AIR+ is closed
 * @param is_air_negative_closed A function to check if AIR- is closed
 * @param close_air_positive A function that can be called to close AIR+
 * @param open_air_positive A function that can be called to open AIR+
 * @return The created pair of AIRs whose ownership is given to the caller
 */
struct Airs *App_Airs_Create(bool (*is_air_positive_closed)(void),
                             bool (*is_air_negative_closed)(void),
                             void (*close_air_positive)(void),
                             void (*open_air_positive)(void));

/**
 * Deallocate the memory used by the given pair of AIRs
 * @param airs The given pair of AIRs to deallocate
 */
void App_Airs_Destroy(struct Airs *airs);

/**
 * Get AIR+ from the given pair of AIRs
 * @param airs The given pair of AIRs to get AIR+ from
 * @return The AIR+ from the given pair of AIRs
 */
bool App_Airs_IsAirPositiveClosed(const struct Airs *const airs);

/**
 * Get AIR- from the given pair of AIRs
 * @param airs The given pair of AIRs to get AIR- from
 * @return The AIR- from the given pair of AIRs
 */
bool App_Airs_IsAirNegativeClosed(const struct Airs *const airs);

/**
 * Close AIR+ from the given pair of AIRs
 * @param airs The given pair of AIRs to close AIR+ for
 */
void App_Airs_CloseAirPositive(const struct Airs *airs);

/**
 * Open AIR+ from the given pair of AIRs
 * @param airs The given pair of AIRs to open AIR+ for
 */
void App_Airs_OpenAirPositive(const struct Airs *airs);
