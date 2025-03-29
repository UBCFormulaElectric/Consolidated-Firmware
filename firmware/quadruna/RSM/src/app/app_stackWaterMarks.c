#include "app_stackWaterMarks.h"

#include "io_taskMonitors.h"
#include "app_canAlerts.h"

/** @brief The stack watermark threshold as a percentage of the stack size */
#define STACK_HIGH_WATERMARK_THRESHOLD 0.7f

void app_stackWaterMark_check(void)
{
    // io_stackWaterMark_check(stack_watermarks, NUM_ELEMENTS_IN_ARRAY(stack_watermarks));
    app_canAlerts_RSM_Warning_StackWaterMarkHighTask1Hz_set(
        io_taskMonitor_getStackUsage(&task_1hz_monitor) > STACK_HIGH_WATERMARK_THRESHOLD);
    app_canAlerts_RSM_Warning_StackWaterMarkHighTask100Hz_set(
        io_taskMonitor_getStackUsage(&task_100hz_monitor) > STACK_HIGH_WATERMARK_THRESHOLD);
    app_canAlerts_RSM_Warning_StackWaterMarkHighTask1kHz_set(
        io_taskMonitor_getStackUsage(&task_1khz_monitor) > STACK_HIGH_WATERMARK_THRESHOLD);
    app_canAlerts_RSM_Warning_StackWaterMarkHighTaskCanRx_set(
        io_taskMonitor_getStackUsage(&task_canrx_monitor) > STACK_HIGH_WATERMARK_THRESHOLD);
    app_canAlerts_RSM_Warning_StackWaterMarkHighTaskCanTx_set(
        io_taskMonitor_getStackUsage(&task_cantx_monitor) > STACK_HIGH_WATERMARK_THRESHOLD);
}
