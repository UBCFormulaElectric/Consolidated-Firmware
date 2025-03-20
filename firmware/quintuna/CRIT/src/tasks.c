#include "tasks.h"
#include "cmsis_os.h"
#include "shared.pb.h"
#include "jobs.h"
#include "main.h"

// io
#include "io_log.h"
#include "io_canQueue.h"
#include "io_chimera_v2.h"
#include "io_chimeraConfig_v2.h"

// hw
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"

void tasks_preInit() {}

void tasks_init()
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("CRIT reset!");

    // Re-enable watchdog.
    __HAL_DBGMCU_FREEZE_IWDG();
    hw_hardFaultHandler_init();

    hw_can_init(&can1);
    hw_usb_init();

    jobs_init();
}

void tasks_runCanTx()
{
    // Setup tasks.
    for (;;)
    {
        CanMsg msg = io_canQueue_popTx();
        hw_can_transmit(&can1, &msg);
    }
}

void tasks_runCanRx()
{
    // Setup tasks.
    for (;;)
    {
        jobs_runCanRx_tick();
    }
}

void tasks_run1Hz()
{
    // Setup tasks.
    static const TickType_t period_ms   = 1000U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        if (!io_chimera_v2_enabled)
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
        io_chimera_v2_mainOrContinue(
            GpioNetName_crit_net_name_tag, id_to_gpio, SpiNetName_crit_net_name_tag, id_to_spi);

        if (!io_chimera_v2_enabled)
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
        if (!io_chimera_v2_enabled)
            jobs_run1kHz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
