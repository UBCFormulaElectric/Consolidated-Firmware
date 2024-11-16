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
    uint8_t segment_temps[ACCUMULATOR_NUM_SEGMENTS];
} data;

void (*cellVoltageSetters[])(float) = {
    app_canTx_BMS_Seg0_Cell0_Voltage_set, app_canTx_BMS_Seg0_Cell1_Voltage_set, app_canTx_BMS_Seg0_Cell2_Voltage_set,
    app_canTx_BMS_Seg0_Cell3_Voltage_set, app_canTx_BMS_Seg0_Cell4_Voltage_set, app_canTx_BMS_Seg0_Cell5_Voltage_set,
    app_canTx_BMS_Seg0_Cell6_Voltage_set, app_canTx_BMS_Seg0_Cell7_Voltage_set, app_canTx_BMS_Seg0_Cell8_Voltage_set,
    app_canTx_BMS_Seg0_Cell9_Voltage_set, app_canTx_BMS_Seg0_Cell10_Voltage_set, app_canTx_BMS_Seg0_Cell11_Voltage_set,
    app_canTx_BMS_Seg0_Cell12_Voltage_set, app_canTx_BMS_Seg0_Cell13_Voltage_set, app_canTx_BMS_Seg0_Cell14_Voltage_set,
    app_canTx_BMS_Seg0_Cell15_Voltage_set,

    app_canTx_BMS_Seg1_Cell0_Voltage_set, app_canTx_BMS_Seg1_Cell1_Voltage_set, app_canTx_BMS_Seg1_Cell2_Voltage_set,
    app_canTx_BMS_Seg1_Cell3_Voltage_set, app_canTx_BMS_Seg1_Cell4_Voltage_set, app_canTx_BMS_Seg1_Cell5_Voltage_set,
    app_canTx_BMS_Seg1_Cell6_Voltage_set, app_canTx_BMS_Seg1_Cell7_Voltage_set, app_canTx_BMS_Seg1_Cell8_Voltage_set,
    app_canTx_BMS_Seg1_Cell9_Voltage_set, app_canTx_BMS_Seg1_Cell10_Voltage_set, app_canTx_BMS_Seg1_Cell11_Voltage_set,
    app_canTx_BMS_Seg1_Cell12_Voltage_set, app_canTx_BMS_Seg1_Cell13_Voltage_set, app_canTx_BMS_Seg1_Cell14_Voltage_set,
    app_canTx_BMS_Seg1_Cell15_Voltage_set,

    app_canTx_BMS_Seg2_Cell0_Voltage_set, app_canTx_BMS_Seg2_Cell1_Voltage_set, app_canTx_BMS_Seg2_Cell2_Voltage_set,
    app_canTx_BMS_Seg2_Cell3_Voltage_set, app_canTx_BMS_Seg2_Cell4_Voltage_set, app_canTx_BMS_Seg2_Cell5_Voltage_set,
    app_canTx_BMS_Seg2_Cell6_Voltage_set, app_canTx_BMS_Seg2_Cell7_Voltage_set, app_canTx_BMS_Seg2_Cell8_Voltage_set,
    app_canTx_BMS_Seg2_Cell9_Voltage_set, app_canTx_BMS_Seg2_Cell10_Voltage_set, app_canTx_BMS_Seg2_Cell11_Voltage_set,
    app_canTx_BMS_Seg2_Cell12_Voltage_set, app_canTx_BMS_Seg2_Cell13_Voltage_set, app_canTx_BMS_Seg2_Cell14_Voltage_set,
    app_canTx_BMS_Seg2_Cell15_Voltage_set,

    app_canTx_BMS_Seg3_Cell0_Voltage_set, app_canTx_BMS_Seg3_Cell1_Voltage_set, app_canTx_BMS_Seg3_Cell2_Voltage_set,
    app_canTx_BMS_Seg3_Cell3_Voltage_set, app_canTx_BMS_Seg3_Cell4_Voltage_set, app_canTx_BMS_Seg3_Cell5_Voltage_set,
    app_canTx_BMS_Seg3_Cell6_Voltage_set, app_canTx_BMS_Seg3_Cell7_Voltage_set, app_canTx_BMS_Seg3_Cell8_Voltage_set,
    app_canTx_BMS_Seg3_Cell9_Voltage_set, app_canTx_BMS_Seg3_Cell10_Voltage_set, app_canTx_BMS_Seg3_Cell11_Voltage_set,
    app_canTx_BMS_Seg3_Cell12_Voltage_set, app_canTx_BMS_Seg3_Cell13_Voltage_set, app_canTx_BMS_Seg3_Cell14_Voltage_set,
    app_canTx_BMS_Seg3_Cell15_Voltage_set,

    app_canTx_BMS_Seg4_Cell0_Voltage_set, app_canTx_BMS_Seg4_Cell1_Voltage_set, app_canTx_BMS_Seg4_Cell2_Voltage_set,
    app_canTx_BMS_Seg4_Cell3_Voltage_set, app_canTx_BMS_Seg4_Cell4_Voltage_set, app_canTx_BMS_Seg4_Cell5_Voltage_set,
    app_canTx_BMS_Seg4_Cell6_Voltage_set, app_canTx_BMS_Seg4_Cell7_Voltage_set, app_canTx_BMS_Seg4_Cell8_Voltage_set,
    app_canTx_BMS_Seg4_Cell9_Voltage_set, app_canTx_BMS_Seg4_Cell10_Voltage_set, app_canTx_BMS_Seg4_Cell11_Voltage_set,
    app_canTx_BMS_Seg4_Cell12_Voltage_set, app_canTx_BMS_Seg4_Cell13_Voltage_set, app_canTx_BMS_Seg4_Cell14_Voltage_set,
    app_canTx_BMS_Seg4_Cell15_Voltage_set
};

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

// Helper function to process a segment
void processSegment(uint8_t segment, uint16_t baseIndex)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        cellVoltageSetters[baseIndex + i](io_ltc6813CellVoltages_getCellVoltage(segment, i));
    }
}

void app_diagnosticsMode_broadcast(void)
{
    // Check which segment's data to broadcast
    switch (app_canRx_Debug_SegmentCellVoltageRequest_get())
    {
        case SEG_0:
            processSegment(0, 0);
            break;
        case SEG_1:
            processSegment(1, 16);
            break;
        case SEG_2:
            processSegment(2, 32);
            break;
        case SEG_3:
            processSegment(3, 48);
            break;
        case SEG_4:
            processSegment(4, 64);
            break;
        case SEG_ALL:
            for (uint8_t segment = 0; segment < 5; segment++)
            {
                processSegment(segment, segment * 16);
            }
            break;
        case NONE:
            break;
        default:
            // Do nothing
            break;
    }
    // Broadcast temperature data if requested
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
