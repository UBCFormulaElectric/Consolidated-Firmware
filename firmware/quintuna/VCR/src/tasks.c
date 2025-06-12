#include "tasks.h"
#include "app_utils.h"
#include "hw_cans.h"
#include "hw_hardFaultHandler.h"
#include "hw_utils.h"
#include "io_bootloaderReroute.h"
#include "io_canMsg.h"
#include "io_log.h"
#include "jobs.h"
#include "app_jsoncan.h"
#include "main.h"
#include "io_canQueues.h"
#include <cmsis_os2.h>
#include <io_canReroute.h>
#include <io_canRx.h>
#include <io_canTx.h>
#include <stdint.h>

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    LOG_ERROR("Stack overflow detected in task %s, resetting...", pcTaskName);

    TaskStatus_t status;
    vTaskGetInfo(xTask, &status, pdFALSE, eRunning);

    BREAK_IF_DEBUGGER_CONNECTED();
    NVIC_SystemReset();
}

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_INFO("VCR tx overflow %d", overflow_count);
}

void tasks_init()
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VCR reset!");
    jobs_init();
    hw_can_init(&fd_can);
    hw_can_init(&sx_can);
    hw_can_init(&inv_can);
    __HAL_DBGMCU_FREEZE_IWDG1();
}

_Noreturn void tasks_run1Hz(void)
{
    static const TickType_t period_ms = 1000;

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        // TODO: Other frequency enqueueing.
        io_canTx_enqueue1HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        // hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_runCanFDTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&fd_can_tx_queue);
        hw_fdcan_transmit(&fd_can, &tx_msg);
    }
}

_Noreturn void tasks_runCanSxTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&sx_can_tx_queue);
        hw_can_transmit(&sx_can, &tx_msg);
    }
}

_Noreturn void tasks_runCanInvTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&inv_can_tx_queue);
        hw_can_transmit(&inv_can, &tx_msg);
    }
}
_Noreturn void tasks_runcanRx(void)
{
    for (;;)
    {
        // Doesnt' receive anything via JSONCAN
        osDelay(osWaitForever);
    }
}
