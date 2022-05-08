#include <stdlib.h>
#include <assert.h>
#include "App_Accumulator.h"

#define MAX_NUM_COMM_TRIES (3U)

enum AccumulatorMonitorState
{
    GET_CELL_VOLTAGE_STATE = 0U,
    GET_CELL_TEMP_STATE,
};

struct Accumulator
{
    uint8_t num_comm_tries;

    // Configure the cell monitoring chip
    bool (*config_monitoring_chip)(void);
    bool (*write_cfg_registers)(void);

    // Cell voltage monitoring functions
    bool (*start_cell_voltage_conv)(void);
    bool (*read_cell_voltages)(void);
    float (*get_min_cell_voltage)(uint8_t *, uint8_t *);
    float (*get_max_cell_voltage)(uint8_t *, uint8_t *);
    float (*get_segment_voltage)(AccumulatorSegments_E);
    float (*get_pack_voltage)(void);
    float (*get_avg_cell_voltage)(void);

    // Cell temperature monitoring functions
    bool (*start_cell_temp_conv)(void);
    bool (*read_cell_temperatures)(void);
};

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
    bool (*read_cell_temperatures)(void))
{
    struct Accumulator *accumulator = malloc(sizeof(struct Accumulator));
    assert(accumulator != NULL);

    accumulator->config_monitoring_chip = config_monitoring_chip;
    accumulator->write_cfg_registers    = write_cfg_registers;

    // Cell voltage monitoring functions
    accumulator->num_comm_tries          = 0U;
    accumulator->read_cell_voltages      = read_cell_voltages;
    accumulator->start_cell_voltage_conv = start_voltage_conv;
    accumulator->get_min_cell_voltage    = get_min_cell_voltage;
    accumulator->get_max_cell_voltage    = get_max_cell_voltage;
    accumulator->get_segment_voltage     = get_segment_voltage;
    accumulator->get_pack_voltage        = get_pack_voltage;
    accumulator->get_avg_cell_voltage    = get_avg_cell_voltage;

    // Cell temperature monitoring functions
    accumulator->start_cell_temp_conv   = start_cell_temp_conv;
    accumulator->read_cell_temperatures = read_cell_temperatures;

    return accumulator;
}

void App_Accumulator_Destroy(struct Accumulator *accumulator)
{
    free(accumulator);
}

bool App_Accumulator_HasCommunicationError(const struct Accumulator *const accumulator)
{
    return accumulator->num_comm_tries >= MAX_NUM_COMM_TRIES;
}

float App_Accumulator_GetMaxCellVoltage(const struct Accumulator *const accumulator)
{
    return accumulator->get_max_cell_voltage(NULL, NULL);
}

void App_Accumulator_InitRunOnEntry(const struct Accumulator *const accumulator)
{
    // Configure the cell monitoring chips. Disable discharge at startup
    accumulator->config_monitoring_chip();
}

void App_Accumulator_RunOnTick100Hz(struct Accumulator *const accumulator)
{
    static enum AccumulatorMonitorState state = GET_CELL_VOLTAGE_STATE;

    switch (state)
    {
        case GET_CELL_VOLTAGE_STATE:

            // Read cell voltages. Increment the number of comm. tries if there
            // is a communication error
            if (!accumulator->read_cell_voltages())
            {
                accumulator->num_comm_tries++;
            }
            else
            {
                // Reset the number of communication tries
                accumulator->num_comm_tries = 0U;
            }

            // Write to configuration register to configure cell discharging
            accumulator->write_cfg_registers();

            // Start cell voltage conversions for the next cycle
            accumulator->start_cell_temp_conv();
            state = GET_CELL_TEMP_STATE;
            break;

        case GET_CELL_TEMP_STATE:

            // Read cell temperatures
            accumulator->read_cell_temperatures();

            // Start cell voltage conversions for the next cycle
            accumulator->start_cell_voltage_conv();
            state = GET_CELL_VOLTAGE_STATE;
            break;

        default:
            break;
    }
}
