#include <stdlib.h>
#include <assert.h>
#include "App_CellMonitors.h"
#include "App_InRangeCheck.h"

struct CellMonitors
{
    ExitCode (*read_die_temperatures)(void);
    float (*get_max_die_temp)(void);
    float die_temp_re_enable_charger_degc;
    float die_temp_re_enable_cell_balancing_degc;
    float die_temp_disable_charger_degc;
    float die_temp_disable_cell_balancing_degc;

    struct InRangeCheck *cell_monitor_0_die_temp_in_range_check;
    struct InRangeCheck *cell_monitor_1_die_temp_in_range_check;
    struct InRangeCheck *cell_monitor_2_die_temp_in_range_check;
    struct InRangeCheck *cell_monitor_3_die_temp_in_range_check;
    struct InRangeCheck *cell_monitor_4_die_temp_in_range_check;
    struct InRangeCheck *cell_monitor_5_die_temp_in_range_check;
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
    float min_die_temp_degc,
    float max_die_temp_degc,
    float die_temp_re_enable_charger_degc,
    float die_temp_re_enable_cell_balancing_degc,
    float die_temp_disable_cell_balancing_degc,
    float die_temp_disable_charger_degc)
{
    struct CellMonitors *cell_monitors = malloc(sizeof(struct CellMonitors));
    assert(cell_monitors != NULL);

    cell_monitors->read_die_temperatures = read_die_temperatures;
    cell_monitors->get_max_die_temp      = get_max_die_temp;

    cell_monitors->die_temp_re_enable_charger_degc =
        die_temp_re_enable_charger_degc;
    cell_monitors->die_temp_re_enable_cell_balancing_degc =
        die_temp_re_enable_cell_balancing_degc;
    cell_monitors->die_temp_disable_cell_balancing_degc =
        die_temp_disable_cell_balancing_degc;
    cell_monitors->die_temp_disable_charger_degc =
        die_temp_disable_charger_degc;

    cell_monitors->cell_monitor_0_die_temp_in_range_check =
        App_InRangeCheck_Create(
            get_monitor_0_die_temp, min_die_temp_degc, max_die_temp_degc);
    cell_monitors->cell_monitor_1_die_temp_in_range_check =
        App_InRangeCheck_Create(
            get_monitor_1_die_temp, min_die_temp_degc, max_die_temp_degc);
    cell_monitors->cell_monitor_2_die_temp_in_range_check =
        App_InRangeCheck_Create(
            get_monitor_2_die_temp, min_die_temp_degc, max_die_temp_degc);
    cell_monitors->cell_monitor_3_die_temp_in_range_check =
        App_InRangeCheck_Create(
            get_monitor_3_die_temp, min_die_temp_degc, max_die_temp_degc);
    cell_monitors->cell_monitor_4_die_temp_in_range_check =
        App_InRangeCheck_Create(
            get_monitor_4_die_temp, min_die_temp_degc, max_die_temp_degc);
    cell_monitors->cell_monitor_5_die_temp_in_range_check =
        App_InRangeCheck_Create(
            get_monitor_5_die_temp, min_die_temp_degc, max_die_temp_degc);

    return cell_monitors;
}

void App_CellMonitors_Destroy(struct CellMonitors *cell_monitors)
{
    free(cell_monitors->cell_monitor_0_die_temp_in_range_check);
    free(cell_monitors->cell_monitor_1_die_temp_in_range_check);
    free(cell_monitors->cell_monitor_2_die_temp_in_range_check);
    free(cell_monitors->cell_monitor_3_die_temp_in_range_check);
    free(cell_monitors->cell_monitor_4_die_temp_in_range_check);
    free(cell_monitors->cell_monitor_5_die_temp_in_range_check);
    free(cell_monitors);
}

ExitCode App_CellMonitors_ReadDieTemperatures(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->read_die_temperatures();
}

struct InRangeCheck *App_CellMonitors_GetCellMonitor0DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->cell_monitor_0_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetCellMonitor1DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->cell_monitor_1_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetCellMonitor2DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->cell_monitor_2_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetCellMonitor3DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->cell_monitor_3_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetCellMonitor4DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->cell_monitor_4_die_temp_in_range_check;
}

struct InRangeCheck *App_CellMonitors_GetCellMonitor5DieTempInRangeCheck(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->cell_monitor_5_die_temp_in_range_check;
}

enum ITMPInRangeCheck App_CellMonitors_GetMaxDieTempDegC(
    const struct CellMonitors *const cell_monitors,
    float *const                     max_die_temp)
{
    enum ITMPInRangeCheck exit_code;
    *max_die_temp = cell_monitors->get_max_die_temp();

    if (*max_die_temp<
             cell_monitors->die_temp_disable_charger_degc && * max_die_temp>
             cell_monitors->die_temp_disable_cell_balancing_degc)
    {
        // Return the exit code to disable cell balancing.
        exit_code = ITMP_CELL_BALANCING_OVERFLOW;
    }
    else if (*max_die_temp > cell_monitors->die_temp_disable_charger_degc)
    {
        // Return the exit code to disable both cell balancing as well as the
        // charger.
        exit_code = ITMP_OVERFLOW;
    }
    else if (*max_die_temp<
                  cell_monitors->die_temp_re_enable_charger_degc && *
                  max_die_temp> cell_monitors
                  ->die_temp_re_enable_cell_balancing_degc)
    {
        // Return the exit code to re-enable the charger.
        exit_code = ITMP_CHARGER_IN_RANGE;
    }
    else
    {
        // Return the exit to re-enable both the charger and cell balancing.
        exit_code = ITMP_IN_RANGE;
    }

    return exit_code;
}
