#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_globals.h"
#include "app_heartbeatMonitor.h"
#include "states/app_initState.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_CommitInfo.h"

#include "io_jsoncan.h"

#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"

extern ADC_HandleTypeDef   *hadc1;
extern ADC_HandleTypeDef   *hadc3;
extern FDCAN_HandleTypeDef *hfdcan1;
extern IWDG_HandleTypeDef  *hiwdg1;

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    App_CanTx_VC_RxOverflowCount_Set(overflow_count);
    App_CanAlerts_VC_Warning_RxOverflow_Set(true);
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    App_CanTx_VC_TxOverflowCount_Set(overflow_count);
    App_CanAlerts_VC_Warning_TxOverflow_Set(true);
}

const CanConfig can_config = {
    .rx_msg_filter        = NULL,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};

void tasks_preInit(void) {}

void tasks_init(void)
{
    __HAL_DBGMCU_FREEZE_IWDG1();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();

    // efuses:
    // HAL_ADC_Start_DMA(
    //     hw_tasks_config->hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(),
    //     hw_tasks_config->hadc1->Init.NbrOfConversion);

    hw_hardFaultHandler_init();
    hw_can_init(hfdcan1);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    Io_CanTx_Init(io_jsoncan_pushTxMsgToQueue);
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);
    io_can_init(&can_config);

    App_CanTx_Init();
    App_CanRx_Init();

    App_CanTx_VC_Hash_Set(GIT_COMMIT_HASH);
    App_CanTx_VC_Clean_Set(GIT_COMMIT_CLEAN);
}

void tasks_run100Hz(void)
{
    WatchdogHandle *watchdog = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, 10U);

    for (;;)
    {
        // test fdcan
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

    for (;;)
    {
        hw_watchdog_checkIn(watchdog);
    }
}

void tasks_run1Hz(void)
{
    WatchdogHandle *watchdog = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, 10U);

    for (;;)
    {
        hw_watchdog_checkIn(watchdog);
    }
}
