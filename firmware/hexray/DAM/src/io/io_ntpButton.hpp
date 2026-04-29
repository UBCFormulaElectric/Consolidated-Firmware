#pragma once

namespace io::ntpButton
{
// Returns true exactly once per rising edge
// Must be called at a fixed cadence from a SINGLE caller
[[nodiscard]] bool wasJustPressed();
} // namespace io::ntpButton