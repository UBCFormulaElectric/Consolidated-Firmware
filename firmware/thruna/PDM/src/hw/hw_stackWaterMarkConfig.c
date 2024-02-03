#include <assert.h>

#include "hw_stackWaterMarkConfig.h"
#include "main.h"
#include "app_utils.h"
#include "hw_stackWaterMark.h"
#include "app_canTx.h"
#include "app_canAlerts.h"

// We check the stack water mark for the following tasks
extern TaskHandle_t Task1HzHandle;
extern TaskHandle_t Task100HzHandle;
extern TaskHandle_t Task1kHzHandle;
extern TaskHandle_t TaskCanRxHandle;
extern TaskHandle_t TaskCanTxHandle;

/** @brief The stack watermark threshold as a percentage of the stack size */
#define STACK_HIGH_WATERMARK_THRESHOLD 0.7f

static void logWaterMarkAboveThresholdTask1Hz(uint8_t error)
{
    app_canAlerts_PDM_Warning_StackWaterMarkHighTask1kHz_set(error);
}

static void logWaterMarkAboveThresholdTask100Hz(uint8_t error)
{
    app_canAlerts_PDM_Warning_StackWaterMarkHighTask100Hz_set(error);
}

static void logWaterMarkAboveThresholdTask1kHz(uint8_t error)
{
    app_canAlerts_PDM_Warning_StackWaterMarkHighTask1Hz_set(error);
}

static void logWaterMarkAboveThresholdTaskCanRx(uint8_t error)
{
    app_canAlerts_PDM_Warning_StackWaterMarkHighTaskCanRx_set(error);
}

static void logWaterMarkAboveThresholdTaskCanTx(uint8_t error)
{
    app_canAlerts_PDM_Warning_StackWaterMarkHighTaskCanTx_set(error);
}
static void logStackRemainingTask1Hz(float percent)
{
    app_canTx_PDM_StackRemainingTask1Hz_set(percent);
}
static void logStackRemainingTask100Hz(float percent)
{
    app_canTx_PDM_StackRemainingTask100Hz_set(percent);
}
static void logStackRemainingTask1kHz(float percent)
{
    app_canTx_PDM_StackRemainingTask1kHz_set(percent);
}
static void logStackRemainingTaskCanRx(float percent)
{
    app_canTx_PDM_StackRemainingTaskCanRx_set(percent);
}
static void logStackRemainingTaskCanTx(float percent)
{
    app_canTx_PDM_StackRemainingTaskCanTx_set(percent);
}
/** @brief Iterate through this table to check stack watermarks for each task */
static StackWaterMark stack_watermarks[] = {
    {
        .handle              = &Task1HzHandle,
        .stack_size          = 512,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask1Hz,
        .stack_remaining     = logStackRemainingTask1Hz,
    },
    {
        .handle              = &Task100HzHandle,
        .stack_size          = 512,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask100Hz,
        .stack_remaining     = logStackRemainingTask100Hz,
    },
    {
        .handle              = &Task1kHzHandle,
        .stack_size          = 512,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask1kHz,
        .stack_remaining     = logStackRemainingTask1kHz,
    },
    {
        .handle              = &TaskCanRxHandle,
        .stack_size          = 512,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTaskCanRx,
        .stack_remaining     = logStackRemainingTaskCanRx,
    },
    {
        .handle              = &TaskCanTxHandle,
        .stack_size          = 512,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTaskCanTx,
        .stack_remaining     = logStackRemainingTaskCanTx,
    },
};

void hw_stackWaterMarkConfig_check(void)
{
    hw_stackWaterMark_check(stack_watermarks, NUM_ELEMENTS_IN_ARRAY(stack_watermarks));
    hw_stackSize_check(stack_watermarks, NUM_ELEMENTS_IN_ARRAY(stack_watermarks));
}
