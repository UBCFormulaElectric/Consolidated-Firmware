#pragma once

#include <stdbool.h>
#include "App_InRangeCheck.h"
#include "App_SharedErrorTable.h"

enum AccumulatorSegments
{
    ACCUMULATOR_SEGMENT_0 = 0U,
    NUM_OF_ACCUMULATOR_SEGMENTS,
};

struct Accumulator;

/**
 * Allocate and initialize an accumulator.
 * @param configure_cell_monitors A function that can be called to configure
 * cell monitors
 * @param start_voltage_conv A function that can be called to trigger voltage
 * conversions
 * @param read_cell_voltages A function that can be called to read cell voltages
 * @param get_min_cell_voltage A function to get the minimum cell voltage (V)
 * @param get_max_cell_voltage A function to get the maximum cell voltage (V)
 * @param get_segment_voltage A function to get a given segment voltage (V)
 * @param get_pack_voltage A function to get the pack voltage (V)
 * @param get_average_voltage A function to get the average voltage (V)
 */
struct Accumulator *App_Accumulator_Create(
    bool (*configure_cell_monitors)(void),
    bool (*start_voltage_conv)(void),
    bool (*read_cell_voltages)(void),
    void (*get_min_cell_location)(uint8_t *, uint8_t *),
    void (*get_max_cell_location)(uint8_t *, uint8_t *),
    float (*get_min_cell_voltage)(void),
    float (*get_max_cell_voltage)(void),
    float (*get_segment_voltage)(enum AccumulatorSegments),
    float (*get_pack_voltage)(void),
    float (*get_average_voltage)(void));

/**
 * Deallocate the memory used by the given accumulator.
 * @param accumulator The accumulator to deallocate.
 */
void App_Accumulator_Destroy(struct Accumulator *accumulator);

/**
 * Update cell voltages measured by the accumulator monitor.
 * @param accumulator The accumulator to start voltage conversions for.
 * @return True if voltages have been successfully converted. Else, false.
 */
bool App_Accumulator_StartCellVoltageConversion(
    const struct Accumulator *const accumulator);

/**
 * Read cell voltages for the given accumulator.
 * @param accumulator The given accumulator to read cell voltages for.
 * @return True, if cell voltages were read successfully from all cell
 * monitors. Else, false.
 */
bool App_Accumulator_ReadCellVoltages(const struct Accumulator *accumulator);

/**
 * Accumulator rate functions
 */
void App_Accumulator_InitRunOnEntry(const struct Accumulator *accumulator);

void App_Accumulator_AllStates100Hz(
    const struct Accumulator *const accumulator,
    struct BmsCanTxInterface *      can_tx,
    struct ErrorTable *             error_table);
