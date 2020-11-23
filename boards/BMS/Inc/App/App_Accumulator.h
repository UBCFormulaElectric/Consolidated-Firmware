#pragma once

#include "App_InRangeCheck.h"
#include "App_SharedExitCode.h"

struct Accumulator;

/**
 * Allocate and initialize an accumulator
 * @param configure_cell_monitors A function to configure all cell monitors for
 * the given accumulator
 * @param read_cell_voltages A function to read cell voltages for the given
 * accumulator
 * @param read_cell_temperatures A function to read cell temperatures from for
 * the given accumulator
 *
 * @param get_min_cell_voltage A function that returns the absolute minimum cell
 * voltage of the accumulator
 * @param get_max_cell_voltage A function that returns the absolute maximum cell
 * voltage of the accumulator
 * @param get_average_cell_voltage A function that returns the average cell
 * voltage for the accumulator
 * @param get_pack_voltage A function that returns the accumulator pack voltage.
 * @param get_segment_0_voltage A function that returns the voltage of the 0th
 * accumulator segment
 * @param get_segment_1_voltage A function that returns the voltage of the 1st
 * accumulator segment
 * @param get_segment_2_voltage A function that returns the voltage of the 2nd
 * accumulator segment
 * @param get_segment_3_voltage A function that returns the voltage of the 3rd
 * accumulator segment
 * @param get_segment_4_voltage A function that returns the voltage of the 4th
 * accumulator segment
 * @param get_segment_5_voltage A function that returns the voltage of the 5th
 * accumulator segment
 *
 * @param get_min_cell_temperature A function that returns the absolute minimum
 * cell temperature of the accumulator
 * @param get_max_cell_temperature A function that returns the absolute maximum
 * cell temperature of the accumulator
 * @param get_average_cell_temperature A function that returns the average cell
 * temperature of the accumulator
 *
 * @param min_cell_voltage The minimum cell voltage for the given accumulator
 * @param max_cell_voltage The maximum cell voltage for the given accumulator
 * @param min_segment_voltage The minimum segment voltage for the given
 * accumulator
 * @param max_segment_voltage The maximum segment voltage for the given
 * accumulator
 * @param min_pack_voltage The minimum pack voltage for the given accumulator
 * @param max_pack_voltage The maximum pack voltage for the given accumulator
 * @param default_min_cell_temperature_degc The minimum cell temperature for the
 * given accumulator
 * @param default_max_cell_temperature_degc The maximum cell temperature for the
 * given accumulator
 *
 * @return A pointer to the created accumulator, whose ownership is given to
 * the caller
 */
struct Accumulator *App_Accumulator_Create(
    ExitCode (*configure_cell_monitors)(void),
    ExitCode (*read_cell_voltages)(void),
    ExitCode (*read_cell_temperatures)(void),
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
    float (*get_min_cell_temperature)(void),
    float (*get_max_cell_temperature)(void),
    float (*get_average_cell_temperature)(void),
    float min_cell_voltage,
    float max_cell_voltage,
    float min_segment_voltage,
    float max_segment_voltage,
    float min_pack_voltage,
    float max_pack_voltage,
    float default_min_cell_temperature_degc,
    float default_max_cell_temperature_degc,
    float charge_state_max_cell_temperature_degc);

/**
 * Deallocate the memory used by the given accumulator
 * @param accumulator The accumulator to deallocate
 */
void App_Accumulator_Destroy(struct Accumulator *accumulator);

/**
 * Configure all cell monitors for the given accumulator
 * @param accumulator The accumulator to configure the cell monitors for
 */
ExitCode App_Accumulator_ConfigureCellMonitors(
    const struct Accumulator *accumulator);

/**
 * Read cell voltages for the given accumulator
 * @param accumulator The given accumulator to read cell voltages for
 * @return EXIT_CODE_OK if cell voltages were read successfully. Else,
 * EXIT_CODE_ERROR
 */
ExitCode
    App_Accumulator_ReadCellVoltages(const struct Accumulator *accumulator);

/**
 * Read cell temperatures for the given accumulator
 * @param accumulator The given accumulator to read cell temperatures for
 * @return EXIT_CODE_OK if the cell temperatures were read successfully. Else if
 * the cell temperatures are out of range, return EXIT_CODE_OUT_OF_RANGE. Else,
 * EXIT_CODE_ERROR
 */
ExitCode
    App_Accumulator_ReadCellTemperatures(const struct Accumulator *accumulator);

/**
 * Get the accumulator's minimum cell voltage in-range check
 * @param accumulator The given accumulator to get the minimum cell voltage
 * in-range check
 * @return The minimum cell voltage in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetMinCellVoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the accumulator's maximum cell voltage in-range check
 * @param accumulator The given accumulator to get the maximum cell voltage
 * in-range check
 * @return The maximum cell voltage in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetMaxCellVoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the accumulator's average cell voltage in-range check
 * @param accumulator The given accumulator to get the average cell voltage
 * in-range check
 * @return The average cell voltage in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetAverageCellVoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the accumulator's pack voltage in-range check
 * @param accumulator The given accumulator to get the pack voltage in-range
 * check
 * @return The pack voltage in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetPackVoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the accumulator's 0th segment voltage in-range check
 * @param accumulator The given accumulator to get the 0th segment voltage
 * in-range check
 * @return The 0th segment voltage in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetSegment0VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the accumulator's 1st segment voltage in-range check
 * @param accumulator The given accumulator to get the 1st segment voltage
 * in-range check
 * @return The 1st segment voltage in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetSegment1VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the accumulator's 2nd segment voltage in-range check
 * @param accumulator The given accumulator to get the 2nd segment voltage
 * in-range check
 * @return The 2nd segment voltage in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetSegment2VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the accumulator's 3rd segment voltage in-range check
 * @param accumulator The given accumulator to get the 3rd segment voltage
 * in-range check
 * @return The 3rd segment voltage in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetSegment3VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the accumulator's 4th segment voltage in-range check
 * @param accumulator The given accumulator to get the 4th segment voltage
 * in-range check
 * @return The 4th segment voltage in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetSegment4VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the accumulator's 5th segment voltage in-range check
 * @param accumulator The given accumulator to get the 5th segment voltage
 * in-range check
 * @return The 5th segment voltage in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetSegment5VoltageInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the min cell temperature in-range check
 * @param accumulator The given accumulator to get the min cell temperature
 * in-range check
 * @return The min cell temperature in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetMinCellTemperatureInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the max cell temperature in-range check
 * @param accumulator The given accumulator to get the max cell temperature
 * in-range check
 * @return The max cell temperature in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetMaxCellTemperatureInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Get the average cell temperature in-range check
 * @param accumulator The given accumulator to get the average cell temperature
 * in-range check
 * @return The average cell temperature in-range check for the given accumulator
 */
struct InRangeCheck *App_Accumulator_GetAverageCellTemperatureInRangeCheck(
    const struct Accumulator *accumulator);

/**
 * Set the max value for all cell temperature in-range checks to the max value
 * for the charge state
 * @param accumulator The accumulator to get the cell temperature in-range
 * checks
 */
void App_Accumulator_SetChargeStateCellTemperatureInRangeCheckMaxValue(
    const struct Accumulator *accumulator);

/**
 * Set the max value for all cell temperatures in-range checks to its default
 * max value (the max value for all states except the charge state)
 * @param accumulator The accumulator to get the cell temperature in-range
 * checks
 */
void App_Accumulator_SetDefaultCellTemperatureInRangeCheckMaxValue(
    const struct Accumulator *accumulator);
