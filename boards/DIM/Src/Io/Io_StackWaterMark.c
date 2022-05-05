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

static struct DimCanTxInterface *_can_tx_interface = NULL;

/** @brief The stack watermark threshold as a percentage of the stack size */
#define STACK_HIGH_WATERMARK_THRESHOLD 0.7f

void Io_StackWaterMark_Init(struct DimCanTxInterface *can_tx_interface)
{
    assert(can_tx_interface != NULL);
    _can_tx_interface = can_tx_interface;
}

static void logWaterMarkAboveThresholdTask1Hz(uint8_t error)
{
    App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ(_can_tx_interface, error);
}

static void logWaterMarkAboveThresholdTask100Hz(uint8_t error)
{
    App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK100_HZ(_can_tx_interface, error);
}

static void logWaterMarkAboveThresholdTask1kHz(uint8_t error)
{
    App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_KHZ(_can_tx_interface, error);
}

static void logWaterMarkAboveThresholdTaskCanRx(uint8_t error)
{
    App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX(_can_tx_interface, error);
}

static void logWaterMarkAboveThresholdTaskCanTx(uint8_t error)
{
    App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX(_can_tx_interface, error);
}

/** @brief Iterate through this table to check stack watermarks for each task */
static struct stack_watermark stack_watermarks[] = {
    {
        .handle              = &Task1HzHandle,
        .stack_size          = TASK1HZ_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask1Hz,
    },
    {
        .handle              = &Task100HzHandle,
        .stack_size          = TASK100HZ_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask100Hz,
    },
    {
        .handle              = &Task1kHzHandle,
        .stack_size          = TASK1KHZ_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask1kHz,
    },
    {
        .handle              = &TaskCanRxHandle,
        .stack_size          = TASKCANRX_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTaskCanRx,
    },
    {
        .handle              = &TaskCanTxHandle,
        .stack_size          = TASKCANTX_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTaskCanTx,
    },
};

void Io_StackWaterMark_Check(void)
{
    Io_SharedStackWaterMark_Check(stack_watermarks, NUM_ELEMENTS_IN_ARRAY(stack_watermarks));
}
