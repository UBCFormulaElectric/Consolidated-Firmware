#include "io_tractiveSystem.h"
#include <math.h>
#include "hw_hal.h"
#include <assert.h>

// Percent error used to compensate for resistor errors. Determined from
// testing with the HW

// TODO: Test with HW to determine compensation
// #define R_ERROR_COMPENSATION (1.0f)

// Isolated amplifier gain
#define AMPLIFIER_GAIN (1.0f)
// Voltage divider for the TS+ voltage sensing
#define TS_VOLTAGE_DIV (20e+3f / (6 * 1e+6f + 20e3f))

// Offset voltage of output 1.
#define OUTPUT_1_OFFSET (2.5f)
// Sensitivity of output 1: 40mV/A
#define OUTPUT_1_SENSITIVITY (1.0f / 40.0e-3f)
// Voltage divider from adc --> current sensor output
#define OUTPUT_1_DIV ((33.0f + 60.4f) / (60.4f))

// Offset voltage of output 2.
#define OUTPUT_2_OFFSET (2.5f)
// Sensitivity of output 2: 5.2mV/A
#define OUTPUT_2_SENSITIVITY (1.0f / 5.2e-3f)
// Voltage divider from adc --> current sensor output
#define OUTPUT_2_DIV ((33.0f + 60.4f) / (60.4f))

// Current Sensor error calibration parameters (based on experimental data)
// TODO: Rerun sensor calibration with new mounting
// #define OUTPUT1_DISCHARGING_ERROR_SLOPE (0.5028f)
// #define OUTPUT1_DISCHARGING_ERROR_OFFSET (-0.0894f)
// #define OUTPUT1_CHARGING_ERROR_SLOPE (0.5045f)
// #define OUTPUT1_CHARGING_ERROR_OFFSET (-0.2677f)

// #define OUTPUT2_DISCHARGING_ERROR_SLOPE (0.2417f)
// #define OUTPUT2_DISCHARGING_ERROR_OFFSET (2.3634f)
// #define OUTPUT2_CHARGING_ERROR_SLOPE (0.2324f)
// #define OUTPUT2_CHARGING_ERROR_OFFSET (2.4038f)

static const AdcChannel *ts_vsense_channel_P = NULL;
static const AdcChannel *ts_vsense_channel_N = NULL;
static const AdcChannel *ts_isense_high_res_channel = NULL;
static const AdcChannel *ts_isense_low_res_channel = NULL;

void io_tractiveSystem_init(const TractiveSystemConfig *ts_config)
{
    assert(ts_config != NULL);
    ts_vsense_channel_P = ts_config->ts_vsense_channel_P;
    ts_vsense_channel_N = ts_config->ts_vsense_channel_N;
    ts_isense_high_res_channel = ts_config->ts_isense_high_res_channel;
    ts_isense_low_res_channel = ts_config->ts_isense_low_res_channel;
}

float io_tractiveSystem_getVoltage()
{
    // The tractive system voltage is divided down by several resistors, then
    // fed through an amplifier.
    //
    // TS Voltage
    //   -+-
    //    |                                       +----------+  +-------------+
    //    +--<1M>--<1M>--<1M>--<1M>--<1M>--<1M>+--| Amplifier|--| ADC Channel |
    //                                       |    +----------+  +-------------+
    //                                       |
    //                                     <20k>
    //                                       |
    //                                      ===
    //                                      GND
    //
    // TS Voltage x Voltage Ratio x Amplifier Gain = ADC voltage, where
    //
    //                                  20k
    // Voltage Ratio = -------------------------------------
    //                   1M + 1M + 1M + 1M + 1M + 1M + 20k
    //
    // Amplifier Gain = 1.0
    //
    // We can rewrite the earlier equation as:
    //
    //                          ADC voltage
    // TS_Voltage = ---------------------------------- * Measurement Error %
    //                Voltage Ratio x Amplifier Gain

    // TODO: Test differential ADC for voltage measurement
    const float ts_vsense_P = hw_adc_getVoltage(ts_vsense_channel_P);
    const float ts_vsense_N = hw_adc_getVoltage(ts_vsense_channel_N);
    const float ts_vsense   = ts_vsense_P - ts_vsense_N;

    if (ts_vsense < 0.0f)
    {
        return 0.0f;
    }
    else
    {
        float real_voltage = ts_vsense / (TS_VOLTAGE_DIV * AMPLIFIER_GAIN);
        return real_voltage;
    }
}

float io_tractiveSystem_getCurrentHighResolution()
{
    float adc_voltage = hw_adc_getVoltage(ts_isense_high_res_channel);

    if (adc_voltage < 0.0f)
    {
        adc_voltage = 0.0f;
    }

    // DHAB S/124 Output 1 (+/- 75A):
    //
    // +------------------+                +-------------+
    // | DHAB S/124       |---<33k>---+---| ADC Channel |
    // | Output 2 Voltage |            |   +-------------+
    // +------------------+            |
    //                              <60.4k>
    //                                 |
    //                                ===
    //                                GND
    //
    //                                                                  1
    // Current = (DHAB S/124 Output 1 Voltage - Offset Voltage) x ---------------
    //                                                             Sensitivity
    //                                              33k + 60.4k
    // DHAB S/124 Output 2 Voltage = ADC Voltage x --------------
    //                                                 60.4k
    // Offset Voltage = 2.5V (Datasheet)
    //
    // Sensitivity = 40mV/A

    // Output from current sensor:
    const float hsnbv_d06_output_1 = adc_voltage * OUTPUT_1_DIV;

    // Calculate the current which corresponds to the output voltage (baed on sensor datasheet)
    const float high_res_current = ((hsnbv_d06_output_1 - OUTPUT_1_OFFSET) * OUTPUT_1_SENSITIVITY);

    // Error Calibration for High Resolution Current Sensor (based on calibration data)
    float high_res_curr_calibration = 0.0f;
    if (high_res_current > -0.2f)
    {
        high_res_curr_calibration =
            high_res_current;
    }
    else
    {
        high_res_curr_calibration = high_res_current;
    }

    return -(high_res_current + high_res_curr_calibration);
}

float io_tractiveSystem_getCurrentLowResolution()
{
    float adc_voltage = hw_adc_getVoltage(ts_isense_low_res_channel);

    if (adc_voltage < 0.0f)
    {
        adc_voltage = 0.0f;
    }

    // DHAB S/124 Output 2 (+/- 500A):
    //
    // +------------------+                +-------------+
    // | DHAB S/124       |---<33k>---+---| ADC Channel |
    // | Output 2 Voltage |            |   +-------------+
    // +------------------+            |
    //                              <60.4k>
    //                                 |
    //                                ===
    //                                GND
    //
    //                                                                  1
    // Current = (DHAB S/124 Output 1 Voltage - Offset Voltage) x ---------------
    //                                                             Sensitivity
    //                                              33k + 60.4k
    // DHAB S/124 Output 2 Voltage = ADC Voltage x --------------
    //                                                 60.4k
    // Offset Voltage = 2.5V (Datasheet)
    //
    // Sensitivity = 5.2mV/A

    // Output from current sensor:
    const float hsnbv_d06_output_2 = adc_voltage * OUTPUT_2_DIV;

    // Calculate the current which corresponds to the output voltage (baed on sensor datasheet)
    const float low_res_current = ((hsnbv_d06_output_2 - OUTPUT_2_OFFSET) * OUTPUT_2_SENSITIVITY);

    // Error Calibration for Low Resolution Current Sensor (based on calibration data)
    float low_res_curr_calibration = 0.0f;
    if (low_res_current > -0.2f)
    {
        low_res_curr_calibration = low_res_current;
    }
    else
    {
        low_res_curr_calibration = low_res_current;
    }

    return -(low_res_current + low_res_curr_calibration);
}
