#pragma once

#include "io_adbms.hpp"

enum class ADBMSState : size_t {
    MEASURE,
    BALANCE,
    DIAGNOSTIC,
};
 
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
    const char *name;
    bool (*run_on_entry)();  // true = entry complete, safe to start ticking
    void (*run_on_tick)();
    bool (*run_on_exit)();   // true = exit complete, safe to leave
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
