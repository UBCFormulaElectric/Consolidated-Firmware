#include "App_AccumulatorTemperatures.h"
#include "configs/App_AccumulatorConfigs.h"

struct AccumulatorTemperatures
{
    size_t total_num_of_cell_temperatures;
    float *cell_temperatures;
};

static struct AccumulatorTemperatures accumulator_temperatures;

void App_AccumulatorTemperatures_Init(float *(*get_cell_temperatures)(size_t *))
{
    size_t cell_temperatures_column_length;
    accumulator_temperatures.cell_temperatures =
        get_cell_temperatures(&cell_temperatures_column_length);
    accumulator_temperatures.total_num_of_cell_temperatures =
        cell_temperatures_column_length * NUM_OF_CELL_MONITOR_CHIPS;
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
