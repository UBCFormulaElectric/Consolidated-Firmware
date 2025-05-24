#include "io_coolants.h"

#include "hw_pwmInputFreqOnly.h"
#include "hw_pwms.h"

// source: https://www.adafruit.com/product/828#:~:text=7.5%20*%20Flow%20rate%20(L/min)
#define FREQ_TO_LITERS_PER_MINUTE (7.5f)

void io_coolant_init()
{
    hw_pwmInputFreqOnly_init(&flow_meter_5v5, &flow_meter_config);
}

float io_coolant_getFlowRate()
{
    const float freq_read = hw_pwmInputFreqOnly_getFrequency(&flow_meter_5v5);
    return freq_read * FREQ_TO_LITERS_PER_MINUTE;
}

void io_coolant_checkIfFlowMeterActive()
{
    hw_pwmInputFreqOnly_checkIfPwmIsActive(&flow_meter_5v5);
}