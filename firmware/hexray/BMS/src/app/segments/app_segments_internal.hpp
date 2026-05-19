#pragma once

#include <cmath>
#include "app_segments.hpp"

inline constexpr uint8_t  MAX_NUM_SEGMENTS           = 10U;
inline constexpr uint16_t VUV                        = 0x01A1; // 2.5V
inline constexpr uint16_t VOV                        = 0x0465; // 4.2V
inline constexpr float    OW_CELL_RELATIVE_THRESHOLD = 0.7f;
inline constexpr float    OW_CELL_ABSOLUTE_THRESHOLD = 0.5f;

inline constexpr float V_REF2             = 3.0f;
inline constexpr float R_SERIES           = 10e3f; // Fixed resistor
inline constexpr float R_NOMINAL          = 10e3f; // Thermistor at 25C
inline constexpr float T_NOMINAL          = 298.15f;
inline constexpr float BETA_COEFF         = 3610.0f;
inline constexpr float KELVIN_OFFSET      = 273.15f;
inline constexpr float OW_THERM_THRESHOLD = 2.8f; // TODO: need to calibrate

namespace app::segments
{
constexpr float convertUVOVToFloat(const uint16_t hex)
{
    return (hex * 16 * 150e-6f + 1.5f);
}

constexpr float convertRegToVoltage(const uint16_t reg)
{
    return static_cast<float>(static_cast<int16_t>(reg)) * 150e-6f + 1.5f;
}

constexpr bool checkCellOwcOk(const float baselineVoltage, const float owcVoltage)
{
    if (owcVoltage < baselineVoltage * OW_CELL_RELATIVE_THRESHOLD)
        return false;
    if (owcVoltage < OW_CELL_ABSOLUTE_THRESHOLD)
        return false;
    return true;
}

constexpr float convertRegToTemp(const uint16_t reg)
{
    const float voltage    = convertRegToVoltage(reg);
    const float resistance = R_SERIES * (voltage / (V_REF2 - voltage));
    const float inv_temp_k = (1.0f / T_NOMINAL) + (1.0f / BETA_COEFF) * std::log(resistance / R_NOMINAL);

    return (1.0f / inv_temp_k) - KELVIN_OFFSET;
}

constexpr bool checkThermOwcOk(const uint16_t reg)
{
    return convertRegToVoltage(reg) > OW_THERM_THRESHOLD;
}
} // namespace app::segments
