#pragma once

#include <io_ltc6813.h>
#include <stdbool.h>
#include <stdint.h>

// Min and Max cell temperatures depending on state
// C_RATE_TO_AMP conversion value is obtained by using the 5.9Ahrs
// that each cell is rated for and multiplying by three to get
// our 17.7amps = 1C for conversion
#define MAX_CELL_DISCHARGE_TEMP_DEGC (60.0f)
#define MAX_CELL_CHARGE_TEMP_DEGC (45.0f)
#define MIN_CELL_DISCHARGE_TEMP_DEGC (-20.0f)
#define MIN_CELL_CHARGE_TEMP_DEGC (0.0f)
#define MAX_CELL_VOLTAGE_NOMINAL (4.2f)
#define MIN_CELL_VOLTAGE (3.0f)
#define C_RATE_TO_AMPS (17.7f)

// Allows balancing of cells even if slight over-charging occurs. Occured prior to Competition 2024, where a fully
// charged pack with max cell V of 4.19 after charging reported as 4.21 after settling. Cause currently unknown, but
// this allows for these over-charged cells to be discharged back to safe limits
#define MAX_CELL_VOLTAGE_BALANCING (4.25f)

// Fault debounce durations.
#define UNDER_VOLTAGE_DEBOUNCE_DURATION_MS (500U)
#define OVER_VOLTAGE_DEBOUNCE_DURATION_MS (500U)
#define UNDER_TEMP_DEBOUNCE_DURATION_MS (1000U)
#define OVER_TEMP_DEBOUNCE_DURATION_MS (1000U)

// Cell Balancing Discharge Parameters
#define CELL_VOLTAGE_BALANCE_WINDOW_V (600e-6f) // 600uV

/**
 * Initialize the accumulator module.
 */
void app_accumulator_init(void);

/**
 * Write default configs to the LTC6813s.
 */
void app_accumulator_writeDefaultConfig(void);

/**
 * Cell Voltage and Temp Measurement function to be called at 100Hz.
 */
void app_accumulator_runCellMeasurements(void);

/**
 * Open Wire Check state machine
 */
bool app_accumulator_runOpenWireCheck(void);

/**
 * Broadcast state of the accumulator over CAN.
 */
void app_accumulator_broadcast(void);

/**
 * Check the status of Accumulator faults, sends warning over CAN bus
 * @return True if faults present, false otherwise
 */
bool app_accumulator_checkFaults(void);

/**
 * Get the total accumulator voltage, in V.
 * @return Accumulator voltage.
 */
float app_accumulator_getPackVoltage(void);

/**
 * Get the minimum cell voltage, in V.
 * @param segment The segment with the min voltage (NULL if you don't care).
 * @param cell The cell with the min voltage (NULL if you don't care).
 * @return Minimum cell voltage.
 */
float app_accumulator_getMinCellVoltage(uint8_t *segment, uint8_t *cell);

/**
 * Get the maximum cell voltage, in V.
 * @param segment The segment with the max voltage (NULL if you don't care).
 * @param cell The cell with the max voltage (NULL if you don't care).
 * @return Maximum cell voltage.
 */
float app_accumulator_getMinCellVoltage(uint8_t *segment, uint8_t *cell);

/**
 * Send command to segments to begin balancing based on cells marked for discharge
 * @attention This will trigger balancing
 */
void app_accumulator_balanceCells(void);

extern float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];
