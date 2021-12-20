#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "App_Accumulator.h"

struct Accumulator
{
    ExitCode (*configure_cell_monitors)(void);
    ExitCode (*read_cell_voltages)(void);

    struct InRangeCheck *pack_voltage_in_range_check;
    struct InRangeCheck *min_cell_voltage_in_range_check;
    struct InRangeCheck *max_cell_voltage_in_range_check;
    struct InRangeCheck *average_cell_voltage_in_range_check;
    struct InRangeCheck *segment_0_voltage_in_range_check;
    struct InRangeCheck *segment_1_voltage_in_range_check;
    struct InRangeCheck *segment_2_voltage_in_range_check;
    struct InRangeCheck *segment_3_voltage_in_range_check;
    struct InRangeCheck *segment_4_voltage_in_range_check;
    struct InRangeCheck *segment_5_voltage_in_range_check;

    float max_pack_voltage;
};

struct Accumulator *App_Accumulator_Create(
    ExitCode (*configure_cell_monitors)(void),
    ExitCode (*read_cell_voltages)(void),
    float (*get_min_cell_voltage)(void),
    float (*get_max_cell_voltage)(void),
    float (*get_average_cell_voltage)(void),
    float (*get_pack_voltage)(void),
    float (*get_segment_0_voltage)(void),
    float (*get_segment_1_voltage)(void),
    float (*get_segment_2_voltage)(void),
    float (*get_segment_3_voltage)(void),
    float (*get_segment_4_voltage)(void),
    float (*get_segment_5_voltage)(void),

    float min_cell_voltage,
    float max_cell_voltage,
    float min_segment_voltage,
    float max_segment_voltage,
    float min_pack_voltage,
    float max_pack_voltage)
{
    struct Accumulator *accumulator = malloc(sizeof(struct Accumulator));
    assert(accumulator != NULL);

    accumulator->configure_cell_monitors = configure_cell_monitors;
    accumulator->read_cell_voltages      = read_cell_voltages;

    accumulator->min_cell_voltage_in_range_check = App_InRangeCheck_Create(
        get_min_cell_voltage, min_cell_voltage, max_cell_voltage);
    accumulator->max_cell_voltage_in_range_check = App_InRangeCheck_Create(
        get_max_cell_voltage, min_cell_voltage, max_cell_voltage);
    accumulator->average_cell_voltage_in_range_check = App_InRangeCheck_Create(
        get_average_cell_voltage, min_cell_voltage, max_cell_voltage);
    accumulator->pack_voltage_in_range_check = App_InRangeCheck_Create(
        get_pack_voltage, min_pack_voltage, max_pack_voltage);

    accumulator->segment_0_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_0_voltage, min_segment_voltage, max_segment_voltage);
    accumulator->segment_1_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_1_voltage, min_segment_voltage, max_segment_voltage);
    accumulator->segment_2_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_2_voltage, min_segment_voltage, max_segment_voltage);
    accumulator->segment_3_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_3_voltage, min_segment_voltage, max_segment_voltage);
    accumulator->segment_4_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_4_voltage, min_segment_voltage, max_segment_voltage);
    accumulator->segment_5_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_5_voltage, min_segment_voltage, max_segment_voltage);

    accumulator->max_pack_voltage = max_pack_voltage;

    return accumulator;
}

void App_Accumulator_Destroy(struct Accumulator *accumulator)
{
    free(accumulator->segment_0_voltage_in_range_check);
    free(accumulator->segment_1_voltage_in_range_check);
    free(accumulator->segment_2_voltage_in_range_check);
    free(accumulator->segment_3_voltage_in_range_check);
    free(accumulator->segment_4_voltage_in_range_check);
    free(accumulator->segment_5_voltage_in_range_check);
    free(accumulator->max_cell_voltage_in_range_check);
    free(accumulator->min_cell_voltage_in_range_check);
    free(accumulator->average_cell_voltage_in_range_check);
    free(accumulator->pack_voltage_in_range_check);
    free(accumulator);
}

ExitCode App_Accumulator_ConfigureCellMonitors(
    const struct Accumulator *const accumulator)
{
    return accumulator->configure_cell_monitors();
}

ExitCode App_Accumulator_ReadCellVoltages(
    const struct Accumulator *const accumulator)
{
    return accumulator->read_cell_voltages();
}

struct InRangeCheck *App_Accumulator_GetPackVoltageInRangeCheck(
    const struct Accumulator *const accumulator)
{
    return accumulator->pack_voltage_in_range_check;
}

struct InRangeCheck *App_Accumulator_GetMinCellVoltageInRangeCheck(
    const struct Accumulator *const accumulator)
{
    return accumulator->min_cell_voltage_in_range_check;
}

struct InRangeCheck *App_Accumulator_GetMaxCellVoltageInRangeCheck(
    const struct Accumulator *const accumulator)
{
    return accumulator->max_cell_voltage_in_range_check;
}

struct InRangeCheck *App_Accumulator_GetAverageCellVoltageInRangeCheck(
    const struct Accumulator *const accumulator)
{
    return accumulator->average_cell_voltage_in_range_check;
}

struct InRangeCheck *App_Accumulator_GetSegment0VoltageInRangeCheck(
    const struct Accumulator *const accumulator)
{
    return accumulator->segment_0_voltage_in_range_check;
}

struct InRangeCheck *App_Accumulator_GetSegment1VoltageInRangeCheck(
    const struct Accumulator *const accumulator)
{
    return accumulator->segment_1_voltage_in_range_check;
}

struct InRangeCheck *App_Accumulator_GetSegment2VoltageInRangeCheck(
    const struct Accumulator *const accumulator)
{
    return accumulator->segment_2_voltage_in_range_check;
}

struct InRangeCheck *App_Accumulator_GetSegment3VoltageInRangeCheck(
    const struct Accumulator *const accumulator)
{
    return accumulator->segment_3_voltage_in_range_check;
}

struct InRangeCheck *App_Accumulator_GetSegment4VoltageInRangeCheck(
    const struct Accumulator *const accumulator)
{
    return accumulator->segment_4_voltage_in_range_check;
}

struct InRangeCheck *App_Accumulator_GetSegment5VoltageInRangeCheck(
    const struct Accumulator *const accumulator)
{
    return accumulator->segment_5_voltage_in_range_check;
}

float App_Accumulator_GetMaxPackVoltage(
    const struct Accumulator *const accumulator)
{
    return accumulator->max_pack_voltage;
}
