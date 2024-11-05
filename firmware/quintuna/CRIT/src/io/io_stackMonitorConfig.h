#pragma once
#include "main.h"

inline io::StackMonitor task_1_hz_stack_monitor{ Task1HzHandle, 0.7f };
inline io::StackMonitor task_100_hz_stack_monitor{ Task100HzHandle, 0.7f };
inline io::StackMonitor task_1_khz_stack_monitor{ Task1kHzHandle, 0.7f };
inline io::StackMonitor task_canrx_stack_monitor{ TaskCanRxHandle, 0.7f };
inline io::StackMonitor task_cantx_stack_monitor{ TaskCanTxHandle, 0.7f };