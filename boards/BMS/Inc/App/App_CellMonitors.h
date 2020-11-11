#pragma once

#include "App_SharedExitCode.h"

struct CellMonitors;

/**
 * Allocate and initialize a group of cell monitors.
 * @param read_die_temperatures A function that can be called to read internal
 * die temperatures from all cell monitors.
 * @param get_monitor_0_die_temp A function that returns the die temperature for
 * the 0th cell monitor.
 * @param get_monitor_1_die_temp A function that returns the die temperature for
 * the 1st cell monitor.
 * @param get_monitor_2_die_temp A function that returns the die temperature for
 * the 2nd cell monitor.
 * @param get_monitor_3_die_temp A function that returns the die temperature for
 * the 3rd cell monitor.
 * @param get_monitor_4_die_temp A function that returns the die temperature for
 * the 4th cell monitor.
 * @param get_monitor_5_die_temp A function that returns the die temperature for
 * the 5th cell monitor.
 * @param get_min_die_temp A function that returns the minimum die temperature.
 * @param get_max_die_temp A function that returns the maximum die temperature.
 * @param min_die_temp_threshold The minimum die temperature threshold for the
 * given cell monitor.
 * @param max_die_temp_threshold The maximum die temperature threshold for the
 * given cell monitor.
 * @return A pointer to the created cell monitor whose ownership is given to the
 * caller.
 */
struct CellMonitors *App_CellMonitors_Create(
    ExitCode (*read_die_temperatures)(void),
    float (*get_monitor_0_die_temp)(void),
    float (*get_monitor_1_die_temp)(void),
    float (*get_monitor_2_die_temp)(void),
    float (*get_monitor_3_die_temp)(void),
    float (*get_monitor_4_die_temp)(void),
    float (*get_monitor_5_die_temp)(void),
    float (*get_min_die_temp)(void),
    float (*get_max_die_temp)(void),
    float min_die_temp_threshold,
    float max_die_temp_threshold);

/**
 * Deallocate the given cell monitor.
 * @param cell_monitors
 */
void App_CellMonitors_Destroy(struct CellMonitors *cell_monitors);

/**
 * Read internal die temperatures for the cell monitors.
 * @param cell_monitors The group of cell monitors to read internal die
 * temperatures for.
 * @return EXIT_CODE_OK if the internal die temperatures can be read
 * successfully. Else, EXIT_CODE_ERROR.
 */
ExitCode App_CellMonitors_ReadDieTemperatures(
    const struct CellMonitors *cell_monitors);

/**
 * Get the die temp 0 in-range check from the group of cell monitors.
 * @param cell_monitors The given group of cell monitors to get the die temp 0
 * in-range check from.
 * @return The die temp 0 in-range check.
 */
struct InRangeCheck *App_CellMonitors_GetMonitor0DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the die temp 1 in-range check from the group of cell monitors.
 * @param cell_monitors The given group of cell monitors to get the die temp 1
 * in-range check from.
 * @return The die temp 1 in-range check.
 */
struct InRangeCheck *App_CellMonitors_GetMonitor1DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the die temp 2 in-range check from the group of cell monitors.
 * @param cell_monitors The given group of cell monitors to get the die temp 2
 * in-range check from.
 * @return The die temp 2 in-range check.
 */
struct InRangeCheck *App_CellMonitors_GetMonitor2DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the die temp 3 in-range check from the group of cell monitors.
 * @param cell_monitors The given group of cell monitors to get the die temp 3
 * in-range check from.
 * @return The die temp 3 in-range check.
 */
struct InRangeCheck *App_CellMonitors_GetMonitor3DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the die temp 4 in-range check from the group of cell monitors.
 * @param cell_monitors The given group of cell monitors to get the die temp 4
 * in-range check from.
 * @return The die temp 4 in-range check.
 */
struct InRangeCheck *App_CellMonitors_GetMonitor4DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the die temp 5 in-range check from the group of cell monitors.
 * @param cell_monitors The given group of cell monitors to get the die temp 5
 * in-range check from.
 * @return The die temp 5 in-range check.
 */
struct InRangeCheck *App_CellMonitors_GetMonitorDieTemp5InRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the min die temperature in-range check from the group of cell monitors.
 * @param cell_monitors The given group of cell monitors to get the min die temp
 * in-range check from.
 * @return The min die temperature in-range check.
 */
struct InRangeCheck *App_CellMonitors_GetMinDieTempInRangeCheck(
    const struct CellMonitors *cell_monitors);

/**
 * Get the max die temperature in-range check from the group of cell monitors.
 * @param cell_monitors The given group of cell monitors to get the max die temp
 * in-range check from.
 * @return The max die temperature in-range check.
 */
struct InRangeCheck *App_CellMonitors_GetMaxDieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors);
