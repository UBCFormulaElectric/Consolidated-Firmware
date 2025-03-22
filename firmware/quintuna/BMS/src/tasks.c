#include "tasks.h"

#include "io_log.h"
#include "io_canQueue.h"
// chimera
#include "io_chimeraConfig_v2.h"
#include "io_chimera_v2.h"
#include "shared.pb.h"

// hw
#include "hw_usb.h"
#include "hw_cans.h"
#include "hw_adcs.h"

void tasks_preInit(void)
{
    LOG_INFO("BMS Reset");
}

void tasks_init(void)
{
    SEGGER_SYSVIEW_Conf();
    hw_usb_init();
    hw_adcs_chipsInit();
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
        io_chimera_v2_mainOrContinue(&chimera_v2_config);

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
