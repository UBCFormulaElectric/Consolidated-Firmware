#pragma once
#include "app_signal.hpp"
extern "C"
{
#include "app_canAlerts.h"
#include "app_canRx.h"
#include "app_canTx.h"
}

namespace vc::app::warningHandling
{
// board warnings
bool app_warningHandling_boardWarningCheck(void);

// bspd
void app_softwareBspd_init(void);
bool app_warningHandling_checkSoftwareBspd(float papps_pedal_percentage);

} // namespace vc::app::warningHandling
