#include "io_taskMonitor.h"
#include "main.h"

const TaskMonitor task_1hz_monitor   = { .handle = &Task1HzHandle, .attributes = &Task1Hz_attributes },
                  task_100hz_monitor = { .handle = &Task100HzHandle, .attributes = &Task100Hz_attributes },
                  task_1khz_monitor  = { .handle = &Task1kHzHandle, .attributes = &Task1kHz_attributes },
                  task_canrx_monitor = { .handle = &TaskCanRxHandle, .attributes = &TaskCanRx_attributes },
                  task_cantx_monitor = { .handle = &TaskCanTxHandle, .attributes = &TaskCanTx_attributes };
