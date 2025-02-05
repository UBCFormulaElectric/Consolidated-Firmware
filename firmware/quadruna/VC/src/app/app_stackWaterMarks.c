#include "app_stackWaterMarks.h"
#include "io_taskMonitors.h"
#include "app_canAlerts.h"

/** @brief The stack watermark threshold as a percentage of the stack size */
#define STACK_HIGH_WATERMARK_THRESHOLD 0.7f

void app_stackWaterMark_check(void)
{
    app_canAlerts_VC_Warning_StackWaterMarkHighTask1Hz_set(
        io_taskMonitor_getStackUsage(&task_1Hz_task_monitor) > STACK_HIGH_WATERMARK_THRESHOLD);
    app_canAlerts_VC_Warning_StackWaterMarkHighTask100Hz_set(
        io_taskMonitor_getStackUsage(&task_100Hz_task_monitor) > STACK_HIGH_WATERMARK_THRESHOLD);
    app_canAlerts_VC_Warning_StackWaterMarkHighTask1kHz_set(
        io_taskMonitor_getStackUsage(&task_1kHz_task_monitor) > STACK_HIGH_WATERMARK_THRESHOLD);
    app_canAlerts_VC_Warning_StackWaterMarkHighTaskCanRx_set(
        io_taskMonitor_getStackUsage(&task_can_rx_task_monitor) > STACK_HIGH_WATERMARK_THRESHOLD);
    app_canAlerts_VC_Warning_StackWaterMarkHighTaskCanTx_set(
        io_taskMonitor_getStackUsage(&task_can_tx_task_monitor) > STACK_HIGH_WATERMARK_THRESHOLD);
}
