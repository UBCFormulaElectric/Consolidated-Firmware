#pragma once

#include "App_SharedBinaryStatus.h"

struct Airs;

/**
 * Allocate and initialize a pair of AIRs
 * @param is_air_positive_closed A function that checks if AIR+ is closed
 * @param is_air_negative_closed A function that checks if AIR- is closed
 * @param close_air_positive A function that can be called to close AIR+
 * @param open_air_positive A function that can be called to open AIR+
 * @return The create pair of AIRs whose ownership is given to the caller
 */
struct Airs *App_Airs_Create(
    bool (*is_air_positive_closed)(void),
    bool (*is_air_negative_closed)(void),
    void (*close_air_positive)(void),
    void (*open_air_positive)(void));

/**
 * Deallocate the memory used by the given pair of AIRs
 * @param airs The given pair of AIRs to deallocate
 */
void App_Airs_Destroy(struct Airs *airs);

/**
 * Get AIR+ from the given pair of airs
 * @param airs The given pair of AIRs to get AIR+ from
 * @return The AIR+ from the given pair of AIRs
 */
struct BinaryStatus *App_Airs_GetAirPositive(const struct Airs *airs);

/**
 * Get AIR- from the given pair of airs
 * @param airs The given pair of AIRs to get AIR- from
 * @return The AIR- from the given pair of AIRs
 */
struct BinaryStatus *App_Airs_GetAirNegative(const struct Airs *airs);

/**
 * Close AIR+ from the given pair of AIRs
 * @param airs The given pair of AIRs to close the AIR+ for
 */
void App_Airs_CloseAirPositive(const struct Airs *airs);

/**
 * Open AIR- from the given pair of AIRs
 * @param airs The given pair of AIRs to open the AIR- for
 */
void App_Airs_OpenAirPositive(const struct Airs *airs);
