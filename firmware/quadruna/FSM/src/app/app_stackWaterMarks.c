#include "app_stackWaterMarks.h"

#include "io_stackWaterMark.h"
#include "main.h"
#include "app_utils.h"
#include "app_canAlerts.h"

/** @brief The stack watermark threshold as a percentage of the stack size */
#define STACK_HIGH_WATERMARK_THRESHOLD 0.7f

static void logWaterMarkAboveThresholdTask1Hz(uint8_t error)
{
    app_canAlerts_FSM_Warning_StackWaterMarkHighTask1Hz_set(true);
}

static void logWaterMarkAboveThresholdTask100Hz(uint8_t error)
{
    app_canAlerts_FSM_Warning_StackWaterMarkHighTask100Hz_set(true);
}

static void logWaterMarkAboveThresholdTask1kHz(uint8_t error)
{
    app_canAlerts_FSM_Warning_StackWaterMarkHighTask1kHz_set(true);
}

static void logWaterMarkAboveThresholdTaskCanRx(uint8_t error)
{
    app_canAlerts_FSM_Warning_StackWaterMarkHighTaskCanRx_set(true);
}

static void logWaterMarkAboveThresholdTaskCanTx(uint8_t error)
{
    app_canAlerts_FSM_Warning_StackWaterMarkHighTaskCanTx_set(true);
}

/** @brief Iterate through this table to check stack watermarks for each task */
static StackWaterMark stack_watermarks[] = {
    {
        .handle              = &Task1HzHandle,
        .stack_size          = 512,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask1Hz,
    },
    {
        .handle              = &Task100HzHandle,
        .stack_size          = 512,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask100Hz,
    },
    {
        .handle              = &Task1kHzHandle,
        .stack_size          = 512,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask1kHz,
    },
    {
        .handle              = &TaskCanRxHandle,
        .stack_size          = 512,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTaskCanRx,
    },
    {
        .handle              = &TaskCanTxHandle,
        .stack_size          = 512,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTaskCanTx,
    },
};

void app_stackWaterMarkConfig_check(void)
{
    io_stackWaterMark_check(stack_watermarks, NUM_ELEMENTS_IN_ARRAY(stack_watermarks));
}
