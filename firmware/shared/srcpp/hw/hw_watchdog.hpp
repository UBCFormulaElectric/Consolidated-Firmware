#pragma once
/**
 * @brief Monitor task deadlines by assigning a software watchdog to each
 *        periodic task.
 * @note The current implementation doesn't really support monitoring
 *       "non-periodic" tasks (e.g. CAN TX).
 * @note Some hints for setting up the hardware watchdog in STM32CubeMX:
 *       - Use __HAL_DBGMCU_FREEZE_IWDG to stop the IWDG clock when the CPU is
 *         halted due to a breakpoint, or else the IWDG can likely trigger an
 *         undesirable system reboot.
 *       - Create User Constants in STM32CubeMX with the following values:
 *
 *             IWDG_WINDOW_DISABLE_VALUE = 4095
 *             LSI_FREQUENCY             = <LSI Clock Frequency in Hertz.
 *                                          For example, it is 40000 for
 *                                          STM32F302R8.>
 *             IWDG_PRESCALER            = 4
 *             IWDG_RESET_FREQUENCY      = 5
 *
 *         ...then set the following parameters for "Watchdog Clocking" under
 *         "Parameter Settings" for IWDG:
 *
 *            IWDG counter clock prescaler   = 4
 *            IWDG window value              = IWDG_WINDOW_DISABLE_VALUE
 *            IWDG down-counter reload value = LSI_FREQUENCY / IWDG_PRESCALER
 *                                             / IWDG_RESET_FREQUENCY
 *
 *         In case it was not clear, IWDG_RESET_FREQUENCY = 5 means that the
 *         IWDG has a frequency of 5Hz, or a period of 200ms.
 */
#include "io_log.hpp"
#include "hw_error.hpp"
#include "hw_bootup.hpp"

#include <cmsis_os.h>
#include <atomic>
#include <array>
#include <optional>
#ifdef STM32H562xx
#include <stm32h5xx_hal.h>
#include <stm32h5xx_hal_iwdg.h>
#elif defined(STM32H733xx)
#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_iwdg.h>
#endif

namespace hw::watchdog
{
struct instance
{
    // The tick period of the task being monitored.
    uint32_t period;
    // The tick grace period of the task being monitored.
    uint32_t grace_period;
    // The current deadline of the task being monitored.
    uint32_t deadline;
    // We use a counter instead of a boolean flag for the following reasons:
    // - We only check for a check-in after the deadline + grace period has passed.
    // - Assume a task mostly finishes in [start, start + grace period).
    // - It can then check in twice before we check once: once for its own tick and once at
    //   [start + period, start + period + grace period) for the next tick.
    // - With a flag, these 2 check-ins collapse into 1, so every check after this effectively uses the
    //   check-in of the next tick. The task now has to finish within the grace period instead of the
    //   period + grace period.
    // - If the task then takes a bit longer once (ex. it finishes at start + grace period + 1ms, which is
    //   still well within its deadline), the watchdog task will think that task had a deadline miss.
    // - ex. period = 10ms, grace = 2ms: the task checks in at 1, 11 and 21ms. The check at 12ms clears both
    //   the 1ms and 11ms check-ins, and the check at 22ms clears the 21ms one. If the next tick finishes
    //   at 33ms, the check at 32ms sees no check-in and the watchdog trips.
    // A counter counts every check-in, so each check uses exactly 1 check-in and every tick always gets
    // the full period + grace period.
    std::atomic<uint32_t> check_in_count = 0;
    // ID to identify the task this watchdog monitors (for debugging only).
    TaskHandle_t task_id = xTaskGetCurrentTaskHandle();

    explicit instance(const uint32_t period_ticks, const uint32_t grace_period_ticks)
      : period(period_ticks), grace_period(grace_period_ticks), deadline(period_ticks + osKernelGetTickCount())
    {
    }

    /**
     * Every periodic task monitored by a software watchdog must call this at the
     * end of each period.
     */
    void checkIn() { check_in_count.fetch_add(1U, std::memory_order_release); }
};

template <size_t WATCHDOG_INSTANCES> class monitor
{
    std::array<std::optional<instance>, WATCHDOG_INSTANCES> watchdogs{};
    bool                                                    timeout_detected = false;

    IWDG_HandleTypeDef &handle;
    void (*timeout_callback)(const instance &){};

  public:
    constexpr explicit monitor(IWDG_HandleTypeDef &handle_in, void (*timeout_callback_in)(const instance &) = nullptr)
      : handle(handle_in), timeout_callback(timeout_callback_in){};

    /**
     * Note that this function must be called from the task being monitored.
     * This is important because the instance is instantiated with the current (monitored) task's ID for debugging
     * purposes.
     * @param period The tick period of the task being monitored.
     * @param grace_period Extra ticks a tick may take past its period before it counts as a deadline miss.
     * @return An instance of the software watchdog monitoring the task that calls this function.
     */
    instance &spawn_instance(const uint32_t period, const uint32_t grace_period)
    {
        for (std::optional<instance> &i : watchdogs)
        {
            if (not i.has_value())
            {
                // Constructed in place: instance holds a std::atomic, so it cannot be copied or moved.
                i.emplace(period, grace_period);
                return i.value();
            }
        }
        LOG_ERROR("Failed to register watchdog instance. Maximum number of watchdog instances reached.");
        Error_Handler();
    }

    /**
     * Check if any software watchdog has expired.
     * @note  If no software watchdog has expired, the hardware watchdog is
     *        refreshed. If any software watchdog has expired, the callback function
     *        is called and the hardware watchdog will no longer be refreshed.
     * @note  This function must be called periodically. It is good practice to call
     *        it from the system tick handler.
     */
    void checkForTimeouts()
    {
        // If a timeout is detected, let the hardware watchdog timeout reset the
        // system. We don't reboot immediately because we need some time to log
        // information for further debugging.
        if (timeout_detected)
        {
            return;
        }

        for (std::optional<instance> &instance : watchdogs)
        {
            if (not instance.has_value())
                continue;

            if (osKernelGetTickCount() >= instance->deadline + instance->grace_period)
            {
                // Check if the check-in counter is non-zero
                if (instance->check_in_count.load(std::memory_order_relaxed) > 0)
                {
                    // Decrement the check-in counter
                    instance->check_in_count.fetch_sub(1U, std::memory_order_relaxed);

                    // Update deadline
                    instance->deadline += instance->period;
#ifndef WATCHDOG_DISABLED
                    HAL_IWDG_Refresh(&handle);
#endif
                }
                else
                {
                    TaskStatus_t status;
                    vTaskGetInfo(instance->task_id, &status, pdFALSE, eRunning);

                    LOG_ERROR(
                        "Software watchdog detected a timeout in a task, letting hardware watchdog reset the MCU (task "
                        "= "
                        "%s, "
                        "id = %d)",
                        pcTaskGetTaskName(instance->task_id), status.xTaskNumber);

#ifdef BOOTLOAD
                    const bootup::BootRequest request = {
                        .target        = bootup::BootTarget::APP,
                        .context       = bootup::BootContext::WATCHDOG_TIMEOUT,
                        ._unused       = 0xffff,
                        .context_value = status.xTaskNumber,
                    };
                    bootup::setBootRequest(request);
#endif
                    timeout_detected = true;
                    if (timeout_callback != nullptr)
                    {
                        timeout_callback(*instance);
                    }
                    return;
                }
            }
        }
    }
};
} // namespace hw::watchdog
// namespace hw::watchdog

/*
 * NOT implemented yet
 * Callback function used to perform additional operations prior to the reset of the microcontroller.
 * For example, a message may be written to a log file.
 */
