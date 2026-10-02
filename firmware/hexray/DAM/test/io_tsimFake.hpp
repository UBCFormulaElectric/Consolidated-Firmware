#pragma once

namespace fakes::tsim
{
enum class Color
{
    OFF,
    RED,
    GREEN,
};

// Reset recorded state. Call from test SetUp.
void reset();

// The color set by the most recent io::tsim::set_*() call. OFF by default.
Color lastColor();

// Number of io::tsim::set_*() calls since reset(). Useful for asserting that
// "no_fault" turns into a single steady-state call, not a flurry.
unsigned int callCount();

} // namespace fakes::tsim
