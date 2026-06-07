#include "tasks.h"
#include "jobs.hpp"
#include "main.h"

#include "app_jsoncan.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include "app_lowVoltageBattery.hpp"

#include "io_time.hpp"
#include "io_canQueues.hpp"
#include "hw_can.hpp"
#include "hw_gpio.hpp"
#include "io_canRx.hpp"
#include "io_semaphore.hpp"

#include "hw_adcs.hpp"
#include "hw_cans.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_hardFaultHandler.hpp"
#include "hw_bootup.hpp"
#include "hw_watchdog.hpp"
#include "hw_resetReason.hpp"
#include "hw_runTimeStat.hpp"
#include "io_pumpControl.hpp"

[[noreturn]] static void tasks_run1Hz(void *arg);
[[noreturn]] static void tasks_run100Hz(void *arg);
[[noreturn]] static void tasks_run1kHz(void *arg);
[[noreturn]] static void tasks_runCan1Tx(void *arg);
[[noreturn]] static void tasks_runCan2Tx(void *arg);
[[noreturn]] static void tasks_runCanRx(void *arg);
[[noreturn]] static void tasks_batteryMonitoring(void *arg);
[[noreturn]] static void tasks_powerMonitoring(void *arg);

// Define the task with StaticTask Class
constexpr size_t                                   TASK_COUNT = 7;
static hw::rtos::StaticTask::StaticTaskStack<8096> Task100HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  Task1kHzStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  Task1HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskCanRxStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskCan1TxStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskCan2TxStack;
static hw::rtos::StaticTask::StaticTaskStack<1024> BatteryMonitoringStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskPowerMonitoringStack;

static hw::rtos::StaticTask Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz, Task100HzStack);
static hw::rtos::StaticTask Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz, Task1kHzStack);
static hw::rtos::StaticTask Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz, Task1HzStack);
static hw::rtos::StaticTask TaskCanRx(osPriorityNormal, "TaskCanRx", tasks_runCanRx, TaskCanRxStack);
static hw::rtos::StaticTask TaskCan1Tx(osPriorityNormal, "TaskCanTx", tasks_runCan1Tx, TaskCan1TxStack);
static hw::rtos::StaticTask TaskCan2Tx(osPriorityHigh, "TaskCanTx", tasks_runCan2Tx, TaskCan2TxStack);
static hw::rtos::StaticTask
    TaskBatteryMonitoring(osPriorityNormal, "TaskBatteryMonitoring", tasks_batteryMonitoring, BatteryMonitoringStack);
static hw::rtos::StaticTask
    TaskPowerMonitoring(osPriorityNormal, "TaskPowerMonitoring", tasks_powerMonitoring, TaskPowerMonitoringStack);

static hw::watchdog::monitor<TASK_COUNT> monitor{
    hiwdg1,
    [](const hw::watchdog::instance &i) { LOG_INFO("Software watchdog timeout for task %d", i.task_id); },
};

void tasks_run1Hz(void *arg)
{
    constexpr uint32_t      period_ms                = 1000U;
    constexpr uint32_t      watchdog_grace_period_ms = 50U;
    hw::watchdog::instance &watchdog1hz              = monitor.spawn_instance(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run1Hz_tick();
        watchdog1hz.checkIn();
        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
        osDelayUntil(start_ticks);
    }
}
void tasks_run100Hz(void *arg)
{
    constexpr uint32_t      period_ms                = 10U;
    constexpr uint32_t      watchdog_grace_period_ms = 2U;
    hw::watchdog::instance &watchdog100hz            = monitor.spawn_instance(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run100Hz_tick();
        watchdog100hz.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
void tasks_run1kHz(void *arg)
{
    constexpr uint32_t      period_ms                = 1U;
    constexpr uint32_t      watchdog_grace_period_ms = 1U;
    hw::watchdog::instance &watchdog1khz             = monitor.spawn_instance(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run1kHz_tick();
        monitor.checkForTimeouts();
        watchdog1khz.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runCan1Tx(void *arg)
{
    forever
    {
        const auto msg = fdcan_tx_queue.pop();
        if (not msg)
            continue;
        if (const auto &m = msg.value(); m.bus == app::can_utils::BusEnum::Bus_FDCAN)
        {
            const auto res = fdcan1.fdcan_transmit(hw::CanMsg{
                m.std_id,
                m.dlc,
                m.data,
            });
            LOG_IF_ERR(res);
        }
        else
        {
            LOG_ERROR("INVALID BUS %d", m.bus);
        }
    }
}
void tasks_runCan2Tx(void *arg)
{
    forever
    {
        const auto msg = invcan_tx_queue.pop();
        if (not msg)
            continue;
        if (const auto &m = msg.value(); m.bus == app::can_utils::BusEnum::Bus_FDCAN)
        {
            const auto res = invcan.can_transmit(hw::CanMsg{
                m.std_id,
                m.dlc,
                m.data,
            });
            LOG_IF_ERR(res);
        }
        else
        {
            LOG_ERROR("INVALID BUS %d", m.bus);
        }
    }
}
void tasks_runCanRx(void *arg)
{
    forever
    {
        const auto msg = can_rx_queue.pop();
        if (not msg)
            continue;
        io::can_rx::updateRxTableWithMessage(app::jsoncan::copyFromCanMsg(msg.value()));
    }
}
[[noreturn]] static void tasks_batteryMonitoring(void *arg)
{
    static const TickType_t period_ms   = 10;
    static uint32_t         start_ticks = 0;
    start_ticks                         = osKernelGetTickCount();

    app::batteryMonitoring::init();
    for (;;)
    {
        jobs_runBatteryMonitoring_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_powerMonitoring(void *arg)
{
    static const TickType_t period_ms   = 10;
    static uint32_t         start_ticks = 0;
    start_ticks                         = osKernelGetTickCount();

    for (;;)
    {
        jobs_runPowerMonitoring_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

static void VC_StartAllTasks()
{
    Task100Hz.start();
    Task1kHz.start();
    Task1Hz.start();
    TaskCanRx.start();
    TaskCan1Tx.start();
    TaskCan2Tx.start();
    TaskBatteryMonitoring.start();
    TaskPowerMonitoring.start();
}

void tasks_preInit()
{
    hw_hardFaultHandler_init();
    hw::bootup::enableInterruptsForApp();
}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();

#ifndef WATCHDOG_DISABLED
    __HAL_DBGMCU_FREEZE_IWDG1();
#endif

    LOG_INFO("VC Reset!");
    osKernelInitialize();
    jobs_init();
    fdcan1.init();
    invcan.init();

    adcChipsInit();

    if (hw::bootup::BootRequest boot_request = hw::bootup::getBootRequest();
        boot_request.context != hw::bootup::BootContext::NONE)
    {
        // Check for stack overflow on a previous boot cycle and populate CAN alert.
        if (boot_request.context == hw::bootup::BootContext::OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app::can_alerts::infos::StackOverflow_set(true);
            app::can_tx::VC_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == hw::bootup::BootContext::WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app::can_alerts::infos::WatchdogTimeout_set(true);
            app::can_tx::VC_WatchdogTimeoutTask_set(boot_request.context_value);
        }

        // Clear stack overflow bootup.
        boot_request.context       = hw::bootup::BootContext::NONE;
        boot_request.context_value = 0;
        hw::bootup::setBootRequest(boot_request);
    }

    // TODO this should surely be managed by the power manager??
    // dam_en.writePin(true);
    // rsm_en.writePin(true);
    // front_en.writePin(true);
    // bms_en.writePin(true);
    // rl_pump_en.writePin(true);
    // rr_pump_en.writePin(true);
    // f_inv_en.writePin(true);
    // r_inv_en.writePin(true);
    // r_rad_fan_en.writePin(true);
    // l_rad_fan_en.writePin(true);
    // misc_fuse_en.writePin(true);
    ResetReason reason = hw::resetReason::get();
    app::can_tx::VC_ResetReason_set(static_cast<app::can_utils::CanResetReason>(reason));
    if (reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app::can_alerts::infos::WatchdogTimeout_set(true);
    }

    jobs_init();
    osKernelInitialize();
    VC_StartAllTasks();
    osKernelStart();
    forever {}
}
