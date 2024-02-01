#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_globals.h"
#include "app_heartbeatMonitor.h"
#include "states/app_initState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"

#include "io_jsoncan.h"
#include "io_log.h"

#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_gpio.h"

extern ADC_HandleTypeDef   hadc1;
extern ADC_HandleTypeDef   hadc3;
extern FDCAN_HandleTypeDef hfdcan1;
// extern IWDG_HandleTypeDef  hiwdg1;

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_VC_RxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Warning_RxOverflow_set(true);
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_VC_TxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Warning_TxOverflow_set(true);
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
    LOG_INFO("VC reset!");

    // efuses:
    // HAL_ADC_Start_DMA(
    //     hw_tasks_config->hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(),
    //     hw_tasks_config->hadc1->Init.NbrOfConversion);
    can.can                   = hfdcan1;
    can.canMsgRecievecallback = NULL;
    
    hw_hardFaultHandler_init();
    hw_can_init(hfdcan1);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);
    io_can_init(&can_config);

    app_canTx_init();
    app_canRx_init();

    app_canTx_VC_Hash_set(GIT_COMMIT_HASH);
    app_canTx_VC_Clean_set(GIT_COMMIT_CLEAN);
}

void tasks_run100Hz(void)
{
    WatchdogHandle *watchdog = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, 10U);

    for (;;)
    {
        // TODO: Implement task behavior.
        osDelay(osWaitForever);
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
        // TODO: Implement task behavior.
        osDelay(osWaitForever);
    }
}

void tasks_run1Hz(void)
{
    WatchdogHandle *watchdog = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, 10U);

    for (;;)
    {
        // TODO: Implement task behavior.
        // Enqueue 1Hz messages for basic CAN testing.
        io_canTx_enqueue1HzMsgs();

        Gpio blinky = { .pin = GPIO_PIN_3, .port = GPIOA };
        hw_gpio_writePin(&blinky, true);
        osDelay(500);
        hw_gpio_writePin(&blinky, false);
        osDelay(500);
    }
}
