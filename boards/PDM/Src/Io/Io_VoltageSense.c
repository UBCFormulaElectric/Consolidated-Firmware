#include "Io_VoltageSense.h"
#include "Io_Adc.h"

// Isolated amplifier gain

#define AMPLIFIER_GAIN (1.0f)

// Percent error used to compensate for resistor errors. Determined from
// testing with the HW
#define VBAT_R_ERROR_COMPENSATION (1.044f)
#define VBOOST_R_ERROR_COMPENSATION (1.0f)
#define VACC_R_ERROR_COMPENSATION (1.0f)

// Voltage divider for voltage sensing
#define VBAT_VOLTAGE_DIV (220e3f / (220e3f + 330e3f))
#define VBOOST_VOLTAGE_DIV (3.1e3f / (20e3f + 3.1e3f))
#define VACC_VOLTAGE_DIV (3.1e3f / (20e3f + 3.1e3f))

float Io_VoltageSense_GetVbatVoltage(void)
{
    // Modified from BMS/Src/Io/Io_VoltageSense.c:
    //
    //  Battery Voltage
    //        -+-
    //         |                          +----------+  +-------------+
    //         +----------<330k>-------+--| Amplifier|--| ADC Channel |
    //                                 |  +----------+  +-------------+
    //                                 |
    //                               <220k>
    //                                 |
    //                                ===
    //                                GND
    //
    //  Battery Voltage x Voltage Ratio x Amplifier Gain = ADC voltage, where
    //
    //                       220k
    // Voltage Ratio = ---------------
    //                   220k + 330k
    //
    // Amplifier Gain = 1
    //
    // We can rewrite the earlier equation as:
    //
    //                          ADC voltage
    // TS_Voltage = ---------------------------------- * Measurement Error %
    //                Voltage Ratio x Amplifier Gain

    float adc_reading = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_10);
    return adc_reading * VBAT_R_ERROR_COMPENSATION / (AMPLIFIER_GAIN * VBAT_VOLTAGE_DIV);
}

float Io_VoltageSense_GetBoostVoltage(void)
{
    float adc_reading = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_12);
    return adc_reading * VBOOST_R_ERROR_COMPENSATION / (AMPLIFIER_GAIN * VBOOST_VOLTAGE_DIV);
}

float Io_VoltageSense_GetAccVoltage(void)
{
    float adc_reading = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_11);
    return adc_reading * VACC_R_ERROR_COMPENSATION / (AMPLIFIER_GAIN * VACC_VOLTAGE_DIV);
}
