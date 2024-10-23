#include "app_diagnosticsMode.h"
#include "states/app_chargeState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "ltc6813/io_ltc6813CellVoltages.h"
#include "ltc6813/io_ltc6813CellTemps.h"

#include <string.h>
#include <float.h>

typedef struct
{
    float       cell_voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];
    float       cell_temps[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];
} BMSDiagnosticStats;

static BMSDiagnosticStats data;

static void app_diagnosticsMode_calculateDiagnosticVoltageStats(void)
{
    float temp_diagnostic_voltage_stats[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT] = { { 0U } };
        // Find the min and max voltages
    for (uint8_t segment = 0U; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            // Collect each cell voltage and store in temp
            const float cell_voltage = io_ltc6813CellVoltages_getCellVoltage(segment, cell);
            temp_diagnostic_voltage_stats[segment][cell] = cell_voltage;
        }
    }
    data.cell_voltages = temp_diagnostic_voltage_stats;
}

static void app_diagnosticsMode_calculateDiagnosticTemperatureStats(void)
{
    float temp_diagnostic_temp_stats[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT] = { { 0U } };
    
}

void app_diagnosticsMode_broadcast(void) {
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
}