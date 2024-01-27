#include <assert.h>

#include "io_stackWaterMark.h"
#include "main.h"
#include "App_SharedMacros.h"
#include "Io_SharedStackWaterMark.h"
#include "App_CanTx.h"
#include "App_CanAlerts.h"

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
    App_CanAlerts_FSM_Warning_StackWaterMarkHighTask1kHz_Set(error);
}

static void logWaterMarkAboveThresholdTask100Hz(uint8_t error)
{
    App_CanAlerts_FSM_Warning_StackWaterMarkHighTask100Hz_Set(error);
}

static void logWaterMarkAboveThresholdTask1kHz(uint8_t error)
{
    App_CanAlerts_FSM_Warning_StackWaterMarkHighTask1Hz_Set(error);
}

static void logWaterMarkAboveThresholdTaskCanRx(uint8_t error)
{
    App_CanAlerts_FSM_Warning_StackWaterMarkHighTaskCanRx_Set(error);
}

static void logWaterMarkAboveThresholdTaskCanTx(uint8_t error)
{
    App_CanAlerts_FSM_Warning_StackWaterMarkHighTaskCanTx_Set(error);
}

static void logStackRemainingTask1Hz(float percent)
{
    App_CanTx_FSM_StackRemainingTask1Hz_Set(percent);
}
static void logStackRemainingTask100Hz(float percent)
{
    App_CanTx_FSM_StackRemainingTask100Hz_Set(percent);
}
static void logStackRemainingTask1kHz(float percent)
{
    App_CanTx_FSM_StackRemainingTask1kHz_Set(percent);
}
static void logStackRemainingTaskCanRx(float percent)
{
    App_CanTx_FSM_StackRemainingTaskCanRx_Set(percent);
}
static void logStackRemainingTaskCanTx(float percent)
{
    App_CanTx_FSM_StackRemainingTaskCanTx_Set(percent);
}

/** @brief Iterate through this table to check stack watermarks for each task */
static struct stack_watermark stack_watermarks[] = {
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

void io_stackWaterMark_check(void)
{
    Io_SharedStackWaterMark_Check(stack_watermarks, NUM_ELEMENTS_IN_ARRAY(stack_watermarks));
    Io_SharedStackSize_Check(stack_watermarks, NUM_ELEMENTS_IN_ARRAY(stack_watermarks));
}
