#include <stdlib.h>
#include <assert.h>
#include "App_CellMonitors.h"
#include "App_InRangeCheck.h"

struct CellMonitors
{
    ExitCode (*read_die_temperatures)(void);
    float reenable_charger_threshold;
    float reenable_cell_balancing_and_charger_threshold;
    float disable_charger_threshold;
    float disable_cell_balancing_threshold;
    float (*get_max_die_temp)(void);

    struct InRangeCheck *monitor_0_die_temp_in_range_check;
    struct InRangeCheck *monitor_1_die_temp_in_range_check;
    struct InRangeCheck *monitor_2_die_temp_in_range_check;
    struct InRangeCheck *monitor_3_die_temp_in_range_check;
    struct InRangeCheck *monitor_4_die_temp_in_range_check;
    struct InRangeCheck *monitor_5_die_temp_in_range_check;
};

struct CellMonitors *App_CellMonitors_Create(
    ExitCode (*read_die_temperatures)(void),
    float (*get_monitor_0_die_temp)(void),
    float (*get_monitor_1_die_temp)(void),
    float (*get_monitor_2_die_temp)(void),
    float (*get_monitor_3_die_temp)(void),
    float (*get_monitor_4_die_temp)(void),
    float (*get_monitor_5_die_temp)(void),
    float (*get_max_die_temp)(void),
    float min_die_temp_threshold,
    float max_die_temp_threshold,
    float reenable_charger_threshold,
    float reenable_cell_balancing_and_charger_threshold,
    float disable_cell_balancing_threshold,
    float disable_charger_threshold)
{
    struct CellMonitors *cell_monitors = malloc(sizeof(struct CellMonitors));
    assert(cell_monitors != NULL);

    cell_monitors->read_die_temperatures      = read_die_temperatures;
    cell_monitors->reenable_charger_threshold = reenable_charger_threshold;
    cell_monitors->reenable_cell_balancing_and_charger_threshold =
        reenable_cell_balancing_and_charger_threshold;
    cell_monitors->disable_charger_threshold = disable_cell_balancing_threshold;
    cell_monitors->disable_charger_threshold = disable_charger_threshold;
    cell_monitors->get_max_die_temp          = get_max_die_temp;

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
    free(cell_monitors);
}

ExitCode App_CellMonitors_ReadDieTemperatures(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->read_die_temperatures();
}

struct InRangeCheck *App_CellMonitors_GetMonitor0DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->monitor_0_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMonitor1DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->monitor_1_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMonitor2DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->monitor_2_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMonitor3DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->monitor_3_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMonitor4DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->monitor_4_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetMonitorDieTemp5InRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->monitor_5_die_temp_in_range_check;
}

enum CellMonitorsThresholds App_CellMonitors_GetMaxDieTempDegC(
    struct CellMonitors *cell_monitors,
    float *              max_die_temp)
{
    enum CellMonitorsThresholds exit_code = CELL_MONITORS_OK;
    *max_die_temp                         = cell_monitors->get_max_die_temp();

    if (*max_die_temp > cell_monitors->disable_charger_threshold)
    {
        exit_code = CELL_MONITORS_DISABLE_CHARGER_AND_CELL_BALANCING;
    }
    else if (*max_die_temp > cell_monitors->disable_cell_balancing_threshold)
    {
        exit_code = CELL_MONITORS_DISABLE_CELL_BALANCING;
    }
    else if (*max_die_temp < cell_monitors->reenable_charger_threshold)
    {
        exit_code = CELL_MONITORS_REENABLE_CHARGER;
    }
    else if (
        *max_die_temp <
        cell_monitors->reenable_cell_balancing_and_charger_threshold)
    {
        exit_code = CELL_MONITORS_REENABLE_CELL_BALANCING_AND_CHARGER;
    }

    return exit_code;
}
