#include "io_coolant.h"
#include <assert.h>
#include "main.h"
#include "hw_pwmInputFreqOnly.h"
#include "hw_adc.h"
#include "app_utils.h"
#include <math.h>

// source: https://www.adafruit.com/product/828#:~:text=7.5%20*%20Flow%20rate%20(L/min)
#define FLOW_RATE_CONVERSION_FACTOR (7.5f)
#define TEMPERATURE_VOLTAGE_MIN (0.0f)
#define TEMPERATURE_VOLTAGE_MAX (4.0f)
// TODO set these values
#define PRESSURE_VOLTAGE_MIN (0.0f)
#define PRESSURE_VOLTAGE_MAX (1.0f)

#define T0 (298.15f)          // 25 degrees celsius is used for T0 in datasheet but for equation we do kelvin
#define R0 (10000.f)          // resistance at T0
#define B_COEFFIECNT (3988.f) // source: https://www.tdk-electronics.tdk.com/inf/50/db/ntc/NTC_Probe_ass_K301_A003.pdf
#define VIN (5.0f)
#define R2 (2200.f) // bottom resistor in the coolant temp sensor circuit

// the coolant temp sensor circuit is made of a voltage divider where the thermistor which we use for the temperature
// readings is the top resistor in the voltage divider and R2 is the bottom this tells us that Vout = Vin (R2/ (Rtherm +
// R2)) and by manipulating this equation we can get -- here Vout is our measured value from ADC 4 and 5 Rtherm = ((Vin
// * R2)/ Vout ) - R2)
#define RTHERM(voltage_out) (VIN * R2 / voltage_out - R2)

// below are constants for Steinhart Hart EQN used to model temprature as a function of a resistor for a thermistor
#define BTERM_STEIN_EQN(rtherm) ((float)log(rtherm / R0) / B_COEFFIECNT)

static PwmInputFreqOnly flow_meter;

void io_coolant_init(TIM_HandleTypeDef *htim)
{
    assert(htim != NULL);

    hw_pwmInputFreqOnly_init(
        &flow_meter, htim, TIMx_FREQUENCY / TIM12_PRESCALER, TIM_CHANNEL_1, TIM12_AUTO_RELOAD_REG,
        HAL_TIM_ACTIVE_CHANNEL_1);
}

void io_coolant_inputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == hw_pwmInputFreqOnly_getTimerHandle(&flow_meter) &&
        htim->Channel == hw_pwmInputFreqOnly_getTimerActiveChannel(&flow_meter))
    {
        hw_pwmInputFreqOnly_tick(&flow_meter);
    }
}

float io_coolant_getFlowRate(void)
{
    return hw_pwmInputFreqOnly_getFrequency(&flow_meter) / FLOW_RATE_CONVERSION_FACTOR;
}

void io_coolant_checkIfFlowMeterActive(void)
{
    hw_pwmInputFreqOnly_checkIfPwmIsActive(&flow_meter);
}

float io_coolant_getTemperatureA(void)
{
    float const v_out =
        CLAMP(hw_adc_getVoltage(ADC1_IN2_COOLANT_TEMP_1), TEMPERATURE_VOLTAGE_MIN, TEMPERATURE_VOLTAGE_MAX);
    float const r_thermistor = RTHERM(v_out);
    float       b_term       = (float)BTERM_STEIN_EQN(r_thermistor);
    float       coolant_temp =
        (1 / (1 / T0 + b_term)); // source: https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
    float coolant_temp_cel = coolant_temp - 273.15f;

    return coolant_temp_cel;
}

float io_coolant_getTemperatureB(void)
{
    float const v_out =
        CLAMP(hw_adc_getVoltage(ADC1_IN3_COOLANT_TEMP_2), TEMPERATURE_VOLTAGE_MIN, TEMPERATURE_VOLTAGE_MAX);
    float const r_thermistor = RTHERM(v_out);
    float       b_term       = (float)BTERM_STEIN_EQN(r_thermistor);
    float       coolant_temp =
        (1 / (1 / T0 + b_term)); // source: https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
    float coolant_temp_cel = coolant_temp - 273.15f;

    return coolant_temp_cel;
}

float io_coolant_getPressureA(void)
{
    // TODO calculate the pressure
    return hw_adc_getVoltage(ADC1_IN12_COOLANT_PRESSURE_1);
}

float io_coolant_getPressureB(void)
{
    // TODO calculate the pressure
    return hw_adc_getVoltage(ADC1_IN11_COOLANT_PRESSURE_2);
}