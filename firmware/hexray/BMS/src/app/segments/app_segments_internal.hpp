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

/**
 * @file segments/app_segments_config.cpp
 */

/**
 * @file segments/app_segments_conversions.cpp
 */
extern array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_voltage_regs;
extern array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_success;
extern array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                     cell_voltages;

extern array<array<array<uint16_t, io::adbms::GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>, 2> cell_temp_regs;
extern array<array<array<expected<void, ErrorCode>, io::adbms::GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>, 2>
                                                                          cell_temp_success;
extern array<array<float, io::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_temps;

extern array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_owc_odd_regs;
extern array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_odd_success;
extern array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_owc_even_regs;
extern array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_even_success;

extern array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  therm_owc_odd_regs;
extern array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_odd_success;
extern array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  therm_owc_even_regs;
extern array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_even_success;

/**
 * @file segments/app_segments_getters.cpp
 */

/**
 * @file segments/app_segments_setters.cpp
 */
using CellVoltageSetters     = void (*)(float);
using CellTemperatureSetters = void (*)(float);
using CellOwcSetters         = void (*)(bool);

static constexpr uint8_t OWC_CHANNELS_PER_SEGMENT = 15U; // CELLS_PER_SEGMENT + 1

extern CellVoltageSetters     cell_voltage_setters[MAX_NUM_SEGMENTS][io::CELLS_PER_SEGMENT];
extern CellTemperatureSetters cell_temperature_setters[MAX_NUM_SEGMENTS][io::THERMISTORS_PER_SEGMENT];
extern CellOwcSetters         cell_owc_setters[MAX_NUM_SEGMENTS][OWC_CHANNELS_PER_SEGMENT];
} // namespace app::segments