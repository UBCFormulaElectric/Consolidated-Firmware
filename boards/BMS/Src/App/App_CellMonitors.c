#include <stdlib.h>
#include <assert.h>
#include "App_CellMonitors.h"
#include "App_InRangeCheck.h"

struct CellMonitors
{
    ExitCode (*read_die_temperatures)(void);

    struct InRangeCheck *monitor_0_die_temp_in_range_check;
    struct InRangeCheck *monitor_1_die_temp_in_range_check;
    struct InRangeCheck *monitor_2_die_temp_in_range_check;
    struct InRangeCheck *monitor_3_die_temp_in_range_check;
    struct InRangeCheck *monitor_4_die_temp_in_range_check;
    struct InRangeCheck *monitor_5_die_temp_in_range_check;
    struct InRangeCheck *min_die_temperature_in_range_check;
    struct InRangeCheck *max_die_temperature_in_range_check;
};

struct CellMonitors *App_CellMonitors_Create(
    ExitCode (*read_die_temperatures)(void),
    float (*get_monitor_0_die_temp)(void),
    float (*get_monitor_1_die_temp)(void),
    float (*get_monitor_2_die_temp)(void),
    float (*get_monitor_3_die_temp)(void),
    float (*get_monitor_4_die_temp)(void),
    float (*get_monitor_5_die_temp)(void),
    float (*get_min_die_temp)(void),
    float (*get_max_die_temp)(void),
    float min_die_temp_threshold,
    float max_die_temp_threshold)
{
    struct CellMonitors *cell_monitors = malloc(sizeof(struct CellMonitors));
    assert(cell_monitors != NULL);

    cell_monitors->read_die_temperatures = read_die_temperatures;

    cell_monitors->monitor_0_die_temp_in_range_check = App_InRangeCheck_Create(
        get_monitor_0_die_temp, min_die_temp_threshold, max_die_temp_threshold);
    cell_monitors->monitor_1_die_temp_in_range_check = App_InRangeCheck_Create(
        get_monitor_1_die_temp, min_die_temp_threshold, max_die_temp_threshold);
    cell_monitors->monitor_2_die_temp_in_range_check = App_InRangeCheck_Create(
        get_monitor_2_die_temp, min_die_temp_threshold, max_die_temp_threshold);
    cell_monitors->monitor_3_die_temp_in_range_check = App_InRangeCheck_Create(
        get_monitor_3_die_temp, min_die_temp_threshold, max_die_temp_threshold);
    cell_monitors->monitor_4_die_temp_in_range_check = App_InRangeCheck_Create(
        get_monitor_4_die_temp, min_die_temp_threshold, max_die_temp_threshold);
    cell_monitors->monitor_5_die_temp_in_range_check = App_InRangeCheck_Create(
        get_monitor_5_die_temp, min_die_temp_threshold, max_die_temp_threshold);

    cell_monitors->min_die_temperature_in_range_check = App_InRangeCheck_Create(
        get_min_die_temp, min_die_temp_threshold, max_die_temp_threshold);
    cell_monitors->max_die_temperature_in_range_check = App_InRangeCheck_Create(
        get_max_die_temp, min_die_temp_threshold, max_die_temp_threshold);

    return cell_monitors;
}

void App_CellMonitors_Destroy(struct CellMonitors *cell_monitors)
{
    free(cell_monitors->monitor_0_die_temp_in_range_check);
    free(cell_monitors->monitor_1_die_temp_in_range_check);
    free(cell_monitors->monitor_2_die_temp_in_range_check);
    free(cell_monitors->monitor_3_die_temp_in_range_check);
    free(cell_monitors->monitor_4_die_temp_in_range_check);
    free(cell_monitors->monitor_5_die_temp_in_range_check);
    free(cell_monitors->min_die_temperature_in_range_check);
    free(cell_monitors->max_die_temperature_in_range_check);
    free(cell_monitors);
}

ExitCode App_CellMonitors_ReadDieTemperatures(
    const struct CellMonitors *cell_monitors)
{
    return cell_monitors->read_die_temperatures();
}

struct InRangeCheck *App_CellMonitors_GetMonitor0DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors)
{
    return cell_monitors->monitor_0_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMonitor1DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors)
{
    return cell_monitors->monitor_1_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMonitor2DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors)
{
    return cell_monitors->monitor_2_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMonitor3DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors)
{
    return cell_monitors->monitor_3_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMonitor4DieTempInRangeCheck(
    const struct CellMonitors *cell_monitors)
{
    return cell_monitors->monitor_4_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMonitorDieTemp5InRangeCheck(
    const struct CellMonitors *cell_monitors)
{
    return cell_monitors->monitor_5_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMinDieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->min_die_temperature_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMaxDieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->max_die_temperature_in_range_check;
}
