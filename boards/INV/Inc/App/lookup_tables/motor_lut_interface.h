#pragma once

#include <stdint.h>

#define LUT_NUM_ROWS 201
#define LUT_NUM_COLUMNS 21

enum
{
    MOTOR_TEMP_80C,
    MOTOR_TEMP_100C,
    MOTOR_TEMP_120C,
    LUT_NUM_TEMPS
};

enum
{
    BUS_VOLTAGE_400V,
    BUS_VOLTAGE_500V,
    BUS_VOLTAGE_600V,
    LUT_NUM_VOLTAGES
};

enum
{
    SHAFT_TORQUE,
    STATOR_CURRENT_PHASE_PEAK,
    STATOR_CURRENT_PHASE_RMS,
    STATOR_CURRENT_LINE_PEAK,
    STATOR_CURRENT_LINE_RMS,
    VOLTAGE_PHASE_PEAK,
    VOLTAGE_PHASE_RMS,
    VOLTAGE_LINE_PEAK,
    VOLTAGE_LINE_RMS,
    ID_PEAK,
    ID_RMS,
    IQ_PEAK,
    IQ_RMS,
    VD_PEAK,
    VD_RMS,
    VQ_PEAK,
    VQ_RMS,
    FUND_FREQ_HZ,
    TOTAL_LOSS,
    STATOR_COPPER_LOSS,
    IRON_LOSS,
    MAGNET_LOSS,
    MECHANICAL_LOSS,
    POWER_FACTOR,
    ELECTROMAGNETIC_TORQUE,
    LUT_NUM_PAGES
};

float look_up_value(
    float   speed_rpm,
    float   torque_request,
    float   bus_voltage,
    float   motor_temp,
    uint8_t table_number);
float intp_from_lut(
    uint8_t voltage_index,
    uint8_t temp_index,
    uint8_t torque_index,
    uint8_t speed_index,
    float   speed_inter_percentage,
    float   torque_inter_percentage,
    uint8_t table_number);
