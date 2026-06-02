#pragma once

namespace fakes::buzzer
{
// Reset recorded state. Call from test SetUp.
void reset();

// True if the most recent io::buzzer::enable/disable left the buzzer enabled.
// False by default (after reset()).
bool isEnabled();

// Number of io::buzzer::enable() calls since reset(). Lets tests verify a single
// enable per pulse rather than a flurry.
unsigned int enableCount();

// Number of io::buzzer::disable() calls since reset().
unsigned int disableCount();

} // namespace fakes::buzzer
