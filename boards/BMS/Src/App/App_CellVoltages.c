#include <stddef.h>
#include <stdint.h>
#include "App_CellVoltages.h"

struct CellVoltages
{
    uint16_t *measured_cells;
    uint32_t  total_num_of_cells;
    uint32_t  num_of_cells_per_segment;
};

static struct CellVoltages cell_voltages;

/**
 * A function used to compute the sum of array elements.
 * @param array A pointer to the given array.
 * @param size The size of the given array.
 * @return The sum of elements for the given array.
 */
static float App_SumOfArrayElements(uint16_t *array, uint32_t size);

static float App_SumOfArrayElements(uint16_t *array, uint32_t size)
{
    uint32_t array_sum = 0U;
    for (size_t i = 0U; i < size; i++)
    {
        array_sum += array[i];
    }
    return (float)array_sum;
}

void App_CellVoltages_Init(
    uint16_t *(*get_cell_voltages)(void),
    uint32_t num_of_cells_per_segment)
{
    // Get the pointer to the 2D array of cell voltages read back from the cell
    // monitoring daisy chain.
    cell_voltages.measured_cells           = get_cell_voltages();
    cell_voltages.num_of_cells_per_segment = num_of_cells_per_segment;
    cell_voltages.total_num_of_cells = num_of_cells_per_segment * NUM_SEGMENTS;
}

float App_CellVoltages_GetMinCellVoltage(void)
{
    uint16_t min_cell_voltage = cell_voltages.measured_cells[0];
    for (size_t current_cell = 1U;
         current_cell < cell_voltages.total_num_of_cells; current_cell++)
    {
        uint16_t current_cell_voltage =
            cell_voltages.measured_cells[current_cell];
        if (min_cell_voltage > current_cell_voltage)
        {
            min_cell_voltage = current_cell_voltage;
        }
    }

    return min_cell_voltage / 10000.0f;
}

float App_CellVoltages_GetMaxCellVoltage(void)
{
    float max_cell_voltage = cell_voltages.measured_cells[0];
    for (size_t current_cell = 1U;
         current_cell < cell_voltages.total_num_of_cells; current_cell++)
    {
        float current_cell_voltage = cell_voltages.measured_cells[current_cell];
        if (max_cell_voltage < current_cell_voltage)
        {
            max_cell_voltage = current_cell_voltage;
        }
    }

    return (float)max_cell_voltage / 10000.0f;
}

float App_CellVoltages_GetPackVoltage(void)
{
    return (float)App_SumOfArrayElements(
               cell_voltages.measured_cells, cell_voltages.total_num_of_cells) /
           10000.0f;
}

float App_CellVoltages_GetAverageCellVoltage(void)
{
    return App_CellVoltages_GetPackVoltage() /
           (float)cell_voltages.total_num_of_cells;
}

float App_CellVoltages_GetSegment0Voltage(void)
{
    return (float)App_SumOfArrayElements(
               &cell_voltages.measured_cells
                    [SEGMENT_0 * cell_voltages.num_of_cells_per_segment],
               cell_voltages.num_of_cells_per_segment) /
           10000.0f;
}

float App_CellVoltages_GetSegment1Voltage(void)
{
    return (float)App_SumOfArrayElements(
               &cell_voltages.measured_cells
                    [SEGMENT_1 * cell_voltages.num_of_cells_per_segment],
               cell_voltages.num_of_cells_per_segment) /
           10000.0f;
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
