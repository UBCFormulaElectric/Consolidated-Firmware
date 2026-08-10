#pragma once

#include "io_adbms.hpp"

enum class Step {
    Cells,
    ThermsMux1,
    ThermsMux2,
    CellOpenWireOdd,
    CellOpenWireEven,
    Config,
    ADBMS6830Flags
};

enum class ThermMux : size_t {
    MUX_0_7,
    MUX_8_13,
    MUX_COUNT,
};

enum class OpenWire : size_t {
    ODD,
    EVEN,
    COUNT,
};

namespace app::pack::config
{
[[nodiscard]] result<void> setThermMuxConfig(ThermMux mux);
[[nodiscard]] result<void> setBalanceConfig(bool balancing_muted, const io::adbms::Cells<uint8_t> &duty);
// Read the registers back off the chain and compare against the in-memory copy. A segment that
// can't be read counts as a mismatch.
[[nodiscard]] bool checkSegmentConfig();
[[nodiscard]] bool checkPwmConfig();
[[nodiscard]] result<void> syncSegmentConfig();
[[nodiscard]] result<void> syncPwmConfig();
} // namespace app::pack::config