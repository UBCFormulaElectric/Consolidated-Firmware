#include "App_AccumulatorTemperatures.h"
#include "configs/App_AccumulatorConfigs.h"

struct AccumulatorTemperatures
{
    float *internal_die_temperatures;

    size_t num_of_cell_temperatures_per_segment;
    size_t total_num_of_cell_temperatures;
    float *cell_temperatures;
};

static struct AccumulatorTemperatures accumulator_temperatures;

void App_AccumulatorTemperatures_Init(
    float *(*get_die_temperatures)(void),
    float *(*get_cell_temperatures)(size_t *))
{
    accumulator_temperatures.internal_die_temperatures = get_die_temperatures();

    size_t cell_temperatures_column_length;
    accumulator_temperatures.cell_temperatures =
        get_cell_temperatures(&cell_temperatures_column_length);
    accumulator_temperatures.num_of_cell_temperatures_per_segment =
        cell_temperatures_column_length;
    accumulator_temperatures.total_num_of_cell_temperatures =
        cell_temperatures_column_length * NUM_OF_CELL_MONITOR_CHIPS;
}

float App_AccumulatorTemperatures_GetSegment0DieTemperatureDegC(void)
{
    return accumulator_temperatures
        .internal_die_temperatures[CELL_MONITOR_CHIP_0];
}

float App_AccumulatorTemperatures_GetSegment1DieTemperatureDegC(void)
{
    return accumulator_temperatures
        .internal_die_temperatures[CELL_MONITOR_CHIP_1];
}

float App_AccumulatorTemperatures_GetSegment2DieTemperatureDegC(void)
{
    return 0.0f;
}

float App_AccumulatorTemperatures_GetSegment3DieTemperatureDegC(void)
{
    return 0.0f;
}

float App_AccumulatorTemperatures_GetSegment4DieTemperatureDegC(void)
{
    return 0.0f;
}

float App_AccumulatorTemperatures_GetSegment5DieTemperatureDegC(void)
{
    return 0.0f;
}

float App_AccumulatorTemperatures_GetMinDieTemperatureDegC(void)
{
    float min_cell_voltage =
        accumulator_temperatures.internal_die_temperatures[0];
    for (enum CellMonitorChip current_chip = CELL_MONITOR_CHIP_1;
         current_chip < NUM_OF_CELL_MONITOR_CHIPS; current_chip++)
    {
        float current_cell_voltage =
            accumulator_temperatures.internal_die_temperatures[current_chip];
        if (min_cell_voltage > current_cell_voltage)
        {
            min_cell_voltage = current_cell_voltage;
        }
    }

    return min_cell_voltage;
}

float App_AccumulatorTemperatures_GetMaxDieTemperatureDegC(void)
{
    float max_cell_voltage =
        accumulator_temperatures.internal_die_temperatures[0];
    for (enum CellMonitorChip current_chip = CELL_MONITOR_CHIP_1;
         current_chip < NUM_OF_CELL_MONITOR_CHIPS; current_chip++)
    {
        float current_cell_voltage =
            accumulator_temperatures.internal_die_temperatures[current_chip];
        if (max_cell_voltage < current_cell_voltage)
        {
            max_cell_voltage = current_cell_voltage;
        }
    }

    return max_cell_voltage;
}

float App_AccumulatorTemperatures_GetMinCellTemperatureDegC(void)
{
    float max_cell_temperature = accumulator_temperatures.cell_temperatures[0];
    for (size_t current_temperature = 1U;
         current_temperature <
         accumulator_temperatures.total_num_of_cell_temperatures;
         current_temperature++)
    {
        float current_cell_voltage =
            accumulator_temperatures.cell_temperatures[current_temperature];
        if (max_cell_temperature > current_cell_voltage)
        {
            max_cell_temperature = current_cell_voltage;
        }
    }

    return max_cell_temperature;
}

float App_AccumulatorTemperatures_GetMaxCellTemperatureDegC(void)
{
    float max_cell_temperature = accumulator_temperatures.cell_temperatures[0];
    for (size_t current_temperature = 1U;
         current_temperature <
         accumulator_temperatures.total_num_of_cell_temperatures;
         current_temperature++)
    {
        float current_cell_voltage =
            accumulator_temperatures.cell_temperatures[current_temperature];
        if (max_cell_temperature < current_cell_voltage)
        {
            max_cell_temperature = current_cell_voltage;
        }
    }

    return max_cell_temperature;
}

float App_AccumulatorTemperatures_GetAverageCellTemperature(void)
{
    float sum_of_cell_temperatures = 0U;
    for (size_t current_cell_temperature = 1U;
         current_cell_temperature <
         accumulator_temperatures.total_num_of_cell_temperatures;
         current_cell_temperature++)
    {
        sum_of_cell_temperatures +=
            accumulator_temperatures
                .cell_temperatures[current_cell_temperature];
    }

    return sum_of_cell_temperatures /
           (float)accumulator_temperatures.total_num_of_cell_temperatures;
}
