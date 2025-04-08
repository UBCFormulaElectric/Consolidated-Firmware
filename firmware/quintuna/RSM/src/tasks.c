#include "tasks.h"
#include "jobs.h"
#include "cmsis_os.h"
#include "main.h"
// io
#include "io_time.h"
#include "io_log.h"
#include "io_canQueue.h"
#include "io_canRx.h"
#include "io_canTx.h"
#include "io_jsoncan.h"
#include "io_coolants.h"
// chimera
#include "hw_chimera_v2.h"
#include "hw_chimeraConfig_v2.h"
#include "shared.pb.h"
// hw
// #include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_cans.h"
#include "hw_gpios.h"
#include "hw_adcs.h"

#include <stdbool.h>

void tasks_preInit()
{
    // hw_bootup_enableInterruptsForApp();
}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();
    hw_hardFaultHandler_init();
    // hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    hw_adcs_chipsInit();
    hw_can_init(&can2);
    jobs_init();
}

void tasks_deinit()
{
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_DeInit(&htim2);

    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_DeInit(&htim4);

    HAL_ADC_Stop_IT(&hadc1);
    HAL_ADC_DeInit(&hadc1);

    HAL_DMA_Abort_IT(&hdma_adc1);
    HAL_DMA_DeInit(&hdma_adc1);
}

_Noreturn void tasks_run1Hz()
{
    static const TickType_t period_ms   = 1000U;
    uint32_t                start_ticks = osKernelGetTickCount();

    for (;;)
    {
        if (!hw_chimera_v2_enabled)
            jobs_run1Hz_tick();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run100Hz()
{
    static const TickType_t period_ms   = 10;
    uint32_t                start_ticks = osKernelGetTickCount();

    for (;;)
    {
        hw_chimera_v2_mainOrContinue(&chimera_v2_config);

        if (!hw_chimera_v2_enabled)
            jobs_run100Hz_tick();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run1kHz()
{
    static const TickType_t period_ms   = 1;
    uint32_t                start_ticks = osKernelGetTickCount();

    for (;;)
    {
        if (!hw_chimera_v2_enabled)
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
        const CanMsg msg      = io_canQueue_popRx();
        JsonCanMsg   json_msg = io_jsoncan_copyFromCanMsg(&msg);
        io_canRx_updateRxTableWithMessage(&json_msg);
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    io_coolant_inputCaptureCallback();
}
