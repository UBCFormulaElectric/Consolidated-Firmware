#include "app/segments_internal.h"
#include "app_canTx.h"

// app_segments_configSync
void (*const comm_ok_setters[MAX_NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Seg0_CommOk_set, app_canTx_BMS_Seg1_CommOk_set, app_canTx_BMS_Seg2_CommOk_set,
    app_canTx_BMS_Seg3_CommOk_set, app_canTx_BMS_Seg4_CommOk_set, app_canTx_BMS_Seg5_CommOk_set,
    app_canTx_BMS_Seg6_CommOk_set, app_canTx_BMS_Seg7_CommOk_set, app_canTx_BMS_Seg8_CommOk_set,
    app_canTx_BMS_Seg9_CommOk_set
};

// app_segments_broadcastCellVoltages
void (*const cell_voltage_setters[MAX_NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
    { app_canTx_BMS_Seg0_Cell0_Voltage_set, app_canTx_BMS_Seg0_Cell1_Voltage_set, app_canTx_BMS_Seg0_Cell2_Voltage_set,
      app_canTx_BMS_Seg0_Cell3_Voltage_set, app_canTx_BMS_Seg0_Cell4_Voltage_set, app_canTx_BMS_Seg0_Cell5_Voltage_set,
      app_canTx_BMS_Seg0_Cell6_Voltage_set, app_canTx_BMS_Seg0_Cell7_Voltage_set, app_canTx_BMS_Seg0_Cell8_Voltage_set,
      app_canTx_BMS_Seg0_Cell9_Voltage_set, app_canTx_BMS_Seg0_Cell10_Voltage_set,
      app_canTx_BMS_Seg0_Cell11_Voltage_set, app_canTx_BMS_Seg0_Cell12_Voltage_set,
      app_canTx_BMS_Seg0_Cell13_Voltage_set },
    { app_canTx_BMS_Seg1_Cell0_Voltage_set, app_canTx_BMS_Seg1_Cell1_Voltage_set, app_canTx_BMS_Seg1_Cell2_Voltage_set,
      app_canTx_BMS_Seg1_Cell3_Voltage_set, app_canTx_BMS_Seg1_Cell4_Voltage_set, app_canTx_BMS_Seg1_Cell5_Voltage_set,
      app_canTx_BMS_Seg1_Cell6_Voltage_set, app_canTx_BMS_Seg1_Cell7_Voltage_set, app_canTx_BMS_Seg1_Cell8_Voltage_set,
      app_canTx_BMS_Seg1_Cell9_Voltage_set, app_canTx_BMS_Seg1_Cell10_Voltage_set,
      app_canTx_BMS_Seg1_Cell11_Voltage_set, app_canTx_BMS_Seg1_Cell12_Voltage_set,
      app_canTx_BMS_Seg1_Cell13_Voltage_set },
    { app_canTx_BMS_Seg2_Cell0_Voltage_set, app_canTx_BMS_Seg2_Cell1_Voltage_set, app_canTx_BMS_Seg2_Cell2_Voltage_set,
      app_canTx_BMS_Seg2_Cell3_Voltage_set, app_canTx_BMS_Seg2_Cell4_Voltage_set, app_canTx_BMS_Seg2_Cell5_Voltage_set,
      app_canTx_BMS_Seg2_Cell6_Voltage_set, app_canTx_BMS_Seg2_Cell7_Voltage_set, app_canTx_BMS_Seg2_Cell8_Voltage_set,
      app_canTx_BMS_Seg2_Cell9_Voltage_set, app_canTx_BMS_Seg2_Cell10_Voltage_set,
      app_canTx_BMS_Seg2_Cell11_Voltage_set, app_canTx_BMS_Seg2_Cell12_Voltage_set,
      app_canTx_BMS_Seg2_Cell13_Voltage_set },
    { app_canTx_BMS_Seg3_Cell0_Voltage_set, app_canTx_BMS_Seg3_Cell1_Voltage_set, app_canTx_BMS_Seg3_Cell2_Voltage_set,
      app_canTx_BMS_Seg3_Cell3_Voltage_set, app_canTx_BMS_Seg3_Cell4_Voltage_set, app_canTx_BMS_Seg3_Cell5_Voltage_set,
      app_canTx_BMS_Seg3_Cell6_Voltage_set, app_canTx_BMS_Seg3_Cell7_Voltage_set, app_canTx_BMS_Seg3_Cell8_Voltage_set,
      app_canTx_BMS_Seg3_Cell9_Voltage_set, app_canTx_BMS_Seg3_Cell10_Voltage_set,
      app_canTx_BMS_Seg3_Cell11_Voltage_set, app_canTx_BMS_Seg3_Cell12_Voltage_set,
      app_canTx_BMS_Seg3_Cell13_Voltage_set },
    { app_canTx_BMS_Seg4_Cell0_Voltage_set, app_canTx_BMS_Seg4_Cell1_Voltage_set, app_canTx_BMS_Seg4_Cell2_Voltage_set,
      app_canTx_BMS_Seg4_Cell3_Voltage_set, app_canTx_BMS_Seg4_Cell4_Voltage_set, app_canTx_BMS_Seg4_Cell5_Voltage_set,
      app_canTx_BMS_Seg4_Cell6_Voltage_set, app_canTx_BMS_Seg4_Cell7_Voltage_set, app_canTx_BMS_Seg4_Cell8_Voltage_set,
      app_canTx_BMS_Seg4_Cell9_Voltage_set, app_canTx_BMS_Seg4_Cell10_Voltage_set,
      app_canTx_BMS_Seg4_Cell11_Voltage_set, app_canTx_BMS_Seg4_Cell12_Voltage_set,
      app_canTx_BMS_Seg4_Cell13_Voltage_set },
    { app_canTx_BMS_Seg5_Cell0_Voltage_set, app_canTx_BMS_Seg5_Cell1_Voltage_set, app_canTx_BMS_Seg5_Cell2_Voltage_set,
      app_canTx_BMS_Seg5_Cell3_Voltage_set, app_canTx_BMS_Seg5_Cell4_Voltage_set, app_canTx_BMS_Seg5_Cell5_Voltage_set,
      app_canTx_BMS_Seg5_Cell6_Voltage_set, app_canTx_BMS_Seg5_Cell7_Voltage_set, app_canTx_BMS_Seg5_Cell8_Voltage_set,
      app_canTx_BMS_Seg5_Cell9_Voltage_set, app_canTx_BMS_Seg5_Cell10_Voltage_set,
      app_canTx_BMS_Seg5_Cell11_Voltage_set, app_canTx_BMS_Seg5_Cell12_Voltage_set,
      app_canTx_BMS_Seg5_Cell13_Voltage_set },
    { app_canTx_BMS_Seg6_Cell0_Voltage_set, app_canTx_BMS_Seg6_Cell1_Voltage_set, app_canTx_BMS_Seg6_Cell2_Voltage_set,
      app_canTx_BMS_Seg6_Cell3_Voltage_set, app_canTx_BMS_Seg6_Cell4_Voltage_set, app_canTx_BMS_Seg6_Cell5_Voltage_set,
      app_canTx_BMS_Seg6_Cell6_Voltage_set, app_canTx_BMS_Seg6_Cell7_Voltage_set, app_canTx_BMS_Seg6_Cell8_Voltage_set,
      app_canTx_BMS_Seg6_Cell9_Voltage_set, app_canTx_BMS_Seg6_Cell10_Voltage_set,
      app_canTx_BMS_Seg6_Cell11_Voltage_set, app_canTx_BMS_Seg6_Cell12_Voltage_set,
      app_canTx_BMS_Seg6_Cell13_Voltage_set },
    { app_canTx_BMS_Seg7_Cell0_Voltage_set, app_canTx_BMS_Seg7_Cell1_Voltage_set, app_canTx_BMS_Seg7_Cell2_Voltage_set,
      app_canTx_BMS_Seg7_Cell3_Voltage_set, app_canTx_BMS_Seg7_Cell4_Voltage_set, app_canTx_BMS_Seg7_Cell5_Voltage_set,
      app_canTx_BMS_Seg7_Cell6_Voltage_set, app_canTx_BMS_Seg7_Cell7_Voltage_set, app_canTx_BMS_Seg7_Cell8_Voltage_set,
      app_canTx_BMS_Seg7_Cell9_Voltage_set, app_canTx_BMS_Seg7_Cell10_Voltage_set,
      app_canTx_BMS_Seg7_Cell11_Voltage_set, app_canTx_BMS_Seg7_Cell12_Voltage_set,
      app_canTx_BMS_Seg7_Cell13_Voltage_set },
    { app_canTx_BMS_Seg8_Cell0_Voltage_set, app_canTx_BMS_Seg8_Cell1_Voltage_set, app_canTx_BMS_Seg8_Cell2_Voltage_set,
      app_canTx_BMS_Seg8_Cell3_Voltage_set, app_canTx_BMS_Seg8_Cell4_Voltage_set, app_canTx_BMS_Seg8_Cell5_Voltage_set,
      app_canTx_BMS_Seg8_Cell6_Voltage_set, app_canTx_BMS_Seg8_Cell7_Voltage_set, app_canTx_BMS_Seg8_Cell8_Voltage_set,
      app_canTx_BMS_Seg8_Cell9_Voltage_set, app_canTx_BMS_Seg8_Cell10_Voltage_set,
      app_canTx_BMS_Seg8_Cell11_Voltage_set, app_canTx_BMS_Seg8_Cell12_Voltage_set,
      app_canTx_BMS_Seg8_Cell13_Voltage_set },
    { app_canTx_BMS_Seg9_Cell0_Voltage_set, app_canTx_BMS_Seg9_Cell1_Voltage_set, app_canTx_BMS_Seg9_Cell2_Voltage_set,
      app_canTx_BMS_Seg9_Cell3_Voltage_set, app_canTx_BMS_Seg9_Cell4_Voltage_set, app_canTx_BMS_Seg9_Cell5_Voltage_set,
      app_canTx_BMS_Seg9_Cell6_Voltage_set, app_canTx_BMS_Seg9_Cell7_Voltage_set, app_canTx_BMS_Seg9_Cell8_Voltage_set,
      app_canTx_BMS_Seg9_Cell9_Voltage_set, app_canTx_BMS_Seg9_Cell10_Voltage_set,
      app_canTx_BMS_Seg9_Cell11_Voltage_set, app_canTx_BMS_Seg9_Cell12_Voltage_set,
      app_canTx_BMS_Seg9_Cell13_Voltage_set }
};

// app_segments_broadcastTempsVRef
void (*const thermistor_setters[MAX_NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
    { app_canTx_BMS_Seg0_Cell0_Temp_set, app_canTx_BMS_Seg0_Cell1_Temp_set, app_canTx_BMS_Seg0_Cell2_Temp_set,
      app_canTx_BMS_Seg0_Cell3_Temp_set, app_canTx_BMS_Seg0_Cell4_Temp_set, app_canTx_BMS_Seg0_Cell5_Temp_set,
      app_canTx_BMS_Seg0_Cell6_Temp_set, app_canTx_BMS_Seg0_Cell7_Temp_set, app_canTx_BMS_Seg0_Cell8_Temp_set,
      app_canTx_BMS_Seg0_Cell9_Temp_set, app_canTx_BMS_Seg0_Cell10_Temp_set, app_canTx_BMS_Seg0_Cell11_Temp_set,
      app_canTx_BMS_Seg0_Cell12_Temp_set, app_canTx_BMS_Seg0_Cell13_Temp_set },
    { app_canTx_BMS_Seg1_Cell0_Temp_set, app_canTx_BMS_Seg1_Cell1_Temp_set, app_canTx_BMS_Seg1_Cell2_Temp_set,
      app_canTx_BMS_Seg1_Cell3_Temp_set, app_canTx_BMS_Seg1_Cell4_Temp_set, app_canTx_BMS_Seg1_Cell5_Temp_set,
      app_canTx_BMS_Seg1_Cell6_Temp_set, app_canTx_BMS_Seg1_Cell7_Temp_set, app_canTx_BMS_Seg1_Cell8_Temp_set,
      app_canTx_BMS_Seg1_Cell9_Temp_set, app_canTx_BMS_Seg1_Cell10_Temp_set, app_canTx_BMS_Seg1_Cell11_Temp_set,
      app_canTx_BMS_Seg1_Cell12_Temp_set, app_canTx_BMS_Seg1_Cell13_Temp_set },
    { app_canTx_BMS_Seg2_Cell0_Temp_set, app_canTx_BMS_Seg2_Cell1_Temp_set, app_canTx_BMS_Seg2_Cell2_Temp_set,
      app_canTx_BMS_Seg2_Cell3_Temp_set, app_canTx_BMS_Seg2_Cell4_Temp_set, app_canTx_BMS_Seg2_Cell5_Temp_set,
      app_canTx_BMS_Seg2_Cell6_Temp_set, app_canTx_BMS_Seg2_Cell7_Temp_set, app_canTx_BMS_Seg2_Cell8_Temp_set,
      app_canTx_BMS_Seg2_Cell9_Temp_set, app_canTx_BMS_Seg2_Cell10_Temp_set, app_canTx_BMS_Seg2_Cell11_Temp_set,
      app_canTx_BMS_Seg2_Cell12_Temp_set, app_canTx_BMS_Seg2_Cell13_Temp_set },
    { app_canTx_BMS_Seg3_Cell0_Temp_set, app_canTx_BMS_Seg3_Cell1_Temp_set, app_canTx_BMS_Seg3_Cell2_Temp_set,
      app_canTx_BMS_Seg3_Cell3_Temp_set, app_canTx_BMS_Seg3_Cell4_Temp_set, app_canTx_BMS_Seg3_Cell5_Temp_set,
      app_canTx_BMS_Seg3_Cell6_Temp_set, app_canTx_BMS_Seg3_Cell7_Temp_set, app_canTx_BMS_Seg3_Cell8_Temp_set,
      app_canTx_BMS_Seg3_Cell9_Temp_set, app_canTx_BMS_Seg3_Cell10_Temp_set, app_canTx_BMS_Seg3_Cell11_Temp_set,
      app_canTx_BMS_Seg3_Cell12_Temp_set, app_canTx_BMS_Seg3_Cell13_Temp_set },
    { app_canTx_BMS_Seg4_Cell0_Temp_set, app_canTx_BMS_Seg4_Cell1_Temp_set, app_canTx_BMS_Seg4_Cell2_Temp_set,
      app_canTx_BMS_Seg4_Cell3_Temp_set, app_canTx_BMS_Seg4_Cell4_Temp_set, app_canTx_BMS_Seg4_Cell5_Temp_set,
      app_canTx_BMS_Seg4_Cell6_Temp_set, app_canTx_BMS_Seg4_Cell7_Temp_set, app_canTx_BMS_Seg4_Cell8_Temp_set,
      app_canTx_BMS_Seg4_Cell9_Temp_set, app_canTx_BMS_Seg4_Cell10_Temp_set, app_canTx_BMS_Seg4_Cell11_Temp_set,
      app_canTx_BMS_Seg4_Cell12_Temp_set, app_canTx_BMS_Seg4_Cell13_Temp_set },
    { app_canTx_BMS_Seg5_Cell0_Temp_set, app_canTx_BMS_Seg5_Cell1_Temp_set, app_canTx_BMS_Seg5_Cell2_Temp_set,
      app_canTx_BMS_Seg5_Cell3_Temp_set, app_canTx_BMS_Seg5_Cell4_Temp_set, app_canTx_BMS_Seg5_Cell5_Temp_set,
      app_canTx_BMS_Seg5_Cell6_Temp_set, app_canTx_BMS_Seg5_Cell7_Temp_set, app_canTx_BMS_Seg5_Cell8_Temp_set,
      app_canTx_BMS_Seg5_Cell9_Temp_set, app_canTx_BMS_Seg5_Cell10_Temp_set, app_canTx_BMS_Seg5_Cell11_Temp_set,
      app_canTx_BMS_Seg5_Cell12_Temp_set, app_canTx_BMS_Seg5_Cell13_Temp_set },
    { app_canTx_BMS_Seg6_Cell0_Temp_set, app_canTx_BMS_Seg6_Cell1_Temp_set, app_canTx_BMS_Seg6_Cell2_Temp_set,
      app_canTx_BMS_Seg6_Cell3_Temp_set, app_canTx_BMS_Seg6_Cell4_Temp_set, app_canTx_BMS_Seg6_Cell5_Temp_set,
      app_canTx_BMS_Seg6_Cell6_Temp_set, app_canTx_BMS_Seg6_Cell7_Temp_set, app_canTx_BMS_Seg6_Cell8_Temp_set,
      app_canTx_BMS_Seg6_Cell9_Temp_set, app_canTx_BMS_Seg6_Cell10_Temp_set, app_canTx_BMS_Seg6_Cell11_Temp_set,
      app_canTx_BMS_Seg6_Cell12_Temp_set, app_canTx_BMS_Seg6_Cell13_Temp_set },
    { app_canTx_BMS_Seg7_Cell0_Temp_set, app_canTx_BMS_Seg7_Cell1_Temp_set, app_canTx_BMS_Seg7_Cell2_Temp_set,
      app_canTx_BMS_Seg7_Cell3_Temp_set, app_canTx_BMS_Seg7_Cell4_Temp_set, app_canTx_BMS_Seg7_Cell5_Temp_set,
      app_canTx_BMS_Seg7_Cell6_Temp_set, app_canTx_BMS_Seg7_Cell7_Temp_set, app_canTx_BMS_Seg7_Cell8_Temp_set,
      app_canTx_BMS_Seg7_Cell9_Temp_set, app_canTx_BMS_Seg7_Cell10_Temp_set, app_canTx_BMS_Seg7_Cell11_Temp_set,
      app_canTx_BMS_Seg7_Cell12_Temp_set, app_canTx_BMS_Seg7_Cell13_Temp_set },
    { app_canTx_BMS_Seg8_Cell0_Temp_set, app_canTx_BMS_Seg8_Cell1_Temp_set, app_canTx_BMS_Seg8_Cell2_Temp_set,
      app_canTx_BMS_Seg8_Cell3_Temp_set, app_canTx_BMS_Seg8_Cell4_Temp_set, app_canTx_BMS_Seg8_Cell5_Temp_set,
      app_canTx_BMS_Seg8_Cell6_Temp_set, app_canTx_BMS_Seg8_Cell7_Temp_set, app_canTx_BMS_Seg8_Cell8_Temp_set,
      app_canTx_BMS_Seg8_Cell9_Temp_set, app_canTx_BMS_Seg8_Cell10_Temp_set, app_canTx_BMS_Seg8_Cell11_Temp_set,
      app_canTx_BMS_Seg8_Cell12_Temp_set, app_canTx_BMS_Seg8_Cell13_Temp_set },
    { app_canTx_BMS_Seg9_Cell0_Temp_set, app_canTx_BMS_Seg9_Cell1_Temp_set, app_canTx_BMS_Seg9_Cell2_Temp_set,
      app_canTx_BMS_Seg9_Cell3_Temp_set, app_canTx_BMS_Seg9_Cell4_Temp_set, app_canTx_BMS_Seg9_Cell5_Temp_set,
      app_canTx_BMS_Seg9_Cell6_Temp_set, app_canTx_BMS_Seg9_Cell7_Temp_set, app_canTx_BMS_Seg9_Cell8_Temp_set,
      app_canTx_BMS_Seg9_Cell9_Temp_set, app_canTx_BMS_Seg9_Cell10_Temp_set, app_canTx_BMS_Seg9_Cell11_Temp_set,
      app_canTx_BMS_Seg9_Cell12_Temp_set, app_canTx_BMS_Seg9_Cell13_Temp_set }
};
void (*const vref_setters[MAX_NUM_SEGMENTS])(float) = { app_canTx_BMS_Seg0_Vref_set, app_canTx_BMS_Seg1_Vref_set,
                                                        app_canTx_BMS_Seg2_Vref_set, app_canTx_BMS_Seg3_Vref_set,
                                                        app_canTx_BMS_Seg4_Vref_set, app_canTx_BMS_Seg5_Vref_set,
                                                        app_canTx_BMS_Seg6_Vref_set, app_canTx_BMS_Seg7_Vref_set,
                                                        app_canTx_BMS_Seg8_Vref_set, app_canTx_BMS_Seg9_Vref_set };
void (*const segment_vref_ok_setters[MAX_NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Seg0_VrefOk_set, app_canTx_BMS_Seg1_VrefOk_set, app_canTx_BMS_Seg2_VrefOk_set,
    app_canTx_BMS_Seg3_VrefOk_set, app_canTx_BMS_Seg4_VrefOk_set, app_canTx_BMS_Seg5_VrefOk_set,
    app_canTx_BMS_Seg6_VrefOk_set, app_canTx_BMS_Seg7_VrefOk_set, app_canTx_BMS_Seg8_VrefOk_set,
    app_canTx_BMS_Seg9_VrefOk_set
};

// app_segments_broadcastStatus
void (*const mux_test_setters[MAX_NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_MuxOk_set, app_canTx_BMS_Seg1_MuxOk_set,
                                                           app_canTx_BMS_Seg2_MuxOk_set, app_canTx_BMS_Seg3_MuxOk_set,
                                                           app_canTx_BMS_Seg4_MuxOk_set, app_canTx_BMS_Seg5_MuxOk_set,
                                                           app_canTx_BMS_Seg6_MuxOk_set, app_canTx_BMS_Seg7_MuxOk_set,
                                                           app_canTx_BMS_Seg8_MuxOk_set, app_canTx_BMS_Seg9_MuxOk_set };
void (*const analog_supply_ok_setters[MAX_NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Seg0_AnalogSupplyOk_set, app_canTx_BMS_Seg1_AnalogSupplyOk_set, app_canTx_BMS_Seg2_AnalogSupplyOk_set,
    app_canTx_BMS_Seg3_AnalogSupplyOk_set, app_canTx_BMS_Seg4_AnalogSupplyOk_set, app_canTx_BMS_Seg5_AnalogSupplyOk_set,
    app_canTx_BMS_Seg6_AnalogSupplyOk_set, app_canTx_BMS_Seg7_AnalogSupplyOk_set, app_canTx_BMS_Seg8_AnalogSupplyOk_set,
    app_canTx_BMS_Seg9_AnalogSupplyOk_set
};
void (*const analog_supply_setters[MAX_NUM_SEGMENTS])(float) = {
    app_canTx_BMS_Seg0_AnalogSupply_set, app_canTx_BMS_Seg1_AnalogSupply_set, app_canTx_BMS_Seg2_AnalogSupply_set,
    app_canTx_BMS_Seg3_AnalogSupply_set, app_canTx_BMS_Seg4_AnalogSupply_set, app_canTx_BMS_Seg5_AnalogSupply_set,
    app_canTx_BMS_Seg6_AnalogSupply_set, app_canTx_BMS_Seg7_AnalogSupply_set, app_canTx_BMS_Seg8_AnalogSupply_set,
    app_canTx_BMS_Seg9_AnalogSupply_set
};
void (*const digital_supply_ok_setters[MAX_NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Seg0_DigitalSupplyOk_set, app_canTx_BMS_Seg1_DigitalSupplyOk_set,
    app_canTx_BMS_Seg2_DigitalSupplyOk_set, app_canTx_BMS_Seg3_DigitalSupplyOk_set,
    app_canTx_BMS_Seg4_DigitalSupplyOk_set, app_canTx_BMS_Seg5_DigitalSupplyOk_set,
    app_canTx_BMS_Seg6_DigitalSupplyOk_set, app_canTx_BMS_Seg7_DigitalSupplyOk_set,
    app_canTx_BMS_Seg8_DigitalSupplyOk_set, app_canTx_BMS_Seg9_DigitalSupplyOk_set
};
void (*const digital_supply_setters[MAX_NUM_SEGMENTS])(float) = {
    app_canTx_BMS_Seg0_DigitalSupply_set, app_canTx_BMS_Seg1_DigitalSupply_set, app_canTx_BMS_Seg2_DigitalSupply_set,
    app_canTx_BMS_Seg3_DigitalSupply_set, app_canTx_BMS_Seg4_DigitalSupply_set, app_canTx_BMS_Seg5_DigitalSupply_set,
    app_canTx_BMS_Seg6_DigitalSupply_set, app_canTx_BMS_Seg7_DigitalSupply_set, app_canTx_BMS_Seg8_DigitalSupply_set,
    app_canTx_BMS_Seg9_DigitalSupply_set
};
void (*const rev_code_setters[MAX_NUM_SEGMENTS])(uint32_t) = {
    app_canTx_BMS_Seg0_RevisionCode_set, app_canTx_BMS_Seg1_RevisionCode_set, app_canTx_BMS_Seg2_RevisionCode_set,
    app_canTx_BMS_Seg3_RevisionCode_set, app_canTx_BMS_Seg4_RevisionCode_set, app_canTx_BMS_Seg5_RevisionCode_set,
    app_canTx_BMS_Seg6_RevisionCode_set, app_canTx_BMS_Seg7_RevisionCode_set, app_canTx_BMS_Seg8_RevisionCode_set,
    app_canTx_BMS_Seg9_RevisionCode_set
};
void (*const thermal_ok_setters[MAX_NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Seg0_ThermalOk_set, app_canTx_BMS_Seg1_ThermalOk_set, app_canTx_BMS_Seg2_ThermalOk_set,
    app_canTx_BMS_Seg3_ThermalOk_set, app_canTx_BMS_Seg4_ThermalOk_set, app_canTx_BMS_Seg5_ThermalOk_set,
    app_canTx_BMS_Seg6_ThermalOk_set, app_canTx_BMS_Seg7_ThermalOk_set, app_canTx_BMS_Seg8_ThermalOk_set,
    app_canTx_BMS_Seg9_ThermalOk_set
};
void (*const temp_setters[MAX_NUM_SEGMENTS])(uint32_t) = {
    app_canTx_BMS_Seg0_DieTemp_set, app_canTx_BMS_Seg1_DieTemp_set, app_canTx_BMS_Seg2_DieTemp_set,
    app_canTx_BMS_Seg3_DieTemp_set, app_canTx_BMS_Seg4_DieTemp_set, app_canTx_BMS_Seg5_DieTemp_set,
    app_canTx_BMS_Seg6_DieTemp_set, app_canTx_BMS_Seg7_DieTemp_set, app_canTx_BMS_Seg8_DieTemp_set,
    app_canTx_BMS_Seg9_DieTemp_set
};

// app_segments_broadcastAdcAccuracyTest
void (*const segment_overlap_adc1_2_test_setters[MAX_NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Seg0_Adc12Equal_set, app_canTx_BMS_Seg1_Adc12Equal_set, app_canTx_BMS_Seg2_Adc12Equal_set,
    app_canTx_BMS_Seg3_Adc12Equal_set, app_canTx_BMS_Seg4_Adc12Equal_set, app_canTx_BMS_Seg5_Adc12Equal_set,
    app_canTx_BMS_Seg6_Adc12Equal_set, app_canTx_BMS_Seg7_Adc12Equal_set, app_canTx_BMS_Seg8_Adc12Equal_set,
    app_canTx_BMS_Seg9_Adc12Equal_set
};
void (*const segment_overalap_adc2_3_test_setters[MAX_NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Seg0_Adc23Equal_set, app_canTx_BMS_Seg1_Adc23Equal_set, app_canTx_BMS_Seg2_Adc23Equal_set,
    app_canTx_BMS_Seg3_Adc23Equal_set, app_canTx_BMS_Seg4_Adc23Equal_set, app_canTx_BMS_Seg5_Adc23Equal_set,
    app_canTx_BMS_Seg6_Adc23Equal_set, app_canTx_BMS_Seg7_Adc23Equal_set, app_canTx_BMS_Seg8_Adc23Equal_set,
    app_canTx_BMS_Seg9_Adc23Equal_set
};

// app_segments_broadcastVoltageSelfTest
void (*const segment_cell_self_test_ok_setters[MAX_NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Seg0_CellSelfTestOk_set, app_canTx_BMS_Seg1_CellSelfTestOk_set, app_canTx_BMS_Seg2_CellSelfTestOk_set,
    app_canTx_BMS_Seg3_CellSelfTestOk_set, app_canTx_BMS_Seg4_CellSelfTestOk_set, app_canTx_BMS_Seg5_CellSelfTestOk_set,
    app_canTx_BMS_Seg6_CellSelfTestOk_set, app_canTx_BMS_Seg7_CellSelfTestOk_set, app_canTx_BMS_Seg8_CellSelfTestOk_set,
    app_canTx_BMS_Seg9_CellSelfTestOk_set
};

// app_segments_broadcastAuxSelfTest
void (*const segment_aux_self_test_ok_setters[MAX_NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Seg0_AuxSelfTestOk_set, app_canTx_BMS_Seg1_AuxSelfTestOk_set, app_canTx_BMS_Seg2_AuxSelfTestOk_set,
    app_canTx_BMS_Seg3_AuxSelfTestOk_set, app_canTx_BMS_Seg4_AuxSelfTestOk_set, app_canTx_BMS_Seg5_AuxSelfTestOk_set,
    app_canTx_BMS_Seg6_AuxSelfTestOk_set, app_canTx_BMS_Seg7_AuxSelfTestOk_set, app_canTx_BMS_Seg8_AuxSelfTestOk_set,
    app_canTx_BMS_Seg9_AuxSelfTestOk_set
};

// app_segments_broadcastStatusSelfTest
void (*const segment_status_self_test_ok_setters[MAX_NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Seg0_StatusSelfTestOk_set, app_canTx_BMS_Seg1_StatusSelfTestOk_set,
    app_canTx_BMS_Seg2_StatusSelfTestOk_set, app_canTx_BMS_Seg3_StatusSelfTestOk_set,
    app_canTx_BMS_Seg4_StatusSelfTestOk_set, app_canTx_BMS_Seg5_StatusSelfTestOk_set,
    app_canTx_BMS_Seg6_StatusSelfTestOk_set, app_canTx_BMS_Seg7_StatusSelfTestOk_set,
    app_canTx_BMS_Seg8_StatusSelfTestOk_set, app_canTx_BMS_Seg9_StatusSelfTestOk_set
};

// app_segments_broadcastOpenWireCheck
void (*const cell_owc_setters[MAX_NUM_SEGMENTS][CELL_TAPS_PER_SEGMENT])(bool) = {
    { app_canTx_BMS_Seg0_C0_OwcOk_set, app_canTx_BMS_Seg0_C1_OwcOk_set, app_canTx_BMS_Seg0_C2_OwcOk_set,
      app_canTx_BMS_Seg0_C3_OwcOk_set, app_canTx_BMS_Seg0_C4_OwcOk_set, app_canTx_BMS_Seg0_C5_OwcOk_set,
      app_canTx_BMS_Seg0_C6_OwcOk_set, app_canTx_BMS_Seg0_C7_OwcOk_set, app_canTx_BMS_Seg0_C8_OwcOk_set,
      app_canTx_BMS_Seg0_C9_OwcOk_set, app_canTx_BMS_Seg0_C10_OwcOk_set, app_canTx_BMS_Seg0_C11_OwcOk_set,
      app_canTx_BMS_Seg0_C12_OwcOk_set, app_canTx_BMS_Seg0_C13_OwcOk_set, app_canTx_BMS_Seg0_C14_OwcOk_set },
    { app_canTx_BMS_Seg1_C0_OwcOk_set, app_canTx_BMS_Seg1_C1_OwcOk_set, app_canTx_BMS_Seg1_C2_OwcOk_set,
      app_canTx_BMS_Seg1_C3_OwcOk_set, app_canTx_BMS_Seg1_C4_OwcOk_set, app_canTx_BMS_Seg1_C5_OwcOk_set,
      app_canTx_BMS_Seg1_C6_OwcOk_set, app_canTx_BMS_Seg1_C7_OwcOk_set, app_canTx_BMS_Seg1_C8_OwcOk_set,
      app_canTx_BMS_Seg1_C9_OwcOk_set, app_canTx_BMS_Seg1_C10_OwcOk_set, app_canTx_BMS_Seg1_C11_OwcOk_set,
      app_canTx_BMS_Seg1_C12_OwcOk_set, app_canTx_BMS_Seg1_C13_OwcOk_set, app_canTx_BMS_Seg1_C14_OwcOk_set },
    { app_canTx_BMS_Seg2_C0_OwcOk_set, app_canTx_BMS_Seg2_C1_OwcOk_set, app_canTx_BMS_Seg2_C2_OwcOk_set,
      app_canTx_BMS_Seg2_C3_OwcOk_set, app_canTx_BMS_Seg2_C4_OwcOk_set, app_canTx_BMS_Seg2_C5_OwcOk_set,
      app_canTx_BMS_Seg2_C6_OwcOk_set, app_canTx_BMS_Seg2_C7_OwcOk_set, app_canTx_BMS_Seg2_C8_OwcOk_set,
      app_canTx_BMS_Seg2_C9_OwcOk_set, app_canTx_BMS_Seg2_C10_OwcOk_set, app_canTx_BMS_Seg2_C11_OwcOk_set,
      app_canTx_BMS_Seg2_C12_OwcOk_set, app_canTx_BMS_Seg2_C13_OwcOk_set, app_canTx_BMS_Seg2_C14_OwcOk_set },
    { app_canTx_BMS_Seg3_C0_OwcOk_set, app_canTx_BMS_Seg3_C1_OwcOk_set, app_canTx_BMS_Seg3_C2_OwcOk_set,
      app_canTx_BMS_Seg3_C3_OwcOk_set, app_canTx_BMS_Seg3_C4_OwcOk_set, app_canTx_BMS_Seg3_C5_OwcOk_set,
      app_canTx_BMS_Seg3_C6_OwcOk_set, app_canTx_BMS_Seg3_C7_OwcOk_set, app_canTx_BMS_Seg3_C8_OwcOk_set,
      app_canTx_BMS_Seg3_C9_OwcOk_set, app_canTx_BMS_Seg3_C10_OwcOk_set, app_canTx_BMS_Seg3_C11_OwcOk_set,
      app_canTx_BMS_Seg3_C12_OwcOk_set, app_canTx_BMS_Seg3_C13_OwcOk_set, app_canTx_BMS_Seg3_C14_OwcOk_set },
    { app_canTx_BMS_Seg4_C0_OwcOk_set, app_canTx_BMS_Seg4_C1_OwcOk_set, app_canTx_BMS_Seg4_C2_OwcOk_set,
      app_canTx_BMS_Seg4_C3_OwcOk_set, app_canTx_BMS_Seg4_C4_OwcOk_set, app_canTx_BMS_Seg4_C5_OwcOk_set,
      app_canTx_BMS_Seg4_C6_OwcOk_set, app_canTx_BMS_Seg4_C7_OwcOk_set, app_canTx_BMS_Seg4_C8_OwcOk_set,
      app_canTx_BMS_Seg4_C9_OwcOk_set, app_canTx_BMS_Seg4_C10_OwcOk_set, app_canTx_BMS_Seg4_C11_OwcOk_set,
      app_canTx_BMS_Seg4_C12_OwcOk_set, app_canTx_BMS_Seg4_C13_OwcOk_set, app_canTx_BMS_Seg4_C14_OwcOk_set },
    { app_canTx_BMS_Seg5_C0_OwcOk_set, app_canTx_BMS_Seg5_C1_OwcOk_set, app_canTx_BMS_Seg5_C2_OwcOk_set,
      app_canTx_BMS_Seg5_C3_OwcOk_set, app_canTx_BMS_Seg5_C4_OwcOk_set, app_canTx_BMS_Seg5_C5_OwcOk_set,
      app_canTx_BMS_Seg5_C6_OwcOk_set, app_canTx_BMS_Seg5_C7_OwcOk_set, app_canTx_BMS_Seg5_C8_OwcOk_set,
      app_canTx_BMS_Seg5_C9_OwcOk_set, app_canTx_BMS_Seg5_C10_OwcOk_set, app_canTx_BMS_Seg5_C11_OwcOk_set,
      app_canTx_BMS_Seg5_C12_OwcOk_set, app_canTx_BMS_Seg5_C13_OwcOk_set, app_canTx_BMS_Seg5_C14_OwcOk_set },
    { app_canTx_BMS_Seg6_C0_OwcOk_set, app_canTx_BMS_Seg6_C1_OwcOk_set, app_canTx_BMS_Seg6_C2_OwcOk_set,
      app_canTx_BMS_Seg6_C3_OwcOk_set, app_canTx_BMS_Seg6_C4_OwcOk_set, app_canTx_BMS_Seg6_C5_OwcOk_set,
      app_canTx_BMS_Seg6_C6_OwcOk_set, app_canTx_BMS_Seg6_C7_OwcOk_set, app_canTx_BMS_Seg6_C8_OwcOk_set,
      app_canTx_BMS_Seg6_C9_OwcOk_set, app_canTx_BMS_Seg6_C10_OwcOk_set, app_canTx_BMS_Seg6_C11_OwcOk_set,
      app_canTx_BMS_Seg6_C12_OwcOk_set, app_canTx_BMS_Seg6_C13_OwcOk_set, app_canTx_BMS_Seg6_C14_OwcOk_set },
    { app_canTx_BMS_Seg7_C0_OwcOk_set, app_canTx_BMS_Seg7_C1_OwcOk_set, app_canTx_BMS_Seg7_C2_OwcOk_set,
      app_canTx_BMS_Seg7_C3_OwcOk_set, app_canTx_BMS_Seg7_C4_OwcOk_set, app_canTx_BMS_Seg7_C5_OwcOk_set,
      app_canTx_BMS_Seg7_C6_OwcOk_set, app_canTx_BMS_Seg7_C7_OwcOk_set, app_canTx_BMS_Seg7_C8_OwcOk_set,
      app_canTx_BMS_Seg7_C9_OwcOk_set, app_canTx_BMS_Seg7_C10_OwcOk_set, app_canTx_BMS_Seg7_C11_OwcOk_set,
      app_canTx_BMS_Seg7_C12_OwcOk_set, app_canTx_BMS_Seg7_C13_OwcOk_set, app_canTx_BMS_Seg7_C14_OwcOk_set },
    { app_canTx_BMS_Seg8_C0_OwcOk_set, app_canTx_BMS_Seg8_C1_OwcOk_set, app_canTx_BMS_Seg8_C2_OwcOk_set,
      app_canTx_BMS_Seg8_C3_OwcOk_set, app_canTx_BMS_Seg8_C4_OwcOk_set, app_canTx_BMS_Seg8_C5_OwcOk_set,
      app_canTx_BMS_Seg8_C6_OwcOk_set, app_canTx_BMS_Seg8_C7_OwcOk_set, app_canTx_BMS_Seg8_C8_OwcOk_set,
      app_canTx_BMS_Seg8_C9_OwcOk_set, app_canTx_BMS_Seg8_C10_OwcOk_set, app_canTx_BMS_Seg8_C11_OwcOk_set,
      app_canTx_BMS_Seg8_C12_OwcOk_set, app_canTx_BMS_Seg8_C13_OwcOk_set, app_canTx_BMS_Seg8_C14_OwcOk_set },
    { app_canTx_BMS_Seg9_C0_OwcOk_set, app_canTx_BMS_Seg9_C1_OwcOk_set, app_canTx_BMS_Seg9_C2_OwcOk_set,
      app_canTx_BMS_Seg9_C3_OwcOk_set, app_canTx_BMS_Seg9_C4_OwcOk_set, app_canTx_BMS_Seg9_C5_OwcOk_set,
      app_canTx_BMS_Seg9_C6_OwcOk_set, app_canTx_BMS_Seg9_C7_OwcOk_set, app_canTx_BMS_Seg9_C8_OwcOk_set,
      app_canTx_BMS_Seg9_C9_OwcOk_set, app_canTx_BMS_Seg9_C10_OwcOk_set, app_canTx_BMS_Seg9_C11_OwcOk_set,
      app_canTx_BMS_Seg9_C12_OwcOk_set, app_canTx_BMS_Seg9_C13_OwcOk_set, app_canTx_BMS_Seg9_C14_OwcOk_set }
};

// app_segments_broadcastVoltageStats
void (*const avg_segment_voltage_setters[MAX_NUM_SEGMENTS])(float) = {
    app_canTx_BMS_Seg0_AvgVoltage_set, app_canTx_BMS_Seg1_AvgVoltage_set, app_canTx_BMS_Seg2_AvgVoltage_set,
    app_canTx_BMS_Seg3_AvgVoltage_set, app_canTx_BMS_Seg4_AvgVoltage_set, app_canTx_BMS_Seg5_AvgVoltage_set,
    app_canTx_BMS_Seg6_AvgVoltage_set, app_canTx_BMS_Seg7_AvgVoltage_set, app_canTx_BMS_Seg8_AvgVoltage_set,
    app_canTx_BMS_Seg9_AvgVoltage_set
};

// app_segments_broadcastTempStats
void (*const avg_segment_temp_setters[MAX_NUM_SEGMENTS])(float) = {
    app_canTx_BMS_Seg0_AvgTemp_set, app_canTx_BMS_Seg1_AvgTemp_set, app_canTx_BMS_Seg2_AvgTemp_set,
    app_canTx_BMS_Seg3_AvgTemp_set, app_canTx_BMS_Seg4_AvgTemp_set, app_canTx_BMS_Seg5_AvgTemp_set,
    app_canTx_BMS_Seg6_AvgTemp_set, app_canTx_BMS_Seg7_AvgTemp_set, app_canTx_BMS_Seg8_AvgTemp_set,
    app_canTx_BMS_Seg9_AvgTemp_set
};
