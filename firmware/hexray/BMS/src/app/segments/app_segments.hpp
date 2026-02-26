#pragma once

#include "io_adbms.hpp"

#include <array>
#include <expected>

namespace app::segments
{
enum class ThermistorMux
{
    THERMISTOR_MUX_0_7,
    THERMISTOR_MUX_8_13,
    THERMISTOR_MUX_COUNT,
};

void setDefaultConfig();
void setBalanceConfig(std::array<std::array<bool, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> balance_config);
void setThermistorConfig(ThermistorMux mux);
std::expected<void, ErrorCode> configSync();
} // namespace app::segments
