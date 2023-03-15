#include <assert.h>

#include "main.h"
#include "App_SharedMacros.h"
#include "Io_StackWaterMark.h"
#include "Io_SharedStackWaterMark.h"
#include "App_CanTx.h"

// We check the stack water mark for the following tasks
extern TaskHandle_t Task1HzHandle;
extern TaskHandle_t Task100HzHandle;
extern TaskHandle_t Task1kHzHandle;
extern TaskHandle_t TaskCanRxHandle;
extern TaskHandle_t TaskCanTxHandle;

/** @brief The stack watermark threshold as a percentage of the stack size */
#define STACK_HIGH_WATERMARK_THRESHOLD 0.7f

static void logWaterMarkAboveThresholdTask1kHz(uint8_t error)
{
    App_CanTx_DCM_Warnings_StackWatermarkAboveThresholdTask1kHz_Set(error);
}

static void logWaterMarkAboveThresholdTask100Hz(uint8_t error)
{
    App_CanTx_DCM_Warnings_StackWatermarkAboveThresholdTask100Hz_Set(error);
}

static void logWaterMarkAboveThresholdTask1Hz(uint8_t error)
{
    App_CanTx_DCM_Warnings_StackWatermarkAboveThresholdTask1Hz_Set(error);
}

static void logWaterMarkAboveThresholdTaskCanRx(uint8_t error)
{
    App_CanTx_DCM_Warnings_StackWatermarkAboveThresholdTaskCanRx_Set(error);
}

static void logWaterMarkAboveThresholdTaskCanTx(uint8_t error)
{
    App_CanTx_DCM_Warnings_StackWatermarkAboveThresholdTaskCanTx_Set(error);
}

/** @brief Iterate through this table to check stack watermarks for each task */
static struct stack_watermark stack_watermarks[] = {
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

void Io_StackWaterMark_Check(void)
{
    Io_SharedStackWaterMark_Check(stack_watermarks, NUM_ELEMENTS_IN_ARRAY(stack_watermarks));
}
