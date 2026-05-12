#pragma once
#include "io_pumpControl.hpp"

namespace app::pumpControl
{
void MonitorPumps();
#ifdef TARGET_TEST
void reset();
#endif
} // namespace app::pumpControl
