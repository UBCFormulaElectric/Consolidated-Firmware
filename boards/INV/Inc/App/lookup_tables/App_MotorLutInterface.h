#pragma once

#include <stdint.h>

#define LUT_NUM_ROWS 200
#define LUT_NUM_COLUMNS 200
#define LUT_MAX_RPM 20000.0f
#define LUT_MAX_TORQUE 23.0f
#define RPM_PER_INTERVAL LUT_MAX_RPM / (float) LUT_NUM_ROWS
#define TORQUE_PER_INTERVAL LUT_MAX_TORQUE / (float) LUT_NUM_COLUMNS

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

float look_up_value(
        float   speed_rpm,
        float   torque_request,
        float max_stator_current,
        float   bus_voltage,
        uint8_t table_number);
float get_value_from_lut(uint8_t voltage_index, uint8_t torque_index, uint8_t speed_index, uint8_t table_number);
uint8_t calculate_derated_torque_index(float derated_stator_current, uint8_t voltage_index_low, int8_t voltage_index_high, uint8_t speed_index, float voltage_inter_percentage);

