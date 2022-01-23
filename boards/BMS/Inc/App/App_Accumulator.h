#pragma once

#include <stdbool.h>
#include "App_InRangeCheck.h"
#include "App_SharedExitCode.h"
#include "configs/App_SharedConstants.h"

struct Accumulator;

/**
 * Allocate and initialize an accumulator.
 * @param configure_cell_monitors
 * @param start_voltage_conv
 * @param read_cell_voltages
 * @param get_min_cell_voltage
 * @param get_max_cell_voltage
 * @param get_segment_voltage
 * @param get_pack_voltage
 * @param get_average_voltage
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
 * Configure all cell monitors for the given accumulator.
 * @param accumulator The accumulator to configure the cell monitors for.
 */
bool App_Accumulator_ConfigureCellMonitors(
    const struct Accumulator *accumulator);

bool App_Accumulator_StartAdcConversions(const struct Accumulator *accumulator);

/**
 * Read cell voltages for the given accumulator.
 * @param accumulator The given accumulator to read cell voltages for.
 * @return EXIT_CODE_OK if cell voltages were read successfully from all cell
 * monitors. Else, return EXIT_CODE_ERROR.
 */
bool App_Accumulator_ReadCellVoltages(const struct Accumulator *accumulator);

void App_Accumulator_PeriodicCanTx100Hz(
    const struct Accumulator *const accumulator,
    struct BmsCanTxInterface *      can_tx);
