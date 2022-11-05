#include <assert.h>
#include <math.h>
#include "main.h"
#include "Io_Coolant.h"
#include "Io_SharedFreqOnlyPwmInput.h"

//TODO set these values
#define TEMPERATURE_VOLTAGE_MIN (0.0f)
#define TEMPERATURE_VOLTAGE_MAX (1.0f)
#define PRESSURE_VOLTAGE_MIN (0.0f)
#define PRESSURE_VOLTAGE_MAX (1.0f)

static struct FreqOnlyPwmInput *flow_meter;

void  Io_FlowMeter_Init(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);

    flow_meter = Io_SharedFreqOnlyPwmInput_Create(
        htim, TIMx_FREQUENCY / TIM4_PRESCALER, TIM_CHANNEL_2, TIM4_AUTO_RELOAD_REG, HAL_TIM_ACTIVE_CHANNEL_2);
}
float Io_FlowMeters_GetFlowRate(void)
{
    return Io_SharedFreqOnlyPwmInput_GetFrequency(flow_meter) / 7.5f;
}
void Io_FlowMeters_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == Io_SharedFreqOnlyPwmInput_GetTimerHandle(flow_meter) &&
        htim->Channel == Io_SharedFreqOnlyPwmInput_GetTimerActiveChannel(flow_meter))
    {
        Io_SharedFreqOnlyPwmInput_Tick(flow_meter);
    }
}
void Io_FlowMeters_CheckIfFlowMeterIsActive(void)
{
    Io_SharedFreqOnlyPwmInput_CheckIfPwmIsActive(flow_meter);
}

float Io_Coolant_GetTemperatureA(void)
{
    float temperature_voltage = 0.5f;
    if(Io_Coolant_TempertureVoltageAlarm(temperature_voltage)){
        return NAN;
    }
    //TODO calculate the temperature
    return 1;
}
float Io_Coolant_GetTemperatureB(void)
{
    float temperature_voltage = 0.5f;
    if(Io_Coolant_TempertureVoltageAlarm(temperature_voltage)){
        return NAN;
    }
    //TODO calculate the temperature
    return 1;
}
bool Io_Coolant_TempertureVoltageAlarm(float thermometer_voltage)
{
    return !(TEMPERATURE_VOLTAGE_MIN <= thermometer_voltage && thermometer_voltage <= TEMPERATURE_VOLTAGE_MAX);
}

float Io_Coolant_GetPressureA(void)
{
    float pressure_voltage = 0.5f;
    if(Io_Coolant_PressureVoltageAlarm(pressure_voltage)){
        return NAN;
    }

    //TODO calculate the pressure
    return 1;
}
float Io_Coolant_GetPressureB(void)
{
    float pressure_voltage = 0.5f;
    if(Io_Coolant_PressureVoltageAlarm(pressure_voltage)){
        return NAN;
    }

    //TODO calculate the pressure
    return 1;
}
bool Io_Coolant_PressureVoltageAlarm(float pressure_voltage){
    return !(PRESSURE_VOLTAGE_MIN <= pressure_voltage && pressure_voltage <= PRESSURE_VOLTAGE_MAX);
}
