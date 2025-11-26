#pragma once

namespace vc::app::warningHandling {
// board warnings
bool app_warningHandling_boardWarningCheck(void);

// bspd
void app_softwareBspd_init(void);
bool app_warningHandling_checkSoftwareBspd(float papps_pedal_percentage);

} // namespace vc::app::warningHandling
