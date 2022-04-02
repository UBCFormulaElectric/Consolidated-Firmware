#include <stdlib.h>
#include <assert.h>
#include "App_Accumulator.h"
#include "App_SharedMacros.h"

#define MIN_CELL_VOLTAGE (3.0f)
#define MAX_CELL_VOLTAGE (4.2f)

enum AccumulatorMonitorState
{
    GET_CELL_VOLTAGE_STATE = 0U,
    GET_CELL_TEMP_STATE,
    GET_DIE_TEMP_STATE,
};

struct Accumulator
{
    // Configure the cell monitoring chip
    bool (*config_monitoring_chip)(void);

    // Cell voltage monitoring functions
    bool (*start_cell_voltage_conv)(void);
    bool (*read_cell_voltages)(void);
    void (*get_min_cell_loc)(uint8_t *, uint8_t *);
    void (*get_max_cell_loc)(uint8_t *, uint8_t *);
    float (*get_min_cell_voltage)(void);
    float (*get_max_cell_voltage)(void);
    float (*get_segment_voltage)(AccumulatorSegments_E);
    float (*get_pack_voltage)(void);
    float (*get_avg_cell_voltage)(void);
};

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
    float (*get_avg_cell_voltage)(void))
{
    struct Accumulator *accumulator = malloc(sizeof(struct Accumulator));
    assert(accumulator != NULL);

    accumulator->config_monitoring_chip = config_monitoring_chip;

    // Cell voltage monitoring functions
    accumulator->read_cell_voltages      = read_cell_voltages;
    accumulator->start_cell_voltage_conv = start_voltage_conv;
    accumulator->get_min_cell_voltage    = get_min_cell_voltage;
    accumulator->get_max_cell_voltage    = get_max_cell_voltage;
    accumulator->get_min_cell_loc        = get_min_cell_loc;
    accumulator->get_max_cell_loc        = get_max_cell_loc;
    accumulator->get_segment_voltage     = get_segment_voltage;
    accumulator->get_pack_voltage        = get_pack_voltage;
    accumulator->get_avg_cell_voltage    = get_avg_cell_voltage;

    return accumulator;
}

void App_Accumulator_Destroy(struct Accumulator *accumulator)
{
    free(accumulator);
}

float App_Accumulator_GetPackVoltage(struct Accumulator *accumulator)
{
    return accumulator->get_pack_voltage();
}

bool App_Accumulator_IsMaxVoltageInRange(struct Accumulator *accumulator)
{
    const float curr_max_cell_voltage = accumulator->get_max_cell_voltage();
    return IS_IN_RANGE(
        MIN_CELL_VOLTAGE, MAX_CELL_VOLTAGE, curr_max_cell_voltage);
}

bool App_Accumulator_IsMinVoltageInRange(struct Accumulator *accumulator)
{
    const float curr_min_cell_voltage = accumulator->get_min_cell_voltage();
    return IS_IN_RANGE(
        MIN_CELL_VOLTAGE, MAX_CELL_VOLTAGE, curr_min_cell_voltage);
}

void App_Accumulator_InitRunOnEntry(const struct Accumulator *const accumulator)
{
    // Configure the cell monitoring chips. Disable discharge at startup
    accumulator->config_monitoring_chip();
}

#include "Io_LTC6813/Io_LTC6813Shared.h"
void App_Accumulator_RunOnTick100Hz(
    struct Accumulator *      accumulator,
    struct BmsCanTxInterface *can_tx)
{
    static enum AccumulatorMonitorState state = GET_CELL_VOLTAGE_STATE;

    switch (state)
    {
        case GET_CELL_VOLTAGE_STATE:

            // Read cell voltages
            accumulator->read_cell_voltages();

            // TODO: Add broadcast BMS CAN messages when fixed pt. CAN PR has
            // been merged i Io_LTC6813Shared_WriteConfigurationRegisters();n
            Io_LTC6813Shared_WriteConfigurationRegisters();

            // Start cell temperature voltage conversions for the next cycle
            accumulator->start_cell_voltage_conv();

            static uint32_t counter = 0U;
            if ((counter % 1000) >= 50U)
            {
                Io_LTC6813Shared_DisableDischarge();
            }
            else
            {
                Io_LTC6813Shared_EnableDischarge();
            }

            counter++;

            break;

        default:
            break;
    }
}
