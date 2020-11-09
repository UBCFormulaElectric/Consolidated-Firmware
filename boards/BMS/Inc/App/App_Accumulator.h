#pragma once

#include "App_InRangeCheck.h"
#include "App_SharedExitCode.h"

struct Accumulator;

/**
 * Allocate and initialize a cell monitor.
 * @param configure_daisy_chain A function that is called to configure the
 * daisy chain of cell monitoring chips for the given cell monitor.
 * @param read_cell_voltages A function that is called to read cell voltages
 * from the daisy chain of cell monitoring chips for the given cell monitor.
 *
 * @param get_min_cell_voltage A function that returns the absolute minimum cell
 * voltage.
 * @param get_max_cell_voltage A function that returns the absolute maximum cell
 * voltage.
 * @param get_average_cell_voltage A function that returns the average cell
 * voltage.
 * @param get_pack_voltage A function that returns the pack voltage.
 * @param get_segment_0_voltage A function that returns the voltage of the 0th
 * segment.
 * @param get_segment_1_voltage A function that returns the voltage of the 1st
 * segment.
 * @param get_segment_2_voltage A function that returns the voltage of the 2nd
 * segment.
 * @param get_segment_3_voltage A function that returns the voltage of the 3rd
 * segment.
 * @param get_segment_4_voltage A function that returns the voltage of the 4th
 * segment.
 * @param get_segment_5_voltage A function that returns the voltage of the 5th
 * segment.
 *
 * @param min_cell_voltage The minimum cell voltage for the given cell monitor.
 * @param max_cell_voltage The maximum cell voltage for the given cell monitor.
 * @param min_segment_voltage The minimum segment voltage for the given cell
 * monitor.
 * @param max_segment_voltage The maximum segment voltage for the given cell
 * monitor.
 * @param min_pack_voltage The minimum pack voltage for the given cell monitor.
 * @param max_pack_voltage The maximum pack voltage for the given cell monitor.
 * @return A pointer to the created cell monitor, whose ownership is given to
 * the caller.
 */
struct Accumulator *App_CellMonitor_Create(
    ExitCode (*configure_daisy_chain)(void),
    ExitCode (*read_cell_voltages)(void),
    float (*get_min_cell_voltage)(void),
    float (*get_max_cell_voltage)(void),
    float (*get_average_cell_voltage)(void),
    float (*get_pack_voltage)(void),
    float (*get_segment_0_voltage)(void),
    float (*get_segment_1_voltage)(void),
    float (*get_segment_2_voltage)(void),
    float (*get_segment_3_voltage)(void),
    float (*get_segment_4_voltage)(void),
    float (*get_segment_5_voltage)(void),
    float min_cell_voltage,
    float max_cell_voltage,
    float min_segment_voltage,
    float max_segment_voltage,
    float min_pack_voltage,
    float max_pack_voltage);

/**
 * Deallocate the memory used by the given cell monitor.
 * @param accumulator The cell monitor to deallocate.
 */
void App_Accumulator_Destroy(struct Accumulator *accumulator);

/**
 * Configure the daisy chain of cell measuring chips.
 * @param accumulator The cell monitor to configure the daisy chain of cell
 * measuring chips for.
 */
ExitCode App_Accumulator_Configure(const struct Accumulator *accumulator);

/**
 * Read cell voltages from the daisy chain of cell measuring chips.
 * @param accumulator The given cell monitor to read cell voltages from the
 * daisy chain of cell measuring chips for.
 * @return EXIT_CODE_OK if cell voltages were read successfully from the daisy
 * chain of cell measuring chips. Else, return EXIT_CODE_ERROR.
 */
ExitCode
    App_Accumulator_ReadCellVoltages(const struct Accumulator *accumulator);

/**
 * Get the 0th segment voltage in-range check.
 * @param accumulator The given cell monitor to get the 0th segment voltage
 * in-range check for.
 * @return The 0th segment voltage in-range check for the given cell monitor.
 */
struct InRangeCheck *App_Accumulator_GetSegment0VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the 1st segment voltage in-range check.
 * @param accumulator The given cell monitor to get the 1st segment voltage
 * in-range check for.
 * @return The 1st segment voltage in-range check for the given cell monitor.
 */
struct InRangeCheck *App_Accumulator_GetSegment1VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the 2nd segment voltage in-range check.
 * @param accumulator The given cell monitor to get the 2nd segment voltage
 * in-range check for.
 * @return The 2nd segment voltage in-range check for the given cell monitor.
 */
struct InRangeCheck *App_Accumulator_GetSegment2VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the 3rd segment voltage in-range check.
 * @param accumulator The given cell monitor to get the 3rd segment voltage
 * in-range check for.
 * @return The 3rd segment voltage in-range check for the given cell monitor.
 */
struct InRangeCheck *App_Accumulator_GetSegment3VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the 4th segment voltage in-range check.
 * @param accumulator The given cell monitor to get the 4th segment voltage
 * in-range check for.
 * @return The 4th segment voltage in-range check for the given cell monitor.
 */
struct InRangeCheck *App_Accumulator_GetSegment4VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the 5th segment voltage in-range check.
 * @param accumulator The given cell monitor to get the 5th segmenet voltage
 * in-range check for.
 * @return The 5th segment voltage in-range check for the given cell monitor.
 */
struct InRangeCheck *App_Accumulator_GetSegment5VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the pack voltage in-range check.
 * @param accumulator The given cell monitor to get the pack voltage in-range
 * check for.
 * @return The pack voltage in-range check for the given cell monitor.
 */
struct InRangeCheck *App_Accumulator_GetPackVoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the minimum cell voltage in-range check.
 * @param accumulator The given cell monitor to get the minimum cell voltage
 * in-range check for.
 * @return The minimum cell voltage in-range check for the given cell monitor.
 */
struct InRangeCheck *App_Accumulator_GetMinCellVoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the maximum cell voltage in-range check.
 * @param accumulator The given cell monitor to get the maximum cell voltage
 * in-range check for.
 * @return The maximum cell voltage in-range check for the given cell monitor.
 */
struct InRangeCheck *App_Accumulator_GetMaxCellVoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the average cell voltage in-range check.
 * @param accumulator The given cell monitor to get the average cell voltage
 * in-range check for.
 * @return The average cell voltage in-range check for the given cell monitor.
 */
struct InRangeCheck *App_Accumulator_GetAverageCellVoltageInRangeCheck(
    const struct Accumulator *accumulator);
