#include <stdlib.h>
#include <assert.h>
#include "App_InRangeCheck.h"
#include "App_Accumulator.h"

#define MIN_CELL_V (3.0f)
#define MAX_CELL_V (4.2f)

struct Accumulator
{
    bool (*config_accumulator_monitors)(void);
    bool (*start_voltage_conv)(void);
    bool (*read_cell_voltages)(void);
    void (*get_min_cell_location)(uint8_t *, uint8_t *);
    void (*get_max_cell_location)(uint8_t *, uint8_t *);
    float (*get_min_cell_voltage)(void);
    float (*get_max_cell_voltage)(void);
    float (*get_segment_voltage)(enum AccumulatorSegments);
    float (*get_pack_voltage)(void);
    float (*get_average_voltage)(void);
};

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
    float (*get_average_voltage)(void))
{
    struct Accumulator *accumulator = malloc(sizeof(struct Accumulator));
    assert(accumulator != NULL);

    accumulator->config_accumulator_monitors = configure_cell_monitors;
    accumulator->read_cell_voltages          = read_cell_voltages;
    accumulator->start_voltage_conv          = start_voltage_conv;

    accumulator->get_min_cell_location = get_min_cell_location;
    accumulator->get_max_cell_location = get_max_cell_location;
    accumulator->get_min_cell_voltage  = get_min_cell_voltage;
    accumulator->get_max_cell_voltage  = get_max_cell_voltage;
    accumulator->get_segment_voltage   = get_segment_voltage;
    accumulator->get_pack_voltage      = get_pack_voltage;
    accumulator->get_average_voltage   = get_average_voltage;

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
    return accumulator->start_voltage_conv();
}

void App_Accumulator_InitRunOnEntry(const struct Accumulator *const accumulator)
{
    // Send random command to wake up accumulator monitoring device
    if (accumulator->start_voltage_conv())
    {
        // After the device is awake, configure monitor
        accumulator->config_accumulator_monitors();
    }

    // Start voltage conversions for the accumulator monitor
    accumulator->start_voltage_conv();
}

#include "Io_CellTemperatures.h"
extern uint16_t raw_therm_v[NUM_OF_ACCUMULATOR_SEGMENTS][8];

void App_Accumulator_AllStates100Hz(
    const struct Accumulator *const accumulator,
    struct BmsCanTxInterface *      can_tx,
    struct ErrorTable *             error_table)
{
    // Send the minimum voltage (V) over CAN. Set errors if the voltage is out
    // of range
    if (App_InRangeCheck_SetPeriodicCanSignal(
            can_tx, accumulator->get_min_cell_voltage(), MIN_CELL_V, MAX_CELL_V,
            App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE_OUT_OF_RANGE,
            App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE) != VALUE_IN_RANGE)
    {
        App_SharedErrorTable_SetError(
            error_table, BMS_AIR_SHUTDOWN_MIN_CELL_VOLTAGE_OUT_OF_RANGE, true);
    }

    // Send the maximum voltage (V) over CAN. Set errors if the voltage is out
    // of range
    if (App_InRangeCheck_SetPeriodicCanSignal(
            can_tx, accumulator->get_max_cell_voltage(), MIN_CELL_V, MAX_CELL_V,
            App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE_OUT_OF_RANGE,
            App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE) != VALUE_IN_RANGE)
    {
        App_SharedErrorTable_SetError(
            error_table, BMS_AIR_SHUTDOWN_MAX_CELL_VOLTAGE_OUT_OF_RANGE, true);
    }

    // Send the segment voltages (V) over CAN
    App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE(
        can_tx, raw_therm_v[0][0]);
    // App_CanTx_SetPeriodicSignal_SEGMENT_1_VOLTAGE(
    //    can_tx, accumulator->get_segment_voltage(ACCUMULATOR_SEGMENT_1));
    // App_CanTx_SetPeriodicSignal_SEGMENT_2_VOLTAGE(
    //    can_tx, accumulator->get_segment_voltage(ACCUMULATOR_SEGMENT_2));

    // Send the pack voltage (V) over CAN
    App_CanTx_SetPeriodicSignal_PACK_VOLTAGE(
        can_tx, accumulator->get_pack_voltage());

    // Send the average voltage (V) over CAN
    App_CanTx_SetPeriodicSignal_AVERAGE_CELL_VOLTAGE(
        can_tx, accumulator->get_average_voltage());
}
