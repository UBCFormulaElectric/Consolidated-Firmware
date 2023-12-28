#include "hw_tasks.h"
#include "Io_SharedHeartbeatMonitor.h"
#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "Io_SharedSoftwareWatchdog.h"
#include "io_watchdogConfig.h"
#include "app_globals.h"
#include "states/app_initState.h"

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "io_jsoncan.h"
#include "App_CommitInfo.h"

void hw_tasks_preinit()
{
    hw_bootup_enableInterruptsForApp();
}

void hw_tasks_init(HwTasksConfig *hw_tasks_config)
{
    __HAL_DBGMCU_FREEZE_IWDG1();

    // efuses:
    // HAL_ADC_Start_DMA(
    //     hw_tasks_config->hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(),
    //     hw_tasks_config->hadc1->Init.NbrOfConversion);

    Io_CanTx_Init(io_jsoncan_pushTxMsgToQueue);
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
    io_can_init(hw_tasks_config->can_config);

    App_CanTx_Init();
    App_CanRx_Init();

    hw_hardFaultHandler_init();
    hw_can_init(hw_tasks_config->hfdcan1);
    io_can_init(hw_tasks_config->can_config);

    Io_SharedSoftwareWatchdog_Init(io_watchdogConfig_refresh, io_watchdogConfig_timeoutCallback);

    hw_tasks_config->heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        Io_SharedHeartbeatMonitor_GetCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);
    hw_tasks_config->state_machine = App_SharedStateMachine_Create(NULL, app_initState_get());
    globals->heartbeat_monitor     = hw_tasks_config->heartbeat_monitor;

    App_CanTx_VC_Hash_Set(GIT_COMMIT_HASH);
    App_CanTx_VC_Clean_Set(GIT_COMMIT_CLEAN);
}

void hw_tasks_100hz(void *argument)
{
    UNUSED(argument);

    SoftwareWatchdogHandle_t watchdog = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_100HZ, 10U);

    /* Infinite loop */
    for (;;)
    {
        // test fdcan
        CanMsg msg = {
            .std_id = 100,
            .dlc    = 0,
        };
        io_can_pushTxMsgToQueue(&msg);

        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
    }
}

void hw_tasks_cantx(void *argument)
{
    UNUSED(argument);

    for (;;)
    {
        io_can_transmitMsgFromQueue();
    }
}

void hw_tasks_canrx(void *argument)
{
    UNUSED(argument);

    for (;;)
    {
        CanMsg rx_msg;
        io_can_popRxMsgFromQueue(&rx_msg);
    }
}

void hw_tasks_1khz(void *argument)
{
    UNUSED(argument);

    SoftwareWatchdogHandle_t watchdog = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_1KHZ, 10U);

    /* Infinite loop */
    for (;;)
    {
        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
    }
}

void hw_tasks_1hz(void *argument)
{
    UNUSED(argument);

    SoftwareWatchdogHandle_t watchdog = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_1HZ, 10U);

    /* Infinite loop */
    for (;;)
    {
        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
    }
}
