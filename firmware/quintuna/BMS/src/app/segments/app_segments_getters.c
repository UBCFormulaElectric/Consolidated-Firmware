#include "app_segments.h"
#include "app_segments_internal.h"

float app_segments_getPackVoltage(void)
{
    return pack_voltage;
}

float app_segments_getMaxCellVoltage(void)
{
    return max_cell_voltage;
}

float app_segments_getMinCellVoltage(void)
{
    return min_cell_voltage;
}

float app_segments_getMaxCellTemp(void)
{
    return max_cell_temp;
}

float app_segments_getMinCellTemp(void)
{
    return min_cell_temp;
}
