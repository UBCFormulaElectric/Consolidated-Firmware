#include "App_Accumulator.h"

// Max number of PEC15 to occur before faulting
#define MAX_NUM_COMM_TRIES (3U)

// Update the counter keeping track of the PEC15 error
#define UPDATE_PEC15_ERROR_COUNT(is_pec_ok, num_comm_tries) \
    if ((is_pec_ok))                                        \
    {                                                       \
        (num_comm_tries) = 0U;                              \
    }                                                       \
    else                                                    \
    {                                                       \
        (num_comm_tries)++;                                 \
    }

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
    float (*get_min_cell_temp)(uint8_t *, uint8_t *);
    float (*get_max_cell_temp)(uint8_t *, uint8_t *);
    float (*get_avg_cell_temp)(void);

    bool (*enable_discharge)(void);
    bool (*disable_discharge)(void);
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
    bool (*read_cell_temperatures)(void),
    float (*get_min_cell_temp)(uint8_t *, uint8_t *),
    float (*get_max_cell_temp)(uint8_t *, uint8_t *),
    float (*get_avg_cell_temp)(void),
    bool (*enable_discharge)(void),
    bool (*disable_discharge)(void))
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
    accumulator->get_min_cell_temp      = get_min_cell_temp;
    accumulator->get_max_cell_temp      = get_max_cell_temp;
    accumulator->get_avg_cell_temp      = get_avg_cell_temp;

    accumulator->enable_discharge  = enable_discharge;
    accumulator->disable_discharge = disable_discharge;

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

float App_Accumulator_GetPackVoltage(struct Accumulator *accumulator)
{
    return accumulator->get_pack_voltage();
}

void App_Accumulator_InitRunOnEntry(const struct Accumulator *const accumulator)
{
    // Configure the cell monitoring chips. Disable discharge at startup
    accumulator->config_monitoring_chip();
}

float App_Accumulator_GetMaxVoltage(const struct Accumulator *const accumulator, uint8_t *segment, uint8_t *cell)
{
    return accumulator->get_max_cell_voltage(segment, cell);
}

float App_Accumulator_GetMinVoltage(const struct Accumulator *const accumulator, uint8_t *segment, uint8_t *cell)
{
    return accumulator->get_min_cell_voltage(segment, cell);
}

float App_Accumulator_GetMinCellTempDegC(
    const struct Accumulator *const accumulator,
    uint8_t *                       segment,
    uint8_t *                       thermistor)
{
    return accumulator->get_min_cell_temp(segment, thermistor);
}

float App_Accumulator_GetMaxCellTempDegC(
    const struct Accumulator *const accumulator,
    uint8_t *                       segment,
    uint8_t *                       thermistor)
{
    return accumulator->get_max_cell_temp(segment, thermistor);
}

float App_Accumulator_GetAvgCellTempDegC(const struct Accumulator *const accumulator)
{
    return accumulator->get_avg_cell_temp();
}

bool App_Accumulator_EnableDischarge(const struct Accumulator *const accumulator)
{
    return accumulator->enable_discharge();
}

bool App_Accumulator_DisableDischarge(const struct Accumulator *const accumulator)
{
    return accumulator->disable_discharge();
}

void App_Accumulator_RunOnTick100Hz(struct Accumulator *const accumulator)
{
    static enum AccumulatorMonitorState state = GET_CELL_VOLTAGE_STATE;

    switch (state)
    {
        case GET_CELL_VOLTAGE_STATE:

            UPDATE_PEC15_ERROR_COUNT(accumulator->read_cell_voltages(), accumulator->num_comm_tries)

            // Write to configuration register to configure cell discharging
            // TODO: re-enable
            // accumulator->write_cfg_registers();
            accumulator->disable_discharge();

            // Start cell voltage conversions for the next cycle
            accumulator->start_cell_temp_conv();
            state = GET_CELL_TEMP_STATE;
            break;

        case GET_CELL_TEMP_STATE:

            UPDATE_PEC15_ERROR_COUNT(accumulator->read_cell_temperatures(), accumulator->num_comm_tries)

            // Start cell voltage conversions for the next cycle
            accumulator->start_cell_voltage_conv();
            accumulator->disable_discharge();

            state = GET_CELL_VOLTAGE_STATE;
            break;

        default:
            break;
    }
}

bool App_Accumulator_CheckFaults(struct Accumulator *const accumulator, struct TractiveSystem *const ts)
{
    uint8_t throwaway_segment = 0U;
    uint8_t throwaway_loc     = 0U;

    float max_allowable_cell_temp = MAX_CELL_DISCHARGE_TEMP_DEGC;
    float min_allowable_cell_temp = MIN_CELL_DISCHARGE_TEMP_DEGC;

    // if we are charging, max cell temp is 45C not 60C
    if (App_TractiveSystem_GetCurrent(ts) < -3.0f)
    {
        max_allowable_cell_temp = MAX_CELL_CHARGE_TEMP_DEGC;
        min_allowable_cell_temp = MIN_CELL_CHARGE_TEMP_DEGC;
    }

    bool overtemp_fault =
        App_Accumulator_GetMaxCellTempDegC(accumulator, &throwaway_segment, &throwaway_loc) > max_allowable_cell_temp;
    bool undertemp_fault =
        App_Accumulator_GetMinCellTempDegC(accumulator, &throwaway_segment, &throwaway_loc) < min_allowable_cell_temp;
    bool overvoltage_fault =
        App_Accumulator_GetMaxVoltage(accumulator, &throwaway_segment, &throwaway_loc) > MAX_CELL_VOLTAGE;
    bool undervoltage_fault =
        App_Accumulator_GetMinVoltage(accumulator, &throwaway_segment, &throwaway_loc) < MIN_CELL_VOLTAGE;
    bool communication_fault = App_Accumulator_HasCommunicationError(accumulator);

    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_CELL_UNDERVOLTAGE_FAULT(can_tx, undervoltage_fault);
    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_CELL_OVERVOLTAGE_FAULT(can_tx, overvoltage_fault);
    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_CELL_UNDERTEMP_FAULT(can_tx, undertemp_fault);
    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_CELL_OVERTEMP_FAULT(can_tx, overtemp_fault);
    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_MODULE_COMM_ERROR(can_tx, communication_fault);

    return (overtemp_fault || undertemp_fault || overvoltage_fault || undervoltage_fault || communication_fault);
}
