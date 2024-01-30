#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_globals.h"
#include "app_heartbeatMonitor.h"
#include "states/app_driveState.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"

#include "io_jsoncan.h"
#include "io_canRx.h"

#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"

extern ADC_HandleTypeDef  *hadc1;
extern CAN_HandleTypeDef  *hcan1;
extern IWDG_HandleTypeDef *hiwdg;
extern TIM_HandleTypeDef  *htim12;

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_FSM_RxOverflowCount_set(overflow_count);
    app_canAlerts_FSM_Warning_TxOverflow_set(true);
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_FSM_TxOverflowCount_set(overflow_count);
    app_canAlerts_FSM_Warning_TxOverflow_set(true);
}

const CanConfig can_config = {
    .rx_msg_filter        = io_canRx_filterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};

void tasks_preInit(void) {}

void tasks_init(void)
{
    __HAL_DBGMCU_FREEZE_IWDG();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();

    hw_hardFaultHandler_init();
    hw_can_init(hcan1);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    app_canTx_init();
    app_canRx_init();

    app_canTx_FSM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_FSM_Clean_set(GIT_COMMIT_CLEAN);
}

void tasks_run100Hz(void)
{
    WatchdogHandle *watchdog = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, 10U);

    /* Infinite loop */
    for (;;)
    {
        // test can
        CanMsg msg = {
            .std_id = 100,
            .dlc    = 0,
        };
        io_can_pushTxMsgToQueue(&msg);

        hw_watchdog_checkIn(watchdog);
    }
}

void tasks_runCanTx(void)
{
    for (;;)
    {
        io_can_transmitMsgFromQueue();
    }
}

void tasks_runCanRx(void)
{
    for (;;)
    {
        CanMsg rx_msg;
        io_can_popRxMsgFromQueue(&rx_msg);
    }
}

void tasks_run1kHz(void)
{
    WatchdogHandle *watchdog = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, 10U);

    /* Infinite loop */
    for (;;)
    {
        hw_watchdog_checkIn(watchdog);
    }
}

void tasks_run1Hz(void)
{
    WatchdogHandle *watchdog = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, 10U);

    /* Infinite loop */
    for (;;)
    {
        hw_watchdog_checkIn(watchdog);
    }
}
