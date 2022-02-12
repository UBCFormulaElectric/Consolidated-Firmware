#include "lookup_tables/motor_lut_interface.h"
#include "lookup_tables/id_peak_lut.h"
#include "lookup_tables/iq_peak_lut.h"
#include "lookup_tables/shaft_torque_lut.h"
#include <math.h>

float look_up_value(
    const float   speed_rpm,
    const float   torque_request,
    const float   bus_voltage,
    const float   motor_temp,
    const uint8_t table_number)
{
    uint8_t  voltage_index;
    uint16_t speed_index;
    uint8_t  torque_index;

    float voltage_inter_percentage;
    float speed_inter_percentage;
    float torque_inter_percentage;

    float abs_speed_rpm      = fabsf(speed_rpm);
    float abs_torque_request = fabsf(torque_request);

    float val1, val2;
    float val1val2_intp;

    speed_inter_percentage = fmodf(abs_speed_rpm, 100.0f) / 100.0f;
    if (speed_inter_percentage > 1.0f)
    {
        speed_inter_percentage = 1.0f;
    }
    speed_index = (uint16_t)floorf(abs_speed_rpm / 100.0f);
    if (speed_index >= LUT_NUM_ROWS - 1)
    {
        speed_index = LUT_NUM_ROWS - 2;
    }

    // Find torque index & percentage to next index
    // Look at speed_index + 1 to avoid field weakening limitations on id/iq
    for (size_t i = 0; i < LUT_NUM_COLUMNS - 2; i++)
    {
        // Torque request is between two adjacent torque values in LUT
        if (abs_torque_request >=
                shaft_torque_lut_array[0][0][speed_index + 1][i] &&
            abs_torque_request <
                shaft_torque_lut_array[0][0][speed_index + 1][i + 1])
        {
            torque_index = i;
            torque_inter_percentage =
                ((abs_torque_request -
                  shaft_torque_lut_array[0][0][speed_index + 1][i])) /
                (shaft_torque_lut_array[0][0][speed_index + 1][i + 1] -
                 shaft_torque_lut_array[0][0][speed_index + 1][i]);
            break;
        }
        torque_inter_percentage = 1.0f;
        torque_index            = LUT_NUM_COLUMNS - 2;
    }

    if (bus_voltage < 400.0f)
    {
        voltage_index            = 0;
        voltage_inter_percentage = 0.0f;
        val1                     = intp_from_lut(
            voltage_index, 0, torque_index, speed_index, speed_inter_percentage,
            torque_inter_percentage, table_number);
        val2 = val1;
    }
    else if (bus_voltage >= 400.0f && bus_voltage < 500.0f)
    {
        voltage_index            = 1;
        voltage_inter_percentage = (bus_voltage - 400.0f) / 100.0f;
        val1                     = intp_from_lut(
            voltage_index - 1, 0, torque_index, speed_index,
            speed_inter_percentage, torque_inter_percentage, table_number);
        val2 = intp_from_lut(
            voltage_index, 0, torque_index, speed_index, speed_inter_percentage,
            torque_inter_percentage, table_number);
    }
    else if (bus_voltage >= 500.0f && bus_voltage < 600.0f)
    {
        voltage_index            = 2;
        voltage_inter_percentage = (bus_voltage - 500.0f) / 100.0f;
        val1                     = intp_from_lut(
            voltage_index - 1, 0, torque_index, speed_index,
            speed_inter_percentage, torque_inter_percentage, table_number);
        val2 = intp_from_lut(
            voltage_index, 0, torque_index, speed_index, speed_inter_percentage,
            torque_inter_percentage, table_number);
    }
    else
    {
        voltage_index            = 2;
        voltage_inter_percentage = 1.0f;
        val1                     = intp_from_lut(
            voltage_index, 0, torque_index, speed_index, speed_inter_percentage,
            torque_inter_percentage, table_number);
        val2 = val1;
    }

    val1val2_intp = voltage_inter_percentage * (val2 - val1) + val1;

    return val1val2_intp;
}

float intp_from_lut(
    const uint8_t voltage_index,
    const uint8_t temp_index,
    uint8_t       torque_index,
    uint8_t       speed_index,
    const float   speed_inter_percentage,
    const float   torque_inter_percentage,
    const uint8_t table_number)
{
    // TODO add out of bounds check for LUT indeces
    float lut_x1y1, lut_x2y1, lut_x1y2, lut_x2y2, lut_y1x1_y1x2_intp,
        lut_y2x1_y2x2_intp, lut_val_intp;

    if (torque_index >= LUT_NUM_COLUMNS - 1)
    {
        torque_index = LUT_NUM_COLUMNS - 2;
    }
    else if (torque_index < 0)
    {
        torque_index = 0;
    }
    if (speed_index >= LUT_NUM_ROWS - 1)
    {
        speed_index = LUT_NUM_ROWS - 2;
    }
    else if (speed_index < 0)
    {
        speed_index = 0;
    }

    switch (table_number)
    {
        case ID_PEAK:
            lut_x1y1 = id_peak_lut_array[voltage_index][temp_index][speed_index]
                                        [torque_index];
            lut_x2y1 = id_peak_lut_array[voltage_index][temp_index][speed_index]
                                        [torque_index + 1];
            lut_x1y2 = id_peak_lut_array[voltage_index][temp_index]
                                        [speed_index + 1][torque_index];
            lut_x2y2 = id_peak_lut_array[voltage_index][temp_index]
                                        [speed_index + 1][torque_index + 1];
            break;
        case IQ_PEAK:
            lut_x1y1 = iq_peak_lut_array[voltage_index][temp_index][speed_index]
                                        [torque_index];
            lut_x2y1 = iq_peak_lut_array[voltage_index][temp_index][speed_index]
                                        [torque_index + 1];
            lut_x1y2 = iq_peak_lut_array[voltage_index][temp_index]
                                        [speed_index + 1][torque_index];
            lut_x2y2 = iq_peak_lut_array[voltage_index][temp_index]
                                        [speed_index + 1][torque_index + 1];
            break;
        default:
            lut_x1y1 = 0;
            lut_x2y1 = 0;
            lut_x1y2 = 0;
            lut_x2y2 = 0;
            break;
    }

    lut_y1x1_y1x2_intp =
        torque_inter_percentage * (lut_x2y1 - lut_x1y1) + lut_x1y1;
    lut_y2x1_y2x2_intp =
        torque_inter_percentage * (lut_x2y2 - lut_x1y2) + lut_x1y2;
    lut_val_intp =
        speed_inter_percentage * (lut_y2x1_y2x2_intp - lut_y1x1_y1x2_intp) +
        lut_y1x1_y1x2_intp;
    return lut_val_intp;
}
