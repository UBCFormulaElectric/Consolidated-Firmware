#include "tasks.h"
#include "main.h"
#include "jobs.hpp"

#include "app_jsoncan.hpp"

#include "io_time.hpp"
#include "io_telemMessage.hpp"
#include "io_canQueues.hpp"
#include "io_canQueues.hpp"
#include "io_log.hpp"
#include "io_telemRx.hpp"
#include <io_canRx.hpp>

#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_uarts.hpp"
#include "io_telemQueue.hpp"

#include <span>

extern "C"
{
#include "io_rtc.h"
}

// static IoRtcTime boot_time{};
#include "hw_cans.hpp"

[[noreturn]] static void tasks_run1Hz(void *arg)
{
    const uint32_t period_ms = 1000U;

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        io::telemRx::transmitNTPStartMsg();
        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_run100Hz(void *arg)
{
    const uint32_t period_ms = 10U;

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run100Hz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_run1kHz(void *arg)
{
    const uint32_t period_ms = 1U;

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        HAL_IWDG_Refresh(&hiwdg);
        jobs_run1kHz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_runLogging(void *arg)
{
    osDelayUntil(osWaitForever);
    forever
    {
        jobs_runLogging_tick();
    }
}
[[noreturn]] static void tasks_runTelem(void *arg)
{
    // const io::telemMessage::BaseTimeRegMsg base_time_msg(boot_time);
    // LOG_IF_ERR(_900k_uart.transmitPoll(
    //     std::span<const uint8_t>{ reinterpret_cast<const uint8_t *>(&base_time_msg), sizeof(base_time_msg) }));

    forever
    {
        const auto result = telem_tx_queue.pop();
        if (not result)
        {
            LOG_ERROR("Failed to pop telem TX message: %d", static_cast<int>(result.error()));
            continue;
        }

        const auto &msg = result.value();
        const auto  tx_result =
            _900k_uart.transmit(std::span<const uint8_t>{ reinterpret_cast<const uint8_t *>(&msg), msg.wireSize() });
        if (not tx_result)
        {
            LOG_ERROR("Failed to transmit telem message: %d", static_cast<int>(tx_result.error()));
        }
    }
}
[[noreturn]] static void tasks_runTelemRx(void *arg)
{
    osDelayUntil(osWaitForever);
    forever
    {
        io::telemRx::pollForRadioMessages();
    }
}

[[noreturn]] static void tasks_runCanTx(void *arg)
{
    forever
    {
        const auto msg = can_tx_queue.pop();
        if (not msg)
            continue;
        if (const auto &m = msg.value(); m.bus == app::can_utils::BusEnum::Bus_FDCAN)
        {
            const auto res = hw::can::fdcan1.fdcan_transmit(hw::CanMsg{
                m.std_id,
                m.dlc,
                m.data,
            });
            if (not res)
            {
                LOG_ERROR("fdcan1.fdcan_transmit(...) exited with an error: %d", static_cast<int>(res.error()));
            }
        }
        else
        {
            LOG_ERROR("INVALID BUS %d", m.bus);
        }
    }
}
[[noreturn]] static void tasks_runCanRx(void *arg)
{
    forever
    {
        const auto msg = can_rx_queue.pop();
        if (not msg)
            continue;
        io::can_rx::updateRxTableWithMessage(app::jsoncan::copyFromCanMsg(msg.value()));
    }
}

// Define the task with StaticTask Class
static hw::rtos::StaticTask<8096>    Task100Hz(osPriorityRealtime, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512>     TaskCanTx(osPriorityAboveNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask<512 * 4> TaskCanRx(osPriorityHigh, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512>     Task1kHz(osPriorityBelowNormal, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<1024>    Task1Hz(osPriorityHigh, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<1024>    TaskLogging(osPriorityHigh, "TaskLogging", tasks_runLogging);
static hw::rtos::StaticTask<512>     TaskTelem(osPriorityHigh, "TaskTelem", tasks_runTelem);
static hw::rtos::StaticTask<1024>    TaskTelemRx(osPriorityHigh, "TaskTelemRx", tasks_runTelemRx);

void DAM_StartAllTasks()
{
    Task100Hz.start();
    TaskCanTx.start();
    TaskCanRx.start();
    Task1kHz.start();
    Task1Hz.start();
    // TaskLogging.start();
    // TaskTelem.start();
    TaskTelemRx.start();
}

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    hw::can::fdcan1.init();
    osKernelInitialize();
    jobs_init();
    DAM_StartAllTasks();
    osKernelStart();
    forever {}
}
