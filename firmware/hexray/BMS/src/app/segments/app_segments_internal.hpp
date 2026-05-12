#pragma once

#include <cmath>
#include "app_segments.hpp"

inline constexpr uint8_t  MAX_NUM_SEGMENTS = 10U;
inline constexpr uint16_t VUV              = 0x01A1; // 2.5V
inline constexpr uint16_t VOV              = 0x0465; // 4.2V

inline constexpr uint8_t VOLT_CONV_TIME_MS          = 2U;  // check later
inline constexpr uint8_t AUX_CONV_TIME_MS           = 10U; // check later
inline constexpr uint8_t OWC_CONVERSION_TIME_MS     = 8U;  // check later
inline constexpr float   OW_CELL_RELATIVE_THRESHOLD = 0.7f;
inline constexpr float   OW_CELL_ABSOLUTE_THRESHOLD = 0.5f;

inline constexpr float V_REF2             = 3.0f;
inline constexpr float R_SERIES           = 10e3f; // Fixed resistor
inline constexpr float R_NOMINAL          = 10e3f; // Thermistor at 25C
inline constexpr float T_NOMINAL          = 298.15f;
inline constexpr float BETA_COEFF         = 3610.0f;
inline constexpr float KELVIN_OFFSET      = 273.15f;
inline constexpr float OW_THERM_THRESHOLD = 0.0f; // TODO: need to calibrate

namespace app::segments
{

constexpr float convertUVOVToFloat(const uint16_t hex)
{
    return (hex * 16 * 150e-6f + 1.5f);
}

constexpr float convertRegToVoltage(const uint16_t reg)
{
    return (static_cast<float>(static_cast<int16_t>(reg)) * 150e-6f) + 1.5f;
}

static constexpr bool checkCellOwcOk(const float baselineVoltage, const float owcVoltage)
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

// CAN setter tables.
using CellVoltageSetters         = void (*)(float);
using FilteredCellVoltageSetters = void (*)(float);
using CellTemperatureSetters     = void (*)(float);
using SegmentVoltageSetters      = void (*)(float);
using CellOwcSetters             = void (*)(bool);
using ThermOwcSetters            = void (*)(bool);
using SegmentCommOkSetters       = void (*)(bool);
using SegmentVref2Setters        = void (*)(float);
using SegmentITMPSetters         = void (*)(float);
using SegmentVDSetters           = void (*)(float);
using SegmentVASetters           = void (*)(float);
using SegmentVRESSetters         = void (*)(float);
using SegmentVaOvSetters         = void (*)(bool);
using SegmentVaUvSetters         = void (*)(bool);
using SegmentVdOvSetters         = void (*)(bool);
using SegmentVdUvSetters         = void (*)(bool);
using SegmentCedSetters          = void (*)(bool);
using SegmentCmedSetters         = void (*)(bool);
using SegmentSedSetters          = void (*)(bool);
using SegmentSmedSetters         = void (*)(bool);
using SegmentVdeSetters          = void (*)(bool);
using SegmentVdelSetters         = void (*)(bool);
using SegmentThsdSetters         = void (*)(bool);
using SegmentTmodchkSetters      = void (*)(bool);
using SegmentOscchkSetters       = void (*)(bool);
using CellOvSetters              = void (*)(bool);
using CellUvSetters              = void (*)(bool);

extern Cell        cell_voltages;
extern CellSuccess cell_voltage_success;
extern Cell        filtered_cell_voltages;
extern CellSuccess filtered_cell_voltage_success;
extern Therm       cell_temps;
extern Owc         cell_owc_ok;
extern Owc         therm_owc_ok;

extern CellParam max_cell_voltage;
extern CellParam min_cell_voltage;
extern CellParam max_cell_temp;
extern CellParam min_cell_temp;

extern CellVoltageSetters         cell_voltage_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT];
extern CellTemperatureSetters     cell_temperature_setters[MAX_NUM_SEGMENTS][io::THERMISTORS_PER_SEGMENT];
extern FilteredCellVoltageSetters filtered_cell_voltage_setters[MAX_NUM_SEGMENTS][io::THERMISTORS_PER_SEGMENT];
extern SegmentVoltageSetters      segment_voltages_setters[MAX_NUM_SEGMENTS];
extern CellOwcSetters             cell_owc_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT];
extern ThermOwcSetters            therm_owc_setters[MAX_NUM_SEGMENTS][io::THERMISTORS_PER_SEGMENT];
extern SegmentCommOkSetters       segment_comm_ok_setters[MAX_NUM_SEGMENTS];
extern SegmentVref2Setters        segment_vref2_setters[MAX_NUM_SEGMENTS];
extern SegmentITMPSetters         segment_itmp_setters[MAX_NUM_SEGMENTS];
extern SegmentVDSetters           segment_vd_setters[MAX_NUM_SEGMENTS];
extern SegmentVASetters           segment_va_setters[MAX_NUM_SEGMENTS];
extern SegmentVRESSetters         segment_vres_setters[MAX_NUM_SEGMENTS];
extern SegmentVaOvSetters         segment_va_ov_setters[MAX_NUM_SEGMENTS];
extern SegmentVaUvSetters         segment_va_uv_setters[MAX_NUM_SEGMENTS];
extern SegmentVdOvSetters         segment_vd_ov_setters[MAX_NUM_SEGMENTS];
extern SegmentVdUvSetters         segment_vd_uv_setters[MAX_NUM_SEGMENTS];
extern SegmentCedSetters          segment_ced_setters[MAX_NUM_SEGMENTS];
extern SegmentCmedSetters         segment_cmed_setters[MAX_NUM_SEGMENTS];
extern SegmentSedSetters          segment_sed_setters[MAX_NUM_SEGMENTS];
extern SegmentSmedSetters         segment_smed_setters[MAX_NUM_SEGMENTS];
extern SegmentVdeSetters          segment_vde_setters[MAX_NUM_SEGMENTS];
extern SegmentVdelSetters         segment_vdel_setters[MAX_NUM_SEGMENTS];
extern SegmentThsdSetters         segment_thsd_setters[MAX_NUM_SEGMENTS];
extern SegmentTmodchkSetters      segment_tmodchk_setters[MAX_NUM_SEGMENTS];
extern SegmentOscchkSetters       segment_oscchk_setters[MAX_NUM_SEGMENTS];
extern CellOvSetters              cell_ov_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT];
extern CellUvSetters              cell_uv_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT];
} // namespace app::segments
