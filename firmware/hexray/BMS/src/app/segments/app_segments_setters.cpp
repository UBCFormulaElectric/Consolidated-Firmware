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
using SegmentCommOkSetters               = void (*)(bool);
using SegmentVref2Setters                = void (*)(float);
using SegmentITMPSetters                 = void (*)(float);
using SegmentVDSetters                   = void (*)(float);
using SegmentVASetters                   = void (*)(float);
using SegmentVRESSetters                 = void (*)(float);
using SegmentVaOvSetters                 = void (*)(bool);
using SegmentVaUvSetters                 = void (*)(bool);
using SegmentVdOvSetters                 = void (*)(bool);
using SegmentVdUvSetters                 = void (*)(bool);
using SegmentCedSetters                  = void (*)(bool);
using SegmentCmedSetters                 = void (*)(bool);
using SegmentSedSetters                  = void (*)(bool);
using SegmentSmedSetters                 = void (*)(bool);
using SegmentVdeSetters                  = void (*)(bool);
using SegmentVdelSetters                 = void (*)(bool);
using SegmentThsdSetters                 = void (*)(bool);
using SegmentTmodchkSetters              = void (*)(bool);
using SegmentOscchkSetters               = void (*)(bool);
using CellOvSetters                      = void (*)(bool);
using CellUvSetters                      = void (*)(bool);

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

CellOwcSetters cell_owc_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT] = {
    // Seg 0
    { &BMS_Seg0_Cell0_OwcOk_set, &BMS_Seg0_Cell1_OwcOk_set, &BMS_Seg0_Cell2_OwcOk_set, &BMS_Seg0_Cell3_OwcOk_set,
      &BMS_Seg0_Cell4_OwcOk_set, &BMS_Seg0_Cell5_OwcOk_set, &BMS_Seg0_Cell6_OwcOk_set, &BMS_Seg0_Cell7_OwcOk_set,
      &BMS_Seg0_Cell8_OwcOk_set, &BMS_Seg0_Cell9_OwcOk_set, &BMS_Seg0_Cell10_OwcOk_set, &BMS_Seg0_Cell11_OwcOk_set,
      &BMS_Seg0_Cell12_OwcOk_set, &BMS_Seg0_Cell13_OwcOk_set},
    // Seg 1
    { &BMS_Seg1_Cell0_OwcOk_set, &BMS_Seg1_Cell1_OwcOk_set, &BMS_Seg1_Cell2_OwcOk_set, &BMS_Seg1_Cell3_OwcOk_set,
      &BMS_Seg1_Cell4_OwcOk_set, &BMS_Seg1_Cell5_OwcOk_set, &BMS_Seg1_Cell6_OwcOk_set, &BMS_Seg1_Cell7_OwcOk_set,
      &BMS_Seg1_Cell8_OwcOk_set, &BMS_Seg1_Cell9_OwcOk_set, &BMS_Seg1_Cell10_OwcOk_set, &BMS_Seg1_Cell11_OwcOk_set,
      &BMS_Seg1_Cell12_OwcOk_set, &BMS_Seg1_Cell13_OwcOk_set },
    // Seg 2
    { &BMS_Seg2_Cell0_OwcOk_set, &BMS_Seg2_Cell1_OwcOk_set, &BMS_Seg2_Cell2_OwcOk_set, &BMS_Seg2_Cell3_OwcOk_set,
      &BMS_Seg2_Cell4_OwcOk_set, &BMS_Seg2_Cell5_OwcOk_set, &BMS_Seg2_Cell6_OwcOk_set, &BMS_Seg2_Cell7_OwcOk_set,
      &BMS_Seg2_Cell8_OwcOk_set, &BMS_Seg2_Cell9_OwcOk_set, &BMS_Seg2_Cell10_OwcOk_set, &BMS_Seg2_Cell11_OwcOk_set,
      &BMS_Seg2_Cell12_OwcOk_set, &BMS_Seg2_Cell13_OwcOk_set },
    // Seg 3
    { &BMS_Seg3_Cell0_OwcOk_set, &BMS_Seg3_Cell1_OwcOk_set, &BMS_Seg3_Cell2_OwcOk_set, &BMS_Seg3_Cell3_OwcOk_set,
      &BMS_Seg3_Cell4_OwcOk_set, &BMS_Seg3_Cell5_OwcOk_set, &BMS_Seg3_Cell6_OwcOk_set, &BMS_Seg3_Cell7_OwcOk_set,
      &BMS_Seg3_Cell8_OwcOk_set, &BMS_Seg3_Cell9_OwcOk_set, &BMS_Seg3_Cell10_OwcOk_set, &BMS_Seg3_Cell11_OwcOk_set,
      &BMS_Seg3_Cell12_OwcOk_set, &BMS_Seg3_Cell13_OwcOk_set },
    // Seg 4
    { &BMS_Seg4_Cell0_OwcOk_set, &BMS_Seg4_Cell1_OwcOk_set, &BMS_Seg4_Cell2_OwcOk_set, &BMS_Seg4_Cell3_OwcOk_set,
      &BMS_Seg4_Cell4_OwcOk_set, &BMS_Seg4_Cell5_OwcOk_set, &BMS_Seg4_Cell6_OwcOk_set, &BMS_Seg4_Cell7_OwcOk_set,
      &BMS_Seg4_Cell8_OwcOk_set, &BMS_Seg4_Cell9_OwcOk_set, &BMS_Seg4_Cell10_OwcOk_set, &BMS_Seg4_Cell11_OwcOk_set,
      &BMS_Seg4_Cell12_OwcOk_set, &BMS_Seg4_Cell13_OwcOk_set },
    // Seg 5
    { &BMS_Seg5_Cell0_OwcOk_set, &BMS_Seg5_Cell1_OwcOk_set, &BMS_Seg5_Cell2_OwcOk_set, &BMS_Seg5_Cell3_OwcOk_set,
      &BMS_Seg5_Cell4_OwcOk_set, &BMS_Seg5_Cell5_OwcOk_set, &BMS_Seg5_Cell6_OwcOk_set, &BMS_Seg5_Cell7_OwcOk_set,
      &BMS_Seg5_Cell8_OwcOk_set, &BMS_Seg5_Cell9_OwcOk_set, &BMS_Seg5_Cell10_OwcOk_set, &BMS_Seg5_Cell11_OwcOk_set,
      &BMS_Seg5_Cell12_OwcOk_set, &BMS_Seg5_Cell13_OwcOk_set },
    // Seg 6
    { &BMS_Seg6_Cell0_OwcOk_set, &BMS_Seg6_Cell1_OwcOk_set, &BMS_Seg6_Cell2_OwcOk_set, &BMS_Seg6_Cell3_OwcOk_set,
      &BMS_Seg6_Cell4_OwcOk_set, &BMS_Seg6_Cell5_OwcOk_set, &BMS_Seg6_Cell6_OwcOk_set, &BMS_Seg6_Cell7_OwcOk_set,
      &BMS_Seg6_Cell8_OwcOk_set, &BMS_Seg6_Cell9_OwcOk_set, &BMS_Seg6_Cell10_OwcOk_set, &BMS_Seg6_Cell11_OwcOk_set,
      &BMS_Seg6_Cell12_OwcOk_set, &BMS_Seg6_Cell13_OwcOk_set },
    // Seg 7
    { &BMS_Seg7_Cell0_OwcOk_set, &BMS_Seg7_Cell1_OwcOk_set, &BMS_Seg7_Cell2_OwcOk_set, &BMS_Seg7_Cell3_OwcOk_set,
      &BMS_Seg7_Cell4_OwcOk_set, &BMS_Seg7_Cell5_OwcOk_set, &BMS_Seg7_Cell6_OwcOk_set, &BMS_Seg7_Cell7_OwcOk_set,
      &BMS_Seg7_Cell8_OwcOk_set, &BMS_Seg7_Cell9_OwcOk_set, &BMS_Seg7_Cell10_OwcOk_set, &BMS_Seg7_Cell11_OwcOk_set,
      &BMS_Seg7_Cell12_OwcOk_set, &BMS_Seg7_Cell13_OwcOk_set },
    // Seg 8
    { &BMS_Seg8_Cell0_OwcOk_set, &BMS_Seg8_Cell1_OwcOk_set, &BMS_Seg8_Cell2_OwcOk_set, &BMS_Seg8_Cell3_OwcOk_set,
      &BMS_Seg8_Cell4_OwcOk_set, &BMS_Seg8_Cell5_OwcOk_set, &BMS_Seg8_Cell6_OwcOk_set, &BMS_Seg8_Cell7_OwcOk_set,
      &BMS_Seg8_Cell8_OwcOk_set, &BMS_Seg8_Cell9_OwcOk_set, &BMS_Seg8_Cell10_OwcOk_set, &BMS_Seg8_Cell11_OwcOk_set,
      &BMS_Seg8_Cell12_OwcOk_set, &BMS_Seg8_Cell13_OwcOk_set },
    // Seg 9
    { &BMS_Seg9_Cell0_OwcOk_set, &BMS_Seg9_Cell1_OwcOk_set, &BMS_Seg9_Cell2_OwcOk_set, &BMS_Seg9_Cell3_OwcOk_set,
      &BMS_Seg9_Cell4_OwcOk_set, &BMS_Seg9_Cell5_OwcOk_set, &BMS_Seg9_Cell6_OwcOk_set, &BMS_Seg9_Cell7_OwcOk_set,
      &BMS_Seg9_Cell8_OwcOk_set, &BMS_Seg9_Cell9_OwcOk_set, &BMS_Seg9_Cell10_OwcOk_set, &BMS_Seg9_Cell11_OwcOk_set,
      &BMS_Seg9_Cell12_OwcOk_set, &BMS_Seg9_Cell13_OwcOk_set },
};

ThermOwcSetters therm_owc_setters[MAX_NUM_SEGMENTS][io::THERMISTORS_PER_SEGMENT] = {
    // Seg 0
    { &BMS_Seg0_Therm0_OwcOk_set, &BMS_Seg0_Therm1_OwcOk_set, &BMS_Seg0_Therm2_OwcOk_set,
      &BMS_Seg0_Therm3_OwcOk_set, &BMS_Seg0_Therm4_OwcOk_set, &BMS_Seg0_Therm5_OwcOk_set,
      &BMS_Seg0_Therm6_OwcOk_set, &BMS_Seg0_Therm7_OwcOk_set, &BMS_Seg0_Therm8_OwcOk_set,
      &BMS_Seg0_Therm9_OwcOk_set, &BMS_Seg0_Therm10_OwcOk_set, &BMS_Seg0_Therm11_OwcOk_set,
      &BMS_Seg0_Therm12_OwcOk_set, &BMS_Seg0_Therm13_OwcOk_set },
    // Seg 1
    { &BMS_Seg1_Therm0_OwcOk_set, &BMS_Seg1_Therm1_OwcOk_set, &BMS_Seg1_Therm2_OwcOk_set,
      &BMS_Seg1_Therm3_OwcOk_set, &BMS_Seg1_Therm4_OwcOk_set, &BMS_Seg1_Therm5_OwcOk_set,
      &BMS_Seg1_Therm6_OwcOk_set, &BMS_Seg1_Therm7_OwcOk_set, &BMS_Seg1_Therm8_OwcOk_set,
      &BMS_Seg1_Therm9_OwcOk_set, &BMS_Seg1_Therm10_OwcOk_set, &BMS_Seg1_Therm11_OwcOk_set,
      &BMS_Seg1_Therm12_OwcOk_set, &BMS_Seg1_Therm13_OwcOk_set },
    // Seg 2
    { &BMS_Seg2_Therm0_OwcOk_set, &BMS_Seg2_Therm1_OwcOk_set, &BMS_Seg2_Therm2_OwcOk_set,
      &BMS_Seg2_Therm3_OwcOk_set, &BMS_Seg2_Therm4_OwcOk_set, &BMS_Seg2_Therm5_OwcOk_set,
      &BMS_Seg2_Therm6_OwcOk_set, &BMS_Seg2_Therm7_OwcOk_set, &BMS_Seg2_Therm8_OwcOk_set,
      &BMS_Seg2_Therm9_OwcOk_set, &BMS_Seg2_Therm10_OwcOk_set, &BMS_Seg2_Therm11_OwcOk_set,
      &BMS_Seg2_Therm12_OwcOk_set, &BMS_Seg2_Therm13_OwcOk_set },
    // Seg 3
    { &BMS_Seg3_Therm0_OwcOk_set, &BMS_Seg3_Therm1_OwcOk_set, &BMS_Seg3_Therm2_OwcOk_set,
      &BMS_Seg3_Therm3_OwcOk_set, &BMS_Seg3_Therm4_OwcOk_set, &BMS_Seg3_Therm5_OwcOk_set,
      &BMS_Seg3_Therm6_OwcOk_set, &BMS_Seg3_Therm7_OwcOk_set, &BMS_Seg3_Therm8_OwcOk_set,
      &BMS_Seg3_Therm9_OwcOk_set, &BMS_Seg3_Therm10_OwcOk_set, &BMS_Seg3_Therm11_OwcOk_set,
      &BMS_Seg3_Therm12_OwcOk_set, &BMS_Seg3_Therm13_OwcOk_set },
    // Seg 4
    { &BMS_Seg4_Therm0_OwcOk_set, &BMS_Seg4_Therm1_OwcOk_set, &BMS_Seg4_Therm2_OwcOk_set,
      &BMS_Seg4_Therm3_OwcOk_set, &BMS_Seg4_Therm4_OwcOk_set, &BMS_Seg4_Therm5_OwcOk_set,
      &BMS_Seg4_Therm6_OwcOk_set, &BMS_Seg4_Therm7_OwcOk_set, &BMS_Seg4_Therm8_OwcOk_set,
      &BMS_Seg4_Therm9_OwcOk_set, &BMS_Seg4_Therm10_OwcOk_set, &BMS_Seg4_Therm11_OwcOk_set,
      &BMS_Seg4_Therm12_OwcOk_set, &BMS_Seg4_Therm13_OwcOk_set },
    // Seg 5
    { &BMS_Seg5_Therm0_OwcOk_set, &BMS_Seg5_Therm1_OwcOk_set, &BMS_Seg5_Therm2_OwcOk_set,
      &BMS_Seg5_Therm3_OwcOk_set, &BMS_Seg5_Therm4_OwcOk_set, &BMS_Seg5_Therm5_OwcOk_set,
      &BMS_Seg5_Therm6_OwcOk_set, &BMS_Seg5_Therm7_OwcOk_set, &BMS_Seg5_Therm8_OwcOk_set,
      &BMS_Seg5_Therm9_OwcOk_set, &BMS_Seg5_Therm10_OwcOk_set, &BMS_Seg5_Therm11_OwcOk_set,
      &BMS_Seg5_Therm12_OwcOk_set, &BMS_Seg5_Therm13_OwcOk_set },
    // Seg 6
    { &BMS_Seg6_Therm0_OwcOk_set, &BMS_Seg6_Therm1_OwcOk_set, &BMS_Seg6_Therm2_OwcOk_set,
      &BMS_Seg6_Therm3_OwcOk_set, &BMS_Seg6_Therm4_OwcOk_set, &BMS_Seg6_Therm5_OwcOk_set,
      &BMS_Seg6_Therm6_OwcOk_set, &BMS_Seg6_Therm7_OwcOk_set, &BMS_Seg6_Therm8_OwcOk_set,
      &BMS_Seg6_Therm9_OwcOk_set, &BMS_Seg6_Therm10_OwcOk_set, &BMS_Seg6_Therm11_OwcOk_set,
      &BMS_Seg6_Therm12_OwcOk_set, &BMS_Seg6_Therm13_OwcOk_set },
    // Seg 7
    { &BMS_Seg7_Therm0_OwcOk_set, &BMS_Seg7_Therm1_OwcOk_set, &BMS_Seg7_Therm2_OwcOk_set,
      &BMS_Seg7_Therm3_OwcOk_set, &BMS_Seg7_Therm4_OwcOk_set, &BMS_Seg7_Therm5_OwcOk_set,
      &BMS_Seg7_Therm6_OwcOk_set, &BMS_Seg7_Therm7_OwcOk_set, &BMS_Seg7_Therm8_OwcOk_set,
      &BMS_Seg7_Therm9_OwcOk_set, &BMS_Seg7_Therm10_OwcOk_set, &BMS_Seg7_Therm11_OwcOk_set,
      &BMS_Seg7_Therm12_OwcOk_set, &BMS_Seg7_Therm13_OwcOk_set },
    // Seg 8
    { &BMS_Seg8_Therm0_OwcOk_set, &BMS_Seg8_Therm1_OwcOk_set, &BMS_Seg8_Therm2_OwcOk_set,
      &BMS_Seg8_Therm3_OwcOk_set, &BMS_Seg8_Therm4_OwcOk_set, &BMS_Seg8_Therm5_OwcOk_set,
      &BMS_Seg8_Therm6_OwcOk_set, &BMS_Seg8_Therm7_OwcOk_set, &BMS_Seg8_Therm8_OwcOk_set,
      &BMS_Seg8_Therm9_OwcOk_set, &BMS_Seg8_Therm10_OwcOk_set, &BMS_Seg8_Therm11_OwcOk_set,
      &BMS_Seg8_Therm12_OwcOk_set, &BMS_Seg8_Therm13_OwcOk_set },
    // Seg 9
    { &BMS_Seg9_Therm0_OwcOk_set, &BMS_Seg9_Therm1_OwcOk_set, &BMS_Seg9_Therm2_OwcOk_set,
      &BMS_Seg9_Therm3_OwcOk_set, &BMS_Seg9_Therm4_OwcOk_set, &BMS_Seg9_Therm5_OwcOk_set,
      &BMS_Seg9_Therm6_OwcOk_set, &BMS_Seg9_Therm7_OwcOk_set, &BMS_Seg9_Therm8_OwcOk_set,
      &BMS_Seg9_Therm9_OwcOk_set, &BMS_Seg9_Therm10_OwcOk_set, &BMS_Seg9_Therm11_OwcOk_set,
      &BMS_Seg9_Therm12_OwcOk_set, &BMS_Seg9_Therm13_OwcOk_set },
};

SegmentCommOkSetters segment_comm_ok_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_CommOk_set, &BMS_Seg1_CommOk_set, &BMS_Seg2_CommOk_set, &BMS_Seg3_CommOk_set,
    &BMS_Seg4_CommOk_set, &BMS_Seg5_CommOk_set, &BMS_Seg6_CommOk_set, &BMS_Seg7_CommOk_set,
    &BMS_Seg8_CommOk_set, &BMS_Seg9_CommOk_set,
};

SegmentVref2Setters segment_vref2_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_Vref2_set, &BMS_Seg1_Vref2_set, &BMS_Seg2_Vref2_set, &BMS_Seg3_Vref2_set,
    &BMS_Seg4_Vref2_set, &BMS_Seg5_Vref2_set, &BMS_Seg6_Vref2_set, &BMS_Seg7_Vref2_set,
    &BMS_Seg8_Vref2_set, &BMS_Seg9_Vref2_set,
};

SegmentITMPSetters segment_itmp_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_ITMP_set, &BMS_Seg1_ITMP_set, &BMS_Seg2_ITMP_set, &BMS_Seg3_ITMP_set,
    &BMS_Seg4_ITMP_set, &BMS_Seg5_ITMP_set, &BMS_Seg6_ITMP_set, &BMS_Seg7_ITMP_set,
    &BMS_Seg8_ITMP_set, &BMS_Seg9_ITMP_set,
};

SegmentVDSetters segment_vd_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_VD_set, &BMS_Seg1_VD_set, &BMS_Seg2_VD_set, &BMS_Seg3_VD_set,
    &BMS_Seg4_VD_set, &BMS_Seg5_VD_set, &BMS_Seg6_VD_set, &BMS_Seg7_VD_set,
    &BMS_Seg8_VD_set, &BMS_Seg9_VD_set,
};

SegmentVASetters segment_va_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_VA_set, &BMS_Seg1_VA_set, &BMS_Seg2_VA_set, &BMS_Seg3_VA_set,
    &BMS_Seg4_VA_set, &BMS_Seg5_VA_set, &BMS_Seg6_VA_set, &BMS_Seg7_VA_set,
    &BMS_Seg8_VA_set, &BMS_Seg9_VA_set,
};

SegmentVRESSetters segment_vres_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_VRES_set, &BMS_Seg1_VRES_set, &BMS_Seg2_VRES_set, &BMS_Seg3_VRES_set,
    &BMS_Seg4_VRES_set, &BMS_Seg5_VRES_set, &BMS_Seg6_VRES_set, &BMS_Seg7_VRES_set,
    &BMS_Seg8_VRES_set, &BMS_Seg9_VRES_set,
};

SegmentVaOvSetters segment_va_ov_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_VA_OV_set, &BMS_Seg1_VA_OV_set, &BMS_Seg2_VA_OV_set, &BMS_Seg3_VA_OV_set,
    &BMS_Seg4_VA_OV_set, &BMS_Seg5_VA_OV_set, &BMS_Seg6_VA_OV_set, &BMS_Seg7_VA_OV_set,
    &BMS_Seg8_VA_OV_set, &BMS_Seg9_VA_OV_set,
};

SegmentVaUvSetters segment_va_uv_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_VA_UV_set, &BMS_Seg1_VA_UV_set, &BMS_Seg2_VA_UV_set, &BMS_Seg3_VA_UV_set,
    &BMS_Seg4_VA_UV_set, &BMS_Seg5_VA_UV_set, &BMS_Seg6_VA_UV_set, &BMS_Seg7_VA_UV_set,
    &BMS_Seg8_VA_UV_set, &BMS_Seg9_VA_UV_set,
};

SegmentVdOvSetters segment_vd_ov_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_VD_OV_set, &BMS_Seg1_VD_OV_set, &BMS_Seg2_VD_OV_set, &BMS_Seg3_VD_OV_set,
    &BMS_Seg4_VD_OV_set, &BMS_Seg5_VD_OV_set, &BMS_Seg6_VD_OV_set, &BMS_Seg7_VD_OV_set,
    &BMS_Seg8_VD_OV_set, &BMS_Seg9_VD_OV_set,
};

SegmentVdUvSetters segment_vd_uv_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_VD_UV_set, &BMS_Seg1_VD_UV_set, &BMS_Seg2_VD_UV_set, &BMS_Seg3_VD_UV_set,
    &BMS_Seg4_VD_UV_set, &BMS_Seg5_VD_UV_set, &BMS_Seg6_VD_UV_set, &BMS_Seg7_VD_UV_set,
    &BMS_Seg8_VD_UV_set, &BMS_Seg9_VD_UV_set,
};

SegmentCedSetters segment_ced_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_CED_set, &BMS_Seg1_CED_set, &BMS_Seg2_CED_set, &BMS_Seg3_CED_set,
    &BMS_Seg4_CED_set, &BMS_Seg5_CED_set, &BMS_Seg6_CED_set, &BMS_Seg7_CED_set,
    &BMS_Seg8_CED_set, &BMS_Seg9_CED_set,
};

SegmentCmedSetters segment_cmed_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_CMED_set, &BMS_Seg1_CMED_set, &BMS_Seg2_CMED_set, &BMS_Seg3_CMED_set,
    &BMS_Seg4_CMED_set, &BMS_Seg5_CMED_set, &BMS_Seg6_CMED_set, &BMS_Seg7_CMED_set,
    &BMS_Seg8_CMED_set, &BMS_Seg9_CMED_set,
};

SegmentSedSetters segment_sed_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_SED_set, &BMS_Seg1_SED_set, &BMS_Seg2_SED_set, &BMS_Seg3_SED_set,
    &BMS_Seg4_SED_set, &BMS_Seg5_SED_set, &BMS_Seg6_SED_set, &BMS_Seg7_SED_set,
    &BMS_Seg8_SED_set, &BMS_Seg9_SED_set,
};

SegmentSmedSetters segment_smed_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_SMED_set, &BMS_Seg1_SMED_set, &BMS_Seg2_SMED_set, &BMS_Seg3_SMED_set,
    &BMS_Seg4_SMED_set, &BMS_Seg5_SMED_set, &BMS_Seg6_SMED_set, &BMS_Seg7_SMED_set,
    &BMS_Seg8_SMED_set, &BMS_Seg9_SMED_set,
};

SegmentVdeSetters segment_vde_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_VDE_set, &BMS_Seg1_VDE_set, &BMS_Seg2_VDE_set, &BMS_Seg3_VDE_set,
    &BMS_Seg4_VDE_set, &BMS_Seg5_VDE_set, &BMS_Seg6_VDE_set, &BMS_Seg7_VDE_set,
    &BMS_Seg8_VDE_set, &BMS_Seg9_VDE_set,
};

SegmentVdelSetters segment_vdel_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_VDEL_set, &BMS_Seg1_VDEL_set, &BMS_Seg2_VDEL_set, &BMS_Seg3_VDEL_set,
    &BMS_Seg4_VDEL_set, &BMS_Seg5_VDEL_set, &BMS_Seg6_VDEL_set, &BMS_Seg7_VDEL_set,
    &BMS_Seg8_VDEL_set, &BMS_Seg9_VDEL_set,
};

SegmentThsdSetters segment_thsd_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_THSD_set, &BMS_Seg1_THSD_set, &BMS_Seg2_THSD_set, &BMS_Seg3_THSD_set,
    &BMS_Seg4_THSD_set, &BMS_Seg5_THSD_set, &BMS_Seg6_THSD_set, &BMS_Seg7_THSD_set,
    &BMS_Seg8_THSD_set, &BMS_Seg9_THSD_set,
};

SegmentTmodchkSetters segment_tmodchk_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_TMODCHK_set, &BMS_Seg1_TMODCHK_set, &BMS_Seg2_TMODCHK_set, &BMS_Seg3_TMODCHK_set,
    &BMS_Seg4_TMODCHK_set, &BMS_Seg5_TMODCHK_set, &BMS_Seg6_TMODCHK_set, &BMS_Seg7_TMODCHK_set,
    &BMS_Seg8_TMODCHK_set, &BMS_Seg9_TMODCHK_set,
};

SegmentOscchkSetters segment_oscchk_setters[MAX_NUM_SEGMENTS] = {
    &BMS_Seg0_OSCCHK_set, &BMS_Seg1_OSCCHK_set, &BMS_Seg2_OSCCHK_set, &BMS_Seg3_OSCCHK_set,
    &BMS_Seg4_OSCCHK_set, &BMS_Seg5_OSCCHK_set, &BMS_Seg6_OSCCHK_set, &BMS_Seg7_OSCCHK_set,
    &BMS_Seg8_OSCCHK_set, &BMS_Seg9_OSCCHK_set,
};

CellOvSetters cell_ov_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT] = {
    // Seg 0
    { &BMS_Seg0_Cell0_OV_set, &BMS_Seg0_Cell1_OV_set, &BMS_Seg0_Cell2_OV_set,
      &BMS_Seg0_Cell3_OV_set, &BMS_Seg0_Cell4_OV_set, &BMS_Seg0_Cell5_OV_set,
      &BMS_Seg0_Cell6_OV_set, &BMS_Seg0_Cell7_OV_set, &BMS_Seg0_Cell8_OV_set,
      &BMS_Seg0_Cell9_OV_set, &BMS_Seg0_Cell10_OV_set, &BMS_Seg0_Cell11_OV_set,
      &BMS_Seg0_Cell12_OV_set, &BMS_Seg0_Cell13_OV_set },
    // Seg 1
    { &BMS_Seg1_Cell0_OV_set, &BMS_Seg1_Cell1_OV_set, &BMS_Seg1_Cell2_OV_set,
      &BMS_Seg1_Cell3_OV_set, &BMS_Seg1_Cell4_OV_set, &BMS_Seg1_Cell5_OV_set,
      &BMS_Seg1_Cell6_OV_set, &BMS_Seg1_Cell7_OV_set, &BMS_Seg1_Cell8_OV_set,
      &BMS_Seg1_Cell9_OV_set, &BMS_Seg1_Cell10_OV_set, &BMS_Seg1_Cell11_OV_set,
      &BMS_Seg1_Cell12_OV_set, &BMS_Seg1_Cell13_OV_set },
    // Seg 2
    { &BMS_Seg2_Cell0_OV_set, &BMS_Seg2_Cell1_OV_set, &BMS_Seg2_Cell2_OV_set,
      &BMS_Seg2_Cell3_OV_set, &BMS_Seg2_Cell4_OV_set, &BMS_Seg2_Cell5_OV_set,
      &BMS_Seg2_Cell6_OV_set, &BMS_Seg2_Cell7_OV_set, &BMS_Seg2_Cell8_OV_set,
      &BMS_Seg2_Cell9_OV_set, &BMS_Seg2_Cell10_OV_set, &BMS_Seg2_Cell11_OV_set,
      &BMS_Seg2_Cell12_OV_set, &BMS_Seg2_Cell13_OV_set },
    // Seg 3
    { &BMS_Seg3_Cell0_OV_set, &BMS_Seg3_Cell1_OV_set, &BMS_Seg3_Cell2_OV_set,
      &BMS_Seg3_Cell3_OV_set, &BMS_Seg3_Cell4_OV_set, &BMS_Seg3_Cell5_OV_set,
      &BMS_Seg3_Cell6_OV_set, &BMS_Seg3_Cell7_OV_set, &BMS_Seg3_Cell8_OV_set,
      &BMS_Seg3_Cell9_OV_set, &BMS_Seg3_Cell10_OV_set, &BMS_Seg3_Cell11_OV_set,
      &BMS_Seg3_Cell12_OV_set, &BMS_Seg3_Cell13_OV_set },
    // Seg 4
    { &BMS_Seg4_Cell0_OV_set, &BMS_Seg4_Cell1_OV_set, &BMS_Seg4_Cell2_OV_set,
      &BMS_Seg4_Cell3_OV_set, &BMS_Seg4_Cell4_OV_set, &BMS_Seg4_Cell5_OV_set,
      &BMS_Seg4_Cell6_OV_set, &BMS_Seg4_Cell7_OV_set, &BMS_Seg4_Cell8_OV_set,
      &BMS_Seg4_Cell9_OV_set, &BMS_Seg4_Cell10_OV_set, &BMS_Seg4_Cell11_OV_set,
      &BMS_Seg4_Cell12_OV_set, &BMS_Seg4_Cell13_OV_set },
    // Seg 5
    { &BMS_Seg5_Cell0_OV_set, &BMS_Seg5_Cell1_OV_set, &BMS_Seg5_Cell2_OV_set,
      &BMS_Seg5_Cell3_OV_set, &BMS_Seg5_Cell4_OV_set, &BMS_Seg5_Cell5_OV_set,
      &BMS_Seg5_Cell6_OV_set, &BMS_Seg5_Cell7_OV_set, &BMS_Seg5_Cell8_OV_set,
      &BMS_Seg5_Cell9_OV_set, &BMS_Seg5_Cell10_OV_set, &BMS_Seg5_Cell11_OV_set,
      &BMS_Seg5_Cell12_OV_set, &BMS_Seg5_Cell13_OV_set },
    // Seg 6
    { &BMS_Seg6_Cell0_OV_set, &BMS_Seg6_Cell1_OV_set, &BMS_Seg6_Cell2_OV_set,
      &BMS_Seg6_Cell3_OV_set, &BMS_Seg6_Cell4_OV_set, &BMS_Seg6_Cell5_OV_set,
      &BMS_Seg6_Cell6_OV_set, &BMS_Seg6_Cell7_OV_set, &BMS_Seg6_Cell8_OV_set,
      &BMS_Seg6_Cell9_OV_set, &BMS_Seg6_Cell10_OV_set, &BMS_Seg6_Cell11_OV_set,
      &BMS_Seg6_Cell12_OV_set, &BMS_Seg6_Cell13_OV_set },
    // Seg 7
    { &BMS_Seg7_Cell0_OV_set, &BMS_Seg7_Cell1_OV_set, &BMS_Seg7_Cell2_OV_set,
      &BMS_Seg7_Cell3_OV_set, &BMS_Seg7_Cell4_OV_set, &BMS_Seg7_Cell5_OV_set,
      &BMS_Seg7_Cell6_OV_set, &BMS_Seg7_Cell7_OV_set, &BMS_Seg7_Cell8_OV_set,
      &BMS_Seg7_Cell9_OV_set, &BMS_Seg7_Cell10_OV_set, &BMS_Seg7_Cell11_OV_set,
      &BMS_Seg7_Cell12_OV_set, &BMS_Seg7_Cell13_OV_set },
    // Seg 8
    { &BMS_Seg8_Cell0_OV_set, &BMS_Seg8_Cell1_OV_set, &BMS_Seg8_Cell2_OV_set,
      &BMS_Seg8_Cell3_OV_set, &BMS_Seg8_Cell4_OV_set, &BMS_Seg8_Cell5_OV_set,
      &BMS_Seg8_Cell6_OV_set, &BMS_Seg8_Cell7_OV_set, &BMS_Seg8_Cell8_OV_set,
      &BMS_Seg8_Cell9_OV_set, &BMS_Seg8_Cell10_OV_set, &BMS_Seg8_Cell11_OV_set,
      &BMS_Seg8_Cell12_OV_set, &BMS_Seg8_Cell13_OV_set },
    // Seg 9
    { &BMS_Seg9_Cell0_OV_set, &BMS_Seg9_Cell1_OV_set, &BMS_Seg9_Cell2_OV_set,
      &BMS_Seg9_Cell3_OV_set, &BMS_Seg9_Cell4_OV_set, &BMS_Seg9_Cell5_OV_set,
      &BMS_Seg9_Cell6_OV_set, &BMS_Seg9_Cell7_OV_set, &BMS_Seg9_Cell8_OV_set,
      &BMS_Seg9_Cell9_OV_set, &BMS_Seg9_Cell10_OV_set, &BMS_Seg9_Cell11_OV_set,
      &BMS_Seg9_Cell12_OV_set, &BMS_Seg9_Cell13_OV_set },
};

CellUvSetters cell_uv_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT] = {
    // Seg 0
    { &BMS_Seg0_Cell0_UV_set, &BMS_Seg0_Cell1_UV_set, &BMS_Seg0_Cell2_UV_set,
      &BMS_Seg0_Cell3_UV_set, &BMS_Seg0_Cell4_UV_set, &BMS_Seg0_Cell5_UV_set,
      &BMS_Seg0_Cell6_UV_set, &BMS_Seg0_Cell7_UV_set, &BMS_Seg0_Cell8_UV_set,
      &BMS_Seg0_Cell9_UV_set, &BMS_Seg0_Cell10_UV_set, &BMS_Seg0_Cell11_UV_set,
      &BMS_Seg0_Cell12_UV_set, &BMS_Seg0_Cell13_UV_set },
    // Seg 1
    { &BMS_Seg1_Cell0_UV_set, &BMS_Seg1_Cell1_UV_set, &BMS_Seg1_Cell2_UV_set,
      &BMS_Seg1_Cell3_UV_set, &BMS_Seg1_Cell4_UV_set, &BMS_Seg1_Cell5_UV_set,
      &BMS_Seg1_Cell6_UV_set, &BMS_Seg1_Cell7_UV_set, &BMS_Seg1_Cell8_UV_set,
      &BMS_Seg1_Cell9_UV_set, &BMS_Seg1_Cell10_UV_set, &BMS_Seg1_Cell11_UV_set,
      &BMS_Seg1_Cell12_UV_set, &BMS_Seg1_Cell13_UV_set },
    // Seg 2
    { &BMS_Seg2_Cell0_UV_set, &BMS_Seg2_Cell1_UV_set, &BMS_Seg2_Cell2_UV_set,
      &BMS_Seg2_Cell3_UV_set, &BMS_Seg2_Cell4_UV_set, &BMS_Seg2_Cell5_UV_set,
      &BMS_Seg2_Cell6_UV_set, &BMS_Seg2_Cell7_UV_set, &BMS_Seg2_Cell8_UV_set,
      &BMS_Seg2_Cell9_UV_set, &BMS_Seg2_Cell10_UV_set, &BMS_Seg2_Cell11_UV_set,
      &BMS_Seg2_Cell12_UV_set, &BMS_Seg2_Cell13_UV_set },
    // Seg 3
    { &BMS_Seg3_Cell0_UV_set, &BMS_Seg3_Cell1_UV_set, &BMS_Seg3_Cell2_UV_set,
      &BMS_Seg3_Cell3_UV_set, &BMS_Seg3_Cell4_UV_set, &BMS_Seg3_Cell5_UV_set,
      &BMS_Seg3_Cell6_UV_set, &BMS_Seg3_Cell7_UV_set, &BMS_Seg3_Cell8_UV_set,
      &BMS_Seg3_Cell9_UV_set, &BMS_Seg3_Cell10_UV_set, &BMS_Seg3_Cell11_UV_set,
      &BMS_Seg3_Cell12_UV_set, &BMS_Seg3_Cell13_UV_set },
    // Seg 4
    { &BMS_Seg4_Cell0_UV_set, &BMS_Seg4_Cell1_UV_set, &BMS_Seg4_Cell2_UV_set,
      &BMS_Seg4_Cell3_UV_set, &BMS_Seg4_Cell4_UV_set, &BMS_Seg4_Cell5_UV_set,
      &BMS_Seg4_Cell6_UV_set, &BMS_Seg4_Cell7_UV_set, &BMS_Seg4_Cell8_UV_set,
      &BMS_Seg4_Cell9_UV_set, &BMS_Seg4_Cell10_UV_set, &BMS_Seg4_Cell11_UV_set,
      &BMS_Seg4_Cell12_UV_set, &BMS_Seg4_Cell13_UV_set },
    // Seg 5
    { &BMS_Seg5_Cell0_UV_set, &BMS_Seg5_Cell1_UV_set, &BMS_Seg5_Cell2_UV_set,
      &BMS_Seg5_Cell3_UV_set, &BMS_Seg5_Cell4_UV_set, &BMS_Seg5_Cell5_UV_set,
      &BMS_Seg5_Cell6_UV_set, &BMS_Seg5_Cell7_UV_set, &BMS_Seg5_Cell8_UV_set,
      &BMS_Seg5_Cell9_UV_set, &BMS_Seg5_Cell10_UV_set, &BMS_Seg5_Cell11_UV_set,
      &BMS_Seg5_Cell12_UV_set, &BMS_Seg5_Cell13_UV_set },
    // Seg 6
    { &BMS_Seg6_Cell0_UV_set, &BMS_Seg6_Cell1_UV_set, &BMS_Seg6_Cell2_UV_set,
      &BMS_Seg6_Cell3_UV_set, &BMS_Seg6_Cell4_UV_set, &BMS_Seg6_Cell5_UV_set,
      &BMS_Seg6_Cell6_UV_set, &BMS_Seg6_Cell7_UV_set, &BMS_Seg6_Cell8_UV_set,
      &BMS_Seg6_Cell9_UV_set, &BMS_Seg6_Cell10_UV_set, &BMS_Seg6_Cell11_UV_set,
      &BMS_Seg6_Cell12_UV_set, &BMS_Seg6_Cell13_UV_set },
    // Seg 7
    { &BMS_Seg7_Cell0_UV_set, &BMS_Seg7_Cell1_UV_set, &BMS_Seg7_Cell2_UV_set,
      &BMS_Seg7_Cell3_UV_set, &BMS_Seg7_Cell4_UV_set, &BMS_Seg7_Cell5_UV_set,
      &BMS_Seg7_Cell6_UV_set, &BMS_Seg7_Cell7_UV_set, &BMS_Seg7_Cell8_UV_set,
      &BMS_Seg7_Cell9_UV_set, &BMS_Seg7_Cell10_UV_set, &BMS_Seg7_Cell11_UV_set,
      &BMS_Seg7_Cell12_UV_set, &BMS_Seg7_Cell13_UV_set },
    // Seg 8
    { &BMS_Seg8_Cell0_UV_set, &BMS_Seg8_Cell1_UV_set, &BMS_Seg8_Cell2_UV_set,
      &BMS_Seg8_Cell3_UV_set, &BMS_Seg8_Cell4_UV_set, &BMS_Seg8_Cell5_UV_set,
      &BMS_Seg8_Cell6_UV_set, &BMS_Seg8_Cell7_UV_set, &BMS_Seg8_Cell8_UV_set,
      &BMS_Seg8_Cell9_UV_set, &BMS_Seg8_Cell10_UV_set, &BMS_Seg8_Cell11_UV_set,
      &BMS_Seg8_Cell12_UV_set, &BMS_Seg8_Cell13_UV_set },
    // Seg 9
    { &BMS_Seg9_Cell0_UV_set, &BMS_Seg9_Cell1_UV_set, &BMS_Seg9_Cell2_UV_set,
      &BMS_Seg9_Cell3_UV_set, &BMS_Seg9_Cell4_UV_set, &BMS_Seg9_Cell5_UV_set,
      &BMS_Seg9_Cell6_UV_set, &BMS_Seg9_Cell7_UV_set, &BMS_Seg9_Cell8_UV_set,
      &BMS_Seg9_Cell9_UV_set, &BMS_Seg9_Cell10_UV_set, &BMS_Seg9_Cell11_UV_set,
      &BMS_Seg9_Cell12_UV_set, &BMS_Seg9_Cell13_UV_set },
};

} // namespace app::segments
