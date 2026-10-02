#pragma once

namespace app::button
{
// Returns true exactly once per debounced rising edge of the NTP button.
// Must be called at a fixed cadence from a single caller.
[[nodiscard]] bool ntpWasJustPressed();
} // namespace app::button
