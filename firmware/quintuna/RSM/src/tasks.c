#include "tasks.h"
#include "jobs.h"
#include "cmsis_os.h"
#include "main.h"

#include "app_canTx.h"
#include "app_utils.h"
#include "app_jsoncan.h"

// io
#include "io_log.h"
#include "io_canQueue.h"
#include "io_canRx.h"
#include "io_canTx.h"
#include "io_bootHandler.h"

// chimera
#include "hw_chimera_v2.h"
#include "hw_chimeraConfig_v2.h"

// hw
#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_adcs.h"
#include "hw_resetReason.h"
#include "hw_usb.h"

void tasks_preInit()
{
    hw_bootup_enableInterruptsForApp();
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();
    // hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    hw_adcs_chipsInit();
    hw_can_init(&can2);
    ASSERT_EXIT_OK(hw_usb_init());

    jobs_init();

    app_canTx_RSM_ResetReason_set((CanResetReason)hw_resetReason_get());
}

_Noreturn void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

_Noreturn void tasks_run1Hz()
{
    static const TickType_t period_ms   = 1000U;
    uint32_t                start_ticks = osKernelGetTickCount();

    for (;;)
    {
        if (!hw_chimera_v2_enabled)
            jobs_run1Hz_tick();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run100Hz()
{
    static const TickType_t period_ms   = 10;
    uint32_t                start_ticks = osKernelGetTickCount();

    for (;;)
    {
        if (!hw_chimera_v2_enabled)
            jobs_run100Hz_tick();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run1kHz()
{
    static const TickType_t period_ms   = 1;
    uint32_t                start_ticks = osKernelGetTickCount();

    for (;;)
    {
        if (!hw_chimera_v2_enabled)
            jobs_run1kHz_tick();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runCanTx()
{
    // Setup tasks.
    for (;;)
    {
        CanMsg msg = io_canQueue_popTx();
        LOG_IF_ERR(hw_can_transmit(&can2, &msg));
    }
}

void tasks_runCanRxCallback(const CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);
    io_canQueue_pushRx(msg);
}

_Noreturn void tasks_runCanRx(void)
{
    for (;;)
    {
        const CanMsg rx_msg   = io_canQueue_popRx();
        JsonCanMsg   json_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&json_msg);
    }
}
