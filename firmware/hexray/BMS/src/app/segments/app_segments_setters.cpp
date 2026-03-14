#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "app_canTx.hpp"

using namespace app::can_tx;
using namespace std;

namespace app::segments
{
    typedef void (*CommOkSetters)(bool);
    typedef void (*CellVoltageSetters)(float);
    typedef void (*CellTempSetters)(float);
    typedef void (*VrefSetters)(float);
    typedef void (*VrefOkSetters)(bool);
    typedef void (*CellOwcSetters)(bool);
    typedef void (*ThermOwcSetters)(bool);

    array<CommOkSetters, MAX_NUM_SEGMENTS> comm_ok_setters = {
        BMS_Seg0_CommOk_set, BMS_Seg1_CommOk_set, BMS_Seg2_CommOk_set, BMS_Seg3_CommOk_set, BMS_Seg4_CommOk_set,
        BMS_Seg5_CommOk_set, BMS_Seg6_CommOk_set, BMS_Seg7_CommOk_set, BMS_Seg8_CommOk_set, BMS_Seg9_CommOk_set
    };

    array<array<CellVoltageSetters, io::CELLS_PER_SEGMENT>, MAX_NUM_SEGMENTS> cell_voltage_setters = {
        {   BMS_Seg0_Cell0_Voltage_set, BMS_Seg0_Cell1_Voltage_set, BMS_Seg0_Cell2_Voltage_set, BMS_Seg0_Cell3_Voltage_set,
            BMS_Seg0_Cell4_Voltage_set, BMS_Seg0_Cell5_Voltage_set, BMS_Seg0_Cell6_Voltage_set, BMS_Seg0_Cell7_Voltage_set, BMS_Seg0_Cell8_Voltage_set,
            BMS_Seg0_Cell9_Voltage_set, BMS_Seg0_Cell10_Voltage_set, BMS_Seg0_Cell11_Voltage_set, BMS_Seg0_Cell12_Voltage_set, BMS_Seg0_Cell13_Voltage_set},
        {   BMS_Seg1_Cell0_Voltage_set, BMS_Seg1_Cell1_Voltage_set, BMS_Seg1_Cell2_Voltage_set, BMS_Seg1_Cell3_Voltage_set,
            BMS_Seg1_Cell4_Voltage_set, BMS_Seg1_Cell5_Voltage_set, BMS_Seg1_Cell6_Voltage_set, BMS_Seg1_Cell7_Voltage_set, BMS_Seg1_Cell8_Voltage_set,
            BMS_Seg1_Cell9_Voltage_set, BMS_Seg1_Cell10_Voltage_set, BMS_Seg1_Cell11_Voltage_set, BMS_Seg1_Cell12_Voltage_set, BMS_Seg1_Cell13_Voltage_set},
        {   BMS_Seg2_Cell0_Voltage_set, BMS_Seg2_Cell1_Voltage_set, BMS_Seg2_Cell2_Voltage_set, BMS_Seg2_Cell3_Voltage_set,
            BMS_Seg2_Cell4_Voltage_set, BMS_Seg2_Cell5_Voltage_set, BMS_Seg2_Cell6_Voltage_set, BMS_Seg2_Cell7_Voltage_set, BMS_Seg2_Cell8_Voltage_set,
            BMS_Seg2_Cell9_Voltage_set, BMS_Seg2_Cell10_Voltage_set, BMS_Seg2_Cell11_Voltage_set, BMS_Seg2_Cell12_Voltage_set, BMS_Seg2_Cell13_Voltage_set},
        {   BMS_Seg3_Cell0_Voltage_set, BMS_Seg3_Cell1_Voltage_set, BMS_Seg3_Cell2_Voltage_set, BMS_Seg3_Cell3_Voltage_set,
            BMS_Seg3_Cell4_Voltage_set, BMS_Seg3_Cell5_Voltage_set, BMS_Seg3_Cell6_Voltage_set, BMS_Seg3_Cell7_Voltage_set, BMS_Seg3_Cell8_Voltage_set,
            BMS_Seg3_Cell9_Voltage_set, BMS_Seg3_Cell10_Voltage_set, BMS_Seg3_Cell11_Voltage_set, BMS_Seg3_Cell12_Voltage_set, BMS_Seg3_Cell13_Voltage_set},
        {   BMS_Seg4_Cell0_Voltage_set, BMS_Seg4_Cell1_Voltage_set, BMS_Seg4_Cell2_Voltage_set, BMS_Seg4_Cell3_Voltage_set,
            BMS_Seg4_Cell4_Voltage_set, BMS_Seg4_Cell5_Voltage_set, BMS_Seg4_Cell6_Voltage_set, BMS_Seg4_Cell7_Voltage_set, BMS_Seg4_Cell8_Voltage_set,
            BMS_Seg4_Cell9_Voltage_set, BMS_Seg4_Cell10_Voltage_set, BMS_Seg4_Cell11_Voltage_set, BMS_Seg4_Cell12_Voltage_set, BMS_Seg4_Cell13_Voltage_set},
        {   BMS_Seg5_Cell0_Voltage_set, BMS_Seg5_Cell1_Voltage_set, BMS_Seg5_Cell2_Voltage_set, BMS_Seg5_Cell3_Voltage_set,
            BMS_Seg5_Cell4_Voltage_set, BMS_Seg5_Cell5_Voltage_set, BMS_Seg5_Cell6_Voltage_set, BMS_Seg5_Cell7_Voltage_set, BMS_Seg5_Cell8_Voltage_set,
            BMS_Seg5_Cell9_Voltage_set, BMS_Seg5_Cell10_Voltage_set, BMS_Seg5_Cell11_Voltage_set, BMS_Seg5_Cell12_Voltage_set, BMS_Seg5_Cell13_Voltage_set},
        {   BMS_Seg6_Cell0_Voltage_set, BMS_Seg6_Cell1_Voltage_set, BMS_Seg6_Cell2_Voltage_set, BMS_Seg6_Cell3_Voltage_set,
            BMS_Seg6_Cell4_Voltage_set, BMS_Seg6_Cell5_Voltage_set, BMS_Seg6_Cell6_Voltage_set, BMS_Seg6_Cell7_Voltage_set, BMS_Seg6_Cell8_Voltage_set,
            BMS_Seg6_Cell9_Voltage_set, BMS_Seg6_Cell10_Voltage_set, BMS_Seg6_Cell11_Voltage_set, BMS_Seg6_Cell12_Voltage_set, BMS_Seg6_Cell13_Voltage_set},
        {   BMS_Seg7_Cell0_Voltage_set, BMS_Seg7_Cell1_Voltage_set, BMS_Seg7_Cell2_Voltage_set, BMS_Seg7_Cell3_Voltage_set,
            BMS_Seg7_Cell4_Voltage_set, BMS_Seg7_Cell5_Voltage_set, BMS_Seg7_Cell6_Voltage_set, BMS_Seg7_Cell7_Voltage_set, BMS_Seg7_Cell8_Voltage_set,
            BMS_Seg7_Cell9_Voltage_set, BMS_Seg7_Cell10_Voltage_set, BMS_Seg7_Cell11_Voltage_set, BMS_Seg7_Cell12_Voltage_set, BMS_Seg7_Cell13_Voltage_set},
        {   BMS_Seg8_Cell0_Voltage_set, BMS_Seg8_Cell1_Voltage_set, BMS_Seg8_Cell2_Voltage_set, BMS_Seg8_Cell3_Voltage_set,
            BMS_Seg8_Cell4_Voltage_set, BMS_Seg8_Cell5_Voltage_set, BMS_Seg8_Cell6_Voltage_set, BMS_Seg8_Cell7_Voltage_set, BMS_Seg8_Cell8_Voltage_set,
            BMS_Seg8_Cell9_Voltage_set, BMS_Seg8_Cell10_Voltage_set, BMS_Seg8_Cell11_Voltage_set, BMS_Seg8_Cell12_Voltage_set, BMS_Seg8_Cell13_Voltage_set},
        {   BMS_Seg9_Cell0_Voltage_set, BMS_Seg9_Cell1_Voltage_set, BMS_Seg9_Cell2_Voltage_set, BMS_Seg9_Cell3_Voltage_set,
            BMS_Seg9_Cell4_Voltage_set, BMS_Seg9_Cell5_Voltage_set, BMS_Seg9_Cell6_Voltage_set, BMS_Seg9_Cell7_Voltage_set, BMS_Seg9_Cell8_Voltage_set,
            BMS_Seg9_Cell9_Voltage_set, BMS_Seg9_Cell10_Voltage_set, BMS_Seg9_Cell11_Voltage_set, BMS_Seg9_Cell12_Voltage_set, BMS_Seg9_Cell13_Voltage_set}
    };

    array<array<CellTempSetters, io::THERMISTORS_PER_SEGMENT>, MAX_NUM_SEGMENTS> cell_temp_setters = {
        {   BMS_Seg0_Cell0_Temp_set, BMS_Seg0_Cell1_Temp_set, BMS_Seg0_Cell2_Temp_set, BMS_Seg0_Cell3_Temp_set,
            BMS_Seg0_Cell4_Temp_set, BMS_Seg0_Cell5_Temp_set, BMS_Seg0_Cell6_Temp_set, BMS_Seg0_Cell7_Temp_set, BMS_Seg0_Cell8_Temp_set,
            BMS_Seg0_Cell9_Temp_set, BMS_Seg0_Cell10_Temp_set, BMS_Seg0_Cell11_Temp_set, BMS_Seg0_Cell12_Temp_set, BMS_Seg0_Cell13_Temp_set},
        {   BMS_Seg1_Cell0_Temp_set, BMS_Seg1_Cell1_Temp_set, BMS_Seg1_Cell2_Temp_set, BMS_Seg1_Cell3_Temp_set,
            BMS_Seg1_Cell4_Temp_set, BMS_Seg1_Cell5_Temp_set, BMS_Seg1_Cell6_Temp_set, BMS_Seg1_Cell7_Temp_set, BMS_Seg1_Cell8_Temp_set,
            BMS_Seg1_Cell9_Temp_set, BMS_Seg1_Cell10_Temp_set, BMS_Seg1_Cell11_Temp_set, BMS_Seg1_Cell12_Temp_set, BMS_Seg1_Cell13_Temp_set},
        {   BMS_Seg2_Cell0_Temp_set, BMS_Seg2_Cell1_Temp_set, BMS_Seg2_Cell2_Temp_set, BMS_Seg2_Cell3_Temp_set,
            BMS_Seg2_Cell4_Temp_set, BMS_Seg2_Cell5_Temp_set, BMS_Seg2_Cell6_Temp_set, BMS_Seg2_Cell7_Temp_set, BMS_Seg2_Cell8_Temp_set,
            BMS_Seg2_Cell9_Temp_set, BMS_Seg2_Cell10_Temp_set, BMS_Seg2_Cell11_Temp_set, BMS_Seg2_Cell12_Temp_set, BMS_Seg2_Cell13_Temp_set},
        {   BMS_Seg3_Cell0_Temp_set, BMS_Seg3_Cell1_Temp_set, BMS_Seg3_Cell2_Temp_set, BMS_Seg3_Cell3_Temp_set,
            BMS_Seg3_Cell4_Temp_set, BMS_Seg3_Cell5_Temp_set, BMS_Seg3_Cell6_Temp_set, BMS_Seg3_Cell7_Temp_set, BMS_Seg3_Cell8_Temp_set,
            BMS_Seg3_Cell9_Temp_set, BMS_Seg3_Cell10_Temp_set, BMS_Seg3_Cell11_Temp_set, BMS_Seg3_Cell12_Temp_set, BMS_Seg3_Cell13_Temp_set},
        {   BMS_Seg4_Cell0_Temp_set, BMS_Seg4_Cell1_Temp_set, BMS_Seg4_Cell2_Temp_set, BMS_Seg4_Cell3_Temp_set,
            BMS_Seg4_Cell4_Temp_set, BMS_Seg4_Cell5_Temp_set, BMS_Seg4_Cell6_Temp_set, BMS_Seg4_Cell7_Temp_set, BMS_Seg4_Cell8_Temp_set,
            BMS_Seg4_Cell9_Temp_set, BMS_Seg4_Cell10_Temp_set, BMS_Seg4_Cell11_Temp_set, BMS_Seg4_Cell12_Temp_set, BMS_Seg4_Cell13_Temp_set},
        {   BMS_Seg5_Cell0_Temp_set, BMS_Seg5_Cell1_Temp_set, BMS_Seg5_Cell2_Temp_set, BMS_Seg5_Cell3_Temp_set,
            BMS_Seg5_Cell4_Temp_set, BMS_Seg5_Cell5_Temp_set, BMS_Seg5_Cell6_Temp_set, BMS_Seg5_Cell7_Temp_set, BMS_Seg5_Cell8_Temp_set,
            BMS_Seg5_Cell9_Temp_set, BMS_Seg5_Cell10_Temp_set, BMS_Seg5_Cell11_Temp_set, BMS_Seg5_Cell12_Temp_set, BMS_Seg5_Cell13_Temp_set},
        {   BMS_Seg6_Cell0_Temp_set, BMS_Seg6_Cell1_Temp_set, BMS_Seg6_Cell2_Temp_set, BMS_Seg6_Cell3_Temp_set,
            BMS_Seg6_Cell4_Temp_set, BMS_Seg6_Cell5_Temp_set, BMS_Seg6_Cell6_Temp_set, BMS_Seg6_Cell7_Temp_set, BMS_Seg6_Cell8_Temp_set,
            BMS_Seg6_Cell9_Temp_set, BMS_Seg6_Cell10_Temp_set, BMS_Seg6_Cell11_Temp_set, BMS_Seg6_Cell12_Temp_set, BMS_Seg6_Cell13_Temp_set},
        {   BMS_Seg7_Cell0_Temp_set, BMS_Seg7_Cell1_Temp_set, BMS_Seg7_Cell2_Temp_set, BMS_Seg7_Cell3_Temp_set,
            BMS_Seg7_Cell4_Temp_set, BMS_Seg7_Cell5_Temp_set, BMS_Seg7_Cell6_Temp_set, BMS_Seg7_Cell7_Temp_set, BMS_Seg7_Cell8_Temp_set,
            BMS_Seg7_Cell9_Temp_set, BMS_Seg7_Cell10_Temp_set, BMS_Seg7_Cell11_Temp_set, BMS_Seg7_Cell12_Temp_set, BMS_Seg7_Cell13_Temp_set},
        {   BMS_Seg8_Cell0_Temp_set, BMS_Seg8_Cell1_Temp_set, BMS_Seg8_Cell2_Temp_set, BMS_Seg8_Cell3_Temp_set,
            BMS_Seg8_Cell4_Temp_set, BMS_Seg8_Cell5_Temp_set, BMS_Seg8_Cell6_Temp_set, BMS_Seg8_Cell7_Temp_set, BMS_Seg8_Cell8_Temp_set,
            BMS_Seg8_Cell9_Temp_set, BMS_Seg8_Cell10_Temp_set, BMS_Seg8_Cell11_Temp_set, BMS_Seg8_Cell12_Temp_set, BMS_Seg8_Cell13_Temp_set},
        {   BMS_Seg9_Cell0_Temp_set, BMS_Seg9_Cell1_Temp_set, BMS_Seg9_Cell2_Temp_set, BMS_Seg9_Cell3_Temp_set,
            BMS_Seg9_Cell4_Temp_set, BMS_Seg9_Cell5_Temp_set, BMS_Seg9_Cell6_Temp_set, BMS_Seg9_Cell7_Temp_set, BMS_Seg9_Cell8_Temp_set,
            BMS_Seg9_Cell9_Temp_set, BMS_Seg9_Cell10_Temp_set, BMS_Seg9_Cell11_Temp_set, BMS_Seg9_Cell12_Temp_set, BMS_Seg9_Cell13_Temp_set}
    };

}
