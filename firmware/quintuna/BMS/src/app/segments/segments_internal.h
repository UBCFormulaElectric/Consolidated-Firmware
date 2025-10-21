#pragma once

#include "app/segments.h"
#include "app/utils.h"
#include "io/ltc6813.h"

#define MAX_NUM_SEGMENTS (10U)
#define CELL_TAPS_PER_SEGMENT (CELLS_PER_SEGMENT + 1)

/**
 * @file segments/app_segments_conversions.c
 */

extern uint16_t cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
extern ExitCode cell_voltage_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];
extern float    cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];

extern uint16_t aux_regs[THERMISTOR_MUX_COUNT][NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
extern ExitCode aux_regs_success[THERMISTOR_MUX_COUNT][NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
extern float    segment_vref[NUM_SEGMENTS];
extern float    cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];
extern ExitCode cell_temps_success[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];

extern StatusRegGroups status_regs[NUM_SEGMENTS];
extern ExitCode        status_regs_success[NUM_SEGMENTS];

extern uint16_t adc_accuracy_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
extern ExitCode adc_accuracy_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];

extern uint16_t voltage_self_test_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
extern ExitCode voltage_self_test_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];

extern uint16_t aux_self_test_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
extern ExitCode aux_self_test_reg_success[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];

extern StatusRegGroups status_self_test_regs[NUM_SEGMENTS];
extern ExitCode        status_self_test_success[NUM_SEGMENTS];

extern uint16_t owc_pucv_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
extern ExitCode owc_pucv_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];
extern uint16_t owc_pdcv_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
extern ExitCode owc_pdcv_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];
extern bool     owc_results[NUM_SEGMENTS][CELL_TAPS_PER_SEGMENT];
extern bool     owc_results_success[NUM_SEGMENTS][CELL_TAPS_PER_SEGMENT];

/**
 * @file segments/app_segments_broadcast.c
 */

extern float     pack_voltage;
extern CellParam max_cell_voltage;
extern CellParam min_cell_voltage;
extern CellParam max_cell_temp;
extern CellParam min_cell_temp;

/**
 * @file segments/app_segments_faults.c
 */

#define ALL_COMM_OK(segment) (voltage_comm_ok[segment] && temp_comm_ok[segment])

extern bool voltage_comm_ok[NUM_SEGMENTS];
extern bool temp_comm_ok[NUM_SEGMENTS];

/**
 * @file segments/app_segments_setters.c
 */

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
