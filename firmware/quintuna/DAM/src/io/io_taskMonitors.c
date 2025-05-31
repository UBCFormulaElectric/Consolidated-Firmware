#include "io_taskMonitors.h"
#include "main.h"

const TaskMonitor task_1_hz_monitor     = { .handle = &Task1HzHandle, .attributes = &Task1Hz_attributes };
const TaskMonitor task_100_hz_monitor   = { .handle = &Task100HzHandle, .attributes = &Task100Hz_attributes };
const TaskMonitor task_1_khz_monitor    = { .handle = &Task1kHzHandle, .attributes = &Task1kHz_attributes };
const TaskMonitor task_can_rx_monitor   = { .handle = &TaskCanRxHandle, .attributes = &TaskCanRx_attributes };
const TaskMonitor task_can_tx_monitor   = { .handle = &TaskCanTxHandle, .attributes = &TaskCanTx_attributes };
const TaskMonitor task_logging_monitor  = { .handle = &TaskLoggingHandle, .attributes = &TaskLogging_attributes };
const TaskMonitor task_telem_monitor    = { .handle = &TaskTelemHandle, .attributes = &TaskTelem_attributes };
const TaskMonitor task_telem_rx_monitor = { .handle = &TaskTelemRxHandle, .attributes = &TaskTelemRx_attributes };