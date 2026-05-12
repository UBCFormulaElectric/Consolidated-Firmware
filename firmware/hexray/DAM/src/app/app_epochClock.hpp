#pragma once

#include <cstdint>
#include <expected>
#include <util_errorCodes.hpp>

#include "io_rtc.hpp"

namespace app::epochClock
{

inline constexpr uint64_t MS_PER_DAY    = 86'400'000ULL;
inline constexpr uint64_t MS_PER_HOUR   = 3'600'000ULL;
inline constexpr uint64_t MS_PER_MINUTE = 60'000ULL;
inline constexpr uint64_t MS_PER_SECOND = 1'000ULL;
inline constexpr uint32_t PREDIV_S      = 999;

// Read both Date and Time from the RTC and return Unix epoch ms.
std::expected<uint64_t, ErrorCode> getEpochMs();

// Split Unix epoch ms into {Date, Time} and program both into the RTC.
std::expected<void, ErrorCode> setEpochMs(uint64_t epoch_ms);

} // namespace app::epochClock
