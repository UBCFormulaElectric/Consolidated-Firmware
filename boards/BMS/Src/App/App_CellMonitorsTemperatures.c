#include "App_CellMonitorsTemperatures.h"
#include "configs/App_AccumulatorConfigs.h"

static float *internal_die_temperatures;

void App_CellMonitorTemperatures_Init(float *(*get_die_temperatures)(void))
{
    internal_die_temperatures = get_die_temperatures();
}

float App_CellMonitorsTemperatures_GetDieTemp0DegC(void)
{
    return internal_die_temperatures[CELL_MONITOR_CHIP_0];
}

float App_CellMonitorsTemperatures_GetDieTemp1DegC(void)
{
    return internal_die_temperatures[CELL_MONITOR_CHIP_1];
}

float App_CellMonitorsTemperatures_GetDieTemp2DegC(void)
{
    return 0.0f;
}

float App_CellMonitorsTemperatures_GetDieTemp3DegC(void)
{
    return 0.0f;
}

float App_CellMonitorsTemperatures_GetDieTemp4DegC(void)
{
    return 0.0f;
}

float App_CellMonitorsTemperatures_GetDieTemp5DegC(void)
{
    return 0.0f;
}

float App_CellMonitorsTemperatures_GetMaxDieTempDegC(void)
{
    float max_cell_voltage = internal_die_temperatures[0];
    for (enum CellMonitorChip current_chip = CELL_MONITOR_CHIP_1;
         current_chip < NUM_OF_CELL_MONITOR_CHIPS; current_chip++)
    {
        float current_cell_voltage = internal_die_temperatures[current_chip];
        if (max_cell_voltage < current_cell_voltage)
        {
            max_cell_voltage = current_cell_voltage;
        }
    }

    return max_cell_voltage;
}
