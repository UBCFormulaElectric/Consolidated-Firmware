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

enum class CellStepState : uint8_t {
    IDLE,
    START_REDUNDANT,          
    START_NO_REDUNDANT,
    READ_VOLTAGES,
    RECOVER,
};

enum class ThermStepState : uint8_t {
    IDLE,
    MUX_0_7_START,
    MUX_0_7_READ,
    MUX_8_13_START,
    MUX_8_13_READ,
};

enum class OwcStepState : uint8_t {
    IDLE,
    START_SADC_OW_ODD,
    POLL_SADC_OW_ODD,
    READ_SADC_OW_ODD,
    START_SADC_OW_EVEN,
    POLL_SADC_OW_EVEN,
    READ_SADC_OW_EVEN,
};

enum class ConfigStepState : uint8_t {
    IDLE,
    WRITE,
    CHECK,
};

enum class FlagsStepState : uint8_t {
    IDLE,
    READ_STATUS,
    CLEAR_STATUS,
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