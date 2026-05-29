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

constexpr float AMPLIFIER_GAIN = 1.0f; // Isolated amplifier gain
constexpr float TS_VOLTAGE_DIV = (20e3f / (6 * 1e6f + 20e3f)); // Voltage divider for the TS+ voltage sensing
constexpr float R_ERROR_COMPENSATION = 1.0f;


// // Current Sensor error calibration parameters (based on experimental data)
// // TODO: Rerun sensor calibration with new mounting
constexpr float OUTPUT1_DISCHARGING_ERROR_SLOPE  = 0.5028f;
constexpr float OUTPUT1_DISCHARGING_ERROR_OFFSET = -0.0894f;
constexpr float OUTPUT1_CHARGING_ERROR_SLOPE     = 0.5045f;
constexpr float OUTPUT1_CHARGING_ERROR_OFFSET    = -0.2677f;

constexpr float OUTPUT2_DISCHARGING_ERROR_SLOPE  = 0.2417f;
constexpr float OUTPUT2_DISCHARGING_ERROR_OFFSET = 2.3634f;
constexpr float OUTPUT2_CHARGING_ERROR_SLOPE     = 0.2324f;
constexpr float OUTPUT2_CHARGING_ERROR_OFFSET    = 2.4038f;


// TS Current Sensing
constexpr float LOW_SIDE_R = 33e3f;
constexpr float HIGH_SIDE_R = 60.4e3f;
constexpr float TSI_TO_CSIN = (LOW_SIDE_R + HIGH_SIDE_R) / HIGH_SIDE_R;
constexpr float OFFSET_V = 2.5f;
constexpr float HIGH_RES_SENS_VA = 26.7e-3f;
constexpr float LOW_RES_SENS_VA = 4e-3f;





constexpr float OUTPUT_1_OFFSET = 2.5f; // Offset voltage of output 1.
constexpr float OUTPUT_1_SENSITIVITY = (1.0f / 40.0e-3f); // Sensitivity of output 1: 40mV/A
constexpr float OUTPUT_2_OFFSET = 2.5f; // Offset voltage of output 2.
constexpr float OUTPUT_2_SENSITIVITY = (1.0f / 5.2e-3f); // Sensitivity of output 2: 5.2mV/A




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

float getCurrentHighResolution()
{
    const float cs_in_1 = MAX(ts_isense_50a.getVoltage(), 0.0f) * TSI_TO_CSIN;
    const float high_res_current = (cs_in_1 - OFFSET_V) / HIGH_RES_SENS_VA;

    // Error Calibration for High Resolution Current Sensor (based on calibration data)
    float high_res_curr_calibration = 0.0f;
    if (high_res_current > -0.2f)
    {
        high_res_curr_calibration =
            high_res_current * OUTPUT1_DISCHARGING_ERROR_SLOPE + OUTPUT1_DISCHARGING_ERROR_OFFSET;
    }
    else
    {
        high_res_curr_calibration = high_res_current * OUTPUT1_CHARGING_ERROR_SLOPE + OUTPUT1_CHARGING_ERROR_OFFSET;
    }

    return -(high_res_current + high_res_curr_calibration);
}

float getCurrentLowResolution()
{
    const float cs_in_2 = MAX(ts_isense_400a.getVoltage(), 0.0f) * TSI_TO_CSIN;
    const float low_res_current = (cs_in_2 - OFFSET_V) / HIGH_RES_SENS_VA;

    // Error Calibration for Low Resolution Current Sensor (based on calibration data)
    float low_res_curr_calibration = 0.0f;
    if (low_res_current > -0.2f)
    {
        low_res_curr_calibration = low_res_current * OUTPUT2_DISCHARGING_ERROR_SLOPE + OUTPUT2_DISCHARGING_ERROR_OFFSET;
    }
    else
    {
        low_res_curr_calibration = low_res_current * OUTPUT2_CHARGING_ERROR_SLOPE + OUTPUT2_CHARGING_ERROR_OFFSET;
    }

    return -(low_res_current + low_res_curr_calibration);
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