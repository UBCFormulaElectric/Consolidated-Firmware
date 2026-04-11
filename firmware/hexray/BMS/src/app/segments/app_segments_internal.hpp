#pragma once

#include "app_segments.hpp"

inline constexpr uint8_t MAX_NUM_SEGMENTS = 10U;
namespace app::segments
{

/**
 * @file segments/app_segments_balancing.cpp
 */
extern float                    pack_voltage;
extern app::segments::CellParam max_cell_voltage;
extern app::segments::CellParam min_cell_voltage;
extern app::segments::CellParam max_cell_temp;
extern app::segments::CellParam min_cell_temp;

/**
 * @file segments/app_segments_broadcast.cpp
 */
extern array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>       cell_voltages;
extern array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>       filtered_cell_voltages;
extern array<array<float, io::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_temps;
extern array<array<bool, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>        cell_owc_ok;
extern array<array<bool, io::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS>  therm_owc_ok;

/**
 * @file segments/app_segments_config.cpp
 */

/**
 * @file segments/app_segments_conversions.cpp
 */
extern array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_voltage_regs;
extern array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_success;

extern array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  filtered_cell_voltage_regs;
extern array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> filtered_cell_voltage_success;

extern array<
    array<array<uint16_t, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>,
    static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
    cell_temp_regs;
extern array<
    array<array<expected<void, ErrorCode>, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>,
    static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
    cell_temp_success;

extern array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_baseline_regs;
extern array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_baseline_success;
extern array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_owc_odd_regs;
extern array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_odd_success;
extern array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_owc_even_regs;
extern array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_even_success;

extern array<array<uint16_t, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_odd_regs;
extern array<array<expected<void, ErrorCode>, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>
                                                                                    therm_owc_odd_success;
extern array<array<uint16_t, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_even_regs;
extern array<array<expected<void, ErrorCode>, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>
    therm_owc_even_success;

extern array<io::adbms::StatusGroups, io::NUM_SEGMENTS>   stat_regs;
extern array<expected<void, ErrorCode>, io::NUM_SEGMENTS> stat_success;

/**
 * @file segments/app_segments_getters.cpp
 */

/**
 * @file segments/app_segments_setters.cpp
 */
using CellVoltageSetters         = void (*)(float);
using FilteredCellVoltageSetters = void (*)(float);
using CellTemperatureSetters     = void (*)(float);
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

extern CellVoltageSetters         cell_voltage_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT];
extern CellTemperatureSetters     cell_temperature_setters[MAX_NUM_SEGMENTS][io::THERMISTORS_PER_SEGMENT];
extern FilteredCellVoltageSetters filtered_cell_voltage_setters[MAX_NUM_SEGMENTS][io::THERMISTORS_PER_SEGMENT];
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