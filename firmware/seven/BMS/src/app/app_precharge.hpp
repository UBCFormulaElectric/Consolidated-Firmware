// Why not just include <cstdint> ??

// new precharge hpp
#pragma once

#include <cstdint>

namespace app::precharge
{
// Constants (compile-time)
inline constexpr std::uint32_t NUM_OF_INVERTERS = 4U;

inline constexpr float PRECHARGE_RESISTOR_OHMS = 1.5e3f + 1.5e3f; // Based on ESF
inline constexpr float INV_CAPACITANCE_F       = 75e-6f;          // 75uF per inverter, based on data sheet
inline constexpr float PRECHARGE_CAPACITANCE_F = INV_CAPACITANCE_F * NUM_OF_INVERTERS; // 300 uF

// RC time (s and ms)
inline constexpr float         PRECHARGE_RC_S  = PRECHARGE_RESISTOR_OHMS * PRECHARGE_CAPACITANCE_F; // 0.9 s
inline constexpr std::uint32_t PRECHARGE_RC_MS = static_cast<std::uint32_t>(PRECHARGE_RC_S * 1000.0f);

// 2.7 RC ≈ 93% charged
inline constexpr float PRECHARGE_COMPLETION_MS_F = PRECHARGE_RC_S * 1000.0f * 2.7f; // RC curve (VERIFY)

// Arbitrary values. Verify in testing
inline constexpr std::uint32_t PRECHARGE_COMPLETION_LOWER_BOUND =
    static_cast<std::uint32_t>(PRECHARGE_COMPLETION_MS_F * 0.5f);
inline constexpr std::uint32_t PRECHARGE_COMPLETION_UPPER_BOUND =
    static_cast<std::uint32_t>(PRECHARGE_COMPLETION_MS_F * 3.0f);
inline constexpr std::uint32_t PRECHARGE_COOLDOWN_MS      = 1000U; // retry pause after fail
inline constexpr std::uint32_t PRECHARGE_LATCH_TIMEOUT_MS = 3000U; // 3s latch timeout

// When TS reaches this fraction of ACC, AIR+ engages; MUST be ≥ 90% (EV.5.6.1 2027)
inline constexpr float        PRECHARGE_ACC_V_THRESHOLD = 0.90f;
inline constexpr std::uint8_t MAX_PRECHARGE_ATTEMPTS    = 3U;

// State
enum class State
{
    RUNNING,
    SUCCESS,
    FAILED,
    FAILED_CRITICAL,
    COOLDOWN,
};

// API (TO DO: CONFIRM)
void  init();
void  restart();
State poll(bool precharge_for_charging);
bool  limitExceeded();
void  resetLimitExceeded();

} // namespace app::precharge