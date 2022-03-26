#pragma once

#include <stdint.h>
#include "control/App_ControlLoop.h"

#define LUT_NUM_ROWS 200
#define LUT_NUM_COLUMNS 200
#define LUT_MAX_RPM 20000.0f
#define LUT_MAX_TORQUE 23.0f
#define RPM_PER_INTERVAL (LUT_MAX_RPM / (float)LUT_NUM_ROWS)
#define TORQUE_PER_INTERVAL (LUT_MAX_TORQUE / (float)LUT_NUM_COLUMNS)

enum
{
    BUS_VOLTAGE_400V,
    BUS_VOLTAGE_500V,
    BUS_VOLTAGE_600V,
    LUT_NUM_VOLTAGES
};

enum
{
    ID_PEAK,
    IQ_PEAK,
    IS_PEAK,
    LUT_NUM_PAGES,
};

void look_up_dqs_current_ref(
    float             speed_rpm,
    float             torque_request,
    float             bus_voltage,
    float             max_stator_current,
    struct DqsValues *dqs_ref_currents);
float intp_values_from_lut(
    uint8_t voltage_index_high,
    uint8_t voltage_index_low,
    float   voltage_inter_percentage,
    uint8_t torque_index,
    uint8_t speed_index,
    uint8_t table_number);
uint8_t calculate_derated_torque_index(
    float   derated_stator_current,
    uint8_t voltage_index_low,
    uint8_t voltage_index_high,
    uint8_t speed_index,
    float   voltage_inter_percentage);
// static const float torque_per_interval = LUT_MAX_TORQUE / (float)
// LUT_NUM_COLUMNS; static const float rpm_per_interval = LUT_MAX_RPM / (float)
// LUT_NUM_ROWS;