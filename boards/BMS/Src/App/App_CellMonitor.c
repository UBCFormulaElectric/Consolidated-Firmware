#include <stdlib.h>
#include <assert.h>
#include "App_CellMonitor.h"

struct CellMonitor
{
    ExitCode (*configure_daisy_chain)(void);
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
};

struct CellMonitor *App_CellMonitor_Create(
    ExitCode (*configure_daisy_chain)(void),
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
    struct CellMonitor *cell_monitor = malloc(sizeof(struct CellMonitor));
    assert(cell_monitor != NULL);

    cell_monitor->configure_daisy_chain = configure_daisy_chain;
    cell_monitor->read_cell_voltages    = read_cell_voltages;

    cell_monitor->min_cell_voltage_in_range_check = App_InRangeCheck_Create(
        get_min_cell_voltage, min_cell_voltage, max_cell_voltage);
    cell_monitor->max_cell_voltage_in_range_check = App_InRangeCheck_Create(
        get_max_cell_voltage, min_cell_voltage, max_cell_voltage);
    cell_monitor->average_cell_voltage_in_range_check = App_InRangeCheck_Create(
        get_average_cell_voltage, min_cell_voltage, max_cell_voltage);
    cell_monitor->pack_voltage_in_range_check = App_InRangeCheck_Create(
        get_pack_voltage, min_pack_voltage, max_pack_voltage);

    cell_monitor->segment_0_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_0_voltage, min_segment_voltage, max_segment_voltage);
    cell_monitor->segment_1_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_1_voltage, min_segment_voltage, max_segment_voltage);
    cell_monitor->segment_2_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_2_voltage, min_segment_voltage, max_segment_voltage);
    cell_monitor->segment_3_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_3_voltage, min_segment_voltage, max_segment_voltage);
    cell_monitor->segment_4_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_4_voltage, min_segment_voltage, max_segment_voltage);
    cell_monitor->segment_5_voltage_in_range_check = App_InRangeCheck_Create(
        get_segment_5_voltage, min_segment_voltage, max_segment_voltage);

    return cell_monitor;
}

void App_CellMonitor_Destroy(struct CellMonitor *cell_monitor)
{
    free(cell_monitor->segment_0_voltage_in_range_check);
    free(cell_monitor->segment_1_voltage_in_range_check);
    free(cell_monitor->segment_2_voltage_in_range_check);
    free(cell_monitor->segment_3_voltage_in_range_check);
    free(cell_monitor->segment_4_voltage_in_range_check);
    free(cell_monitor->segment_5_voltage_in_range_check);
    free(cell_monitor->max_cell_voltage_in_range_check);
    free(cell_monitor->min_cell_voltage_in_range_check);
    free(cell_monitor->average_cell_voltage_in_range_check);
    free(cell_monitor->pack_voltage_in_range_check);
    free(cell_monitor);
}

ExitCode App_CellMonitor_Configure(const struct CellMonitor *cell_monitor)
{
    return cell_monitor->configure_daisy_chain();
}

ExitCode App_CellMonitor_ReadCellVoltages(
    const struct CellMonitor *const cell_monitor)
{
    return cell_monitor->read_cell_voltages();
}

struct InRangeCheck *App_CellMonitor_GetPackVoltageInRangeCheck(
    const struct CellMonitor *cell_monitor)
{
    return cell_monitor->pack_voltage_in_range_check;
}

struct InRangeCheck *App_CellMonitor_GetMinCellVoltageInRangeCheck(
    const struct CellMonitor *cell_monitor)
{
    return cell_monitor->min_cell_voltage_in_range_check;
}

struct InRangeCheck *App_CellMonitor_GetMaxCellVoltageInRangeCheck(
    const struct CellMonitor *cell_monitor)
{
    return cell_monitor->max_cell_voltage_in_range_check;
}

struct InRangeCheck *App_CellMonitor_GetAverageCellVoltageInRangeCheck(
    const struct CellMonitor *cell_monitor)
{
    return cell_monitor->average_cell_voltage_in_range_check;
}

struct InRangeCheck *App_CellMonitor_GetSegment0VoltageInRangeCheck(
    const struct CellMonitor *const cell_monitor)
{
    return cell_monitor->segment_0_voltage_in_range_check;
}

struct InRangeCheck *App_CellMonitor_GetSegment1VoltageInRangeCheck(
    const struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_1_voltage_in_range_check;
}

struct InRangeCheck *App_CellMonitor_GetSegment2VoltageInRangeCheck(
    const struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_2_voltage_in_range_check;
}

struct InRangeCheck *App_CellMonitor_GetSegment3VoltageInRangeCheck(
    const struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_3_voltage_in_range_check;
}

struct InRangeCheck *App_CellMonitor_GetSegment4VoltageInRangeCheck(
    const struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_4_voltage_in_range_check;
}

struct InRangeCheck *App_CellMonitor_GetSegment5VoltageInRangeCheck(
    const struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_5_voltage_in_range_check;
}
