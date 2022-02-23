#pragma once

#include <stdbool.h>
#include "App_Accumulator.h"
#include "App_SharedExitCode.h"

struct CellMonitors;

enum ITMPInRangeCheck
{
    // An exit code for when the ITMP (internal die temperature) is above the
    // disable charger and disable cell balancing thresholds. Both cell
    // balancing and charging are stopped when ITMP exceeds 120°C
    ITMP_OVERFLOW,

    // An exit code for when the ITMP is above the disable cell balancing
    // threshold. Cell balancing is stopped when ITMP exceeds 115°C
    ITMP_CELL_BALANCING_OVERFLOW,

    // An exit code for when the ITMP is below the re-enable charger threshold.
    // Charging is re-enabled when ITMP decreases below 115°C
    ITMP_CHARGER_IN_RANGE,

    // An exit code for when the ITMP is below the re-enable charger
    // and the re-enable cell balancing thresholds. Both charging and cell
    // balancing are re-enabled when ITMP decreases below 110°C
    ITMP_IN_RANGE
};

struct CellMonitors *App_CellMonitors_Create(
    bool (*read_die_temps)(void),
    float (*get_die_temp)(AccumulatorSegments_E),
    float (*get_max_die_temp)(void));

/**
 * Deallocate the given group of cell monitors
 * @param cell_monitors The group of cell monitors to deallocate
 */
void App_CellMonitors_Destroy(struct CellMonitors *cell_monitors);

/**
 * Read internal die temperatures for the cell monitors
 * @param cell_monitors The given group of cell monitors to read internal die
 * temperatures for
 * @return EXIT_CODE_OK if the internal die temperatures (°C) can be read
 * successfully. Else, EXIT_CODE_ERROR
 */
bool App_CellMonitors_ReadDieTemps(const struct CellMonitors *cell_monitors);

/**
 * Get the cell monitor 0 die temp in-range check from the group of cell
 * monitors
 * @param cell_monitors The given group of cell monitors to get the cell monitor
 * 0 die temp in-range check from
 * @return The cell monitor 0 die temp in-range check
 */
struct InRangeCheck *App_CellMonitors_GetCellMonitor0DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the cell monitor 1 die temp in-range check from the group of cell
 * monitors
 * @param cell_monitors The given group of cell monitors to get the cell monitor
 * 1 die temp in-range check from
 * @return The cell monitor 1 die temp in-range check
 */
struct InRangeCheck *App_CellMonitors_GetCellMonitor1DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the cell monitor 2 die temp in-range check from the group of cell
 * monitors
 * @param cell_monitors The given group of cell monitors to get the cell monitor
 * 2 die temp in-range check from.
 * @return The cell monitor 2 die temp in-range check
 */
struct InRangeCheck *App_CellMonitors_GetCellMonitor2DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the cell monitor 3 die temp in-range check from the group of cell
 * monitors
 * @param cell_monitors The given group of cell monitors to get the cell monitor
 * 3 die temp in-range check from
 * @return The cell monitor 3 die temp in-range check
 */
struct InRangeCheck *App_CellMonitors_GetCellMonitor3DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the cell monitor 4 die temp in-range check from the group of cell
 * monitors
 * @param cell_monitors The given group of cell monitors to get the cell monitor
 * 4 die temp in-range check from
 * @return The cell monitor 4 die temp in-range check
 */
struct InRangeCheck *App_CellMonitors_GetCellMonitor4DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the cell monitor 5 die temp in-range check from the group of cell
 * monitors
 * @param cell_monitors The given group of cell monitors to get the cell monitor
 * 5 die temp in-range check from
 * @return The cell monitor 5 die temp in-range check
 */
struct InRangeCheck *App_CellMonitors_GetCellMonitor5DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the current maximum die temperature from the group of cell monitors.
 * @param cell_monitors The given group of cell monitors to get the maximum die
 * temperature from
 * @param max_die_temp The current maximum internal die temperature (°C).
 * @return An enum value indicating whether the internal die temperature exceeds
 * any thresholds
 */
enum ITMPInRangeCheck App_CellMonitors_GetMaxDieTempDegC(
    const struct CellMonitors *cell_monitors,
    float *                    max_die_temp);
