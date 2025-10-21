#include "app/segments.h"
#include "app/segments/segments_internal.h"

float app_segments_getPackVoltage(void)
{
    return pack_voltage;
}

CellParam app_segments_getMaxCellVoltage(void)
{
    return max_cell_voltage;
}

CellParam app_segments_getMinCellVoltage(void)
{
    return min_cell_voltage;
}

CellParam app_segments_getMaxCellTemp(void)
{
    return max_cell_temp;
}

CellParam app_segments_getMinCellTemp(void)
{
    return min_cell_temp;
}
