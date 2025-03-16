#include "tasks.h"
#include "cmsis_os.h"
#include "shared.pb.h"
#include "jobs.h"

// io
#include "io_log.h"
#include "io_canQueue.h"

// hw
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"

#include "io_chimeraConfig_v2.h"
#include "io_chimera_v2.h"

void tasks_preInit() {}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("FSM reset!");

    // Re-enable watchdog.
    __HAL_DBGMCU_FREEZE_IWDG();
    hw_hardFaultHandler_init();
    hw_usb_init();

    jobs_init();
}

void tasks_runCanTx(void)
{
    // Setup tasks.
    for (;;)
    {
        CanMsg msg = io_canQueue_popTx();
        //        hw_can_transmit(&can, &msg);
    }
}

void tasks_runCanRx(void)
{
    // Setup tasks.
    for (;;)
    {
        jobs_runCanRx_tick();
    }
}

void tasks_run1Hz(void)
{
    // Setup tasks.
    static const TickType_t period_ms   = 1000U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        jobs_run1Hz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run100Hz(void)
{
    // Setup tasks.
    static const TickType_t period_ms   = 10;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        io_chimera_v2_mainOrContinue(
            GpioNetName_fsm_net_name_tag, id_to_gpio, AdcNetName_fsm_net_name_tag, id_to_adc,
            I2cNetName_fsm_net_name_tag, id_to_i2c);
        jobs_run100Hz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run1kHz(void)
{
    // Setup tasks.
    static const TickType_t period_ms   = 1;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        jobs_run1kHz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
