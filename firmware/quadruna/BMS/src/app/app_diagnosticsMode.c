#include "app_diagnosticsMode.h"
#include "states/app_chargeState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "ltc6813/io_ltc6813CellVoltages.h"
#include "ltc6813/io_ltc6813CellTemps.h"

#include <string.h>
#include <float.h>

typedef struct
{
    float       cell_voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];
    float       cell_temps[ACCUMULATOR_NUM_SEGMENTS][NUM_OF_AUX_REGISTER_GROUPS][NUM_OF_READINGS_PER_REG_GROUP];
    float       segment_temps[ACCUMULATOR_NUM_SEGMENTS];
} BMSDiagnosticStats;

static BMSDiagnosticStats data;

static void app_diagnosticsMode_calculateDiagnosticVoltageStats(void)
{
    // Find the min and max voltages
    for (uint8_t segment = 0U; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            // Collect each cell voltage and store in temp
            const float cell_voltage                     = io_ltc6813CellVoltages_getCellVoltage(segment, cell);
            data.cell_voltages[segment][cell]            = cell_voltage;
        }
    }
}

static void app_diagnosticsMode_calculateDiagnosticTemperatureStats(void)
{
    for (uint8_t curr_segment = 0U; curr_segment < ACCUMULATOR_NUM_SEGMENTS; curr_segment++)
    {
        float sum_segment_temp = 0U;

        for (uint8_t curr_reg_group = 0U; curr_reg_group < NUM_OF_AUX_REGISTER_GROUPS; curr_reg_group++)
        {
            for (uint8_t curr_thermistor = 0U; curr_thermistor < NUM_OF_READINGS_PER_REG_GROUP; curr_thermistor++)
            {
                if (IS_CELL_TEMP_READING(curr_reg_group, curr_thermistor))
                {
                    const float curr_cell_temp = io_ltc6813CellTemperatures_getSpecificCellTempDegC(curr_segment, curr_reg_group, curr_thermistor);
                    data.cell_temps[curr_segment][curr_reg_group][curr_thermistor] = curr_cell_temp;
                    sum_segment_temp += curr_cell_temp;
                }
            }
        }

        data.segment_temps[curr_segment] = sum_segment_temp / NUM_OF_THERMISTORS_PER_SEGMENT;
    }
}

void app_diagnosticsMode_broadcast(void)
{
    app_diagnosticsMode_calculateDiagnosticVoltageStats();
    app_diagnosticsMode_calculateDiagnosticTemperatureStats();

    app_canTx_BMS_Seg0_Cell0_Voltage_set(data.cell_voltages[0][0]);
    app_canTx_BMS_Seg0_Cell1_Voltage_set(data.cell_voltages[0][1]);
    app_canTx_BMS_Seg0_Cell2_Voltage_set(data.cell_voltages[0][2]);
    app_canTx_BMS_Seg0_Cell3_Voltage_set(data.cell_voltages[0][3]);
    app_canTx_BMS_Seg0_Cell4_Voltage_set(data.cell_voltages[0][4]);
    app_canTx_BMS_Seg0_Cell5_Voltage_set(data.cell_voltages[0][5]);
    app_canTx_BMS_Seg0_Cell6_Voltage_set(data.cell_voltages[0][6]);
    app_canTx_BMS_Seg0_Cell7_Voltage_set(data.cell_voltages[0][7]);
    app_canTx_BMS_Seg0_Cell8_Voltage_set(data.cell_voltages[0][8]);
    app_canTx_BMS_Seg0_Cell9_Voltage_set(data.cell_voltages[0][9]);
    app_canTx_BMS_Seg0_Cell10_Voltage_set(data.cell_voltages[0][10]);
    app_canTx_BMS_Seg0_Cell11_Voltage_set(data.cell_voltages[0][11]);
    app_canTx_BMS_Seg0_Cell12_Voltage_set(data.cell_voltages[0][12]);
    app_canTx_BMS_Seg0_Cell13_Voltage_set(data.cell_voltages[0][13]);
    app_canTx_BMS_Seg0_Cell14_Voltage_set(data.cell_voltages[0][14]);
    app_canTx_BMS_Seg0_Cell15_Voltage_set(data.cell_voltages[0][15]);

    app_canTx_BMS_Seg0_Thermistor0_Temp_set(data.cell_temps[0][0][0]);
    app_canTx_BMS_Seg0_Thermistor1_Temp_set(data.cell_temps[0][0][1]);
    app_canTx_BMS_Seg0_Thermistor2_Temp_set(data.cell_temps[0][0][2]);
    app_canTx_BMS_Seg0_Thermistor3_Temp_set(data.cell_temps[0][1][0]);
    app_canTx_BMS_Seg0_Thermistor4_Temp_set(data.cell_temps[0][1][1]);
    app_canTx_BMS_Seg0_Thermistor5_Temp_set(data.cell_temps[0][2][0]);
    app_canTx_BMS_Seg0_Thermistor6_Temp_set(data.cell_temps[0][2][1]);
    app_canTx_BMS_Seg0_Thermistor7_Temp_set(data.cell_temps[0][2][2]);

    app_canTx_BMS_Seg0AvgTemp_set(data.segment_temps[0]);
}