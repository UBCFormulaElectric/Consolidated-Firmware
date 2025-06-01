#include "io_taskMonitors.h"
#include "main.h"

const StackMonitor task_1_hz_monitor   = { .handle = &Task1HzHandle, .attributes = &Task1Hz_attributes },
                  task_100_hz_monitor = { .handle = &Task100HzHandle, .attributes = &Task100Hz_attributes },
                  task_1_khz_monitor  = { .handle = &Task1kHzHandle, .attributes = &Task1kHz_attributes },
                  task_can_rx_monitor = { .handle = &TaskCanRxHandle, .attributes = &TaskCanRx_attributes },
                  task_can_tx_monitor = { .handle = &TaskCanTxHandle, .attributes = &TaskCanTx_attributes },
                  task_ltc_monitor   = { .handle = &TaskLTCHandle, .attributes = &TaskLTC_attributes };