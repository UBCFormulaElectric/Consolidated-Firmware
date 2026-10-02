#pragma once

namespace app::tsim
{
// Reset internal state and start the bootup-ignore window.
void init();

// Advance the TSIM state machine. Call at 100 Hz.
//
// Behavior:
//  - During the first 3 s after init(), fault state is suppressed and TSIM is steady green.
//  - After that, steady green when all of BmsLatchOk/ImdLatchOk/BspdLatchOk are true.
//  - Otherwise, red/off toggle every 150 ms.
void tick();

} // namespace app::tsim
