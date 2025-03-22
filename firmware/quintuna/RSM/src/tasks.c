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
// chimera
<<<<<<< HEAD
#include "hw_chimera_v2.h"
#include "hw_chimeraConfig_v2.h"
=======
#include "io_chimera_v2.h"
#include "io_chimeraConfig_v2.h"
>>>>>>> 5e8451f7b (Squashed commit of the following:)
#include "shared.pb.h"
// hw
// #include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_cans.h"
#include "hw_gpios.h"
<<<<<<< HEAD
#include "hw_adcs.h"
=======
>>>>>>> 5e8451f7b (Squashed commit of the following:)

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
    hw_gpio_writePin(&brake_light_en_pin, false);

<<<<<<< HEAD
    hw_adcs_chipsInit();
    hw_can_init(&can2);
=======
>>>>>>> 5e8451f7b (Squashed commit of the following:)
    jobs_init();
}

_Noreturn void tasks_run1Hz()
{
    static const TickType_t period_ms   = 1000U;
    uint32_t                start_ticks = osKernelGetTickCount();

    for (;;)
    {
<<<<<<< HEAD
        if (!hw_chimera_v2_enabled)
            jobs_run1Hz_tick();
=======
        jobs_run1Hz_tick();
>>>>>>> 5e8451f7b (Squashed commit of the following:)

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
<<<<<<< HEAD
        hw_chimera_v2_mainOrContinue(&chimera_v2_config);

        if (!hw_chimera_v2_enabled)
            jobs_run100Hz_tick();
=======
        io_chimera_v2_mainOrContinue(
            GpioNetName_rsm_net_name_tag, id_to_gpio, AdcNetName_rsm_net_name_tag, id_to_adc,
            I2cNetName_rsm_net_name_tag, id_to_i2c, SpiNetName_rsm_net_name_tag, id_to_spi);

        jobs_run100Hz_tick();
>>>>>>> 5e8451f7b (Squashed commit of the following:)

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
<<<<<<< HEAD
        if (!hw_chimera_v2_enabled)
            jobs_run1kHz_tick();
=======
        jobs_run1kHz_tick();
>>>>>>> 5e8451f7b (Squashed commit of the following:)

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
