#include "io_coolant.h"

#include <math.h>

#include "hw_pwmInputFreqOnly.h"
#include "hw_adc.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "io_log.h"

// source: https://www.adafruit.com/product/828#:~:text=7.5%20*%20Flow%20rate%20(L/min)
#define FLOW_RATE_CONVERSION_FACTOR (7.5f)

// source: https://www.tdk-electronics.tdk.com/inf/50/db/ntc/NTC_Probe_ass_K301_A003.pdf
#define TEMPERATURE_VOLTAGE_MIN (0.0f)
#define TEMPERATURE_VOLTAGE_MAX (4.0f)
// TODO set these values
#define PRESSURE_TOP_RESISTOR (28.0f)
#define PRESSURE_BOT_RESISTOR (56.0f)
// from the schematic we see that the voltage value coming is passes through a voltage dividor before it is read at the
// ADC PIN.
#define PRESSURE_VOLTAGE_DIVIDOR (PRESSURE_BOT_RESISTOR / (PRESSURE_BOT_RESISTOR + PRESSURE_TOP_RESISTOR))
#define PRESSURE_VOLTAGE_MIN (0.5f * PRESSURE_VOLTAGE_DIVIDOR)
#define PRESSURE_VOLTAGE_MAX (4.5f * PRESSURE_VOLTAGE_DIVIDOR)
#define PRESSURE_PSI_MAX (100.0f)
#define VOLTAGE_PRESSURE_CONVERSION(voltage) \
    (((voltage)-PRESSURE_VOLTAGE_MIN) * (PRESSURE_PSI_MAX / (PRESSURE_VOLTAGE_MAX - PRESSURE_VOLTAGE_MIN)))

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
#define BTERM_STEIN_EQN(rtherm) ((float)log((double)(rtherm / R0)) / B_COEFFICIENT)

// source:
// https://m.media-amazon.com/images/S/aplus-media-library-service-media/413eb684-16a6-4eca-8ea6-b5e5bb1c657e.__CR0,0,970,600_PT0_SX970_V1___.jpg
// NOTE: this assumes a linear relationship between voltage and temperature
#define PRESSURE_VOLTAGE_MIN (0.5f)
#define PRESSURE_VOLTAGE_MAX (4.5f)
#define MIN_PRESSURE_PSI (0.0f)
#define MAX_PRESSURE_PSI (200.0f)
#define PSI_PER_VOLT ((MAX_PRESSURE_PSI - MIN_PRESSURE_PSI) / (PRESSURE_VOLTAGE_MAX - PRESSURE_VOLTAGE_MIN))

static PwmInputFreqOnly flow_meter;

void io_coolant_init(const PwmInputFreqOnlyConfig *config)
{
    hw_pwmInputFreqOnly_init(&flow_meter, config);
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
    const float freq_read = hw_pwmInputFreqOnly_getFrequency(&flow_meter);
    return freq_read / FLOW_RATE_CONVERSION_FACTOR;
}

void io_coolant_checkIfFlowMeterActive(void)
{
    hw_pwmInputFreqOnly_checkIfPwmIsActive(&flow_meter);
}

bool temperature_ocsc(float v)
{
    return v < TEMPERATURE_VOLTAGE_MIN || v > TEMPERATURE_VOLTAGE_MAX;
}

float io_coolant_getTemperatureA(void)
{
    const float v_read = hw_adc_getVoltage(ADC1_IN2_COOLANT_TEMP_1);
    app_canTx_RSM_Warning_CoolantTempAOCSC_set(temperature_ocsc(v_read));
    float const v_out        = CLAMP(v_read, TEMPERATURE_VOLTAGE_MIN, TEMPERATURE_VOLTAGE_MAX);
    float const r_thermistor = RTHERM(v_out);
    float       b_term       = BTERM_STEIN_EQN(r_thermistor);
    float       coolant_temp =
        (1 / (1 / T0 + b_term)); // source: https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
    float coolant_temp_cel = coolant_temp - 273.15f;

    return coolant_temp_cel;
}

float io_coolant_getTemperatureB(void)
{
    const float v_read = hw_adc_getVoltage(ADC1_IN3_COOLANT_TEMP_2);
    app_canTx_RSM_Warning_CoolantTempBOCSC_set(temperature_ocsc(v_read));
    float const v_out        = CLAMP(v_read, TEMPERATURE_VOLTAGE_MIN, TEMPERATURE_VOLTAGE_MAX);
    float const r_thermistor = RTHERM(v_out);
    float       b_term       = BTERM_STEIN_EQN(r_thermistor);
    float       coolant_temp =
        (1 / (1 / T0 + b_term)); // source: https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
    float coolant_temp_cel = coolant_temp - 273.15f;

    return coolant_temp_cel;
}

bool pressure_ocsc(float v)
{
    return v < PRESSURE_VOLTAGE_MIN || v > PRESSURE_VOLTAGE_MAX;
}

float io_coolant_getPressureA(void)
{
    return CLAMP(VOLTAGE_PRESSURE_CONVERSION(ADC1_IN12_COOLANT_PRESSURE_1), PRESSURE_VOLTAGE_MIN, PRESSURE_PSI_MAX);
}

bool io_coolant_PressureAOCSC(void)
{
    return (
        PRESSURE_VOLTAGE_MIN > (ADC1_IN12_COOLANT_PRESSURE_1) || PRESSURE_VOLTAGE_MAX < (ADC1_IN12_COOLANT_PRESSURE_1));
}

float io_coolant_getPressureB(void)
{
    return CLAMP(VOLTAGE_PRESSURE_CONVERSION(ADC1_IN11_COOLANT_PRESSURE_2), PRESSURE_VOLTAGE_MIN, PRESSURE_VOLTAGE_MAX);
}

bool io_coolant_PressureBOCSC(void)
{
    return (
        PRESSURE_VOLTAGE_MIN > (ADC1_IN11_COOLANT_PRESSURE_2) || PRESSURE_VOLTAGE_MAX < (ADC1_IN12_COOLANT_PRESSURE_1));
}