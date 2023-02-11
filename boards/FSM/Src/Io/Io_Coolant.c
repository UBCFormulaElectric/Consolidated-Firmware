#include <assert.h>
#include "main.h"
#include "Io_Coolant.h"
#include "Io_SharedFreqOnlyPwmInput.h"

// source: https://www.adafruit.com/product/828#:~:text=7.5%20*%20Flow%20rate%20(L/min)
#define FLOW_RATE_CONVERSION_FACTOR (7.5f)
// TODO set these values
#define TEMPERATURE_VOLTAGE_MIN (0.0f)
#define TEMPERATURE_VOLTAGE_MAX (1.0f)
#define PRESSURE_VOLTAGE_MIN (0.0f)
#define PRESSURE_VOLTAGE_MAX (1.0f)

static struct FreqOnlyPwmInput *flow_meter;

void Io_FlowMeter_Init(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);

    flow_meter = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIM8_PRESCALER, TIM_CHANNEL_2, TIM8_AUTO_RELOAD_REG, HAL_TIM_ACTIVE_CHANNEL_2);
}
float Io_FlowMeter_GetFlowRate(void)
{
    return Io_SharedFreqOnlyPwmInput_GetFrequency(flow_meter) / FLOW_RATE_CONVERSION_FACTOR;
}
void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == Io_SharedFreqOnlyPwmInput_GetTimerHandle(flow_meter) &&
        htim->Channel == Io_SharedFreqOnlyPwmInput_GetTimerActiveChannel(flow_meter))
    {
        Io_SharedFreqOnlyPwmInput_Tick(flow_meter);
    }
}
void Io_FlowMeter_CheckIfFlowMeterIsActive(void)
{
    Io_SharedFreqOnlyPwmInput_CheckIfPwmIsActive(flow_meter);
}

float Io_Coolant_TemperatureAVoltage(void)
{
    return 0.5f;
}
float Io_Coolant_GetTemperatureA(void)
{
    float temperature_voltage = Io_Coolant_TemperatureAVoltage();
    // TODO calculate the temperature
    return 1;
}

float Io_Coolant_TemperatureBVoltage(void)
{
    return 0.5f;
}
float Io_Coolant_GetTemperatureB(void)
{
    float temperature_voltage = Io_Coolant_TemperatureBVoltage();
    // TODO calculate the temperature
    return 1;
}

float Io_Coolant_PressureAVoltage(void)
{
    return 0.5f;
}
float Io_Coolant_GetPressureA(void)
{
    float pressure_voltage = Io_Coolant_PressureAVoltage();
    // TODO calculate the pressure
    return 1;
}

float Io_Coolant_PressureBVoltage(void)
{
    return 0.5f;
}
float Io_Coolant_GetPressureB(void)
{
    float pressure_voltage = Io_Coolant_PressureBVoltage();
    // TODO calculate the pressure
    return 1;
}
