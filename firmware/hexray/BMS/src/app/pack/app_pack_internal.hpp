#pragma once

#include "io_adbms.hpp"

extern const float V_FAULT_UV;
extern const float V_FAULT_OV;

struct SequenceState {
    result<void> (*run_on_entry)();
    void (*run_on_tick)();
};

namespace app::pack::config
{
[[nodiscard]] result<void> setSegmentConfig(io::adbms::ThermistorMux mux);
[[nodiscard]] result<void> setPWMConfig(const io::adbms::Cells<uint8_t> &duty);
[[nodiscard]] result<void> checkSegmentConfig();
[[nodiscard]] result<void> checkPWMConfig();
} // namespace app::pack::config
