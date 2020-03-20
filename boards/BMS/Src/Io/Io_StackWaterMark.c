#include "main.h"
#include "Io/Io_SharedMacros.h"
#include "Io/Io_StackWaterMark.h"
#include "Io/Io_SharedStackWaterMark.h"
#include "auto_generated/App_CanTx.h"

// We check the stack water mark for the following tasks
extern TaskHandle_t Task1HzHandle;
extern TaskHandle_t Task1kHzHandle;
extern TaskHandle_t TaskCanRxHandle;
extern TaskHandle_t TaskCanTxHandle;

/** @brief The stack watermark threshold as a percentage of the stack size */
#define STACK_HIGH_WATERMARK_THRESHOLD 0.7f

/** @brief Iterate through this table to check stack watermarks for each task */
static struct stack_watermark stack_watermarks[] = {
    {
        .handle              = &Task1HzHandle,
        .stack_size          = TASK1HZ_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error =
            App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ,
    },
    {
        .handle              = &Task1kHzHandle,
        .stack_size          = TASK1KHZ_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error =
            App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_KHZ,
    },
    {
        .handle              = &TaskCanRxHandle,
        .stack_size          = TASKCANRX_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error =
            App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
    },
    {
        .handle              = &TaskCanTxHandle,
        .stack_size          = TASKCANTX_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error =
            App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
    },
};

void App_StackWaterMark_Check(void)
{
    App_SharedStackWaterMark_Check(
        stack_watermarks, NUM_ELEMENTS_IN_ARRAY(stack_watermarks));
}
