#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "App_SharedConstants.h"
#include "App_CanTx.h"
#include "App_TractiveSystem.h"

// Min and Max cell temperatures depending on state
#define MAX_CELL_DISCHARGE_TEMP_DEGC (60.0f)
#define MAX_CELL_CHARGE_TEMP_DEGC (45.0f)
#define MIN_CELL_DISCHARGE_TEMP_DEGC (-20.0f)
#define MIN_CELL_CHARGE_TEMP_DEGC (0.0f)
#define MAX_CELL_VOLTAGE (4.2f)
#define MIN_CELL_VOLTAGE (3.0f)

struct Accumulator;

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
    bool (*read_cell_temperatures)(void),
    float (*get_min_cell_temp)(uint8_t *, uint8_t *),
    float (*get_max_cell_temp)(uint8_t *, uint8_t *),
    float (*get_avg_cell_temp)(void),
    bool (*enable_discharge)(void),
    bool (*disable_discharge)(void));

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
 * Get min voltage for the accumulator
 * @param accumulator The accumulator to get the min voltage for
 * @param segment The segment containing the min cell voltage
 * @param cell The cell location for the min voltage
 * @return The min voltage in V
 */
float App_Accumulator_GetMinVoltage(const struct Accumulator *const accumulator, uint8_t *segment, uint8_t *cell);

/**
 * Get max voltage for the accumulator
 * @param accumulator The accumulator to get the max voltage for
 * @param segment The segment containing the max cell voltage
 * @param cell The cell location for the max voltage
 * @return The max voltage in V
 */
float App_Accumulator_GetMaxVoltage(const struct Accumulator *const accumulator, uint8_t *segment, uint8_t *cell);

/**
 * Get the min cell temp
 * @param accumulator The accumulator to get the min cell temp for
 * @param segment The segment where the min cell temp is located
 * @param thermistor The thermistor measuring the min cell temp
 * @return The min cell temp in degC
 */
float App_Accumulator_GetMinCellTempDegC(
    const struct Accumulator *const accumulator,
    uint8_t *                       segment,
    uint8_t *                       thermistor);

/**
 * Get the max cell temp
 * @param accumulator The accumulator to get the max cell temp for
 * @param segment The segment where the max cell temp is located
 * @param thermistor The thermistor measuring the max cell temp
 * @return The max cell temp in degC
 */
float App_Accumulator_GetMaxCellTempDegC(
    const struct Accumulator *const accumulator,
    uint8_t *                       segment,
    uint8_t *                       thermistor);

/**
 * Get the average cell temp
 * @param accumulator The accumulator to get the average cell temp from
 * @return The average cell temp in degC
 */
float App_Accumulator_GetAvgCellTempDegC(const struct Accumulator *const accumulator);

/**
 * Get the pack voltage in V
 * @param accumulator The accumulator to get the pack voltage
 * @return The accumulator pack voltage in V
 */
float App_Accumulator_GetPackVoltage(struct Accumulator *accumulator);

// Rate functions to be called within the state machine
void App_Accumulator_InitRunOnEntry(const struct Accumulator *accumulator);
void App_Accumulator_RunOnTick100Hz(struct Accumulator *accumulator);

/**
 * Check the status of Accumulator faults, sends warning over CAN bus
 * @param can_tx CAN interface to send messages over
 * @param accumulator The accumulator to check faults
 * @param ts TractiveSystem used to check ts_current to check charge/discharge condition
 * @return True if faults present, false otherwise
 */
bool App_Accumulator_CheckFaults(struct Accumulator *const accumulator, struct TractiveSystem *const ts);
