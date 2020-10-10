#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "App_InRangeCheck.h"
#include "App_CellVoltages.h"
#include "App_CellMonitor.h"

struct CellMonitor
{
    void (*configure_daisy_chain)(void);
    ExitCode (*read_cell_voltages)(void);

    struct InRangeCheck *pack;
    struct InRangeCheck *min;
    struct InRangeCheck *max;
    struct InRangeCheck *average_cell;
    struct InRangeCheck *segment_0;
    struct InRangeCheck *segment_1;
    struct InRangeCheck *segment_2;
    struct InRangeCheck *segment_3;
    struct InRangeCheck *segment_4;
    struct InRangeCheck *segment_5;
};

struct CellMonitor *App_CellMonitor_Create(
    void (*configure_daisy_chain)(void),
    ExitCode (*read_cell_voltages)(void),
    uint16_t *(*get_cell_voltages)(void),
    float    min_cell_voltage,
    float    max_cell_voltage,
    uint32_t num_of_cells_per_segment)
{
    struct CellMonitor *cell_monitor = malloc(sizeof(struct CellMonitor));
    assert(cell_monitor != NULL);

    // Initialize functions used to calculate voltages.
    App_CellVoltages_Init(get_cell_voltages, num_of_cells_per_segment);

    cell_monitor->configure_daisy_chain = configure_daisy_chain;
    cell_monitor->read_cell_voltages    = read_cell_voltages;

    cell_monitor->segment_0 = App_InRangeCheck_Create(
        App_CellVoltages_GetSegment0Average, min_cell_voltage,
        max_cell_voltage);
    cell_monitor->segment_1 = App_InRangeCheck_Create(
        App_CellVoltages_GetSegment1Average, min_cell_voltage,
        max_cell_voltage);
    cell_monitor->segment_2 = App_InRangeCheck_Create(
        App_CellVoltages_GetSegment2Average, min_cell_voltage,
        max_cell_voltage);
    cell_monitor->segment_3 = App_InRangeCheck_Create(
        App_CellVoltages_GetSegment3Average, min_cell_voltage,
        max_cell_voltage);
    cell_monitor->segment_4 = App_InRangeCheck_Create(
        App_CellVoltages_GetSegment4Average, min_cell_voltage,
        max_cell_voltage);
    cell_monitor->segment_5 = App_InRangeCheck_Create(
        App_CellVoltages_GetSegment5Average, min_cell_voltage,
        max_cell_voltage);

    cell_monitor->pack = App_InRangeCheck_Create(
        App_CellVoltages_GetPack, min_cell_voltage, max_cell_voltage);
    cell_monitor->min = App_InRangeCheck_Create(
        App_CellVoltages_GetMinCell, min_cell_voltage, max_cell_voltage);
    cell_monitor->min = App_InRangeCheck_Create(
        App_CellVoltages_GetMaxCell, min_cell_voltage, max_cell_voltage);
    cell_monitor->average_cell = App_InRangeCheck_Create(
        App_CellVoltages_GetAverageCell, min_cell_voltage, max_cell_voltage);

    return cell_monitor;
}

void App_CellMonitor_Destroy(struct CellMonitor *cell_monitor)
{
    free(cell_monitor->segment_0);
    free(cell_monitor->segment_1);
    free(cell_monitor->segment_2);
    free(cell_monitor->segment_3);
    free(cell_monitor->segment_4);
    free(cell_monitor->segment_5);
    free(cell_monitor->max);
    free(cell_monitor->min);
    free(cell_monitor->pack);
    free(cell_monitor);
}

void App_CellMonitor_Configure(const struct CellMonitor *cell_monitor)
{
    cell_monitor->configure_daisy_chain();
}

ExitCode App_CellMonitor_ReadCellVoltages(
    const struct CellMonitor *const cell_monitor)
{
    return cell_monitor->read_cell_voltages();
}

struct InRangeCheck *
    App_CellMonitor_GetAverageSegment0Voltage(struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_0;
}

struct InRangeCheck *
    App_CellMonitor_GetAverageSegment1Voltage(struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_1;
}

struct InRangeCheck *App_CellMonitoring_GetAverageSegment2Voltage(
    struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_2;
}

struct InRangeCheck *
    App_CellMonitor_GetAverageSegment3Voltage(struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_3;
}

struct InRangeCheck *
    App_CellMonitor_GetAverageSegment4Voltage(struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_4;
}

struct InRangeCheck *
    App_CellMonitor_GetAverageSegment5Voltage(struct CellMonitor *cell_monitor)
{
    return cell_monitor->segment_5;
}

struct InRangeCheck *
    App_CellMonitor_GetPackVoltage(struct CellMonitor *cell_monitor)
{
    return cell_monitor->pack;
}

struct InRangeCheck *
    App_CellMonitor_GetMinVoltage(struct CellMonitor *cell_monitor)
{
    return cell_monitor->min;
}

struct InRangeCheck *
    App_CellMonitor_GetMaxVoltage(struct CellMonitor *cell_monitor)
{
    return cell_monitor->max;
}

struct InRangeCheck *
    App_CellMonitor_GetAverageCellVoltage(struct CellMonitor *cell_monitor)
{
    return cell_monitor->average_cell;
}
