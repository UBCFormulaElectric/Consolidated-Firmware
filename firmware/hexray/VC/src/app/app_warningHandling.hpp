#pragma once
// TODO: refactor the signal
#include "app_signal.hpp"
namespace app::warningHandling
{
// board warnings
bool app_warningHandling_boardWarningCheck(void);

// bspd
void app_softwareBspd_init(void);

// Accelerator Brake System Plausibility device (bad user input safety issues)
// Protect against brake/apps active at same time
// Brakes disagreement is detected if brakes are actuated and apps are past 25% threshold
// Allowed to exit disagreement only when apps is released (< 5%)
bool app_warningHandling_checkSoftwareBspd(float papps_pedal_percentage);

} // namespace app::warningHandling
