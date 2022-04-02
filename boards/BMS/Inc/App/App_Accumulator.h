#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    ACCUMULATOR_SEGMENT_0 = 0U,
    ACCUMULATOR_SEGMENT_1,
    NUM_OF_ACCUMULATOR_SEGMENTS,
} AccumulatorSegments_E;

struct Accumulator;
struct BmsCanTxInterface;

/**
 * Allocate and initialize an accumulator.
 * @param config_monitoring_chip A function that can be called to configure
 * cell monitors
 * @param start_voltage_conv A function that can be called to trigger voltage
 * conversions
 * @param read_cell_voltages A function that can be called to read cell voltages
 * @param get_min_cell_loc A function to get the minimum cell voltage (V)
 * @param get_max_cell_loc A function to get the maximum cell voltage (V)
 * @param get_segment_voltage A function to get a given segment voltage (V)
 * @param get_pack_voltage A function to get the pack voltage (V)
 * @param get_avg_cell_voltage A function to get the average cell voltage (V)
 */
struct Accumulator *App_Accumulator_Create(
    bool (*config_monitoring_chip)(void),
    bool (*start_voltage_conv)(void),
    bool (*read_cell_voltages)(void),
    void (*get_min_cell_loc)(uint8_t *, uint8_t *),
    void (*get_max_cell_loc)(uint8_t *, uint8_t *),
    float (*get_min_cell_voltage)(void),
    float (*get_max_cell_voltage)(void),
    float (*get_segment_voltage)(AccumulatorSegments_E),
    float (*get_pack_voltage)(void),
    float (*get_avg_cell_voltage)(void));

/**
 * Deallocate the memory used by the given accumulator.
 * @param accumulator The accumulator to deallocate.
 */
void App_Accumulator_Destroy(struct Accumulator *accumulator);

/**
 * Get the pack voltage in V
 * @param accumulator The accumulator to get the pack voltage
 * @return The accumulator pack voltage in V
 */
float App_Accumulator_GetPackVoltage(struct Accumulator *accumulator);

bool App_Accumulator_IsMaxVoltageInRange(struct Accumulator *accumulator);

bool App_Accumulator_IsMinVoltageInRange(struct Accumulator *accumulator);

// Rate functions to be called within the state machine
void App_Accumulator_InitRunOnEntry(const struct Accumulator *accumulator);
void App_Accumulator_RunOnTick100Hz(
    struct Accumulator *      accumulator,
    struct BmsCanTxInterface *can_tx);
