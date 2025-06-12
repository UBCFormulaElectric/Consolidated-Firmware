#include "tasks.h"
#include "jobs.h"
#include "cmsis_os.h"

// app
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "app_jsoncan.h"

// io
#include "io_log.h"
#include "io_canQueue.h"
#include "io_canRx.h"
#include "io_bootHandler.h"

#include "app_jsoncan.h"
// chimera
#include "hw_chimera_v2.h"
#include "hw_chimeraConfig_v2.h"

// hw
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"
#include "hw_bootup.h"
#include "hw_adcs.h"
#include "hw_resetReason.h"

void tasks_preInit(void)
{
    hw_hardFaultHandler_init();
    hw_bootup_enableInterruptsForApp();
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("FSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();

    ASSERT_EXIT_OK(hw_usb_init());
    hw_adcs_chipsInit();
    hw_can_init(&can);

    jobs_init();

    app_canTx_FSM_ResetReason_set((CanResetReason)hw_resetReason_get());

    // Check for stack overflow on a previous boot cycle and populate CAN alert.
    BootRequest boot_request = hw_bootup_getBootRequest();
    if (boot_request.context == BOOT_CONTEXT_STACK_OVERFLOW)
    {
        app_canAlerts_FSM_Info_StackOverflow_set(true);
        app_canTx_FSM_StackOverflowTask_set(boot_request.context_value);

        // Clear stack overflow bootup.
        boot_request.context       = BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw_bootup_setBootRequest(boot_request);
    }
}

_Noreturn void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

void tasks_run1Hz(void)
{
    // Setup tasks.
    static const TickType_t period_ms   = 1000;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        // if (!hw_chimera_v2_enabled)
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
        // if (!hw_chimera_v2_enabled)
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
        // if (!hw_chimera_v2_enabled)
        jobs_run1kHz_tick();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runCanTx(void)
{
    // Setup tasks.
    for (;;)
    {
        CanMsg msg = io_canQueue_popTx(&can_tx_queue);
        LOG_IF_ERR(hw_can_transmit(&can, &msg));
    }
}

void tasks_runCanRxCallback(const CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);

    if (io_canRx_filterMessageId_can2(msg->std_id))
    {
        io_canQueue_pushRx(msg);
    }
}

void tasks_runCanRx(void)
{
    // Setup tasks.
    for (;;)
    {
        const CanMsg rx_msg      = io_canQueue_popRx();
        JsonCanMsg   rx_json_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&rx_json_msg);
    }
}
