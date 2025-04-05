#include "hw_pwmInputFreqOnly.h"
#include "hw_adc.h"
#include "hw_pwms.h"
#include "hw_adcs.h"
#include "io_coolants.h"
#include "io_log.h"
#include "app_utils.h"
#include "app_canTx.h"

#include <math.h>

// source: https://www.adafruit.com/product/828#:~:text=7.5%20*%20Flow%20rate%20(L/min)
#define LITER_PER_MINUTE_SENSOR_PERIOD_SECS (7.5f)

void io_coolant_init()
{
    hw_pwmInputFreqOnly_init(&flow_meter_5v5, &flow_meter_config);
}

void io_coolant_inputCaptureCallback()
{
    hw_pwmInputFreqOnly_tick(&flow_meter_5v5);
}

float io_coolant_getFlowRate()
{
    const float freq_read = hw_pwmInputFreqOnly_getFrequency(&flow_meter_5v5);
    return freq_read / LITER_PER_MINUTE_SENSOR_PERIOD_SECS;
}

void io_coolant_checkIfFlowMeterActive()
{
    hw_pwmInputFreqOnly_checkIfPwmIsActive(&flow_meter_5v5);
}