#include <math.h>
#include "Io_Adc.h"
#include "App_CanTx.h"

#define HIGH_RES_MAX_CURRENT_READING (50.0f)

#define R_ERROR_COMPENSATION \
    (1.05f) // Percent error used to compensate for resistor errors. Determined from testing with the HW

#define AMPLIFIER_GAIN (8.0f) // Isolated amplifier gain
#define TS_VOLTAGE_DIV \
    (1.24e+3f / (499e+3f + 499e+3f + 499e+3f + 499e+3f + 1.24e+3f)) // Voltage divider for the TS+ voltage sensing

#define OUTPUT_1_OFFSET (2.45f)                   // Offset voltage of output 1. Found to be 2.45V through testing
#define OUTPUT_1_SENSITIVITY (1.0f / 40e-3f)      // Sensitivity of output 1: 40mV/A
#define OUTPUT_1_DIV ((1.1f + 2.155f) / (2.155f)) // Voltage divider from adc --> current sensor output

#define OUTPUT_2_OFFSET (2.45f)                   // Offset voltage of output 2. Found to be 2.45V through testing
#define OUTPUT_2_SENSITIVITY (1.0f / 6.67e-3f)    // Sensitivity of output 2: 6.67mV/A
#define OUTPUT_2_DIV ((1.1f + 2.155f) / (2.155f)) // Voltage divider from adc --> current sensor output

extern struct BmsCanTxInterface can_tx;

static float ts_getLowResCurrent(void);
static float ts_getHighResCurrent(void);

static float ts_getHighResCurrent(void)
{
    const float raw_adc_voltage = Io_Adc_GetAdc2Channel1Voltage();

    // HSNBV-D06 Output 1 (+/- 50A):
    //
    // +------------------+                +-------------+
    // | HSNBV-D06        |---<1.1k>---+---| ADC Channel |
    // | Output 1 Voltage |            |   +-------------+
    // +------------------+            |
    //                               <2.2k>
    //                                 |
    //                                ===
    //                                GND
    //
    //                                                                 1
    // Current = (HSNBV-D06 Output 1 Voltage - Offset Voltage) x ---------------
    //                                                             Sensitivity
    //                                              1k + 2.2k
    // HSNBV-D06 Output 1 Voltage = ADC Voltage x --------------
    //                                                 2.2k
    // Offset Voltage = 2.5V
    //
    // Sensitivity = 40mV/A

    float high_res_current = NAN;
    if (raw_adc_voltage >= 0.0f)
    {
        high_res_current = (raw_adc_voltage * OUTPUT_1_DIV - OUTPUT_1_OFFSET) * OUTPUT_1_SENSITIVITY;
    }

    return high_res_current;
}

static float ts_getLowResCurrent(void)
{
    const float raw_adc_voltage = Io_Adc_GetAdc2Channel3Voltage();

    // HSNBV-D06 Output 2 (+/- 300A):
    //
    // +------------------+                +-------------+
    // | HSNBV-D06        |---<1.1k>---+---| ADC Channel |
    // | Output 2 Voltage |            |   +-------------+
    // +------------------+            |
    //                               <2.2k>
    //                                 |
    //                                ===
    //                                GND
    //
    //                                                                 1
    // Current = (HSNBV-D06 Output 1 Voltage - Offset Voltage) x ---------------
    //                                                             Sensitivity
    //                                              1k + 2.2k
    // HSNBV-D06 Output 2 Voltage = ADC Voltage x --------------
    //                                                 2.2k
    // Offset Voltage = 2.5V
    //
    // Sensitivity = 6.67mV/A

    float low_res_current = NAN;
    if (raw_adc_voltage >= 0.0f)
    {
        low_res_current = (raw_adc_voltage * OUTPUT_2_DIV - OUTPUT_1_OFFSET) * OUTPUT_2_SENSITIVITY;
    }

    return low_res_current;
}

float ts_getVoltage(void)
{
    const float raw_adc_voltage = Io_Adc_GetAdc1Channel3Voltage();

    float ts_voltage = NAN;
    if (raw_adc_voltage >= 0.0f)
    {
        ts_voltage = raw_adc_voltage * R_ERROR_COMPENSATION / (TS_VOLTAGE_DIV * AMPLIFIER_GAIN);
    }

    return ts_voltage;
}

void ts_tick100Hz(void)
{
    const float low_res_current = ts_getLowResCurrent();
    const float ts_current =
        (low_res_current < HIGH_RES_MAX_CURRENT_READING) ? ts_getHighResCurrent() : low_res_current;

    App_CanTx_SetPeriodicSignal_TS_VOLTAGE(&can_tx, ts_getVoltage());
    App_CanTx_SetPeriodicSignal_TS_CURRENT(&can_tx, ts_current);
}
