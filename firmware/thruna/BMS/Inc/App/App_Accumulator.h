#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "App_SharedConstants.h"
#include "App_CanTx.h"
#include "App_TractiveSystem.h"

typedef enum
{
    ACCUMULATOR_SEGMENT_0 = 0U,
    ACCUMULATOR_SEGMENT_1,
    ACCUMULATOR_SEGMENT_2,
    ACCUMULATOR_SEGMENT_3,
    ACCUMULATOR_SEGMENT_4,
    ACCUMULATOR_NUM_SEGMENTS,
} AccumulatorSegment;

#define ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT (16U)
#define ACCUMULATOR_NUM_SERIES_CELLS_TOTAL (ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT * ACCUMULATOR_NUM_SEGMENTS)

// Min and Max cell temperatures depending on state
// C_RATE_TO_AMP conversion value is obtained by using the 5.9Ahrs
// that each cell is rated for and multiplying by three to get
// our 17.7amps = 1C for conversion
#define MAX_CELL_DISCHARGE_TEMP_DEGC (60.0f)
#define MAX_CELL_CHARGE_TEMP_DEGC (45.0f)
#define MIN_CELL_DISCHARGE_TEMP_DEGC (-20.0f)
#define MIN_CELL_CHARGE_TEMP_DEGC (0.0f)
#define MAX_CELL_VOLTAGE (4.2f)
#define MIN_CELL_VOLTAGE (3.0f)
#define C_RATE_TO_AMPS (17.7f)

// Cell Balancing Discharge Parameters
#define CELL_VOLTAGE_BALANCE_WINDOW_V (600e-6f) // 600uV

struct Accumulator;

struct Accumulator *App_Accumulator_Create(
    bool (*config_monitoring_chip)(void),
    bool (*write_cfg_registers)(bool[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT]),
    bool (*start_voltage_conv)(void),
    bool (*read_cell_voltages)(void),
    float (*get_cell_voltage)(uint8_t, uint8_t),
    bool (*start_cell_temp_conv)(void),
    bool (*read_cell_temperatures)(void),
    float (*get_min_cell_temp)(uint8_t *, uint8_t *),
    float (*get_max_cell_temp)(uint8_t *, uint8_t *),
    float (*get_avg_cell_temp)(void),
    bool (*enable_discharge)(void),
    bool (*disable_discharge)(void),
    bool (*check_imd_latched_fault)(void),
    bool (*check_bspd_latched_fault)(void),
    bool (*check_bms_latched_fault)(void),
    void (*select_mux_channel)(uint8_t),
    float (*read_thermistor_temp)(void));

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
 * Get a voltage for a specific cell
 * @param accumulator The accumulator to get the voltage from
 * @param segment The segment containing the cell voltage
 * @param cell The cell location for the voltage
 * @return The voltage at the location given in V
 */
float App_Accumulator_GetCellVoltage(
    const struct Accumulator *const accumulator,
    AccumulatorSegment              segment,
    uint8_t                         cell);

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
 * Get average cell voltage in a given segment
 * @param accumulator The accumulator to get the average voltage for
 * @param segment The segment to get the average voltage for
 * @return The average voltage in V
 */
float App_Accumulator_GetAverageCellVoltage(const struct Accumulator *const accumulator, uint8_t segment);

/**
 * Get voltage for an entire segment in an accumulator
 * @param accumulator The accumulator to get the segment voltage from
 * @param segment The segment to get the voltage of
 * @return The segment voltage in V
 */
float App_Accumulator_GetSegmentVoltage(const struct Accumulator *const accumulator, uint8_t segment);

/**
 * Get average voltage of all segments
 * @param accumulator The accumulator to get the average voltage for
 * @return The average voltage in V
 */
float App_Accumulator_GetAverageSegmentVoltage(const struct Accumulator *const accumulator);

/**
 * Get voltage for an entire accumulator
 * @param accumulator The accumulator to get the voltage of
 * @return The accumulator voltage in V
 */
float App_Accumulator_GetAccumulatorVoltage(const struct Accumulator *const accumulator);

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

// Rate functions to be called within the state machine
void App_Accumulator_InitRunOnEntry(const struct Accumulator *accumulator);
void App_Accumulator_RunOnTick100Hz(struct Accumulator *accumulator);

/**
 * Check the status of Accumulator faults, sends warning over CAN bus
 * @param accumulator The accumulator to check faults
 * @param ts TractiveSystem used to check ts_current to check charge/discharge condition
 * @return True if faults present, false otherwise
 */
bool App_Accumulator_CheckFaults(struct Accumulator *const accumulator, struct TractiveSystem *const ts);

/**
 * Broadcast latched BMS faults over CAN
 * @param accumulator The accumulator to check faults
 */
void App_Accumulator_BroadcastLatchedFaults(struct Accumulator *const accumulator);

/**
 * Enable or disable cell balancing.
 * @param accumulator The accumulator to check faults
 * @param enabled Whether or not to enable balancing
 */
void App_Accumulator_EnableBalancing(struct Accumulator *const accumulator, bool enabled);

/**
 * Get if cells are currently balancing.
 * @param accumulator The accumulator to check faults
 * @return True if BMS is balancing cells, false otherwise
 */
bool App_Accumulator_BalancingEnabled(struct Accumulator *const accumulator);

/**
 * Read a thermistor value then update the MUX channel to give time to switch before next read
 * @param accumulator The accumulator to update auxiliary thermistor temps for
 */
void App_Accumulator_UpdateAuxThermistorTemps(struct Accumulator *const accumulator);

/**
 * Broadcast thermistor temperatures over CAN
 * @param accumulator The accumulator to broadcast thermistor temps for
 */
void App_Accumulator_BroadcastThermistorTemps(struct Accumulator *const accumulator);
