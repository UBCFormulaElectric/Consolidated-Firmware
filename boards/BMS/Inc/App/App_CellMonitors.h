#pragma once

#include "App_SharedExitCode.h"

struct CellMonitors;

enum ITMPInRangeCheck
{
    // An exit code for when ITMP is above the threshold to disable the charger
    // as well as the threshold to disable cell balancing
    ITMP_OVERFLOW,

    // An exit code for when ITMP is above the threshold to disable cell
    // balancing and below the threshold to disable the charger
    ITMP_CELL_BALANCING_OVERFLOW,

    // An exit code for when ITMP is below the threshold to re-enable the
    // charger
    ITMP_CHARGER_IN_RANGE,

    // An exit code for when ITMP is below the threshold to re-enable the
    // charger as well as the threshold to re-enable cell balancing
    ITMP_IN_RANGE
};

/**
 * Allocate and initialize a group of cell monitors
 * @param read_die_temps A function that can be called to read internal
 * die temperatures (°C) from all cell monitors
 * @param get_monitor_0_die_temp A function that returns the internal die
 * temperature (°C) for the 0th cell monitor
 * @param get_monitor_1_die_temp A function that returns the internal die
 * temperature (°C) for the 1st cell monitor
 * @param get_monitor_2_die_temp A function that returns the internal die
 * temperature (°C) for the 2nd cell monitor
 * @param get_monitor_3_die_temp A function that returns the internal die
 * temperature (°C) for the 3rd cell monitor
 * @param get_monitor_4_die_temp A function that returns the internal die
 * temperature (°C) for the 4th cell monitor
 * @param get_monitor_5_die_temp A function that returns the internal die
 * temperature (°C) for the 5th cell monitor
 * @param get_max_die_temp A function that returns the current maximum internal
 * die temperature (°C) out of all the cell monitors
 * @param min_die_temp_degc The minimum die temperature (°C) threshold for the
 * given cell monitor
 * @param max_die_temp_degc The maximum die temperature (°C) threshold for the
 * given cell monitor
 * @param die_temp_re_enable_charger_degc The die temperature (°C) to re-enable
 * the charger
 * @param die_temp_re_enable_cell_balancing_degc The die temperature (°C) to
 * re-enable cell balancing
 * @param die_temp_disable_cell_balancing_degc The die temperature (°C) to
 * disable cell balancing
 * @param die_temp_disable_charger_degc The die temperature (°C) to disable the
 * charger
 * @return A pointer to the created cell monitor whose ownership is given to the
 * caller
 */
struct CellMonitors *App_CellMonitors_Create(
    ExitCode (*read_die_temps)(void),
    float (*get_monitor_0_die_temp)(void),
    float (*get_monitor_1_die_temp)(void),
    float (*get_monitor_2_die_temp)(void),
    float (*get_monitor_3_die_temp)(void),
    float (*get_monitor_4_die_temp)(void),
    float (*get_monitor_5_die_temp)(void),
    float (*get_max_die_temp)(void),
    float min_die_temp_degc,
    float max_die_temp_degc,
    float die_temp_re_enable_charger_degc,
    float die_temp_re_enable_cell_balancing_degc,
    float die_temp_disable_cell_balancing_degc,
    float die_temp_disable_charger_degc);

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
ExitCode
    App_CellMonitors_ReadDieTemps(const struct CellMonitors *cell_monitors);

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
