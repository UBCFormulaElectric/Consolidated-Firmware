#include <stdlib.h>
#include <assert.h>
#include "App_CellMonitors.h"
#include "App_InRangeCheck.h"

struct CellMonitors
{
    bool (*read_die_temps)(void);
    float (*get_die_temp)(AccumulatorSegments_E);
    float (*get_max_die_temp)(void);
};

struct CellMonitors *App_CellMonitors_Create(
    bool (*read_die_temps)(void),
    float (*get_die_temp)(AccumulatorSegments_E),
    float (*get_max_die_temp)(void))
{
    struct CellMonitors *cell_monitors = malloc(sizeof(struct CellMonitors));
    assert(cell_monitors != NULL);

    cell_monitors->read_die_temps   = read_die_temps;
    cell_monitors->get_die_temp     = get_die_temp;
    cell_monitors->get_max_die_temp = get_max_die_temp;

    return cell_monitors;
}

void App_CellMonitors_Destroy(struct CellMonitors *cell_monitors)
{
    free(cell_monitors);
}

bool App_CellMonitors_ReadDieTemps(
    const struct CellMonitors *const cell_monitors)
{
    return cell_monitors->read_die_temps();
}

enum ITMPInRangeCheck App_CellMonitors_GetMaxDieTempDegC(
    const struct CellMonitors *const cell_monitors,
    float *const                     max_die_temp)
{
    enum ITMPInRangeCheck exit_code = ITMP_IN_RANGE;
    *max_die_temp                   = cell_monitors->get_max_die_temp();

    return exit_code;
}
