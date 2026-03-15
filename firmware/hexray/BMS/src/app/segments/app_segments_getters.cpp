#include "app_segments.hpp"
#include "app_segments_internal.hpp"

using namespace std;

namespace app::segments {
    float getPackVoltage(void)
{
    return pack_voltage;
}

CellParam getMaxCellVoltage(void)
{
    return max_cell_voltage;
}

CellParam getMinCellVoltage(void)
{
    return min_cell_voltage;
}

CellParam getMaxCellTemp(void)
{
    return max_cell_temp;
}

CellParam getMinCellTemp(void)
{
    return min_cell_temp;
}
}