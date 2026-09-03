#pragma once

#include "io_adbms.hpp"

enum class ThermMux : size_t {
    MUX_0_7,
    MUX_8_13,
};

enum class OwcParity : size_t {
    NONE,
    EVEN,
    ODD,
};

struct SequenceState {
    result<void> (*run_on_entry)();
    void (*run_on_tick)();
};

namespace app::pack::config
{
[[nodiscard]] result<void> setThermMuxConfig(ThermMux mux);
[[nodiscard]] result<void> setBalanceConfig(bool balancing_muted, const io::adbms::Cells<uint8_t> &duty);
[[nodiscard]] bool checkSegmentConfig();
[[nodiscard]] bool checkPwmConfig();
[[nodiscard]] result<void> writeSegmentConfig();
[[nodiscard]] result<void> writePwmConfig();
} // namespace app::pack::config
