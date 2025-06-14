#include "tasks.h"
#include "jobs.h"
#include <cmsis_os2.h>

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "app_canAlerts.h"
#include "app_jsoncan.h"

#include "io_log.h"
#include "io_canQueue.h"
#include "io_canRx.h"
#include "io_canTx.h"
#include "io_semaphore.h"

// hw
#include "hw_usb.h"
#include "hw_cans.h"
#include "hw_adcs.h"
#include "hw_pwms.h"
#include "hw_watchdogConfig.h"
#include "hw_hardFaultHandler.h"

// chimera
#include "hw_chimeraConfig_v2.h"
#include "hw_chimera_v2.h"
#include "hw_resetReason.h"
#include "hw_bootup.h"
#include "hw_gpios.h"

// Define this guy to use CAN2 for talking to the Elcon.
// #define CHARGER_CAN

static Semaphore init_complete_locks;

void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

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
    LOG_INFO("BMS Reset");

    __HAL_DBGMCU_FREEZE_IWDG1();

    LOG_IF_ERR(hw_usb_init());
    hw_adcs_chipsInit();
    hw_pwms_init();
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    // TODO: Start CAN1/CAN2 based on if we're charging at runtime.
    // #ifdef CHARGER_CAN
    // hw_can_init(&can2);
    // #else
    hw_can_init(&can2);
    // #endif

    jobs_init();

    app_canTx_BMS_ResetReason_set((CanResetReason)hw_resetReason_get());

    // Check for stack overflow on a previous boot cycle and populate CAN alert.
    BootRequest boot_request = hw_bootup_getBootRequest();
    if (boot_request.context == BOOT_CONTEXT_STACK_OVERFLOW)
    {
        app_canAlerts_BMS_Info_StackOverflow_set(true);
        app_canTx_BMS_StackOverflowTask_set(boot_request.context_value);

        // Clear stack overflow bootup.
        boot_request.context       = BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw_bootup_setBootRequest(boot_request);
    }

    // Shutdown loop power comes from a load switch on the BMS.
    hw_gpio_writePin(&shdn_en_pin, true);
}

void tasks_run1Hz(void)
{
    static const TickType_t period_ms   = 1000U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        if (!hw_chimera_v2_enabled)
        {
            jobs_run1Hz_tick();
        }

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
        if (!hw_chimera_v2_enabled)
        {
            jobs_run100Hz_tick();
        }

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
        // hw_watchdog_checkForTimeouts();
        jobs_run1kHz_tick();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runCanTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&can_tx_queue);

        // #ifdef CHARGER_CAN
        //         // Elcon only supports regular CAN but we have some debug messages that are >8 bytes long. Use FDCAN
        //         for those
        //         // (they won't get seen by the charger, but they'll show up on CANoe).
        //         // TODO: Bit-rate-switching wasn't working for me when the BMS was connected to the charger, so the
        //         FD
        //         // peripheral is configured without BRS. Figure out why it wasn't working?
        //         if (tx_msg.dlc > 8)
        //         {
        //             LOG_IF_ERR(hw_fdcan_transmit(&can2, &tx_msg));
        //         }
        //         else
        //         {
        //             LOG_IF_ERR(hw_can_transmit(&can2, &tx_msg));
        //         }
        // #else
        LOG_IF_ERR(hw_fdcan_transmit(&can2, &tx_msg));
        // #endif
    }
}

void tasks_runCanRx(void)
{
    for (;;)
    {
        const CanMsg rx_msg       = io_canQueue_popRx();
        JsonCanMsg   json_can_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&json_can_msg);
    }
}

// TODO: Add watchdogs to the LTC commands

void tasks_runLtcVoltages(void)
{
    static const TickType_t period_ms = 1000U; // 1Hz

    for (;;)
    {
        const uint32_t start_ticks = osKernelGetTickCount();
        jobs_runLTCVoltages();
        LOG_INFO("LTC voltage period remaining: %dms", start_ticks + period_ms - osKernelGetTickCount());
        osDelayUntil(start_ticks + period_ms);
    }
}

void tasks_runLtcTemps(void)
{
    static const TickType_t period_ms = 1000U; // 1Hz

    for (;;)
    {
        const uint32_t start_ticks = osKernelGetTickCount();
        jobs_runLTCTemperatures();
        LOG_INFO("LTC temp period remaining: %dms", start_ticks + period_ms - osKernelGetTickCount());
        osDelayUntil(start_ticks + period_ms);
    }
}

void tasks_runLtcDiagnostics(void)
{
    static const TickType_t period_ms = 10000U; // Every 10s

    for (;;)
    {
        const uint32_t start_ticks = osKernelGetTickCount();
        jobs_runLTCDiagnostics();
        osDelayUntil(start_ticks + period_ms);
    }
}
