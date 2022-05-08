#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "App_SharedConstants.h"

struct Accumulator;

/**
 * Allocate and initialize an accumulator.
 * @param config_monitoring_chip A function that can be called to configure
 * cell monitors
 * @param write_cfg_registers A function that can be called to write to the
 * configuration registers. This is done to control cell discharging
 * @param start_voltage_conv A function that can be called to trigger voltage
 * conversions
 * @param read_cell_voltages A function that can be called to read cell voltages
 * @param get_min_cell_voltage A function to get the minimum cell voltage (V),
 * as well as the cell and the segment of the minimum cell voltage
 * @param get_max_cell_voltage A function to get the maximum cell voltage (V),
 * as well as the cell and the segment of the maximum cell voltage
 * @param get_segment_voltage A function to get a given segment voltage (V)
 * @param get_pack_voltage A function to get the pack voltage (V)
 * @param get_avg_cell_voltage A function to get the average cell voltage (V)
 * @param start_cell_temp_conv A function that is called to start cell temp
 * conversions
 * @param read_cell_temperatures A function called to read cell temperatures
 */
struct Accumulator *App_Accumulator_Create(
    bool (*config_monitoring_chip)(void),
    bool (*write_cfg_registers)(void),
    bool (*start_voltage_conv)(void),
    bool (*read_cell_voltages)(void),
    float (*get_min_cell_voltage)(uint8_t *, uint8_t *),
    float (*get_max_cell_voltage)(uint8_t *, uint8_t *),
    float (*get_segment_voltage)(AccumulatorSegments_E),
    float (*get_pack_voltage)(void),
    float (*get_avg_cell_voltage)(void),
    bool (*start_cell_temp_conv)(void),
    bool (*read_cell_temperatures)(void));

/**
 * Deallocate the memory used by the given accumulator.
 * @param accumulator The accumulator to deallocate.
 */
void App_Accumulator_Destroy(struct Accumulator *accumulator);

/**
 * Check if a communication error has occurred with the cell monitoring chip
 * @param accumulator The accumulator to monitor cell voltages and temperatures
 * @return True if a communication error has occured. Else, false
 */
bool App_Accumulator_HasCommunicationError(const struct Accumulator *accumulator);

/**
 * Get the maximum cell voltage in [V]
 * @param accumulator The accumulator to monitor cell voltages and temperatures
 * @return The maximum cell voltage in [V]
 */
float App_Accumulator_GetMaxCellVoltage(const struct Accumulator *const accumulator);

// Rate functions to be called within the state machine
void App_Accumulator_InitRunOnEntry(const struct Accumulator *accumulator);
void App_Accumulator_RunOnTick100Hz(struct Accumulator *accumulator);
