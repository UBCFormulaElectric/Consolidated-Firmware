#include "io_coolant.h"

#include <math.h>

#include "hw_pwmInputFreqOnly.h"
#include "hw_adc.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "io_log.h"
#include "hw_pwms.h"
#include "hw_adcs.h"
#include "app_utils.h"
#include "app_canTx.h"

// source: https://www.adafruit.com/product/828#:~:text=7.5%20*%20Flow%20rate%20(L/min)
#define FLOW_RATE_CONVERSION_FACTOR (7.5f)

// from the schematic we see that the voltage value coming is passes through a voltage dividor before it is read at the
// ADC PIN.
#define PRESSURE_VOLTAGE_DIVIDOR (PRESSURE_BOT_RESISTOR / (PRESSURE_BOT_RESISTOR + PRESSURE_TOP_RESISTOR))
#define PRESURE_VOLTAGE_MIN (0.5f * PRESSURE_VOLTAGE_DIVIDOR)
#define PRESSURE_VOLTAGE_MAX (4.5f * PRESSURE_VOLTAGE_DIVIDOR)
#define PRESSURE_PSI_MAX (100.0f)
// min pressure is 0 PSI
#define VOLTAGE_PRESSURE_CONVERSION(voltage) \
    (((voltage)-PRESSURE_VOLTAGE_MIN) * (PRESSURE_PSI_MAX / (PRESSURE_VOLTAGE_MAX - PRESSURE_VOLTAGE_MIN)))

// source: https://www.tdk-electronics.tdk.com/inf/50/db/ntc/NTC_Probe_ass_K301_A003.pdf
#define TEMPERATURE_VOLTAGE_MIN (0.0f)
#define TEMPERATURE_VOLTAGE_MAX (4.0f)
#define PRESSURE_TOP_RESISTOR (28.0f)
#define PRESSURE_BOT_RESISTOR (56.0f)

#define T0 (298.15f)           // 25 degrees celsius is used for T0 in datasheet but for equation we do kelvin
#define R0 (10000.f)           // resistance at T0
#define B_COEFFIECENT (3988.f) // source: https://www.tdk-electronics.tdk.com/inf/50/db/ntc/NTC_Probe_ass_K301_A003.pdf
#define VIN (5.0f)
#define R2 (2200.f) // bottom resistor in the coolant temp sensor circuit
// the coolant temp sensor circuit is made of a voltage divider where the thermistor which we use for the temperature
// readings is the top resistor in the voltage divider and R2 is the bottom this tells us that Vout = Vin (R2/ (Rtherm +
// R2)) and by manipulating this equation we can get -- here Vout is our measured value from ADC 4 and 5 Rtherm = ((Vin
// * R2)/ Vout ) - R2)
#define RTHERM(voltage_out) (VIN * R2 / voltage_out - R2)
// below are constants for Steinhart Hart EQN used to model temprature as a function of a resistor for a thermistor
#define BTERM_STEIN_EQN(rtherm) ((float)log((float)(rtherm / R0)) / B_COEFFIECENT)

static const Coolant a(coolantpressure1_3v3);
static const Coolant b(coolantpressure2_3v3);

void io_coolant_init(void)
{
    coolant_config.init();
}

void io_coolant_inputCaptureCallback(void)
{
    coolant_config.tick();
}

float io_coolant_getFlowRate(void)
{
    const float freq_read = coolant_config.getFrequency();
    return freq_read / FLOW_RATE_CONVERSION_FACTOR;
}

void io_coolant_checkIfFlowMeterActive(void)
{
    coolant_config.checkIfPwmIsActive();
}

bool io_coolant_temperature_ocsc(float v)
{
    return v < TEMPERATURE_VOLTAGE_MIN || v > TEMPERATURE_VOLTAGE_MAX;
}

float io_coolant_getTemperature(Coolant *coolant)
{
    const float v_read = hw_adc_getVoltage(&coolant->src);
    app_canTx_RSM_Warning_CoolantTempAOCSC_set(io_coolant_temperature_ocsc(v_read));

    const float v_out        = CLAMP(v_read, TEMPERATURE_VOLTAGE_MIN, TEMPERATURE_VOLTAGE_MAX);
    const float r_thermistor = RTHERM(v_out);
    float       b_term       = BTERM_STEIN_EQN(r_thermistor);
    float       coolant_temp =
        (1 / (1 / T0 + b_term)); // source: https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
    float coolant_temp_cel = coolant_temp - 273.15f;

    return coolant_temp_cel;
}

bool io_coolant_pressure_ocsc(float v)
{
    return v < PRESSURE_VOLTAGE_MIN || v > PRESSURE_VOLTAGE_MAX;
}

float io_coolant_getPressure(Coolant *coolant)
{
    const float water_pressure = hw_adc_getVoltage(&coolant->src);
    app_canTx_RSM_Warning_CoolantPressureAOCSC_set(io_coolant_pressure_ocsc(water_pressure));
    return CLAMP(water_pressure, 0.0f, PRESSURE_PSI_MAX);
}

bool io_coolant_PressureAOCSC(void)
{
    return (
        PRESSURE_VOLTAGE_MIN > (hw_adc_getVoltage(ADC1_IN12_COOLANT_PRESSURE_1)) ||
        PRESSURE_VOLTAGE_MAX < (hw_adc_getVoltage(ADC1_IN12_COOLANT_PRESSURE_1)));
}

bool io_coolant_PressureBOCSC(void)
{
    return (
        PRESSURE_VOLTAGE_MIN > (hw_adc_getVoltage(ADC1_IN12_COOLANT_PRESSURE_2)) ||
        PRESSURE_VOLTAGE_MAX < (hw_adc_getVoltage(ADC1_IN12_COOLANT_PRESSURE_2)));
}
