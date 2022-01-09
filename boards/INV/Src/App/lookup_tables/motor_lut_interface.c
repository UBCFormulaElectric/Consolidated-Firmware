#include "lookup_tables/motor_lut_interface.h"
#include "lookup_tables/id_peak_lut.h"
#include "lookup_tables/iq_peak_lut.h"
#include "lookup_tables/shaft_torque_lut.h"
#include <math.h>

float look_up_value(const float speed_rpm, const float torque_request, const float bus_voltage, const float motor_temp, const uint8_t table_number)
{
    uint8_t temp_index;
    uint8_t voltage_index;
	uint8_t speed_index;
    uint8_t torque_index;

	float temp_inter_percentage;
	float voltage_inter_percentage;
	float speed_inter_percentage;
	float torque_inter_percentage;

	float val1, val2, val3, val4;
	float val1val2_intp, val3val4_intp, val1val2val3val4_intp;

	speed_inter_percentage = fmodf(speed_rpm, 100.0f);
	speed_index = (uint8_t)floorf(speed_rpm/100.0f);

	//Find torque index & percentage to next index
	//Look at speed_index + 1 to avoid field weakening limitations on id/iq
	for(size_t i = 0; i < LUT_NUM_COLUMNS - 1; i++)
	{
        //Torque request is between two torque adjacent values in LUT
        if (torque_request >= shaft_torque_lut_array[1][1][speed_index + 1][i] &&
            torque_request < shaft_torque_lut_array[1][1][speed_index + 1][i + 1])
        {
            torque_index = i;
            torque_inter_percentage = (torque_request - shaft_torque_lut_array[1][1][speed_index + 1][i]) /
                                      (shaft_torque_lut_array[1][1][speed_index + 1][i + 1] -
                                       shaft_torque_lut_array[1][1][speed_index + 1][i]);
            break;
        }
    }

	if(bus_voltage < 400.0f)
	{
		voltage_index = 0;
		voltage_inter_percentage = 0.0f;
	}
	else if(bus_voltage >= 400.0f && bus_voltage < 500.0f)
	{
		voltage_index = 1;
		voltage_inter_percentage = (bus_voltage - 400.0f)/100.0f;
	}
	else if(bus_voltage >= 500.0f && bus_voltage < 600.0f)
	{
		voltage_index = 2;
        voltage_inter_percentage = (bus_voltage - 500.0f)/100.0f;
	}
	else
	{
		voltage_index = 2;
		voltage_inter_percentage = 1.0f;
	}

	if(motor_temp < 80.0f)
	{
		temp_index = 0;
		temp_inter_percentage = 0.0f;
	}
	else if(motor_temp >= 80.0f && motor_temp < 100.0f)
	{
		temp_index = 1;
		temp_inter_percentage = (motor_temp - 80.0f)/20.0f;
	}
	else if(motor_temp >= 100.0f && motor_temp < 120.0f)
	{
		temp_index = 2;
        temp_inter_percentage = (motor_temp - 100.0f)/20.0f;
	}
	else
	{
		temp_index = 2;
		temp_inter_percentage = 1.0f;
	}

	if(temp_index == 0 && voltage_index == 2)
    {
        val1 = intp_from_lut(voltage_index, temp_index, torque_index, speed_index, speed_inter_percentage, torque_inter_percentage, table_number);
        val2 = val1;
        val3 = intp_from_lut(voltage_index, temp_index+1, torque_index, speed_index, speed_inter_percentage, torque_inter_percentage, table_number);
        val4 = val3;
    }
    else if(temp_index == 0 && voltage_index == 2)
    {
        val1 = intp_from_lut(voltage_index, temp_index, torque_index, speed_index, speed_inter_percentage, torque_inter_percentage, table_number);
        val2 = intp_from_lut(voltage_index+1, temp_index, torque_index, speed_index, speed_inter_percentage, torque_inter_percentage, table_number);
        val3 = val1;
        val4 = val2;
    }
    else if(temp_index == 2 && voltage_index == 2)
    {
        val1 = intp_from_lut(voltage_index, temp_index, torque_index, speed_index, speed_inter_percentage, torque_inter_percentage, table_number);
        val2 = val1;
        val3 = val1;
        val4 = val1;
    }
    else
    {
        val1 = intp_from_lut(voltage_index, temp_index, torque_index, speed_index, speed_inter_percentage, torque_inter_percentage, table_number);
        val2 = intp_from_lut(voltage_index+1, temp_index, torque_index, speed_index, speed_inter_percentage, torque_inter_percentage, table_number);
        val3 = intp_from_lut(voltage_index, temp_index+1, torque_index, speed_index, speed_inter_percentage, torque_inter_percentage, table_number);
        val4 = intp_from_lut(voltage_index+1, temp_index+1, torque_index, speed_index, speed_inter_percentage, torque_inter_percentage, table_number);
    }

    val1val2_intp = voltage_inter_percentage * (val2-val1) + val1;
    val3val4_intp = voltage_inter_percentage * (val4-val3) + val3;
    val1val2val3val4_intp = temp_inter_percentage * (val1val2_intp-val3val4_intp) + val1val2_intp;

    return val1val2val3val4_intp;
}

float intp_from_lut(const uint8_t voltage_index, const uint8_t temp_index, const uint8_t torque_index, const uint8_t speed_index, const float speed_inter_percentage, const float torque_inter_percentage, const uint8_t table_number)
{

    float lut_x1y1, lut_x2y1, lut_x1y2, lut_x2y2, lut_y1x1_y1x2_intp, lut_y2x1_y2x2_intp, lut_val_intp;

    switch(table_number)
    {
        case ID_PEAK:
            lut_x1y1 = id_peak_lut_array[voltage_index][temp_index][torque_index][speed_index];
            lut_x2y1 = id_peak_lut_array[voltage_index][temp_index][torque_index + 1][speed_index];
            lut_x1y2 = id_peak_lut_array[voltage_index][temp_index][torque_index][speed_index];
            lut_x2y2 = id_peak_lut_array[voltage_index][temp_index][torque_index][speed_index + 1];
            break;
        case IQ_PEAK:
            lut_x1y1 = iq_peak_lut_array[voltage_index][temp_index][torque_index][speed_index];
            lut_x2y1 = iq_peak_lut_array[voltage_index][temp_index][torque_index + 1][speed_index];
            lut_x1y2 = iq_peak_lut_array[voltage_index][temp_index][torque_index][speed_index];
            lut_x2y2 = iq_peak_lut_array[voltage_index][temp_index][torque_index][speed_index + 1];
            break;
        default:
            lut_x1y1 = 0;
            lut_x2y1 = 0;
            lut_x1y2 = 0;
            lut_x2y2 = 0;
            break;
    }

    lut_y1x1_y1x2_intp = torque_inter_percentage * (lut_x2y1-lut_x1y1) + lut_x1y1;
    lut_y2x1_y2x2_intp = torque_inter_percentage * (lut_x2y2-lut_x1y2) + lut_x1y2;
    lut_val_intp = speed_inter_percentage * (lut_y2x1_y2x2_intp-lut_y1x1_y1x2_intp) + lut_y1x1_y1x2_intp;
    return lut_val_intp;
}

