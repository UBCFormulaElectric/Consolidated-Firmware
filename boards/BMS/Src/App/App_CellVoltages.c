#include <stdint.h>
#include <math.h>
#include "App_CellVoltages.h"

struct CellVoltages
{
    uint16_t *measured_cells;
    uint32_t  num_of_cells_per_segment;
    uint32_t  total_num_of_cells;

    // An array containing the average cell voltage for each accumulator
    // segment, measured in V.
    float average_segment[NUM_OF_SEGMENTS];

    // The average cell voltage value read back from the cell monitoring chips,
    // measured in V.
    float average_cell;

    // The voltage of the battery pack, measured in V.
    float pack;

    // The maximum cell voltage value, measured in V.
    float max_cell;

    // The minimum cell voltage value, measured in V.
    float min_cell;
};

static struct CellVoltages voltages;

/**
 * Calculate the average value of the given array.
 * @param array The given array to calculate the average value for.
 * @param array_size The size of the given array.
 * @return The average value of the given array.
 */
static float App_CalculateAverage(float array[], uint32_t array_size);

static float App_CalculateAverage(float array[], uint32_t array_size)
{
    // The number of samples used to compute the average cannot exceed the size
    // of the array.
    float calculated_average = 0.0f;
    for (size_t i = 0U; i < array_size; i++)
    {
        calculated_average += array[i];
    }

    return calculated_average / (float)array_size;
}

void App_CellVoltages_Init(
    uint16_t *(*get_cell_voltages)(void),
    uint32_t num_of_cells_per_segment)
{
    // Get the pointer to the cell voltages read back from the cell monitoring
    // daisy chain.
    voltages.measured_cells           = get_cell_voltages();
    voltages.num_of_cells_per_segment = num_of_cells_per_segment;
    voltages.total_num_of_cells = num_of_cells_per_segment * NUM_OF_SEGMENTS;

    // Initialize average cell voltage values.
    for (size_t i = 0U; i < NUM_OF_SEGMENTS; i++)
    {
        voltages.average_segment[i] = NAN;
    }
    voltages.average_cell = NAN;
    voltages.pack         = NAN;
    voltages.min_cell     = NAN;
    voltages.max_cell     = NAN;
}

void App_CellVoltages_Tick(void)
{
    // Get the pointer to the 2D array containing the cell voltages for each
    // cell monitoring IC.
    float *cell_voltages = (float *)voltages.measured_cells;

    // Get the accumulator's minimum cell voltage.
    float min_cell_voltage = *cell_voltages;
    for (size_t current_cell = 1U; current_cell < voltages.total_num_of_cells;
         current_cell++)
    {
        float current_cell_voltage = cell_voltages[current_cell];
        if (min_cell_voltage > current_cell_voltage)
        {
            min_cell_voltage = current_cell_voltage;
        }
    }
    voltages.min_cell = (float)min_cell_voltage / 10000.0f;

    // Get the accumulator's maximum cell voltage.
    float max_cell_voltage = *cell_voltages;
    for (size_t current_cell = 1U; current_cell < voltages.total_num_of_cells;
         current_cell++)
    {
        float current_cell_voltage = cell_voltages[current_cell];
        if (max_cell_voltage < current_cell_voltage)
        {
            max_cell_voltage = current_cell_voltage;
        }
    }
    voltages.max_cell = (float)max_cell_voltage / 10000.0f;

    // Calculate the average segment voltages.
    for (size_t current_segment = SEGMENT_0; current_segment < NUM_OF_SEGMENTS;
         current_segment++)
    {
        // Get the address for the current segment.
        float *segment_cell_voltages =
            (float
                 *)(&voltages.measured_cells
                         [current_segment * voltages.num_of_cells_per_segment]);

        // Calculate the average segment voltage for the given segment.
        voltages.average_segment[current_segment] =
            App_CalculateAverage(
                segment_cell_voltages, voltages.num_of_cells_per_segment) /
            10000.0f;
    }

    // Calculate the average cell voltage. Use the previously calculated segment
    // voltages to compute the average cell voltage.
    voltages.average_cell =
        App_CalculateAverage(voltages.average_segment, NUM_OF_SEGMENTS);

    // Calculate the pack voltage of the accumulator using the previously
    // calculated value for the average cell.
    voltages.pack = voltages.average_cell * (float)voltages.total_num_of_cells;
}

float App_CellVoltages_GetSegment0Average(void)
{
    return voltages.average_segment[SEGMENT_0];
}

float App_CellVoltages_GetSegment1Average(void)
{
    return voltages.average_segment[SEGMENT_1];
}

float App_CellVoltages_GetSegment2Average(void)
{
    return voltages.average_segment[SEGMENT_2];
}

float App_CellVoltages_GetSegment3Average(void)
{
    return voltages.average_segment[SEGMENT_3];
}

float App_CellVoltages_GetSegment4Average(void)
{
    return voltages.average_segment[SEGMENT_4];
}

float App_CellVoltages_GetSegment5Average(void)
{
    return voltages.average_segment[SEGMENT_5];
}

float App_CellVoltages_GetPack(void)
{
    return voltages.pack;
}

float App_CellVoltages_GetAverageCell(void)
{
    return voltages.average_cell;
}

float App_CellVoltages_GetMinCell(void)
{
    return voltages.min_cell;
}

float App_CellVoltages_GetMaxCell(void)
{
    return voltages.max_cell;
}
