#pragma once
#include "app_signal.hpp"

namespace app::warningHandling
{

constexpr uint32_t app_brake_diagreement_time_to_fault = 10u;
constexpr uint32_t app_brake_diagreement_time_to_clear = 10u;

// bspd
app::Signal softwareBspd_init(void);

// board warnings
bool boardWarningCheck(void);

// Accelerator Brake System Plausibility device (bad user input safety issues)
// Protect against brake/apps active at same time
// Brakes disagreement is detected if brakes are actuated and apps are past 25% threshold
// Allowed to exit disagreement only when apps is released (< 5%)
bool checkSoftwareBspd(float papps_pedal_percentage, app::Signal signal);

} // namespace app::warningHandling
