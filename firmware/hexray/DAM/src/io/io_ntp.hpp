#pragma once

namespace io::ntp
{
// Returns true exactly once per rising edge
// Must be called at a fixed cadence from a SINGLE caller
bool wasJustPressed();
} // namespace io::ntp