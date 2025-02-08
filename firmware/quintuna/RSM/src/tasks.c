#include "tasks.h"
#include "cmsis_os.h"

#include "io_coolants.h"


#include "hw_pwms.h"
#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"




void canRxQueueOverflowCallBack(uint32_t overflow_count) {

}

void canTxQueueOverflowCallBack(uint32_t overflow_count) {

}

void canTxQueueOverflowClearCallback(void) {

}

void canRxQueueOverflowClearCallback(void) {

}

void tasks_preInit()
{
    hw_bootup_enableInterruptsForApp();
}

static void jsoncan_transmit()
{

}

void tasks_init() {
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();

    hw_adcs_chipsInit();

    hw_hardFaultHandler_init();
    
    io_coolant_init(&coolant_flow_meter);
}

void tasks_deinit() {
   
}

_Noreturn void tasks_run1Hz() {
    //io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 1000U;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;) {
        hw_stackWaterMarkConfig_check();
        //app_stateMachine_tick1Hz();

        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run100Hz() {
    //io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 10;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;) {
        //app_stateMachine_tick100Hz();
    
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }

}

_Noreturn void tasks_run1kHz() {
    //io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 1U;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;) {
        hw_watchdog_checkForTimeouts();

        const uint32_t task_start_ms = TICK_TO_MS(osKernelGetTickCount());
        io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);

        if ((TICK_TO_MS(osKernelGetTickCount()) - task_start_ms) <= period_ms)
        {
            hw_watchdog_checkIn(watchdog);
        }

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_runCanTx(void)
{
    //io_chimera_sleepTaskIfEnabled();

    for (;;)
    {
        
    }
}

_Noreturn void tasks_runCanRx(void)
{
    //io_chimera_sleepTaskIfEnabled();
    
    for (;;)
    {
        
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    
}

void HAL_TIM_IC_CaptureCallback()
{
    io_coolant_inputCaptureCallback(&coolant_flow_meter);
}