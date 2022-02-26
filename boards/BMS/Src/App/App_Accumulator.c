#include <stdlib.h>
#include <assert.h>
#include "App_InRangeCheck.h"
#include "App_Accumulator.h"

#define MIN_CELL_V (3.0f)
#define MAX_CELL_V (4.2f)

enum CellMonitorState
{
    GET_CELL_VOLTAGE = 0U,
    GET_CELL_TEMP,
    GET_DIE_TEMP,
};

struct Accumulator
{
    // Configure the cell monitoring chip
    bool (*config_monitoring_chip)(void);

    // Cell voltage monitoring functions
    bool (*start_cell_voltage_conv)(void);
    bool (*read_cell_voltages)(void);
    void (*get_min_cell_v_loc)(uint8_t *, uint8_t *);
    void (*get_max_cell_v_loc)(uint8_t *, uint8_t *);
    float (*get_min_cell_voltage)(void);
    float (*get_max_cell_voltage)(void);
    float (*get_segment_voltage)(AccumulatorSegments_E);
    float (*get_pack_voltage)(void);
    float (*get_avg_voltage)(void);

    // Cell temperature monitoring functions
    bool (*start_cell_temp_conv)(void);
    bool (*read_cell_temp)(void);
    void (*get_min_cell_temp_loc)(uint8_t *, uint8_t *);
    void (*get_max_cell_temp_loc)(uint8_t *, uint8_t *);
    float (*get_min_cell_temp)(void);
    float (*get_max_cell_temp)(void);
    float (*get_avg_cell_temp)(void);

    // Die temperature monitoring functions
    bool (*start_die_temp_conv)(void);
    bool (*read_die_temp)(void);
    void (*get_min_die_temp_loc)(uint8_t *, uint8_t *);
    void (*get_max_die_temp_loc)(uint8_t *, uint8_t *);
    float (*get_min_die_temp)(void);
    float (*get_max_die_temp)(void);
    float (*get_avg_die_temp)(void);
};

// TODO: Make this nicer
static inline uint8_t temp_scale_for_can(float temp)
{
    return (uint8_t)(temp * (127U / 63.5f));
}

struct Accumulator *App_Accumulator_Create(
    bool (*configure_monitoring_chip)(void),
    bool (*start_voltage_conv)(void),
    bool (*read_cell_voltages)(void),
    void (*get_min_cell_location)(uint8_t *, uint8_t *),
    void (*get_max_cell_location)(uint8_t *, uint8_t *),
    float (*get_min_cell_voltage)(void),
    float (*get_max_cell_voltage)(void),
    float (*get_segment_voltage)(AccumulatorSegments_E),
    float (*get_pack_voltage)(void),
    float (*get_average_voltage)(void))
{
    struct Accumulator *accumulator = malloc(sizeof(struct Accumulator));
    assert(accumulator != NULL);

    accumulator->config_monitoring_chip = configure_monitoring_chip;

    // Cell voltage monitoring functions
    accumulator->read_cell_voltages      = read_cell_voltages;
    accumulator->start_cell_voltage_conv = start_voltage_conv;
    accumulator->get_min_cell_v_loc      = get_min_cell_location;
    accumulator->get_max_cell_v_loc      = get_max_cell_location;
    accumulator->get_min_cell_voltage    = get_min_cell_voltage;
    accumulator->get_max_cell_voltage    = get_max_cell_voltage;
    accumulator->get_segment_voltage     = get_segment_voltage;
    accumulator->get_pack_voltage        = get_pack_voltage;
    accumulator->get_avg_voltage         = get_average_voltage;

    // Cell temperature monitoring functions
    accumulator->start_cell_temp_conv  = NULL;
    accumulator->read_cell_temp        = NULL;
    accumulator->get_min_cell_temp_loc = NULL;
    accumulator->get_max_cell_temp_loc = NULL;
    accumulator->get_min_cell_temp     = NULL;
    accumulator->get_max_cell_temp     = NULL;
    accumulator->get_avg_cell_temp     = NULL;

    // Die temperature monitoring functions
    accumulator->start_die_temp_conv  = NULL;
    accumulator->read_die_temp        = NULL;
    accumulator->get_min_die_temp_loc = NULL;
    accumulator->get_max_die_temp_loc = NULL;
    accumulator->get_min_die_temp     = NULL;
    accumulator->get_max_die_temp     = NULL;
    accumulator->get_avg_die_temp     = NULL;

    return accumulator;
}

void App_Accumulator_Destroy(struct Accumulator *accumulator)
{
    free(accumulator);
}

bool App_Accumulator_ReadCellVoltages(
    const struct Accumulator *const accumulator)
{
    return accumulator->read_cell_voltages();
}

bool App_Accumulator_StartCellVoltageConversion(
    const struct Accumulator *const accumulator)
{
    return accumulator->start_cell_voltage_conv();
}

void App_Accumulator_InitRunOnEntry(const struct Accumulator *const accumulator)
{
    // Configure the cell monitoring chips. Disable discharge at startup
    accumulator->config_monitoring_chip();
}

void App_Accumulator_AllStates100Hz(
    const struct Accumulator *const accumulator,
    struct BmsCanTxInterface *      can_tx,
    struct ErrorTable *             error_table)
{
    UNUSED(error_table);

    // Send the minimum voltage (V) over CAN. Set errors if the voltage is out
    // of range
    // if (App_InRangeCheck_SetPeriodicCanSignal(
    //        can_tx, accumulator->get_min_cell_voltage(), MIN_CELL_V,
    //        MAX_CELL_V,
    //        App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE_OUT_OF_RANGE,
    //        App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE) != VALUE_IN_RANGE)
    //{
    //    App_SharedErrorTable_SetError(
    //        error_table, BMS_AIR_SHUTDOWN_MIN_CELL_VOLTAGE_OUT_OF_RANGE,
    //        true);
    //}

    // Send the maximum voltage (V) over CAN. Set errors if the voltage is out
    // of range
    // if (App_InRangeCheck_SetPeriodicCanSignal(
    //        can_tx, accumulator->get_max_cell_voltage(), MIN_CELL_V,
    //        MAX_CELL_V,
    //        App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE_OUT_OF_RANGE,
    //        App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE) != VALUE_IN_RANGE)
    //{
    //    App_SharedErrorTable_SetError(
    //        error_table, BMS_AIR_SHUTDOWN_MAX_CELL_VOLTAGE_OUT_OF_RANGE,
    //        true);
    //}

    // Send the segment voltages (V) over CAN
    // App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE(
    //    can_tx, raw_thermistor_voltages[0][0]);
    // App_CanTx_SetPeriodicSignal_SEGMENT_1_VOLTAGE(
    //    can_tx, accumulator->get_segment_voltage(ACCUMULATOR_SEGMENT_1));
    // App_CanTx_SetPeriodicSignal_SEGMENT_2_VOLTAGE(
    //    can_tx, accumulator->get_segment_voltage(ACCUMULATOR_SEGMENT_2));

    // Send the pack voltage (V) over CAN
    App_CanTx_SetPeriodicSignal_PACK_VOLTAGE(
        can_tx, accumulator->get_pack_voltage());

    // Send the average voltage (V) over CAN
    App_CanTx_SetPeriodicSignal_AVERAGE_CELL_VOLTAGE(
        can_tx, accumulator->get_avg_voltage());
}

void App_Accumulator_GetCellVoltage(struct Accumulator *accumulator)
{
    accumulator->read_cell_voltages();
    accumulator->start_cell_temp_conv();
}

static inline void App_SetCellVoltagePeriodicSignals(
    struct Accumulator *      accumulator,
    struct BmsCanTxInterface *can_tx)
{
    // Set periodic signals for cell voltage
    App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE(
        can_tx, accumulator->get_segment_voltage(ACCUMULATOR_SEGMENT_0));
    // App_CanTx_SetPeriodicSignal_SEGMENT_1_VOLTAGE(
    //    can_tx,
    //    accumulator->get_segment_voltage(ACCUMULATOR_SEGMENT_1));
    // App_CanTx_SetPeriodicSignal_SEGMENT_2_VOLTAGE(
    //    can_tx,
    //    accumulator->get_segment_voltage(ACCUMULATOR_SEGMENT_2));
    // App_CanTx_SetPeriodicSignal_SEGMENT_3_VOLTAGE(
    //    can_tx,
    //    accumulator->get_segment_voltage(ACCUMULATOR_SEGMENT_3));
}

static void App_SetCellTemperaturePeriodicSignals(
    struct Accumulator *      accumulator,
    struct BmsCanTxInterface *can_tx)
{
    uint8_t segment = 0U;
    uint8_t loc     = 0U;

    // Report min and max cell temp
    App_CanTx_SetPeriodicSignal_MAX_CELL_TEMP_DEGC(
        can_tx, temp_scale_for_can(accumulator->get_max_cell_temp()));
    App_CanTx_SetPeriodicSignal_MIN_CELL_TEMP_DEGC(
        can_tx, temp_scale_for_can(accumulator->get_min_cell_temp()));
    App_CanTx_SetPeriodicSignal_AVERAGE_CELL_TEMP_DEGC(
        can_tx, temp_scale_for_can(accumulator->get_avg_cell_temp()));

    // Report min cell location
    accumulator->get_min_cell_temp_loc(&segment, &loc);
    App_CanTx_SetPeriodicSignal_MIN_CELL_TEMP_SEGMENT(can_tx, segment);
    App_CanTx_SetPeriodicSignal_MIN_CELL_TEMP_THERM(can_tx, loc);

    // Report max cell location
    accumulator->get_max_cell_temp_loc(&segment, &loc);
    App_CanTx_SetPeriodicSignal_MAX_CELL_TEMP_SEGMENT(can_tx, segment);
    App_CanTx_SetPeriodicSignal_MAX_CELL_TEMP_THERM(can_tx, loc);
}

void App_Accumulator_RunOnTick100Hz(
    struct Accumulator *      accumulator,
    struct BmsCanTxInterface *can_tx)
{
    static enum CellMonitorState state = GET_CELL_VOLTAGE;

    switch (state)
    {
        case GET_CELL_VOLTAGE:
            accumulator->read_cell_voltages();

            // Start cell temperature voltage conversions for the next cycle
            accumulator->start_cell_temp_conv();

            App_SetCellVoltagePeriodicSignals(accumulator, can_tx);

            state = GET_CELL_TEMP;
            break;

        case GET_CELL_TEMP:
            accumulator->read_cell_temp();

            // Start die temperature voltage conversions for the next cycle
            accumulator->start_die_temp_conv();
            App_SetCellTemperaturePeriodicSignals(accumulator, can_tx);

            state = GET_DIE_TEMP;
            break;

        case GET_DIE_TEMP:
            accumulator->read_die_temp();

            // Start cell voltage conversions for the next cycle
            accumulator->start_cell_voltage_conv();
            state = GET_CELL_VOLTAGE;
            break;

        default:
            break;
    }
}
