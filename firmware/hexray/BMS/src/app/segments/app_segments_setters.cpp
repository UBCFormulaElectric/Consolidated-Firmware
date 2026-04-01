#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "app_canTx.hpp"

using namespace app::can_tx;

namespace app::segments
{
using CellVoltageSetters                 = void (*)(float);
using FilteredCellVoltageSetters         = void (*)(float);
using CellTemperatureSetters             = void (*)(float);
using CellOwcSetters                     = void (*)(bool);
using ThermOwcSetters                    = void (*)(bool);
using SegmentCommOk                      = void (*)(bool);

CellVoltageSetters cell_voltage_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT] = {
    // Seg 0
    { &BMS_Seg0_Cell0_Voltage_set, &BMS_Seg0_Cell1_Voltage_set, &BMS_Seg0_Cell2_Voltage_set,
      &BMS_Seg0_Cell3_Voltage_set, &BMS_Seg0_Cell4_Voltage_set, &BMS_Seg0_Cell5_Voltage_set,
      &BMS_Seg0_Cell6_Voltage_set, &BMS_Seg0_Cell7_Voltage_set, &BMS_Seg0_Cell8_Voltage_set,
      &BMS_Seg0_Cell9_Voltage_set, &BMS_Seg0_Cell10_Voltage_set, &BMS_Seg0_Cell11_Voltage_set,
      &BMS_Seg0_Cell12_Voltage_set, &BMS_Seg0_Cell13_Voltage_set },
    // Seg 1
    { &BMS_Seg1_Cell0_Voltage_set, &BMS_Seg1_Cell1_Voltage_set, &BMS_Seg1_Cell2_Voltage_set,
      &BMS_Seg1_Cell3_Voltage_set, &BMS_Seg1_Cell4_Voltage_set, &BMS_Seg1_Cell5_Voltage_set,
      &BMS_Seg1_Cell6_Voltage_set, &BMS_Seg1_Cell7_Voltage_set, &BMS_Seg1_Cell8_Voltage_set,
      &BMS_Seg1_Cell9_Voltage_set, &BMS_Seg1_Cell10_Voltage_set, &BMS_Seg1_Cell11_Voltage_set,
      &BMS_Seg1_Cell12_Voltage_set, &BMS_Seg1_Cell13_Voltage_set },
    // Seg 2
    { &BMS_Seg2_Cell0_Voltage_set, &BMS_Seg2_Cell1_Voltage_set, &BMS_Seg2_Cell2_Voltage_set,
      &BMS_Seg2_Cell3_Voltage_set, &BMS_Seg2_Cell4_Voltage_set, &BMS_Seg2_Cell5_Voltage_set,
      &BMS_Seg2_Cell6_Voltage_set, &BMS_Seg2_Cell7_Voltage_set, &BMS_Seg2_Cell8_Voltage_set,
      &BMS_Seg2_Cell9_Voltage_set, &BMS_Seg2_Cell10_Voltage_set, &BMS_Seg2_Cell11_Voltage_set,
      &BMS_Seg2_Cell12_Voltage_set, &BMS_Seg2_Cell13_Voltage_set },
    // Seg 3
    { &BMS_Seg3_Cell0_Voltage_set, &BMS_Seg3_Cell1_Voltage_set, &BMS_Seg3_Cell2_Voltage_set,
      &BMS_Seg3_Cell3_Voltage_set, &BMS_Seg3_Cell4_Voltage_set, &BMS_Seg3_Cell5_Voltage_set,
      &BMS_Seg3_Cell6_Voltage_set, &BMS_Seg3_Cell7_Voltage_set, &BMS_Seg3_Cell8_Voltage_set,
      &BMS_Seg3_Cell9_Voltage_set, &BMS_Seg3_Cell10_Voltage_set, &BMS_Seg3_Cell11_Voltage_set,
      &BMS_Seg3_Cell12_Voltage_set, &BMS_Seg3_Cell13_Voltage_set },
    // Seg 4
    { &BMS_Seg4_Cell0_Voltage_set, &BMS_Seg4_Cell1_Voltage_set, &BMS_Seg4_Cell2_Voltage_set,
      &BMS_Seg4_Cell3_Voltage_set, &BMS_Seg4_Cell4_Voltage_set, &BMS_Seg4_Cell5_Voltage_set,
      &BMS_Seg4_Cell6_Voltage_set, &BMS_Seg4_Cell7_Voltage_set, &BMS_Seg4_Cell8_Voltage_set,
      &BMS_Seg4_Cell9_Voltage_set, &BMS_Seg4_Cell10_Voltage_set, &BMS_Seg4_Cell11_Voltage_set,
      &BMS_Seg4_Cell12_Voltage_set, &BMS_Seg4_Cell13_Voltage_set },
    // Seg 5
    { &BMS_Seg5_Cell0_Voltage_set, &BMS_Seg5_Cell1_Voltage_set, &BMS_Seg5_Cell2_Voltage_set,
      &BMS_Seg5_Cell3_Voltage_set, &BMS_Seg5_Cell4_Voltage_set, &BMS_Seg5_Cell5_Voltage_set,
      &BMS_Seg5_Cell6_Voltage_set, &BMS_Seg5_Cell7_Voltage_set, &BMS_Seg5_Cell8_Voltage_set,
      &BMS_Seg5_Cell9_Voltage_set, &BMS_Seg5_Cell10_Voltage_set, &BMS_Seg5_Cell11_Voltage_set,
      &BMS_Seg5_Cell12_Voltage_set, &BMS_Seg5_Cell13_Voltage_set },
    // Seg 6
    { &BMS_Seg6_Cell0_Voltage_set, &BMS_Seg6_Cell1_Voltage_set, &BMS_Seg6_Cell2_Voltage_set,
      &BMS_Seg6_Cell3_Voltage_set, &BMS_Seg6_Cell4_Voltage_set, &BMS_Seg6_Cell5_Voltage_set,
      &BMS_Seg6_Cell6_Voltage_set, &BMS_Seg6_Cell7_Voltage_set, &BMS_Seg6_Cell8_Voltage_set,
      &BMS_Seg6_Cell9_Voltage_set, &BMS_Seg6_Cell10_Voltage_set, &BMS_Seg6_Cell11_Voltage_set,
      &BMS_Seg6_Cell12_Voltage_set, &BMS_Seg6_Cell13_Voltage_set },
    // Seg 7
    { &BMS_Seg7_Cell0_Voltage_set, &BMS_Seg7_Cell1_Voltage_set, &BMS_Seg7_Cell2_Voltage_set,
      &BMS_Seg7_Cell3_Voltage_set, &BMS_Seg7_Cell4_Voltage_set, &BMS_Seg7_Cell5_Voltage_set,
      &BMS_Seg7_Cell6_Voltage_set, &BMS_Seg7_Cell7_Voltage_set, &BMS_Seg7_Cell8_Voltage_set,
      &BMS_Seg7_Cell9_Voltage_set, &BMS_Seg7_Cell10_Voltage_set, &BMS_Seg7_Cell11_Voltage_set,
      &BMS_Seg7_Cell12_Voltage_set, &BMS_Seg7_Cell13_Voltage_set },
    // Seg 8
    { &BMS_Seg8_Cell0_Voltage_set, &BMS_Seg8_Cell1_Voltage_set, &BMS_Seg8_Cell2_Voltage_set,
      &BMS_Seg8_Cell3_Voltage_set, &BMS_Seg8_Cell4_Voltage_set, &BMS_Seg8_Cell5_Voltage_set,
      &BMS_Seg8_Cell6_Voltage_set, &BMS_Seg8_Cell7_Voltage_set, &BMS_Seg8_Cell8_Voltage_set,
      &BMS_Seg8_Cell9_Voltage_set, &BMS_Seg8_Cell10_Voltage_set, &BMS_Seg8_Cell11_Voltage_set,
      &BMS_Seg8_Cell12_Voltage_set, &BMS_Seg8_Cell13_Voltage_set },
    // Seg 9
    { &BMS_Seg9_Cell0_Voltage_set, &BMS_Seg9_Cell1_Voltage_set, &BMS_Seg9_Cell2_Voltage_set,
      &BMS_Seg9_Cell3_Voltage_set, &BMS_Seg9_Cell4_Voltage_set, &BMS_Seg9_Cell5_Voltage_set,
      &BMS_Seg9_Cell6_Voltage_set, &BMS_Seg9_Cell7_Voltage_set, &BMS_Seg9_Cell8_Voltage_set,
      &BMS_Seg9_Cell9_Voltage_set, &BMS_Seg9_Cell10_Voltage_set, &BMS_Seg9_Cell11_Voltage_set,
      &BMS_Seg9_Cell12_Voltage_set, &BMS_Seg9_Cell13_Voltage_set },
};

FilteredCellVoltageSetters filtered_cell_voltage_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT] = {
    // Seg 0
    { &BMS_Seg0_Cell0_FilteredVoltage_set, &BMS_Seg0_Cell1_FilteredVoltage_set, &BMS_Seg0_Cell2_FilteredVoltage_set,
      &BMS_Seg0_Cell3_FilteredVoltage_set, &BMS_Seg0_Cell4_FilteredVoltage_set, &BMS_Seg0_Cell5_FilteredVoltage_set,
      &BMS_Seg0_Cell6_FilteredVoltage_set, &BMS_Seg0_Cell7_FilteredVoltage_set, &BMS_Seg0_Cell8_FilteredVoltage_set,
      &BMS_Seg0_Cell9_FilteredVoltage_set, &BMS_Seg0_Cell10_FilteredVoltage_set, &BMS_Seg0_Cell11_FilteredVoltage_set,
      &BMS_Seg0_Cell12_FilteredVoltage_set, &BMS_Seg0_Cell13_FilteredVoltage_set },
    // Seg 1
    { &BMS_Seg1_Cell0_FilteredVoltage_set, &BMS_Seg1_Cell1_FilteredVoltage_set, &BMS_Seg1_Cell2_FilteredVoltage_set,
      &BMS_Seg1_Cell3_FilteredVoltage_set, &BMS_Seg1_Cell4_FilteredVoltage_set, &BMS_Seg1_Cell5_FilteredVoltage_set,
      &BMS_Seg1_Cell6_FilteredVoltage_set, &BMS_Seg1_Cell7_FilteredVoltage_set, &BMS_Seg1_Cell8_FilteredVoltage_set,
      &BMS_Seg1_Cell9_FilteredVoltage_set, &BMS_Seg1_Cell10_FilteredVoltage_set, &BMS_Seg1_Cell11_FilteredVoltage_set,
      &BMS_Seg1_Cell12_FilteredVoltage_set, &BMS_Seg1_Cell13_FilteredVoltage_set },
    // Seg 2
    { &BMS_Seg2_Cell0_FilteredVoltage_set, &BMS_Seg2_Cell1_FilteredVoltage_set, &BMS_Seg2_Cell2_FilteredVoltage_set,
      &BMS_Seg2_Cell3_FilteredVoltage_set, &BMS_Seg2_Cell4_FilteredVoltage_set, &BMS_Seg2_Cell5_FilteredVoltage_set,
      &BMS_Seg2_Cell6_FilteredVoltage_set, &BMS_Seg2_Cell7_FilteredVoltage_set, &BMS_Seg2_Cell8_FilteredVoltage_set,
      &BMS_Seg2_Cell9_FilteredVoltage_set, &BMS_Seg2_Cell10_FilteredVoltage_set, &BMS_Seg2_Cell11_FilteredVoltage_set,
      &BMS_Seg2_Cell12_FilteredVoltage_set, &BMS_Seg2_Cell13_FilteredVoltage_set },
    // Seg 3
    { &BMS_Seg3_Cell0_FilteredVoltage_set, &BMS_Seg3_Cell1_FilteredVoltage_set, &BMS_Seg3_Cell2_FilteredVoltage_set,
      &BMS_Seg3_Cell3_FilteredVoltage_set, &BMS_Seg3_Cell4_FilteredVoltage_set, &BMS_Seg3_Cell5_FilteredVoltage_set,
      &BMS_Seg3_Cell6_FilteredVoltage_set, &BMS_Seg3_Cell7_FilteredVoltage_set, &BMS_Seg3_Cell8_FilteredVoltage_set,
      &BMS_Seg3_Cell9_FilteredVoltage_set, &BMS_Seg3_Cell10_FilteredVoltage_set, &BMS_Seg3_Cell11_FilteredVoltage_set,
      &BMS_Seg3_Cell12_FilteredVoltage_set, &BMS_Seg3_Cell13_FilteredVoltage_set },
    // Seg 4
    { &BMS_Seg4_Cell0_FilteredVoltage_set, &BMS_Seg4_Cell1_FilteredVoltage_set, &BMS_Seg4_Cell2_FilteredVoltage_set,
      &BMS_Seg4_Cell3_FilteredVoltage_set, &BMS_Seg4_Cell4_FilteredVoltage_set, &BMS_Seg4_Cell5_FilteredVoltage_set,
      &BMS_Seg4_Cell6_FilteredVoltage_set, &BMS_Seg4_Cell7_FilteredVoltage_set, &BMS_Seg4_Cell8_FilteredVoltage_set,
      &BMS_Seg4_Cell9_FilteredVoltage_set, &BMS_Seg4_Cell10_FilteredVoltage_set, &BMS_Seg4_Cell11_FilteredVoltage_set,
      &BMS_Seg4_Cell12_FilteredVoltage_set, &BMS_Seg4_Cell13_FilteredVoltage_set },
    // Seg 5
    { &BMS_Seg5_Cell0_FilteredVoltage_set, &BMS_Seg5_Cell1_FilteredVoltage_set, &BMS_Seg5_Cell2_FilteredVoltage_set,
      &BMS_Seg5_Cell3_FilteredVoltage_set, &BMS_Seg5_Cell4_FilteredVoltage_set, &BMS_Seg5_Cell5_FilteredVoltage_set,
      &BMS_Seg5_Cell6_FilteredVoltage_set, &BMS_Seg5_Cell7_FilteredVoltage_set, &BMS_Seg5_Cell8_FilteredVoltage_set,
      &BMS_Seg5_Cell9_FilteredVoltage_set, &BMS_Seg5_Cell10_FilteredVoltage_set, &BMS_Seg5_Cell11_FilteredVoltage_set,
      &BMS_Seg5_Cell12_FilteredVoltage_set, &BMS_Seg5_Cell13_FilteredVoltage_set },
    // Seg 6
    { &BMS_Seg6_Cell0_FilteredVoltage_set, &BMS_Seg6_Cell1_FilteredVoltage_set, &BMS_Seg6_Cell2_FilteredVoltage_set,
      &BMS_Seg6_Cell3_FilteredVoltage_set, &BMS_Seg6_Cell4_FilteredVoltage_set, &BMS_Seg6_Cell5_FilteredVoltage_set,
      &BMS_Seg6_Cell6_FilteredVoltage_set, &BMS_Seg6_Cell7_FilteredVoltage_set, &BMS_Seg6_Cell8_FilteredVoltage_set,
      &BMS_Seg6_Cell9_FilteredVoltage_set, &BMS_Seg6_Cell10_FilteredVoltage_set, &BMS_Seg6_Cell11_FilteredVoltage_set,
      &BMS_Seg6_Cell12_FilteredVoltage_set, &BMS_Seg6_Cell13_FilteredVoltage_set },
    // Seg 7
    { &BMS_Seg7_Cell0_FilteredVoltage_set, &BMS_Seg7_Cell1_FilteredVoltage_set, &BMS_Seg7_Cell2_FilteredVoltage_set,
      &BMS_Seg7_Cell3_FilteredVoltage_set, &BMS_Seg7_Cell4_FilteredVoltage_set, &BMS_Seg7_Cell5_FilteredVoltage_set,
      &BMS_Seg7_Cell6_FilteredVoltage_set, &BMS_Seg7_Cell7_FilteredVoltage_set, &BMS_Seg7_Cell8_FilteredVoltage_set,
      &BMS_Seg7_Cell9_FilteredVoltage_set, &BMS_Seg7_Cell10_FilteredVoltage_set, &BMS_Seg7_Cell11_FilteredVoltage_set,
      &BMS_Seg7_Cell12_FilteredVoltage_set, &BMS_Seg7_Cell13_FilteredVoltage_set },
    // Seg 8
    { &BMS_Seg8_Cell0_FilteredVoltage_set, &BMS_Seg8_Cell1_FilteredVoltage_set, &BMS_Seg8_Cell2_FilteredVoltage_set,
      &BMS_Seg8_Cell3_FilteredVoltage_set, &BMS_Seg8_Cell4_FilteredVoltage_set, &BMS_Seg8_Cell5_FilteredVoltage_set,
      &BMS_Seg8_Cell6_FilteredVoltage_set, &BMS_Seg8_Cell7_FilteredVoltage_set, &BMS_Seg8_Cell8_FilteredVoltage_set,
      &BMS_Seg8_Cell9_FilteredVoltage_set, &BMS_Seg8_Cell10_FilteredVoltage_set, &BMS_Seg8_Cell11_FilteredVoltage_set,
      &BMS_Seg8_Cell12_FilteredVoltage_set, &BMS_Seg8_Cell13_FilteredVoltage_set },
    // Seg 9
    { &BMS_Seg9_Cell0_FilteredVoltage_set, &BMS_Seg9_Cell1_FilteredVoltage_set, &BMS_Seg9_Cell2_FilteredVoltage_set,
      &BMS_Seg9_Cell3_FilteredVoltage_set, &BMS_Seg9_Cell4_FilteredVoltage_set, &BMS_Seg9_Cell5_FilteredVoltage_set,
      &BMS_Seg9_Cell6_FilteredVoltage_set, &BMS_Seg9_Cell7_FilteredVoltage_set, &BMS_Seg9_Cell8_FilteredVoltage_set,
      &BMS_Seg9_Cell9_FilteredVoltage_set, &BMS_Seg9_Cell10_FilteredVoltage_set, &BMS_Seg9_Cell11_FilteredVoltage_set,
      &BMS_Seg9_Cell12_FilteredVoltage_set, &BMS_Seg9_Cell13_FilteredVoltage_set },
};

CellTemperatureSetters cell_temperature_setters[MAX_NUM_SEGMENTS][io::THERMISTORS_PER_SEGMENT] = {
    // Seg 0
    { &BMS_Seg0_Cell0_Temp_set, &BMS_Seg0_Cell1_Temp_set, &BMS_Seg0_Cell2_Temp_set, &BMS_Seg0_Cell3_Temp_set,
      &BMS_Seg0_Cell4_Temp_set, &BMS_Seg0_Cell5_Temp_set, &BMS_Seg0_Cell6_Temp_set, &BMS_Seg0_Cell7_Temp_set,
      &BMS_Seg0_Cell8_Temp_set, &BMS_Seg0_Cell9_Temp_set, &BMS_Seg0_Cell10_Temp_set, &BMS_Seg0_Cell11_Temp_set,
      &BMS_Seg0_Cell12_Temp_set, &BMS_Seg0_Cell13_Temp_set },
    // Seg 1
    { &BMS_Seg1_Cell0_Temp_set, &BMS_Seg1_Cell1_Temp_set, &BMS_Seg1_Cell2_Temp_set, &BMS_Seg1_Cell3_Temp_set,
      &BMS_Seg1_Cell4_Temp_set, &BMS_Seg1_Cell5_Temp_set, &BMS_Seg1_Cell6_Temp_set, &BMS_Seg1_Cell7_Temp_set,
      &BMS_Seg1_Cell8_Temp_set, &BMS_Seg1_Cell9_Temp_set, &BMS_Seg1_Cell10_Temp_set, &BMS_Seg1_Cell11_Temp_set,
      &BMS_Seg1_Cell12_Temp_set, &BMS_Seg1_Cell13_Temp_set },
    // Seg 2
    { &BMS_Seg2_Cell0_Temp_set, &BMS_Seg2_Cell1_Temp_set, &BMS_Seg2_Cell2_Temp_set, &BMS_Seg2_Cell3_Temp_set,
      &BMS_Seg2_Cell4_Temp_set, &BMS_Seg2_Cell5_Temp_set, &BMS_Seg2_Cell6_Temp_set, &BMS_Seg2_Cell7_Temp_set,
      &BMS_Seg2_Cell8_Temp_set, &BMS_Seg2_Cell9_Temp_set, &BMS_Seg2_Cell10_Temp_set, &BMS_Seg2_Cell11_Temp_set,
      &BMS_Seg2_Cell12_Temp_set, &BMS_Seg2_Cell13_Temp_set },
    // Seg 3
    { &BMS_Seg3_Cell0_Temp_set, &BMS_Seg3_Cell1_Temp_set, &BMS_Seg3_Cell2_Temp_set, &BMS_Seg3_Cell3_Temp_set,
      &BMS_Seg3_Cell4_Temp_set, &BMS_Seg3_Cell5_Temp_set, &BMS_Seg3_Cell6_Temp_set, &BMS_Seg3_Cell7_Temp_set,
      &BMS_Seg3_Cell8_Temp_set, &BMS_Seg3_Cell9_Temp_set, &BMS_Seg3_Cell10_Temp_set, &BMS_Seg3_Cell11_Temp_set,
      &BMS_Seg3_Cell12_Temp_set, &BMS_Seg3_Cell13_Temp_set },
    // Seg 4
    { &BMS_Seg4_Cell0_Temp_set, &BMS_Seg4_Cell1_Temp_set, &BMS_Seg4_Cell2_Temp_set, &BMS_Seg4_Cell3_Temp_set,
      &BMS_Seg4_Cell4_Temp_set, &BMS_Seg4_Cell5_Temp_set, &BMS_Seg4_Cell6_Temp_set, &BMS_Seg4_Cell7_Temp_set,
      &BMS_Seg4_Cell8_Temp_set, &BMS_Seg4_Cell9_Temp_set, &BMS_Seg4_Cell10_Temp_set, &BMS_Seg4_Cell11_Temp_set,
      &BMS_Seg4_Cell12_Temp_set, &BMS_Seg4_Cell13_Temp_set },
    // Seg 5
    { &BMS_Seg5_Cell0_Temp_set, &BMS_Seg5_Cell1_Temp_set, &BMS_Seg5_Cell2_Temp_set, &BMS_Seg5_Cell3_Temp_set,
      &BMS_Seg5_Cell4_Temp_set, &BMS_Seg5_Cell5_Temp_set, &BMS_Seg5_Cell6_Temp_set, &BMS_Seg5_Cell7_Temp_set,
      &BMS_Seg5_Cell8_Temp_set, &BMS_Seg5_Cell9_Temp_set, &BMS_Seg5_Cell10_Temp_set, &BMS_Seg5_Cell11_Temp_set,
      &BMS_Seg5_Cell12_Temp_set, &BMS_Seg5_Cell13_Temp_set },
    // Seg 6
    { &BMS_Seg6_Cell0_Temp_set, &BMS_Seg6_Cell1_Temp_set, &BMS_Seg6_Cell2_Temp_set, &BMS_Seg6_Cell3_Temp_set,
      &BMS_Seg6_Cell4_Temp_set, &BMS_Seg6_Cell5_Temp_set, &BMS_Seg6_Cell6_Temp_set, &BMS_Seg6_Cell7_Temp_set,
      &BMS_Seg6_Cell8_Temp_set, &BMS_Seg6_Cell9_Temp_set, &BMS_Seg6_Cell10_Temp_set, &BMS_Seg6_Cell11_Temp_set,
      &BMS_Seg6_Cell12_Temp_set, &BMS_Seg6_Cell13_Temp_set },
    // Seg 7
    { &BMS_Seg7_Cell0_Temp_set, &BMS_Seg7_Cell1_Temp_set, &BMS_Seg7_Cell2_Temp_set, &BMS_Seg7_Cell3_Temp_set,
      &BMS_Seg7_Cell4_Temp_set, &BMS_Seg7_Cell5_Temp_set, &BMS_Seg7_Cell6_Temp_set, &BMS_Seg7_Cell7_Temp_set,
      &BMS_Seg7_Cell8_Temp_set, &BMS_Seg7_Cell9_Temp_set, &BMS_Seg7_Cell10_Temp_set, &BMS_Seg7_Cell11_Temp_set,
      &BMS_Seg7_Cell12_Temp_set, &BMS_Seg7_Cell13_Temp_set },
    // Seg 8
    { &BMS_Seg8_Cell0_Temp_set, &BMS_Seg8_Cell1_Temp_set, &BMS_Seg8_Cell2_Temp_set, &BMS_Seg8_Cell3_Temp_set,
      &BMS_Seg8_Cell4_Temp_set, &BMS_Seg8_Cell5_Temp_set, &BMS_Seg8_Cell6_Temp_set, &BMS_Seg8_Cell7_Temp_set,
      &BMS_Seg8_Cell8_Temp_set, &BMS_Seg8_Cell9_Temp_set, &BMS_Seg8_Cell10_Temp_set, &BMS_Seg8_Cell11_Temp_set,
      &BMS_Seg8_Cell12_Temp_set, &BMS_Seg8_Cell13_Temp_set },
    // Seg 9
    { &BMS_Seg9_Cell0_Temp_set, &BMS_Seg9_Cell1_Temp_set, &BMS_Seg9_Cell2_Temp_set, &BMS_Seg9_Cell3_Temp_set,
      &BMS_Seg9_Cell4_Temp_set, &BMS_Seg9_Cell5_Temp_set, &BMS_Seg9_Cell6_Temp_set, &BMS_Seg9_Cell7_Temp_set,
      &BMS_Seg9_Cell8_Temp_set, &BMS_Seg9_Cell9_Temp_set, &BMS_Seg9_Cell10_Temp_set, &BMS_Seg9_Cell11_Temp_set,
      &BMS_Seg9_Cell12_Temp_set, &BMS_Seg9_Cell13_Temp_set },
};

CellOwcSetters cell_owc_setters[MAX_NUM_SEGMENTS][OWC_CHANNELS_PER_SEGMENT] = {
    // Seg 0
    { &BMS_Seg0_Cell0_OwcOk_set, &BMS_Seg0_Cell1_OwcOk_set, &BMS_Seg0_Cell2_OwcOk_set, &BMS_Seg0_Cell3_OwcOk_set,
      &BMS_Seg0_Cell4_OwcOk_set, &BMS_Seg0_Cell5_OwcOk_set, &BMS_Seg0_Cell6_OwcOk_set, &BMS_Seg0_Cell7_OwcOk_set,
      &BMS_Seg0_Cell8_OwcOk_set, &BMS_Seg0_Cell9_OwcOk_set, &BMS_Seg0_Cell10_OwcOk_set, &BMS_Seg0_Cell11_OwcOk_set,
      &BMS_Seg0_Cell12_OwcOk_set, &BMS_Seg0_Cell13_OwcOk_set, &BMS_Seg0_Cell14_OwcOk_set },
    // Seg 1
    { &BMS_Seg1_Cell0_OwcOk_set, &BMS_Seg1_Cell1_OwcOk_set, &BMS_Seg1_Cell2_OwcOk_set, &BMS_Seg1_Cell3_OwcOk_set,
      &BMS_Seg1_Cell4_OwcOk_set, &BMS_Seg1_Cell5_OwcOk_set, &BMS_Seg1_Cell6_OwcOk_set, &BMS_Seg1_Cell7_OwcOk_set,
      &BMS_Seg1_Cell8_OwcOk_set, &BMS_Seg1_Cell9_OwcOk_set, &BMS_Seg1_Cell10_OwcOk_set, &BMS_Seg1_Cell11_OwcOk_set,
      &BMS_Seg1_Cell12_OwcOk_set, &BMS_Seg1_Cell13_OwcOk_set, &BMS_Seg1_Cell14_OwcOk_set },
    // Seg 2
    { &BMS_Seg2_Cell0_OwcOk_set, &BMS_Seg2_Cell1_OwcOk_set, &BMS_Seg2_Cell2_OwcOk_set, &BMS_Seg2_Cell3_OwcOk_set,
      &BMS_Seg2_Cell4_OwcOk_set, &BMS_Seg2_Cell5_OwcOk_set, &BMS_Seg2_Cell6_OwcOk_set, &BMS_Seg2_Cell7_OwcOk_set,
      &BMS_Seg2_Cell8_OwcOk_set, &BMS_Seg2_Cell9_OwcOk_set, &BMS_Seg2_Cell10_OwcOk_set, &BMS_Seg2_Cell11_OwcOk_set,
      &BMS_Seg2_Cell12_OwcOk_set, &BMS_Seg2_Cell13_OwcOk_set, &BMS_Seg2_Cell14_OwcOk_set },
    // Seg 3
    { &BMS_Seg3_Cell0_OwcOk_set, &BMS_Seg3_Cell1_OwcOk_set, &BMS_Seg3_Cell2_OwcOk_set, &BMS_Seg3_Cell3_OwcOk_set,
      &BMS_Seg3_Cell4_OwcOk_set, &BMS_Seg3_Cell5_OwcOk_set, &BMS_Seg3_Cell6_OwcOk_set, &BMS_Seg3_Cell7_OwcOk_set,
      &BMS_Seg3_Cell8_OwcOk_set, &BMS_Seg3_Cell9_OwcOk_set, &BMS_Seg3_Cell10_OwcOk_set, &BMS_Seg3_Cell11_OwcOk_set,
      &BMS_Seg3_Cell12_OwcOk_set, &BMS_Seg3_Cell13_OwcOk_set, &BMS_Seg3_Cell14_OwcOk_set },
    // Seg 4
    { &BMS_Seg4_Cell0_OwcOk_set, &BMS_Seg4_Cell1_OwcOk_set, &BMS_Seg4_Cell2_OwcOk_set, &BMS_Seg4_Cell3_OwcOk_set,
      &BMS_Seg4_Cell4_OwcOk_set, &BMS_Seg4_Cell5_OwcOk_set, &BMS_Seg4_Cell6_OwcOk_set, &BMS_Seg4_Cell7_OwcOk_set,
      &BMS_Seg4_Cell8_OwcOk_set, &BMS_Seg4_Cell9_OwcOk_set, &BMS_Seg4_Cell10_OwcOk_set, &BMS_Seg4_Cell11_OwcOk_set,
      &BMS_Seg4_Cell12_OwcOk_set, &BMS_Seg4_Cell13_OwcOk_set, &BMS_Seg4_Cell14_OwcOk_set },
    // Seg 5
    { &BMS_Seg5_Cell0_OwcOk_set, &BMS_Seg5_Cell1_OwcOk_set, &BMS_Seg5_Cell2_OwcOk_set, &BMS_Seg5_Cell3_OwcOk_set,
      &BMS_Seg5_Cell4_OwcOk_set, &BMS_Seg5_Cell5_OwcOk_set, &BMS_Seg5_Cell6_OwcOk_set, &BMS_Seg5_Cell7_OwcOk_set,
      &BMS_Seg5_Cell8_OwcOk_set, &BMS_Seg5_Cell9_OwcOk_set, &BMS_Seg5_Cell10_OwcOk_set, &BMS_Seg5_Cell11_OwcOk_set,
      &BMS_Seg5_Cell12_OwcOk_set, &BMS_Seg5_Cell13_OwcOk_set, &BMS_Seg5_Cell14_OwcOk_set },
    // Seg 6
    { &BMS_Seg6_Cell0_OwcOk_set, &BMS_Seg6_Cell1_OwcOk_set, &BMS_Seg6_Cell2_OwcOk_set, &BMS_Seg6_Cell3_OwcOk_set,
      &BMS_Seg6_Cell4_OwcOk_set, &BMS_Seg6_Cell5_OwcOk_set, &BMS_Seg6_Cell6_OwcOk_set, &BMS_Seg6_Cell7_OwcOk_set,
      &BMS_Seg6_Cell8_OwcOk_set, &BMS_Seg6_Cell9_OwcOk_set, &BMS_Seg6_Cell10_OwcOk_set, &BMS_Seg6_Cell11_OwcOk_set,
      &BMS_Seg6_Cell12_OwcOk_set, &BMS_Seg6_Cell13_OwcOk_set, &BMS_Seg6_Cell14_OwcOk_set },
    // Seg 7
    { &BMS_Seg7_Cell0_OwcOk_set, &BMS_Seg7_Cell1_OwcOk_set, &BMS_Seg7_Cell2_OwcOk_set, &BMS_Seg7_Cell3_OwcOk_set,
      &BMS_Seg7_Cell4_OwcOk_set, &BMS_Seg7_Cell5_OwcOk_set, &BMS_Seg7_Cell6_OwcOk_set, &BMS_Seg7_Cell7_OwcOk_set,
      &BMS_Seg7_Cell8_OwcOk_set, &BMS_Seg7_Cell9_OwcOk_set, &BMS_Seg7_Cell10_OwcOk_set, &BMS_Seg7_Cell11_OwcOk_set,
      &BMS_Seg7_Cell12_OwcOk_set, &BMS_Seg7_Cell13_OwcOk_set, &BMS_Seg7_Cell14_OwcOk_set },
    // Seg 8
    { &BMS_Seg8_Cell0_OwcOk_set, &BMS_Seg8_Cell1_OwcOk_set, &BMS_Seg8_Cell2_OwcOk_set, &BMS_Seg8_Cell3_OwcOk_set,
      &BMS_Seg8_Cell4_OwcOk_set, &BMS_Seg8_Cell5_OwcOk_set, &BMS_Seg8_Cell6_OwcOk_set, &BMS_Seg8_Cell7_OwcOk_set,
      &BMS_Seg8_Cell8_OwcOk_set, &BMS_Seg8_Cell9_OwcOk_set, &BMS_Seg8_Cell10_OwcOk_set, &BMS_Seg8_Cell11_OwcOk_set,
      &BMS_Seg8_Cell12_OwcOk_set, &BMS_Seg8_Cell13_OwcOk_set, &BMS_Seg8_Cell14_OwcOk_set },
    // Seg 9
    { &BMS_Seg9_Cell0_OwcOk_set, &BMS_Seg9_Cell1_OwcOk_set, &BMS_Seg9_Cell2_OwcOk_set, &BMS_Seg9_Cell3_OwcOk_set,
      &BMS_Seg9_Cell4_OwcOk_set, &BMS_Seg9_Cell5_OwcOk_set, &BMS_Seg9_Cell6_OwcOk_set, &BMS_Seg9_Cell7_OwcOk_set,
      &BMS_Seg9_Cell8_OwcOk_set, &BMS_Seg9_Cell9_OwcOk_set, &BMS_Seg9_Cell10_OwcOk_set, &BMS_Seg9_Cell11_OwcOk_set,
      &BMS_Seg9_Cell12_OwcOk_set, &BMS_Seg9_Cell13_OwcOk_set, &BMS_Seg9_Cell14_OwcOk_set },
};

ThermOwcSetters therm_owc_setters[MAX_NUM_SEGMENTS][io::adbms::THERM_GPIOS_PER_SEGMENT] = {
    // Seg 0
    { &BMS_Seg0_ThermGpio0_OwcOk_set, &BMS_Seg0_ThermGpio1_OwcOk_set, &BMS_Seg0_ThermGpio2_OwcOk_set,
      &BMS_Seg0_ThermGpio3_OwcOk_set, &BMS_Seg0_ThermGpio4_OwcOk_set, &BMS_Seg0_ThermGpio5_OwcOk_set,
      &BMS_Seg0_ThermGpio6_OwcOk_set, &BMS_Seg0_ThermGpio7_OwcOk_set },
    // Seg 1
    { &BMS_Seg1_ThermGpio0_OwcOk_set, &BMS_Seg1_ThermGpio1_OwcOk_set, &BMS_Seg1_ThermGpio2_OwcOk_set,
      &BMS_Seg1_ThermGpio3_OwcOk_set, &BMS_Seg1_ThermGpio4_OwcOk_set, &BMS_Seg1_ThermGpio5_OwcOk_set,
      &BMS_Seg1_ThermGpio6_OwcOk_set, &BMS_Seg1_ThermGpio7_OwcOk_set },
    // Seg 2
    { &BMS_Seg2_ThermGpio0_OwcOk_set, &BMS_Seg2_ThermGpio1_OwcOk_set, &BMS_Seg2_ThermGpio2_OwcOk_set,
      &BMS_Seg2_ThermGpio3_OwcOk_set, &BMS_Seg2_ThermGpio4_OwcOk_set, &BMS_Seg2_ThermGpio5_OwcOk_set,
      &BMS_Seg2_ThermGpio6_OwcOk_set, &BMS_Seg2_ThermGpio7_OwcOk_set },
    // Seg 3
    { &BMS_Seg3_ThermGpio0_OwcOk_set, &BMS_Seg3_ThermGpio1_OwcOk_set, &BMS_Seg3_ThermGpio2_OwcOk_set,
      &BMS_Seg3_ThermGpio3_OwcOk_set, &BMS_Seg3_ThermGpio4_OwcOk_set, &BMS_Seg3_ThermGpio5_OwcOk_set,
      &BMS_Seg3_ThermGpio6_OwcOk_set, &BMS_Seg3_ThermGpio7_OwcOk_set },
    // Seg 4
    { &BMS_Seg4_ThermGpio0_OwcOk_set, &BMS_Seg4_ThermGpio1_OwcOk_set, &BMS_Seg4_ThermGpio2_OwcOk_set,
      &BMS_Seg4_ThermGpio3_OwcOk_set, &BMS_Seg4_ThermGpio4_OwcOk_set, &BMS_Seg4_ThermGpio5_OwcOk_set,
      &BMS_Seg4_ThermGpio6_OwcOk_set, &BMS_Seg4_ThermGpio7_OwcOk_set },
    // Seg 5
    { &BMS_Seg5_ThermGpio0_OwcOk_set, &BMS_Seg5_ThermGpio1_OwcOk_set, &BMS_Seg5_ThermGpio2_OwcOk_set,
      &BMS_Seg5_ThermGpio3_OwcOk_set, &BMS_Seg5_ThermGpio4_OwcOk_set, &BMS_Seg5_ThermGpio5_OwcOk_set,
      &BMS_Seg5_ThermGpio6_OwcOk_set, &BMS_Seg5_ThermGpio7_OwcOk_set },
    // Seg 6
    { &BMS_Seg6_ThermGpio0_OwcOk_set, &BMS_Seg6_ThermGpio1_OwcOk_set, &BMS_Seg6_ThermGpio2_OwcOk_set,
      &BMS_Seg6_ThermGpio3_OwcOk_set, &BMS_Seg6_ThermGpio4_OwcOk_set, &BMS_Seg6_ThermGpio5_OwcOk_set,
      &BMS_Seg6_ThermGpio6_OwcOk_set, &BMS_Seg6_ThermGpio7_OwcOk_set },
    // Seg 7
    { &BMS_Seg7_ThermGpio0_OwcOk_set, &BMS_Seg7_ThermGpio1_OwcOk_set, &BMS_Seg7_ThermGpio2_OwcOk_set,
      &BMS_Seg7_ThermGpio3_OwcOk_set, &BMS_Seg7_ThermGpio4_OwcOk_set, &BMS_Seg7_ThermGpio5_OwcOk_set,
      &BMS_Seg7_ThermGpio6_OwcOk_set, &BMS_Seg7_ThermGpio7_OwcOk_set },
    // Seg 8
    { &BMS_Seg8_ThermGpio0_OwcOk_set, &BMS_Seg8_ThermGpio1_OwcOk_set, &BMS_Seg8_ThermGpio2_OwcOk_set,
      &BMS_Seg8_ThermGpio3_OwcOk_set, &BMS_Seg8_ThermGpio4_OwcOk_set, &BMS_Seg8_ThermGpio5_OwcOk_set,
      &BMS_Seg8_ThermGpio6_OwcOk_set, &BMS_Seg8_ThermGpio7_OwcOk_set },
    // Seg 9
    { &BMS_Seg9_ThermGpio0_OwcOk_set, &BMS_Seg9_ThermGpio1_OwcOk_set, &BMS_Seg9_ThermGpio2_OwcOk_set,
      &BMS_Seg9_ThermGpio3_OwcOk_set, &BMS_Seg9_ThermGpio4_OwcOk_set, &BMS_Seg9_ThermGpio5_OwcOk_set,
      &BMS_Seg9_ThermGpio6_OwcOk_set, &BMS_Seg9_ThermGpio7_OwcOk_set },
};

SegmentCommOkSetters segment_comm_ok_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_CommOk_set, &BMS_Seg1_CommOk_set, &BMS_Seg2_CommOk_set, &BMS_Seg3_CommOk_set,
    &BMS_Seg4_CommOk_set, &BMS_Seg5_CommOk_set, &BMS_Seg6_CommOk_set, &BMS_Seg7_CommOk_set,
    &BMS_Seg8_CommOk_set, &BMS_Seg9_CommOk_set,
};
} // namespace app::segments
