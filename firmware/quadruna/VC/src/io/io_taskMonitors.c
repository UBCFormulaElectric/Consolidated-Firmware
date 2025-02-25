#include "io_taskMonitors.h"
#include "main.h"

const TaskMonitor task_1Hz_task_monitor = {
    .handle     = &Task1HzHandle,
    .attributes = &Task1Hz_attributes,
}, task_100Hz_task_monitor = {
    .handle     = &Task100HzHandle,
    .attributes = &Task100Hz_attributes,
}, task_1kHz_task_monitor = {
    .handle     = &Task1kHzHandle,
    .attributes = &Task1kHz_attributes,
}, task_can_rx_task_monitor = {
    .handle     = &TaskCanRxHandle,
    .attributes = &TaskCanRx_attributes,
}, task_can_tx_task_monitor = {
    .handle     = &TaskCanTxHandle,
    .attributes = &TaskCanTx_attributes,
}, task_logging_task_monitor = {
    .handle     = &TaskLoggingHandle,
    .attributes = &TaskLogging_attributes,
}, task_telem_task_monitor = {
    .handle = &TaskTelemHandle,
    .attributes = &TaskTelem_attributes,
};
