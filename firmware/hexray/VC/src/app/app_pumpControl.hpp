#pragma once
#include "io_pumps.hpp"

namespace app::pumpControl
{
void MonitorPumps();
#ifdef TARGET_TEST
void reset();
#endif
} // namespace app::pumpControl
