#include "app_diagnosticsMode.h"
#include <app_accumulator.h>
#include "app_canTx.h"
#include "io_ltc6813.h"

static uint8_t segment_temps[NUM_SEGMENTS];

void (*cellVoltageSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
    { app_canTx_BMS_Seg0_Cell0_Voltage_set, app_canTx_BMS_Seg0_Cell1_Voltage_set, app_canTx_BMS_Seg0_Cell2_Voltage_set,
      app_canTx_BMS_Seg0_Cell3_Voltage_set, app_canTx_BMS_Seg0_Cell4_Voltage_set, app_canTx_BMS_Seg0_Cell5_Voltage_set,
      app_canTx_BMS_Seg0_Cell6_Voltage_set, app_canTx_BMS_Seg0_Cell7_Voltage_set, app_canTx_BMS_Seg0_Cell8_Voltage_set,
      app_canTx_BMS_Seg0_Cell9_Voltage_set, app_canTx_BMS_Seg0_Cell10_Voltage_set,
      app_canTx_BMS_Seg0_Cell11_Voltage_set, app_canTx_BMS_Seg0_Cell12_Voltage_set,
      app_canTx_BMS_Seg0_Cell13_Voltage_set },
    // app_canTx_BMS_Seg0_Cell14_Voltage_set,
    // app_canTx_BMS_Seg0_Cell15_Voltage_set },
    // { app_canTx_BMS_Seg1_Cell0_Voltage_set, app_canTx_BMS_Seg1_Cell1_Voltage_set,
    // app_canTx_BMS_Seg1_Cell2_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell3_Voltage_set, app_canTx_BMS_Seg1_Cell4_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell5_Voltage_set, app_canTx_BMS_Seg1_Cell6_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell7_Voltage_set, app_canTx_BMS_Seg1_Cell8_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell9_Voltage_set, app_canTx_BMS_Seg1_Cell10_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell11_Voltage_set, app_canTx_BMS_Seg1_Cell12_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell13_Voltage_set, app_canTx_BMS_Seg1_Cell14_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell15_Voltage_set },
    // { app_canTx_BMS_Seg2_Cell0_Voltage_set, app_canTx_BMS_Seg2_Cell1_Voltage_set,
    // app_canTx_BMS_Seg2_Cell2_Voltage_set,
    //   app_canTx_BMS_Seg2_Cell3_Voltage_set, app_canTx_BMS_Seg2_Cell4_Voltage_set,
    //   app_canTx_BMS_Seg2_Cell5_Voltage_set, app_canTx_BMS_Seg2_Cell6_Voltage_set,
    //   app_canTx_BMS_Seg2_Cell7_Voltage_set, app_canTx_BMS_Seg2_Cell8_Voltage_set,
    //   app_canTx_BMS_Seg2_Cell9_Voltage_set, app_canTx_BMS_Seg2_Cell10_Voltage_set,
    //   app_canTx_BMS_Seg2_Cell11_Voltage_set, app_canTx_BMS_Seg2_Cell12_Voltage_set,
    //   app_canTx_BMS_Seg2_Cell13_Voltage_set, app_canTx_BMS_Seg2_Cell14_Voltage_set,
    //   app_canTx_BMS_Seg2_Cell15_Voltage_set },
    // { app_canTx_BMS_Seg3_Cell0_Voltage_set, app_canTx_BMS_Seg3_Cell1_Voltage_set,
    // app_canTx_BMS_Seg3_Cell2_Voltage_set,
    //   app_canTx_BMS_Seg3_Cell3_Voltage_set, app_canTx_BMS_Seg3_Cell4_Voltage_set,
    //   app_canTx_BMS_Seg3_Cell5_Voltage_set, app_canTx_BMS_Seg3_Cell6_Voltage_set,
    //   app_canTx_BMS_Seg3_Cell7_Voltage_set, app_canTx_BMS_Seg3_Cell8_Voltage_set,
    //   app_canTx_BMS_Seg3_Cell9_Voltage_set, app_canTx_BMS_Seg3_Cell10_Voltage_set,
    //   app_canTx_BMS_Seg3_Cell11_Voltage_set, app_canTx_BMS_Seg3_Cell12_Voltage_set,
    //   app_canTx_BMS_Seg3_Cell13_Voltage_set, app_canTx_BMS_Seg3_Cell14_Voltage_set,
    //   app_canTx_BMS_Seg3_Cell15_Voltage_set }
};

void app_diagnosticsMode_calculateDiagnosticTemperatureStats(void)
{
    // for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    // {
    //     float sum_segment_temp = 0U;
    //     for (uint8_t curr_reg_group = 0U; curr_reg_group < AUX_REGISTER_GROUPS; curr_reg_group++)
    //     {
    //         for (uint8_t curr_thermistor = 0U; curr_thermistor < NUM_OF_READINGS_PER_REG_GROUP; curr_thermistor++)
    //         {
    //             if (IS_CELL_TEMP_READING(curr_reg_group, curr_thermistor))
    //             {
    //                 const float curr_cell_temp = io_ltc6813CellTemperatures_getSpecificCellTempDegC(
    //                     curr_segment, curr_reg_group, curr_thermistor);
    //                 sum_segment_temp += curr_cell_temp;
    //             }
    //         }
    //     }
    //
    //     segment_temps[curr_segment] = (uint8_t)(sum_segment_temp / NUM_OF_THERMISTORS_PER_SEGMENT);
    // }
}

void app_diagnosticsMode_broadcast(void)
{
    // Update all cell voltages
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            cellVoltageSetters[segment][cell](cell_voltages[segment][cell]);
        }
    }

    // Calculate and update all segment temperatures
    app_diagnosticsMode_calculateDiagnosticTemperatureStats();
    app_canTx_BMS_Seg0_Temp_set(segment_temps[0]);
    // app_canTx_BMS_Seg1_Temp_set(segment_temps[1]);
    // app_canTx_BMS_Seg2_Temp_set(segment_temps[2]);
    // app_canTx_BMS_Seg3_Temp_set(segment_temps[3]);
}
