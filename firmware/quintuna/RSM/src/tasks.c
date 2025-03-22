#include "tasks.h"
#include "jobs.h"
#include "cmsis_os.h"
#include "main.h"
// io
#include "io_time.h"
#include "io_log.h"
#include "io_canQueue.h"
#include "io_canRx.h"
#include "io_canTx.h"
#include "io_jsoncan.h"
// chimera
#include "io_chimera_v2.h"
#include "io_chimeraConfig_v2.h"
#include "shared.pb.h"
// hw
// #include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_cans.h"
#include "hw_gpios.h"
#include "hw_bootup.h"

void tasks_preInit()
{
    hw_bootup_enableInterruptsForApp();
}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();
    hw_hardFaultHandler_init();
    // hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);
    hw_gpio_writePin(&brake_light_en_pin, false);

    jobs_init();
}

_Noreturn void tasks_run1Hz()
{
    static const TickType_t period_ms   = 1000U;
    uint32_t                start_ticks = osKernelGetTickCount();

    for (;;)
    {
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
        io_chimera_v2_mainOrContinue(&chimera_v2_config);

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
        hw_can_transmit(&can2, &msg);
    }
}

_Noreturn void tasks_runCanRx(void)
{
    for (;;)
    {
        const CanMsg msg      = io_canQueue_popRx();
        JsonCanMsg   json_msg = io_jsoncan_copyFromCanMsg(&msg);
        io_canRx_updateRxTableWithMessage(&json_msg);
    }
}
