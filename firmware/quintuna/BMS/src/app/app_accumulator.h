#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Initialize the accumulator module.
 */
void app_accumulator_init(void);

void app_accumulator_calculateVoltageStats(void);

void app_accumulator_calculateTemperatureStats(void);

void app_accumulator_calculateCellsToBalance(void);

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
 * Send command to segments to begin balancing based on cells marked for discharge
 * @attention This will trigger balancing
 */
void app_accumulator_balanceCells(void);
