#include "tasks.h"
#include "cmsis_os.h"
#include "shared.pb.h"
#include "jobs.h"
#include "main.h"

#include "app_canTx.h"
#include "app_utils.h"

// io
#include "io_log.h"
#include "io_canQueue.h"
#include "io_canRx.h"
#include "io_bootHandler.h"

// hw
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"
#include "hw_bootup.h"
#include "hw_chimera_v2.h"
#include "hw_chimeraConfig_v2.h"
#include "hw_resetReason.h"

void tasks_preInit()
{
    hw_bootup_enableInterruptsForApp();
}

void tasks_init()
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("CRIT reset!");

    // Re-enable watchdog.
    __HAL_DBGMCU_FREEZE_IWDG();

    hw_can_init(&can1);
    ASSERT_EXIT_OK(hw_usb_init());

    jobs_init();

    app_canTx_CRIT_ResetReason_set((CanResetReason)hw_resetReason_get());
}

_Noreturn void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

void tasks_runCanTx()
{
    // Setup tasks.
    for (;;)
    {
        CanMsg msg = io_canQueue_popTx();
        LOG_IF_ERR(hw_can_transmit(&can1, &msg));
    }
}

void tasks_runCanRx()
{
    // Setup tasks.
    for (;;)
    {
        CanMsg     rx_msg = io_canQueue_popRx();
        JsonCanMsg jsoncan_rx_msg;
        io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
        io_bootHandler_processBootRequest(&rx_msg);
    }
}

void tasks_run1Hz()
{
    // Setup tasks.
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

void tasks_run100Hz()
{
    // Setup tasks.
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

void tasks_run1kHz()
{
    // Setup tasks.
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
