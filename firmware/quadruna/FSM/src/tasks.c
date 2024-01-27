#include "tasks.h"
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

extern ADC_HandleTypeDef * hadc1;
extern CAN_HandleTypeDef * hcan1;
extern IWDG_HandleTypeDef *hiwdg;
extern TIM_HandleTypeDef * htim12;
struct StateMachine *      state_machine;
struct HeartbeatMonitor *  heartbeat_monitor;

void CanRxQueueOverflowCallBack(uint32_t overflow_count)
{
    App_CanTx_FSM_RxOverflowCount_Set(overflow_count);
    App_CanAlerts_FSM_Warning_RxOverflow_Set(true);
}

void CanTxQueueOverflowCallBack(uint32_t overflow_count)
{
    App_CanTx_FSM_TxOverflowCount_Set(overflow_count);
    App_CanAlerts_FSM_Warning_TxOverflow_Set(true);
}

const CanConfig can_config = {
    .rx_msg_filter        = NULL,
    .tx_overflow_callback = CanTxQueueOverflowCallBack,
    .rx_overflow_callback = CanRxQueueOverflowCallBack,
};

void tasks_preInit() {}

void tasks_init()
{
    __HAL_DBGMCU_FREEZE_IWDG1();

    // efuses:
    // HAL_ADC_Start_DMA(
    //     hw_tasks_config->hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(),
    //     hw_tasks_config->hadc1->Init.NbrOfConversion);

    Io_CanTx_Init(io_jsoncan_pushTxMsgToQueue);
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    App_CanTx_Init();
    App_CanRx_Init();

    hw_hardFaultHandler_init();
    hw_can_init(hcan1);
    io_can_init(&can_config);

    Io_SharedSoftwareWatchdog_Init(io_watchdogConfig_refresh, io_watchdogConfig_timeoutCallback);

    App_CanTx_FSM_Hash_Set(GIT_COMMIT_HASH);
    App_CanTx_FSM_Clean_Set(GIT_COMMIT_CLEAN);
}

void tasks_100Hz(void *argument)
{
    UNUSED(argument);

    SoftwareWatchdogHandle_t watchdog = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_100HZ, 10U);

    /* Infinite loop */
    for (;;)
    {
        // test can
        CanMsg msg = {
            .std_id = 100,
            .dlc    = 0,
        };
        io_can_pushTxMsgToQueue(&msg);

        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
    }
}

void tasks_canTx(void *argument)
{
    UNUSED(argument);

    for (;;)
    {
        io_can_transmitMsgFromQueue();
    }
}

void tasks_canRx(void *argument)
{
    UNUSED(argument);

    for (;;)
    {
        CanMsg rx_msg;
        io_can_popRxMsgFromQueue(&rx_msg);
    }
}

void tasks_1Khz(void *argument)
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

void tasks_1Hz(void *argument)
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
