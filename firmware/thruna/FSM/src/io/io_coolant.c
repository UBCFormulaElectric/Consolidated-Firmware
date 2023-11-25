#include "io_coolant.h"
#include <assert.h>
#include "main.h"
#include "Io_SharedFreqOnlyPwmInput.h"
#include "hw_adc.h"

// source: https://www.adafruit.com/product/828#:~:text=7.5%20*%20Flow%20rate%20(L/min)
#define FLOW_RATE_CONVERSION_FACTOR (7.5f)
// TODO set these values
#define TEMPERATURE_VOLTAGE_MIN (0.0f)
#define TEMPERATURE_VOLTAGE_MAX (1.0f)
#define PRESSURE_VOLTAGE_MIN (0.0f)
#define PRESSURE_VOLTAGE_MAX (1.0f)

static struct FreqOnlyPwmInput *flow_meter;

void io_coolant_init(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);

    flow_meter = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIM8_PRESCALER, TIM_CHANNEL_1, TIM8_AUTO_RELOAD_REG, HAL_TIM_ACTIVE_CHANNEL_1);
}

void io_coolant_inputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == Io_SharedFreqOnlyPwmInput_GetTimerHandle(flow_meter) &&
        htim->Channel == Io_SharedFreqOnlyPwmInput_GetTimerActiveChannel(flow_meter))
    {
        Io_SharedFreqOnlyPwmInput_Tick(flow_meter);
    }
}

float io_coolant_getFlowRate(void)
{
    return Io_SharedFreqOnlyPwmInput_GetFrequency(flow_meter) / FLOW_RATE_CONVERSION_FACTOR;
}

void io_coolant_checkIfFlowMeterActive(void)
{
    Io_SharedFreqOnlyPwmInput_CheckIfPwmIsActive(flow_meter);
}

float io_coolant_getTemperatureA(void)
{
    // TODO calculate the temperature
    return hw_adc_getVoltage(ADC_CHANNEL_4);
}

float io_coolant_getTemperatureB(void)
{
    // TODO calculate the temperature
    return hw_adc_getVoltage(ADC_CHANNEL_5);
}

float io_coolant_getPressureA(void)
{
    // TODO calculate the pressure
    return hw_adc_getVoltage(ADC_CHANNEL_0);
}

float io_coolant_getPressureB(void)
{
    // TODO calculate the pressure
    return hw_adc_getVoltage(ADC_CHANNEL_3);
}
