#include "tasks.h"
#include "main.h"
#include "jobs.hpp"

#include "FreeRTOS.h"
#include "task.h"

#include "app_jsoncan.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_canAlerts.hpp"
#include "app_canDataCapture.hpp"
#include "app_epochClock.hpp"
#include "app_ntp.hpp"
#include "app_telemRx.hpp"

#include "io_canQueues.hpp"
#include "io_canRx.hpp"
#include "io_log.hpp"
#include "io_logQueue.hpp"
#include "io_telemMessage.hpp"
#include "io_telemQueue.hpp"
#include "io_telemRx.hpp"
#include "io_time.hpp"

#include "hw_bootup.hpp"
#include "hw_cans.hpp"
#include "hw_gpios.hpp"
#include "hw_hardFaultHandler.hpp"
#include "hw_resetReason.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_runTimeStat.hpp"
#include "hw_sds.hpp"
#include "hw_uarts.hpp"
#include "hw_watchdog.hpp"

constexpr size_t         TASK_COUNT = 9;
[[noreturn]] static void tasks_run1Hz(void *arg);
[[noreturn]] static void tasks_run100Hz(void *arg);
[[noreturn]] static void tasks_run1kHz(void *arg);
[[noreturn]] static void tasks_runLogging(void *arg);
[[noreturn]] static void tasks_runTelemTx(void *arg);
[[noreturn]] static void tasks_runTelemRx(void *arg);
[[noreturn]] static void tasks_runTelemParse(void *arg);
[[noreturn]] static void tasks_runCanTx(void *arg);
[[noreturn]] static void tasks_runCanRx(void *arg);

// Define the task with StaticTask Class
static hw::rtos::StaticTask::StaticTaskStack<8096> Task100HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskCanTxStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskCanRxStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  Task1kHzStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  Task1HzStack;
static hw::rtos::StaticTask::StaticTaskStack<1024> TaskLoggingStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskTelemRxStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskTelemParseStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskTelemTxStack;

static hw::rtos::StaticTask Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz, Task1kHzStack);
static hw::rtos::StaticTask Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz, Task100HzStack);
static hw::rtos::StaticTask Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz, Task1HzStack);
static hw::rtos::StaticTask TaskCanRx(osPriorityNormal, "TaskCanRx", tasks_runCanRx, TaskCanRxStack);
static hw::rtos::StaticTask TaskCanTx(osPriorityNormal, "TaskCanTx", tasks_runCanTx, TaskCanTxStack);
static hw::rtos::StaticTask TaskLogging(osPriorityBelowNormal, "TaskLogging", tasks_runLogging, TaskLoggingStack);
static hw::rtos::StaticTask TaskTelemRx(osPriorityBelowNormal, "TaskTelemRx", tasks_runTelemRx, TaskTelemRxStack);
static hw::rtos::StaticTask
    TaskTelemParse(osPriorityBelowNormal, "TaskTelemParse", tasks_runTelemParse, TaskTelemParseStack);
static hw::rtos::StaticTask TaskTelemTx(osPriorityBelowNormal, "TaskTelemTx", tasks_runTelemTx, TaskTelemTxStack);

#if 0 // disabled
static hw::runtimeStat::monitor<TASK_COUNT> runtimeMonitor{
    { app::can_tx::DAM_CoreCpuUsage_set, app::can_tx::DAM_CoreCpuUsageMax_set },
    {
        { { Task1kHz, app::can_tx::DAM_TaskRun1kHzCpuUsage_set, app::can_tx::DAM_TaskRun1kHzCpuUsageMax_set,
            app::can_tx::DAM_TaskRun1kHzStackUsage_set },
          { Task1Hz, app::can_tx::DAM_TaskRun1HzCpuUsage_set, app::can_tx::DAM_TaskRun1HzCpuUsageMax_set,
            app::can_tx::DAM_TaskRun1HzStackUsage_set },
          { Task100Hz, app::can_tx::DAM_TaskRun100HzCpuUsage_set, app::can_tx::DAM_TaskRun100HzCpuUsageMax_set,
            app::can_tx::DAM_TaskRun100HzStackUsage_set },
          { TaskCanRx, app::can_tx::DAM_TaskRunCanRxCpuUsage_set, app::can_tx::DAM_TaskRunCanRxCpuUsageMax_set,
            app::can_tx::DAM_TaskRunCanRxStackUsage_set },
          { TaskCanTx, app::can_tx::DAM_TaskRunCanTxCpuUsage_set, app::can_tx::DAM_TaskRunCanTxCpuUsageMax_set,
            app::can_tx::DAM_TaskRunCanTxStackUsage_set },
          { TaskLogging, app::can_tx::DAM_TaskRunLoggingCpuUsage_set, app::can_tx::DAM_TaskRunLoggingCpuUsageMax_set,
            app::can_tx::DAM_TaskRunLoggingStackUsage_set },
          { TaskTelemRx, app::can_tx::DAM_TaskRunTelemRxCpuUsage_set, app::can_tx::DAM_TaskRunTelemRxCpuUsageMax_set,
            app::can_tx::DAM_TaskRunTelemRxStackUsage_set },
          { TaskTelemTx, app::can_tx::DAM_TaskRunTelemTxCpuUsage_set, app::can_tx::DAM_TaskRunTelemTxCpuUsageMax_set,
            app::can_tx::DAM_TaskRunTelemTxStackUsage_set },
          { TaskTelemParse, app::can_tx::DAM_TaskRunTelemParseCpuUsage_set,
            app::can_tx::DAM_TaskRunTelemParseCpuUsageMax_set, app::can_tx::DAM_TaskRunTelemParseStackUsage_set } },
    },
};
#endif

static hw::watchdog::monitor<TASK_COUNT> monitor{
    hiwdg,
    [](const hw::watchdog::instance &i) { LOG_ERROR("Watchdog timeout detected in task %d", i.task_id); },
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

        // runtimeMonitor.checkin();
        watchdog1hz.checkIn();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_run100Hz(void *arg)
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

        watchdog1khz.checkIn();
        monitor.checkForTimeouts();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runLogging(void *arg)
{
    // uint32_t           start_ticks = osKernelGetTickCount();
    // constexpr uint32_t period_ms   = 100U;

    jobs_initLogFs();
    forever
    {
        jobs_runLogging_tick();
        // start_ticks += period_ms;
        // osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_runTelemTx(void *arg)
{
    constexpr uint32_t telemTxTimeoutMs  = 500U;
    constexpr uint32_t faultBackoffMinMs = 50U;
    constexpr uint32_t faultBackoffMaxMs = 1000U;
    bool               radioLinkFaulted  = false;
    uint32_t           faultBackoffMs    = faultBackoffMinMs;

    forever
    {
        const auto result = telem_tx_queue.pop();
        if (not result)
        {
            LOG_ERROR("Failed to pop telem TX message: %d", static_cast<int>(result.error()));
            continue;
        }

        const auto &entry = result.value();

        if (std::holds_alternative<io::telemMessage::NTPMsg>(entry))
        {
            if (!app::ntp::tryBeginAndCaptureT0())
            {
                LOG_WARN("NTP: dismissing TX, already in progress or RTC unavailable");
                continue;
            }
        }

        const auto  bytes     = std::visit([](const auto &m) { return m.asBytes(); }, entry);
        const auto  tx_result = _900k_uart.transmit(bytes, telemTxTimeoutMs);
        const auto *can_msg   = std::get_if<io::telemMessage::TelemCanMsg>(&entry);
        if (not tx_result)
        {
            if (tx_result.error() == ErrorCode::TIMEOUT)
                continue;

            if (!radioLinkFaulted)
            {
                LOG_ERROR("telemTx: radio transmit failed: %s", error_code_to_string(tx_result.error()));
                radioLinkFaulted = true;
            }

            osDelay(faultBackoffMs);
            faultBackoffMs = (faultBackoffMs > faultBackoffMaxMs / 2U) ? faultBackoffMaxMs : faultBackoffMs * 2U;
            continue;
        }

        if (radioLinkFaulted)
        {
            LOG_INFO("telemTx: radio transmit recovered");
            radioLinkFaulted = false;
            faultBackoffMs   = faultBackoffMinMs;
        }

        if (can_msg != nullptr)
        {
            // LOG_INFO("UART telem sent: type=CAN can_id=0x%03lX", static_cast<unsigned long>(can_msg->msg.can_id));
        }
        else if (std::holds_alternative<io::telemMessage::NTPMsg>(entry))
        {
            LOG_INFO("UART telem sent: type=NTP");
        }
    }
}
void tasks_runTelemRx(void *arg)
{
    // ReceiveToIdle arms the peripheral for the whole buffer in one HAL call,
    // then returns whatever bytes arrived once the line idles (or the buffer
    // fills / half-fills). 64 bytes is several frames worth of headroom at
    // 57600 baud without delaying ingest — a single idle gap of ~1 byte time
    // (~174 us) already wakes the task.
    constexpr auto                        telemRxChunkSize  = 64U;
    constexpr uint32_t                    faultBackoffMinMs = 50U;
    constexpr uint32_t                    faultBackoffMaxMs = 1000U;
    std::array<uint8_t, telemRxChunkSize> scratch{};
    bool                                  radioLinkFaulted = false;
    uint32_t                              faultBackoffMs   = faultBackoffMinMs;
    forever
    {
        const auto rx_result = io::telemRx::read(scratch);
        if (!rx_result)
        {
            if (!radioLinkFaulted)
            {
                LOG_ERROR("telemRx: radio receive failed: %s", error_code_to_string(rx_result.error()));
                radioLinkFaulted = true;
            }

            osDelay(faultBackoffMs);
            faultBackoffMs = (faultBackoffMs > faultBackoffMaxMs / 2U) ? faultBackoffMaxMs : faultBackoffMs * 2U;
            continue;
        }

        if (radioLinkFaulted)
        {
            LOG_INFO("telemRx: radio receive recovered");
            radioLinkFaulted = false;
            faultBackoffMs   = faultBackoffMinMs;
        }

        app::telemRx::ingest(*rx_result);
        xTaskNotifyGive(static_cast<TaskHandle_t>(TaskTelemParse.id()));
    }
}

void tasks_runTelemParse(void *arg)
{
    forever
    {
        // Block until TaskTelemRx pushes new bytes into the ring.
        (void)ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        app::telemRx::drain();
    }
}

void tasks_runCanTx(void *arg)
{
    forever
    {
        const auto msg = can_tx_queue.pop();
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
void tasks_runCanRx(void *arg)
{
    forever
    {
        const auto msg = can_rx_queue.pop();
        if (not msg)
            continue;
        const auto    &can_msg = msg.value();
        const uint32_t now_ms  = io::time::getCurrentMs();
        // LOG_INFO("Received CAN msg with ID: 0x%03lX", static_cast<unsigned long>(can_msg.std_id));
        // const auto e = app::epochClock::getEpochMsFast();
        // if (e)
        // {
        //     (void)telem_tx_queue.push(io::telemMessage::TelemCanMsg(can_msg, *e));
        //     (void)log_queue.push(can_msg);
        // }

        io::can_rx::updateRxTableWithMessage(app::jsoncan::copyFromCanMsg(can_msg));
        if (app::can_data_capture::needsTelem(can_msg.std_id, now_ms))
        {
            // Telem timestamps go straight into InfluxDB as Unix epoch ms,
            // so they must come from the RTC, not the boot-monotonic clock.
            const auto epoch_ms = app::epochClock::getEpochMsFast();
            if (epoch_ms)
            {
                (void)telem_tx_queue.push(io::telemMessage::TelemCanMsg(can_msg, *epoch_ms));
            }
            else
            {
                LOG_WARN(
                    "telem RX timestamp unavailable, dropping CAN 0x%03lX", static_cast<unsigned long>(can_msg.std_id));
            }
        }
        if (app::can_data_capture::needsLog(can_msg.std_id, now_ms))
        {
            // Log the raw CAN frame. Framing/CRC happen in jobs_runLogging_tick
            // so the on-disk layout matches the shared Quintuna format and can
            // be decoded by firmware/logfs/python/logfs/can_logger.py.
            (void)log_queue.push(can_msg);
        }
        // Special alerts: aperiodic frames without data_capture still need telem forwarding.
        if (can_msg.std_id == app::can_utils::CRIT_TelemMarkEvent_Signals::MSG_ID)
        {
            const auto epoch_ms = app::epochClock::getEpochMsFast();
            if (epoch_ms)
            {
                (void)telem_tx_queue.push(io::telemMessage::TelemCanMsg(can_msg, *epoch_ms));
            }
            else
            {
                LOG_WARN(
                    "telem RX timestamp unavailable, dropping CAN 0x%03lX", static_cast<unsigned long>(can_msg.std_id));
            }
        }
    }
}

static void DAM_StartAllTasks()
{
    Task100Hz.start();
    TaskCanTx.start();
    TaskCanRx.start();
    Task1kHz.start();
    Task1Hz.start();
    TaskLogging.start();
    TaskTelemTx.start();
    TaskTelemRx.start();
    TaskTelemParse.start();
}

void tasks_preInit()
{
    hw::bootup::enableInterruptsForApp();
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();

#ifndef WATCHDOG_DISABLED
    __HAL_DBGMCU_FREEZE_IWDG();
#endif

    osKernelInitialize();

    fdcan1.init();
    hw::runtimeStat::init(htim7);

    const ResetReason reason = hw::resetReason::get();
    app::can_tx::DAM_ResetReason_set(static_cast<app::can_utils::CanResetReason>(reason));
    if (reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app::can_alerts::infos::WatchdogTimeout_set(true);
    }

    if (hw::bootup::BootRequest boot_request = hw::bootup::getBootRequest();
        boot_request.context != hw::bootup::BootContext::NONE)
    {
        // Check for stack overflow on a previous boot cycle and populate CAN alert.
        if (boot_request.context == hw::bootup::BootContext::OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app::can_alerts::infos::StackOverflow_set(true);
            app::can_tx::DAM_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == hw::bootup::BootContext::WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app::can_alerts::infos::WatchdogTimeout_set(true);
            app::can_tx::DAM_WatchdogTimeoutTask_set(boot_request.context_value);
        }

        // Clear stack overflow bootup.
        boot_request.context       = hw::bootup::BootContext::NONE;
        boot_request.context_value = 0;
        hw::bootup::setBootRequest(boot_request);
    }

    jobs_init();
    DAM_StartAllTasks();
    osKernelStart();
    forever {}
}

void tasks_tim_callback(const TIM_HandleTypeDef *tim)
{
#ifndef USE_CHIMERA
    if (tim == &htim7)
    {
        hw::runtimeStat::inc();
    }
#endif
}