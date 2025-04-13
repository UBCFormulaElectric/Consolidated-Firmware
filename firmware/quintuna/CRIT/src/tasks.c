#include "tasks.h"
#include "cmsis_os.h"
#include "hw_gpios.h"
#include "shared.pb.h"
#include "jobs.h"
#include "main.h"

// io
#include "io_log.h"
#include "io_canQueue.h"
#include "io_chimera_v2.h"
#include "io_chimeraConfig_v2.h"
#include "io_rotary.h"

// hw
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"
#include "hw_chimera_v2.h"
#include "hw_chimeraConfig_v2.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == rot_a.pin)
    {
        // Forward to your rotary A handler.
        io_roatary_rotA_IRQHandler();
    }
    else if (GPIO_Pin == rot_b.pin)
    {
        // Forward to your rotary B handler.
        io_rotary_rotB_IRQHandler();
    }
    else if (GPIO_Pin == rot_s.pin)
    {
        // Forward to your push button handler.
        io_rotary_push_IRQHandler();
    }
}

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
