#include "tasks.h"
#include "jobs.h"
#include "cmsis_os.h"
#include "main.h"

#include "io_time.h"
#include "io_log.h"
#include "io_canQueue.h"

#include "hw_pwms.h"
#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_can.h"


void tasks_preInit()
{
    hw_bootup_enableInterruptsForApp();
}

//void tasks_preInitWatchdog() {}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();
    hw_hardFaultHandler_init();
    //hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);
    
    jobs_init();
}

void tasks_deinit() {
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_Base_DeInit(&htim3);

    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_DeInit(&htim4);

    HAL_ADC_Stop_IT(&hadc1);
    HAL_ADC_DeInit(&hadc1);

    HAL_DMA_Abort_IT(&hdma_adc1);
    HAL_DMA_DeInit(&hdma_adc1);

}

_Noreturn void tasks_run1Hz()
{

    static const TickType_t period_ms = 1000U;
    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        jobs_run1Hz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run100Hz()
{

    static const TickType_t period_ms = 10;
    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        jobs_run100Hz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run1kHz()
{
    static const TickType_t period_ms = 1;
    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        jobs_run1kHz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runCanTx()
{
    // Setup tasks.
    for (;;)
    {
        CanMsg msg = io_canQueue_popTx();
        hw_can_transmit(&can2, &msg);
    }
}

_Noreturn void tasks_runCanRx(void)
{
    for (;;)
    {
        jobs_runCanRx_tick();
    }
}

