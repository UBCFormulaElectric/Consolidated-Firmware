#pragma once
#include "main.h"

inline io::StackMonitor task_1_hz_stack_monitor{ Task1HzHandle };
inline io::StackMonitor task_100_hz_stack_monitor{ Task100HzHandle };
inline io::StackMonitor task_1_khz_stack_monitor{ Task1kHzHandle };
inline io::StackMonitor task_canrx_stack_monitor{ TaskCanRxHandle };
inline io::StackMonitor task_cantx_stack_monitor{ TaskCanTxHandle };