#pragma once
#include "app_stackMonitor.h"

#include "io_stackMonitorConfig.h"

namespace app
{
inline StackMonitor<5> crit_stack_monitor {
  {
    task_1_hz_stack_monitor,
    task_100_hz_stack_monitor,
    task_1_khz_stack_monitor,
    task_canrx_stack_monitor,
    task_cantx_stack_monitor
  }
};
}
