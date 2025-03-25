#include "hw_stackMonitor.h"
#include "main.h"
#include <app_canAlerts.h>
#include <app_canTx.h>
#include <app_canUtils.h>

static void watermarkExceededCallback(uint32_t id)
{
    app_canTx_VC_StackWatermarkHighTask_set(id);
    app_canAlerts_VC_Warning_StackWatermarkHigh_set(true);
}

const StackMonitorConfig stack_monitor_config = { .num_tasks = 7,
                                                  .tasks     = {
                                                      {
                                                              .handle = &Task1HzHandle,
                                                              .attributes = &Task1Hz_attributes,
                                                              .id     = VC_TASK_1HZ,
                                                      },
                                                      {
                                                              .handle = &Task100HzHandle,
                                                              .attributes = &Task100Hz_attributes,
                                                              .id     = VC_TASK_100HZ,
                                                      },
                                                      {
                                                              .handle = &Task1kHzHandle,
                                                              .attributes = &Task1kHz_attributes,
                                                              .id     = VC_TASK_1KHZ,
                                                      },
                                                      {
                                                              .handle = &TaskCanRxHandle,
                                                              .attributes = &TaskCanRx_attributes,
                                                              .id     = VC_TASK_CAN_RX,
                                                      },
                                                      {
                                                              .handle = &TaskCanTxHandle,
                                                              .attributes = &TaskCanTx_attributes,
                                                              .id     = VC_TASK_CAN_TX,
                                                      },
                                                      {
                                                              .handle = &TaskLoggingHandle,
                                                              .attributes = &TaskLogging_attributes,
                                                              .id     = VC_TASK_LOGGING,
                                                      },
                                                      {
                                                              .handle = &TaskTelemHandle,
                                                              .attributes = &TaskTelem_attributes,
                                                              .id     = VC_TASK_TELEM,
                                                      },
                                                  }, .watermark_exceeded_callback =  watermarkExceededCallback};
