#include "tasks.h"
#include "jobs.h"

#include "io_log.h"
#include "io_canQueue.h"

// hw
#include "hw_usb.h"
#include "hw_cans.h"
#include "hw_adcs.h"
#include "hw_pwms.h"
#include "hw_watchdogConfig.h"

// chimera
#include "hw_chimeraConfig_v2.h"
#include "hw_chimera_v2.h"
#include "shared.pb.h"

void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

void tasks_preInit(void)
{
    LOG_INFO("BMS Reset");
}

void tasks_init(void)
{
    SEGGER_SYSVIEW_Conf();
    hw_usb_init();
    hw_adcs_chipsInit();
    hw_pwms_init();
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);
}

void tasks_run1Hz(void)
{
    static const TickType_t period_ms   = 1000U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run100Hz(void)
{
    static const TickType_t period_ms   = 10U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run1kHz(void)
{
    static const TickType_t period_ms   = 1U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        hw_watchdog_checkForTimeouts();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runCanTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx();
        hw_can_transmit(&can1, &tx_msg);
    }
}

void tasks_runCanRx(void)
{
    for (;;)
    {
        const CanMsg rx_msg = io_canQueue_popRx();
        UNUSED(rx_msg);
    }
}

void tasks_runLtc(void)
{
    for (;;)
    {
        jobs_runLtc();
    }
}
