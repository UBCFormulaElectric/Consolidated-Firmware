#include "app_diagnosticsMode.h"
#include "states/app_chargeState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "ltc6813/io_ltc6813CellVoltages.h"
#include "ltc6813/io_ltc6813CellTemps.h"

#include <string.h>
#include <float.h>

static struct
{
    float   cell_voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];
    uint8_t segment_temps[ACCUMULATOR_NUM_SEGMENTS];
} data;

void app_diagnosticsMode_calculateDiagnosticVoltageStats(uint8_t seg_requested)
{
    if (seg_requested == SEG_ALL)
    {
        for (uint8_t segment = 0U; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
        {
            for (uint8_t cell = 0U; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
            {
                // Collect each cell voltage and store in temp
                const float cell_voltage          = io_ltc6813CellVoltages_getCellVoltage(segment, cell);
                data.cell_voltages[segment][cell] = cell_voltage;
            }
        }
    }
    else if (seg_requested != NONE)
    {
        for (uint8_t cell = 0U; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            // Collect each cell voltage and store in temp
            // turn seg_requested into 0 index
            uint8_t     seg_num               = seg_requested - 1;
            const float cell_voltage          = io_ltc6813CellVoltages_getCellVoltage(seg_num, cell);
            data.cell_voltages[seg_num][cell] = cell_voltage;
        }
    }
}

void app_diagnosticsMode_calculateDiagnosticTemperatureStats(void)
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
                    const float curr_cell_temp = io_ltc6813CellTemperatures_getSpecificCellTempDegC(
                        curr_segment, curr_reg_group, curr_thermistor);
                    sum_segment_temp += curr_cell_temp;
                }
            }
        }

        data.segment_temps[curr_segment] = (uint8_t)(sum_segment_temp / NUM_OF_THERMISTORS_PER_SEGMENT);
    }
}

void app_diagnosticsMode_broadcast(void)
{
    uint8_t cellVoltageDiagnosticSegment = app_canRx_Debug_SegmentCellVoltageRequest_get();
    if (cellVoltageDiagnosticSegment != NONE)
    {
        app_diagnosticsMode_calculateDiagnosticVoltageStats(cellVoltageDiagnosticSegment);
    }

    switch (cellVoltageDiagnosticSegment)
    {
        case SEG_0:
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
            break;
        case SEG_1:
            app_canTx_BMS_Seg1_Cell0_Voltage_set(data.cell_voltages[1][0]);
            app_canTx_BMS_Seg1_Cell1_Voltage_set(data.cell_voltages[1][1]);
            app_canTx_BMS_Seg1_Cell2_Voltage_set(data.cell_voltages[1][2]);
            app_canTx_BMS_Seg1_Cell3_Voltage_set(data.cell_voltages[1][3]);
            app_canTx_BMS_Seg1_Cell4_Voltage_set(data.cell_voltages[1][4]);
            app_canTx_BMS_Seg1_Cell5_Voltage_set(data.cell_voltages[1][5]);
            app_canTx_BMS_Seg1_Cell6_Voltage_set(data.cell_voltages[1][6]);
            app_canTx_BMS_Seg1_Cell7_Voltage_set(data.cell_voltages[1][7]);
            app_canTx_BMS_Seg1_Cell8_Voltage_set(data.cell_voltages[1][8]);
            app_canTx_BMS_Seg1_Cell9_Voltage_set(data.cell_voltages[1][9]);
            app_canTx_BMS_Seg1_Cell10_Voltage_set(data.cell_voltages[1][10]);
            app_canTx_BMS_Seg1_Cell11_Voltage_set(data.cell_voltages[1][11]);
            app_canTx_BMS_Seg1_Cell12_Voltage_set(data.cell_voltages[1][12]);
            app_canTx_BMS_Seg1_Cell13_Voltage_set(data.cell_voltages[1][13]);
            app_canTx_BMS_Seg1_Cell14_Voltage_set(data.cell_voltages[1][14]);
            app_canTx_BMS_Seg1_Cell15_Voltage_set(data.cell_voltages[1][15]);
            break;
        case SEG_2:
            app_canTx_BMS_Seg2_Cell0_Voltage_set(data.cell_voltages[2][0]);
            app_canTx_BMS_Seg2_Cell1_Voltage_set(data.cell_voltages[2][1]);
            app_canTx_BMS_Seg2_Cell2_Voltage_set(data.cell_voltages[2][2]);
            app_canTx_BMS_Seg2_Cell3_Voltage_set(data.cell_voltages[2][3]);
            app_canTx_BMS_Seg2_Cell4_Voltage_set(data.cell_voltages[2][4]);
            app_canTx_BMS_Seg2_Cell5_Voltage_set(data.cell_voltages[2][5]);
            app_canTx_BMS_Seg2_Cell6_Voltage_set(data.cell_voltages[2][6]);
            app_canTx_BMS_Seg2_Cell7_Voltage_set(data.cell_voltages[2][7]);
            app_canTx_BMS_Seg2_Cell8_Voltage_set(data.cell_voltages[2][8]);
            app_canTx_BMS_Seg2_Cell9_Voltage_set(data.cell_voltages[2][9]);
            app_canTx_BMS_Seg2_Cell10_Voltage_set(data.cell_voltages[2][10]);
            app_canTx_BMS_Seg2_Cell11_Voltage_set(data.cell_voltages[2][11]);
            app_canTx_BMS_Seg2_Cell12_Voltage_set(data.cell_voltages[2][12]);
            app_canTx_BMS_Seg2_Cell13_Voltage_set(data.cell_voltages[2][13]);
            app_canTx_BMS_Seg2_Cell14_Voltage_set(data.cell_voltages[2][14]);
            app_canTx_BMS_Seg2_Cell15_Voltage_set(data.cell_voltages[2][15]);
            break;
        case SEG_3:
            app_canTx_BMS_Seg3_Cell0_Voltage_set(data.cell_voltages[3][0]);
            app_canTx_BMS_Seg3_Cell1_Voltage_set(data.cell_voltages[3][1]);
            app_canTx_BMS_Seg3_Cell2_Voltage_set(data.cell_voltages[3][2]);
            app_canTx_BMS_Seg3_Cell3_Voltage_set(data.cell_voltages[3][3]);
            app_canTx_BMS_Seg3_Cell4_Voltage_set(data.cell_voltages[3][4]);
            app_canTx_BMS_Seg3_Cell5_Voltage_set(data.cell_voltages[3][5]);
            app_canTx_BMS_Seg3_Cell6_Voltage_set(data.cell_voltages[3][6]);
            app_canTx_BMS_Seg3_Cell7_Voltage_set(data.cell_voltages[3][7]);
            app_canTx_BMS_Seg3_Cell8_Voltage_set(data.cell_voltages[3][8]);
            app_canTx_BMS_Seg3_Cell9_Voltage_set(data.cell_voltages[3][9]);
            app_canTx_BMS_Seg3_Cell10_Voltage_set(data.cell_voltages[3][10]);
            app_canTx_BMS_Seg3_Cell11_Voltage_set(data.cell_voltages[3][11]);
            app_canTx_BMS_Seg3_Cell12_Voltage_set(data.cell_voltages[3][12]);
            app_canTx_BMS_Seg3_Cell13_Voltage_set(data.cell_voltages[3][13]);
            app_canTx_BMS_Seg3_Cell14_Voltage_set(data.cell_voltages[3][14]);
            app_canTx_BMS_Seg3_Cell15_Voltage_set(data.cell_voltages[3][15]);
            break;
        case SEG_4:
            app_canTx_BMS_Seg4_Cell0_Voltage_set(data.cell_voltages[4][0]);
            app_canTx_BMS_Seg4_Cell1_Voltage_set(data.cell_voltages[4][1]);
            app_canTx_BMS_Seg4_Cell2_Voltage_set(data.cell_voltages[4][2]);
            app_canTx_BMS_Seg4_Cell3_Voltage_set(data.cell_voltages[4][3]);
            app_canTx_BMS_Seg4_Cell4_Voltage_set(data.cell_voltages[4][4]);
            app_canTx_BMS_Seg4_Cell5_Voltage_set(data.cell_voltages[4][5]);
            app_canTx_BMS_Seg4_Cell6_Voltage_set(data.cell_voltages[4][6]);
            app_canTx_BMS_Seg4_Cell7_Voltage_set(data.cell_voltages[4][7]);
            app_canTx_BMS_Seg4_Cell8_Voltage_set(data.cell_voltages[4][8]);
            app_canTx_BMS_Seg4_Cell9_Voltage_set(data.cell_voltages[4][9]);
            app_canTx_BMS_Seg4_Cell10_Voltage_set(data.cell_voltages[4][10]);
            app_canTx_BMS_Seg4_Cell11_Voltage_set(data.cell_voltages[4][11]);
            app_canTx_BMS_Seg4_Cell12_Voltage_set(data.cell_voltages[4][12]);
            app_canTx_BMS_Seg4_Cell13_Voltage_set(data.cell_voltages[4][13]);
            app_canTx_BMS_Seg4_Cell14_Voltage_set(data.cell_voltages[4][14]);
            app_canTx_BMS_Seg4_Cell15_Voltage_set(data.cell_voltages[4][15]);
            break;
        case SEG_ALL:
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

            app_canTx_BMS_Seg1_Cell0_Voltage_set(data.cell_voltages[1][0]);
            app_canTx_BMS_Seg1_Cell1_Voltage_set(data.cell_voltages[1][1]);
            app_canTx_BMS_Seg1_Cell2_Voltage_set(data.cell_voltages[1][2]);
            app_canTx_BMS_Seg1_Cell3_Voltage_set(data.cell_voltages[1][3]);
            app_canTx_BMS_Seg1_Cell4_Voltage_set(data.cell_voltages[1][4]);
            app_canTx_BMS_Seg1_Cell5_Voltage_set(data.cell_voltages[1][5]);
            app_canTx_BMS_Seg1_Cell6_Voltage_set(data.cell_voltages[1][6]);
            app_canTx_BMS_Seg1_Cell7_Voltage_set(data.cell_voltages[1][7]);
            app_canTx_BMS_Seg1_Cell8_Voltage_set(data.cell_voltages[1][8]);
            app_canTx_BMS_Seg1_Cell9_Voltage_set(data.cell_voltages[1][9]);
            app_canTx_BMS_Seg1_Cell10_Voltage_set(data.cell_voltages[1][10]);
            app_canTx_BMS_Seg1_Cell11_Voltage_set(data.cell_voltages[1][11]);
            app_canTx_BMS_Seg1_Cell12_Voltage_set(data.cell_voltages[1][12]);
            app_canTx_BMS_Seg1_Cell13_Voltage_set(data.cell_voltages[1][13]);
            app_canTx_BMS_Seg1_Cell14_Voltage_set(data.cell_voltages[1][14]);
            app_canTx_BMS_Seg1_Cell15_Voltage_set(data.cell_voltages[1][15]);

            app_canTx_BMS_Seg2_Cell0_Voltage_set(data.cell_voltages[2][0]);
            app_canTx_BMS_Seg2_Cell1_Voltage_set(data.cell_voltages[2][1]);
            app_canTx_BMS_Seg2_Cell2_Voltage_set(data.cell_voltages[2][2]);
            app_canTx_BMS_Seg2_Cell3_Voltage_set(data.cell_voltages[2][3]);
            app_canTx_BMS_Seg2_Cell4_Voltage_set(data.cell_voltages[2][4]);
            app_canTx_BMS_Seg2_Cell5_Voltage_set(data.cell_voltages[2][5]);
            app_canTx_BMS_Seg2_Cell6_Voltage_set(data.cell_voltages[2][6]);
            app_canTx_BMS_Seg2_Cell7_Voltage_set(data.cell_voltages[2][7]);
            app_canTx_BMS_Seg2_Cell8_Voltage_set(data.cell_voltages[2][8]);
            app_canTx_BMS_Seg2_Cell9_Voltage_set(data.cell_voltages[2][9]);
            app_canTx_BMS_Seg2_Cell10_Voltage_set(data.cell_voltages[2][10]);
            app_canTx_BMS_Seg2_Cell11_Voltage_set(data.cell_voltages[2][11]);
            app_canTx_BMS_Seg2_Cell12_Voltage_set(data.cell_voltages[2][12]);
            app_canTx_BMS_Seg2_Cell13_Voltage_set(data.cell_voltages[2][13]);
            app_canTx_BMS_Seg2_Cell14_Voltage_set(data.cell_voltages[2][14]);
            app_canTx_BMS_Seg2_Cell15_Voltage_set(data.cell_voltages[2][15]);

            app_canTx_BMS_Seg3_Cell0_Voltage_set(data.cell_voltages[3][0]);
            app_canTx_BMS_Seg3_Cell1_Voltage_set(data.cell_voltages[3][1]);
            app_canTx_BMS_Seg3_Cell2_Voltage_set(data.cell_voltages[3][2]);
            app_canTx_BMS_Seg3_Cell3_Voltage_set(data.cell_voltages[3][3]);
            app_canTx_BMS_Seg3_Cell4_Voltage_set(data.cell_voltages[3][4]);
            app_canTx_BMS_Seg3_Cell5_Voltage_set(data.cell_voltages[3][5]);
            app_canTx_BMS_Seg3_Cell6_Voltage_set(data.cell_voltages[3][6]);
            app_canTx_BMS_Seg3_Cell7_Voltage_set(data.cell_voltages[3][7]);
            app_canTx_BMS_Seg3_Cell8_Voltage_set(data.cell_voltages[3][8]);
            app_canTx_BMS_Seg3_Cell9_Voltage_set(data.cell_voltages[3][9]);
            app_canTx_BMS_Seg3_Cell10_Voltage_set(data.cell_voltages[3][10]);
            app_canTx_BMS_Seg3_Cell11_Voltage_set(data.cell_voltages[3][11]);
            app_canTx_BMS_Seg3_Cell12_Voltage_set(data.cell_voltages[3][12]);
            app_canTx_BMS_Seg3_Cell13_Voltage_set(data.cell_voltages[3][13]);
            app_canTx_BMS_Seg3_Cell14_Voltage_set(data.cell_voltages[3][14]);
            app_canTx_BMS_Seg3_Cell15_Voltage_set(data.cell_voltages[3][15]);

            app_canTx_BMS_Seg4_Cell0_Voltage_set(data.cell_voltages[4][0]);
            app_canTx_BMS_Seg4_Cell1_Voltage_set(data.cell_voltages[4][1]);
            app_canTx_BMS_Seg4_Cell2_Voltage_set(data.cell_voltages[4][2]);
            app_canTx_BMS_Seg4_Cell3_Voltage_set(data.cell_voltages[4][3]);
            app_canTx_BMS_Seg4_Cell4_Voltage_set(data.cell_voltages[4][4]);
            app_canTx_BMS_Seg4_Cell5_Voltage_set(data.cell_voltages[4][5]);
            app_canTx_BMS_Seg4_Cell6_Voltage_set(data.cell_voltages[4][6]);
            app_canTx_BMS_Seg4_Cell7_Voltage_set(data.cell_voltages[4][7]);
            app_canTx_BMS_Seg4_Cell8_Voltage_set(data.cell_voltages[4][8]);
            app_canTx_BMS_Seg4_Cell9_Voltage_set(data.cell_voltages[4][9]);
            app_canTx_BMS_Seg4_Cell10_Voltage_set(data.cell_voltages[4][10]);
            app_canTx_BMS_Seg4_Cell11_Voltage_set(data.cell_voltages[4][11]);
            app_canTx_BMS_Seg4_Cell12_Voltage_set(data.cell_voltages[4][12]);
            app_canTx_BMS_Seg4_Cell13_Voltage_set(data.cell_voltages[4][13]);
            app_canTx_BMS_Seg4_Cell14_Voltage_set(data.cell_voltages[4][14]);
            app_canTx_BMS_Seg4_Cell15_Voltage_set(data.cell_voltages[4][15]);
            break;
        case NONE:
            break;
        default:
            break;
    }

    if (app_canRx_Debug_SegmentTemperaturesRequest_get())
    {
        app_diagnosticsMode_calculateDiagnosticTemperatureStats();

        app_canTx_BMS_Seg0_Temp_set(data.segment_temps[0]);
        app_canTx_BMS_Seg1_Temp_set(data.segment_temps[1]);
        app_canTx_BMS_Seg2_Temp_set(data.segment_temps[2]);
        app_canTx_BMS_Seg3_Temp_set(data.segment_temps[3]);
        app_canTx_BMS_Seg4_Temp_set(data.segment_temps[4]);
    }
}
