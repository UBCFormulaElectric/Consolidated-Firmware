#pragma once

#include "io_ltc6813.h"

#define MAX_NUM_SEGMENTS (10U)
#define CELL_TAPS_PER_SEGMENT (CELLS_PER_SEGMENT + 1)

// app_segments_configSync
extern void (*const comm_ok_setters[MAX_NUM_SEGMENTS])(bool);

// app_segments_broadcastCellVoltages
extern void (*const cell_voltage_setters[MAX_NUM_SEGMENTS][CELLS_PER_SEGMENT])(float);

// app_segments_broadcastTempsVRef
extern void (*const thermistor_setters[MAX_NUM_SEGMENTS][CELLS_PER_SEGMENT])(float);
extern void (*const vref_setters[MAX_NUM_SEGMENTS])(float);
extern void (*const segment_vref_ok_setters[MAX_NUM_SEGMENTS])(bool);

// app_segments_broadcastStatus
extern void (*const mux_test_setters[MAX_NUM_SEGMENTS])(bool);
extern void (*const analog_supply_ok_setters[MAX_NUM_SEGMENTS])(bool);
extern void (*const analog_supply_setters[MAX_NUM_SEGMENTS])(float);
extern void (*const digital_supply_ok_setters[MAX_NUM_SEGMENTS])(bool);
extern void (*const digital_supply_setters[MAX_NUM_SEGMENTS])(float);
extern void (*const rev_code_setters[MAX_NUM_SEGMENTS])(uint32_t);
extern void (*const thermal_ok_setters[MAX_NUM_SEGMENTS])(bool);
extern void (*const temp_setters[MAX_NUM_SEGMENTS])(uint32_t);

// app_segments_broadcastAdcAccuracyTest
extern void (*const segment_overlap_adc1_2_test_setters[MAX_NUM_SEGMENTS])(bool);
extern void (*const segment_overalap_adc2_3_test_setters[MAX_NUM_SEGMENTS])(bool);

// app_segments_broadcastVoltageSelfTest
extern void (*const segment_cell_self_test_ok_setters[MAX_NUM_SEGMENTS])(bool);

// app_segments_broadcastAuxSelfTest
extern void (*const segment_aux_self_test_ok_setters[MAX_NUM_SEGMENTS])(bool);

// app_segments_broadcastStatusSelfTest
extern void (*const segment_status_self_test_ok_setters[MAX_NUM_SEGMENTS])(bool);

// app_segments_broadcastOpenWireCheck
extern void (*const cell_owc_setters[MAX_NUM_SEGMENTS][CELL_TAPS_PER_SEGMENT])(bool);

// app_segments_broadcastVoltageStats
extern void (*const avg_segment_voltage_setters[MAX_NUM_SEGMENTS])(float);

// app_segments_broadcastTempStats
extern void (*const avg_segment_temp_setters[MAX_NUM_SEGMENTS])(float);
