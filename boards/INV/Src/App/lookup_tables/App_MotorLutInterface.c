#include "lookup_tables/App_MotorLutInterface.h"
#include "configs/App_ControlSystemConfig.h"
#include "lookup_tables/id_peak_lut.h"
#include "lookup_tables/iq_peak_lut.h"
#include "lookup_tables/is_peak_lut.h"
#include <math.h>

float look_up_value(
    const float   speed_rpm,
    const float   torque_request,
    const float   bus_voltage,
    const float max_stator_current,
    const uint8_t table_number) {
    uint8_t speed_index;
    uint8_t torque_index;

    //Always round to lower torque/speed interval operating region
    torque_index = (uint8_t) (fabsf(torque_request) / TORQUE_PER_INTERVAL);
    speed_index = (uint8_t) (fabsf(speed_rpm) / RPM_PER_INTERVAL);

    float voltage_inter_percentage = 0.0f;
    float value_from_table = 0.0f;
    uint8_t derated_torque_index = 0;

    float intp1_val, intp2_val;

    //Take values directly from 400V table
    if (bus_voltage < 400.0f) {
        voltage_inter_percentage = 0.0f;
        derated_torque_index = calculate_derated_torque_index(max_stator_current, BUS_VOLTAGE_400V, BUS_VOLTAGE_400V, speed_index, voltage_inter_percentage);
        intp1_val = get_value_from_lut(BUS_VOLTAGE_400V, derated_torque_index, speed_index, table_number);
        value_from_table = intp1_val;
    }
    //Interpolate between 400V/500V tables
    else if (bus_voltage >= 400.0f && bus_voltage < 500.0f) {
        voltage_inter_percentage = (bus_voltage - 400.0f) / 100.0f;
        derated_torque_index = calculate_derated_torque_index(max_stator_current, BUS_VOLTAGE_400V, BUS_VOLTAGE_500V, speed_index, voltage_inter_percentage);
        intp2_val = get_value_from_lut(BUS_VOLTAGE_500V, derated_torque_index, speed_index, table_number);
        intp1_val = get_value_from_lut(BUS_VOLTAGE_400V, derated_torque_index, speed_index, table_number);
        value_from_table = (intp2_val - intp1_val)*voltage_inter_percentage + intp1_val;
    }
    //Interpolate between 500V/600V tables
    else if (bus_voltage >= 500.0f && bus_voltage < 600.0f) {
        voltage_inter_percentage = (bus_voltage - 500.0f) / 100.0f;
        derated_torque_index = calculate_derated_torque_index(max_stator_current, BUS_VOLTAGE_500V, BUS_VOLTAGE_600V, speed_index, voltage_inter_percentage);
        intp2_val = get_value_from_lut(BUS_VOLTAGE_600V, derated_torque_index, speed_index, table_number);
        intp1_val = get_value_from_lut(BUS_VOLTAGE_500V, derated_torque_index, speed_index, table_number);
        value_from_table = (intp2_val - intp1_val)*voltage_inter_percentage + intp1_val;
    }
    //Take values directly from 600V table
    else {
        voltage_inter_percentage = 1.0f;
        derated_torque_index = calculate_derated_torque_index(max_stator_current, BUS_VOLTAGE_600V, BUS_VOLTAGE_600V, speed_index, voltage_inter_percentage);
        intp1_val = get_value_from_lut(BUS_VOLTAGE_600V, derated_torque_index, speed_index, table_number);
        value_from_table = intp1_val;
    }
    return value_from_table;
}

float get_value_from_lut(uint8_t voltage_index, uint8_t torque_index, uint8_t speed_index, uint8_t table_number)
{
    float value = 0.0f;

    if(table_number == ID_PEAK)
    {
        value = (float)(id_peak_lut_times100[voltage_index][speed_index][torque_index])/100.0f;
    }
    else if(table_number == IQ_PEAK)
    {
        value = (float)(iq_peak_lut_times100[voltage_index][speed_index][torque_index])/100.0f;
    }
    //Invalid table number
    else
    {
        value = 0;
    }
    return value;
}

uint8_t calculate_derated_torque_index(float derated_stator_current, uint8_t voltage_index_low, int8_t voltage_index_high, uint8_t speed_index, float voltage_inter_percentage)
{
    //Approximate torque from torque per ampere constant
    float torque_at_derated_approx = 1/TORQUE_CONST*derated_stator_current;
    uint8_t torque_index_at_derated = (uint8_t) (fabsf(torque_at_derated_approx) / TORQUE_PER_INTERVAL);
    float lut_intp_1 = (float)(is_peak_lut_times100[voltage_index_high][speed_index][torque_index_at_derated])/100.0f;
    float lut_intp_2 = (float)(is_peak_lut_times100[voltage_index_low][speed_index][torque_index_at_derated])/100.0f;
    float stator_current_guess = (lut_intp_2 - lut_intp_1)*voltage_inter_percentage + lut_intp_1;

    //Only go down for now
    while(stator_current_guess > derated_stator_current && torque_index_at_derated > 0)
    {
        torque_index_at_derated -= 1;
        lut_intp_1 = (float)(is_peak_lut_times100[voltage_index_high][speed_index][torque_index_at_derated])/100.0f;
        lut_intp_2 = (float)(is_peak_lut_times100[voltage_index_low][speed_index][torque_index_at_derated])/100.0f;
        stator_current_guess = (lut_intp_2 - lut_intp_1)*voltage_inter_percentage + lut_intp_1;
    }
    return torque_index_at_derated;
}
