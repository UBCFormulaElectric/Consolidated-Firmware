#include "io_tractiveSystem.hpp"
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"
#include "hw_hal.hpp"
#include "util_utils.hpp"
#include <cassert>
#include <cmath>

// Percent error used to compensate for resistor errors. Determined from testing with the HW
// TODO: Test with HW to determine compensation
namespace io::ts
{

constexpr float AMPLIFIER_GAIN       = 1.0f;                         // Isolated amplifier gain
constexpr float TS_VOLTAGE_DIV       = (20e3f / (6 * 1e6f + 20e3f)); // Voltage divider for the TS+ voltage sensing
constexpr float R_ERROR_COMPENSATION = 1.0f;

// TS Current Sensing
constexpr float LOW_SIDE_R       = 33e3f;
constexpr float HIGH_SIDE_R      = 60.4e3f;
constexpr float TSI_TO_CSIN      = (LOW_SIDE_R + HIGH_SIDE_R) / HIGH_SIDE_R;
constexpr float OFFSET_V         = 2.5f;
constexpr float HIGH_RES_SENS_VA = 26.7e-3f;
constexpr float LOW_RES_SENS_VA  = 4e-3f;

// TS Current Sensing Calibration
constexpr float OUTPUT1_CHARGING_ERROR_SLOPE     = -0.009863f;
constexpr float OUTPUT1_CHARGING_ERROR_OFFSET    = -2.751494f;
constexpr float OUTPUT1_DISCHARGING_ERROR_SLOPE  = -0.009940f;
constexpr float OUTPUT1_DISCHARGING_ERROR_OFFSET = -2.806835f;

constexpr float OUTPUT2_CHARGING_ERROR_SLOPE     = -0.020666f;
constexpr float OUTPUT2_CHARGING_ERROR_OFFSET    = -18.145014f;
constexpr float OUTPUT2_DISCHARGING_ERROR_SLOPE  = -0.019513f;
constexpr float OUTPUT2_DISCHARGING_ERROR_OFFSET = -18.518876f;

float getVoltage(void)
{
    // The tractive system voltage is divided down by several resistors, then fed through an amplifier.
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
    const float ts_vsense_P = ts_vsense_p.getVoltage();
    const float ts_vsense_N = ts_vsense_n.getVoltage();

    if (const float ts_vsense = ts_vsense_P - ts_vsense_N; ts_vsense < 0.0f)
    {
        return 0.0f;
    }
    else
    {
        const float real_voltage = ts_vsense * R_ERROR_COMPENSATION / (TS_VOLTAGE_DIV * AMPLIFIER_GAIN);
        return real_voltage;
    }
}

// Charging => current >= 0, discharging => current < 0
float getCurrentHighResolution()
{
    const float cs_in_1          = MAX(ts_isense_50a.getVoltage(), 0.0f) * TSI_TO_CSIN;
    const float reported_current = -((cs_in_1 - OFFSET_V) / HIGH_RES_SENS_VA);

    // Error calibration for the high resolution current sensor (channel 1).
    const float slope  = (reported_current >= 0.0f) ? OUTPUT1_CHARGING_ERROR_SLOPE : OUTPUT1_DISCHARGING_ERROR_SLOPE;
    const float offset = (reported_current >= 0.0f) ? OUTPUT1_CHARGING_ERROR_OFFSET : OUTPUT1_DISCHARGING_ERROR_OFFSET;
    const float error  = reported_current * slope + offset;

    return reported_current - error;
}

// Charging => current >= 0, discharging => current < 0
float getCurrentLowResolution()
{
    const float cs_in_2          = MAX(ts_isense_400a.getVoltage(), 0.0f) * TSI_TO_CSIN;
    const float reported_current = -((cs_in_2 - OFFSET_V) / LOW_RES_SENS_VA);

    // Error calibration for the low resolution current sensor (channel 2).
    const float slope  = (reported_current >= 0.0f) ? OUTPUT2_CHARGING_ERROR_SLOPE : OUTPUT2_DISCHARGING_ERROR_SLOPE;
    const float offset = (reported_current >= 0.0f) ? OUTPUT2_CHARGING_ERROR_OFFSET : OUTPUT2_DISCHARGING_ERROR_OFFSET;
    const float error  = reported_current * slope + offset;

    return reported_current - error;
}

bool getVoltageSnsDiagState()
{
    return diag_ts.readPin();
}

bool getCurrentSnsDiagState()
{
    return ts_isense_ocsc_ok.readPin();
}

} // namespace io::ts