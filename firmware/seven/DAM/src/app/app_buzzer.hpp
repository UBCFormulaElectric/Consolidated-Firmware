#pragma once

namespace app::buzzer
{
// Reset the buzzer pulse timer and ensure the buzzer is off.
void init();

// Drive the buzzer. Call at 100 Hz.
//
// Behavior:
//  - On the rising edge of VC_State == VC_DRIVE_STATE, sound the buzzer for ~1 s.
//  - While drive state stays asserted past 1 s, buzzer is silent (one-shot).
//  - Leaving and re-entering drive state re-arms the pulse.
void tick();

} // namespace app::buzzer
