#include "lookup_tables/App_MotorLutInterface.h"
#include "configs/App_ControlSystemConfig.h"
#include "lookup_tables/id_peak_lut.h"
#include "lookup_tables/iq_peak_lut.h"
#include "lookup_tables/is_peak_lut.h"
#include <math.h>

void look_up_dqs_current_ref(
    const float   speed_rpm,
    const float   torque_request,
    const float   bus_voltage,
    const float max_stator_current,
    struct DqsValues* dqs_ref_currents) {

    //Always round to lower torque/speed interval operating region
    uint8_t torque_index = (uint8_t) (fabsf(torque_request) / TORQUE_PER_INTERVAL);
    uint8_t speed_index = (uint8_t) (fabsf(speed_rpm) / RPM_PER_INTERVAL);

    float voltage_inter_percentage = (fmodf(bus_voltage, 100.0f))/100.0f;
    uint8_t voltage_index_low = bus_voltage < 500.0f ? BUS_VOLTAGE_400V : (bus_voltage < 600.0f ? BUS_VOLTAGE_500V : BUS_VOLTAGE_600V);
    uint8_t voltage_index_high = bus_voltage > 500.0f ? BUS_VOLTAGE_600V : (bus_voltage > 400.0f ? BUS_VOLTAGE_500V : BUS_VOLTAGE_400V);

    float requested_stator_current = intp_values_from_lut(voltage_index_high, voltage_index_low, voltage_inter_percentage, torque_index, speed_index, IS_PEAK);

    //Lower the torque index if the stator current is too high
    if(requested_stator_current > max_stator_current)
    {
        torque_index = calculate_derated_torque_index(max_stator_current, voltage_index_low, voltage_index_high, speed_index, voltage_inter_percentage);
    }

    dqs_ref_currents->d = intp_values_from_lut(voltage_index_high, voltage_index_low, voltage_inter_percentage, torque_index, speed_index, ID_PEAK);
    dqs_ref_currents->q = intp_values_from_lut(voltage_index_high, voltage_index_low, voltage_inter_percentage, torque_index, speed_index, IQ_PEAK);
    dqs_ref_currents->s = intp_values_from_lut(voltage_index_high, voltage_index_low, voltage_inter_percentage, torque_index, speed_index, IS_PEAK);
}

float intp_values_from_lut(uint8_t voltage_index_high, uint8_t voltage_index_low, float voltage_inter_percentage, uint8_t torque_index, uint8_t speed_index, uint8_t table_number)
{
    float intp1_high_volt = 0.0f;
    float intp2_low_volt = 0.0f;
    float intp2p1_val = 0.0f;

    speed_index = speed_index > LUT_NUM_COLUMNS-1 ? LUT_NUM_COLUMNS-1 : (speed_index < 0 ? 0 : speed_index);
    torque_index = torque_index > LUT_NUM_ROWS-1 ? LUT_NUM_ROWS-1 : (torque_index < 0 ? 0 : torque_index);

    if(table_number == ID_PEAK)
    {
        intp1_high_volt = (float)(id_peak_lut_times100[voltage_index_high][speed_index][torque_index])/100.0f;
        intp2_low_volt = (float)(id_peak_lut_times100[voltage_index_low][speed_index][torque_index])/100.0f;
        intp2p1_val = intp1_high_volt > intp2_low_volt ? (intp2_low_volt + fabsf(intp2_low_volt - intp1_high_volt)*voltage_inter_percentage)
                                            : (intp2_low_volt - fabsf(intp2_low_volt - intp1_high_volt)*voltage_inter_percentage);
    }
    else if(table_number == IQ_PEAK)
    {
        intp1_high_volt = (float)(iq_peak_lut_times100[voltage_index_high][speed_index][torque_index])/100.0f;
        intp2_low_volt = (float)(iq_peak_lut_times100[voltage_index_low][speed_index][torque_index])/100.0f;
        intp2p1_val = intp1_high_volt > intp2_low_volt ? (intp2_low_volt + fabsf(intp2_low_volt - intp1_high_volt)*voltage_inter_percentage)
                                            : (intp2_low_volt - fabsf(intp2_low_volt - intp1_high_volt)*voltage_inter_percentage);
    }
    else if(table_number == IS_PEAK)
    {
        intp1_high_volt = (float)(is_peak_lut_times100[voltage_index_high][speed_index][torque_index])/100.0f;
        intp2_low_volt = (float)(is_peak_lut_times100[voltage_index_low][speed_index][torque_index])/100.0f;
        intp2p1_val = intp1_high_volt > intp2_low_volt ? (intp2_low_volt + fabsf(intp2_low_volt - intp1_high_volt)*voltage_inter_percentage)
                                            : (intp2_low_volt - fabsf(intp2_low_volt - intp1_high_volt)*voltage_inter_percentage);
    }
    //Invalid table number
    else
    {
        intp2p1_val = 0;
    }
    return intp2p1_val;
}

uint8_t calculate_derated_torque_index(float derated_stator_current, uint8_t voltage_index_low, uint8_t voltage_index_high, uint8_t speed_index, float voltage_inter_percentage)
{
    //Approximate torque from torque per ampere constant
    float torque_at_derated_guess = TORQUE_CONST*derated_stator_current;
    uint8_t derated_torque_index_guess = (uint8_t) (fabsf(torque_at_derated_guess) / (float)TORQUE_PER_INTERVAL);
    float stator_current_guess = intp_values_from_lut(voltage_index_high, voltage_index_low, voltage_inter_percentage, derated_torque_index_guess, speed_index, IS_PEAK);

    //Use the fact that stator current is monotonic with the torque index to find an appropriate torque index for the current limit.
    if(stator_current_guess >= derated_stator_current)
    {
        while(stator_current_guess > derated_stator_current && derated_torque_index_guess > 0)
        {
            derated_torque_index_guess -= 1;
            stator_current_guess = intp_values_from_lut(voltage_index_high, voltage_index_low, voltage_inter_percentage, derated_torque_index_guess, speed_index, IS_PEAK);
        }
    }
    else
    {
        int times_ran = 0;
        float last_stator_current_guess = -1;
        //Keep going up in stator current until it stops increasing or we surpass the limit
        while(stator_current_guess < derated_stator_current && derated_torque_index_guess < LUT_NUM_ROWS && stator_current_guess != last_stator_current_guess)
        {
            times_ran += 1;
            derated_torque_index_guess += 1;
            last_stator_current_guess = stator_current_guess;
            //stator_current_guess = intp_values_from_lut(voltage_index_high, voltage_index_low, voltage_inter_percentage, derated_torque_index_guess, speed_index, IS_PEAK);
            //TODO derate on 600V value for now - doesn't show much error, but will cause things to get hotter. The derating system should prevent this from getting out of control
            //Note: not quite enough cycles to compute full interpolated values between voltages
            stator_current_guess = (float)(is_peak_lut_times100[BUS_VOLTAGE_600V][speed_index][derated_torque_index_guess])/100.0f;
        }
        //We're using one torque index too high since we want to be lower than the limit, so subtract one
        derated_torque_index_guess -= 1;
    }

    return derated_torque_index_guess;
}
