#include <stddef.h>
#include "App_AccumulatorVoltages.h"
#include "configs/App_AccumulatorConfigs.h"

#define V_PER_100UV 1E-4f

struct CellVoltages
{
    uint16_t *raw_cell_voltages;
    size_t    total_num_of_cells;
    size_t    num_of_cells_per_segment;
};

static struct CellVoltages cell_voltages;

/**
 * A function used to compute the sum of array elements.
 * @param array A pointer to the given array.
 * @param length The length of the given array.
 * @return The sum of elements for the given array.
 */
static uint32_t App_SumOfArrayElements(uint16_t *array, size_t length);

static uint32_t App_SumOfArrayElements(uint16_t *array, size_t length)
{
    uint32_t array_sum = 0U;
    for (size_t i = 0U; i < length; i++)
    {
        array_sum += array[i];
    }

    return array_sum;
}

void App_CellVoltages_Init(
    uint16_t *(*get_raw_cell_voltages)(void),
    uint32_t num_of_cells_per_segment)
{
    // Get the pointer to the 2D array of cell voltages read back from the cell
    // monitoring daisy chain.
    cell_voltages.raw_cell_voltages        = get_raw_cell_voltages();
    cell_voltages.num_of_cells_per_segment = num_of_cells_per_segment;
    cell_voltages.total_num_of_cells =
        num_of_cells_per_segment * NUM_OF_CELL_MONITOR_ICS;
}

float App_CellVoltages_GetMinCellVoltage(void)
{
    uint16_t min_cell_voltage = cell_voltages.raw_cell_voltages[0];
    for (size_t current_cell = 1U;
         current_cell < cell_voltages.total_num_of_cells; current_cell++)
    {
        uint16_t current_cell_voltage =
            cell_voltages.raw_cell_voltages[current_cell];
        if (min_cell_voltage > current_cell_voltage)
        {
            min_cell_voltage = current_cell_voltage;
        }
    }

    return min_cell_voltage * V_PER_100UV;
}

float App_CellVoltages_GetMaxCellVoltage(void)
{
    float max_cell_voltage = cell_voltages.raw_cell_voltages[0];
    for (size_t current_cell = 1U;
         current_cell < cell_voltages.total_num_of_cells; current_cell++)
    {
        float current_cell_voltage =
            cell_voltages.raw_cell_voltages[current_cell];
        if (max_cell_voltage < current_cell_voltage)
        {
            max_cell_voltage = current_cell_voltage;
        }
    }

    return (float)max_cell_voltage * V_PER_100UV;
}

float App_CellVoltages_GetPackVoltage(void)
{
    return (float)App_SumOfArrayElements(
               cell_voltages.raw_cell_voltages,
               cell_voltages.total_num_of_cells) *
           V_PER_100UV;
}

float App_CellVoltages_GetAverageCellVoltage(void)
{
    return App_CellVoltages_GetPackVoltage() /
           (float)cell_voltages.total_num_of_cells;
}

float App_CellVoltages_GetSegment0Voltage(void)
{
    return (float)App_SumOfArrayElements(
               &cell_voltages.raw_cell_voltages
                    [CELL_MONITOR_IC_0 *
                     cell_voltages.num_of_cells_per_segment],
               cell_voltages.num_of_cells_per_segment) *
           V_PER_100UV;
}

float App_CellVoltages_GetSegment1Voltage(void)
{
    return (float)App_SumOfArrayElements(
               &cell_voltages.raw_cell_voltages
                    [CELL_MONITOR_IC_1 *
                     cell_voltages.num_of_cells_per_segment],
               cell_voltages.num_of_cells_per_segment) *
           V_PER_100UV;
}

float App_CellVoltages_GetSegment2Voltage(void)
{
    return 0.0f;
}

float App_CellVoltages_GetSegment3Voltage(void)
{
    return 0.0f;
}

float App_CellVoltages_GetSegment4Voltage(void)
{
    return 0.0f;
}

float App_CellVoltages_GetSegment5Voltage(void)
{
    return 0.0f;
}
