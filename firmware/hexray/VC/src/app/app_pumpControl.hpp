#pragma once
#include "io_pumps.hpp"

namespace app::pumpControl
{
void MonitorPumps();
void restart();
#ifdef TARGET_TEST
void reset();
#endif
} // namespace app::pumpControl
