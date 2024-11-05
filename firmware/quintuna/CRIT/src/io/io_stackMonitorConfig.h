#pragma once
#include "main.h"

inline io::StackMonitor task_1_hz_stack_monitor{ &Task1HzHandle, Task1Hz_attributes.stack_size };
inline io::StackMonitor task_100_hz_stack_monitor{ &Task100HzHandle, Task100Hz_attributes.stack_size };
inline io::StackMonitor task_1_khz_stack_monitor{ &Task1kHzHandle, Task1kHz_attributes.stack_size };
inline io::StackMonitor task_canrx_stack_monitor{ &TaskCanRxHandle, TaskCanRx_attributes.stack_size };
inline io::StackMonitor task_cantx_stack_monitor{ &TaskCanTxHandle, TaskCanTx_attributes.stack_size };