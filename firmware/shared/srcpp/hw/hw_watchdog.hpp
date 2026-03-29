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

#include <cstdint>
#include <cassert>
#include <cmsis_os.h>
#include "io_log.hpp"
#include <array>
#ifdef STM32H562xx
#include <stm32h5xx_hal.h>
#include <stm32h5xx_hal_iwdg.h>
#elif defined(STM32H733xx)
#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_iwdg.h>
#endif

namespace hw::watchdog
{
struct WatchdogInstance
{
    // Is this watchdog ready to be used?
    bool initialized;
    // The tick period of the task being monitored.
    uint32_t period;
    // The current deadline of the task being monitored.
    uint32_t deadline;
    // Has the task being monitored checked in for the current period?
    bool check_in_status;
    // ID to identify the task this watchdog monitors (for debugging only).
    xTaskHandle task_id;

    explicit WatchdogInstance(const uint32_t period_in_ticks)
      : initialized(true),
        period(period_in_ticks),
        deadline(period_in_ticks),
        check_in_status(true),
        task_id(xTaskGetCurrentTaskHandle())
    {
    }

    /**
     * Every periodic task monitored by a software watchdog must call this at the
     * end of each period.
     * @param watchdog: Handle to the software watchdog
     */
    void checkIn()
    {
        assert(initialized == true);
        check_in_status = true;
    }
};
class monitor
{
  private:
    constexpr static int                                   MAX_WATCHDOG_INSTANCES = 10;
    std::array<WatchdogInstance *, MAX_WATCHDOG_INSTANCES> watchdogs{};
    bool                                                   timeout_detected = false;

    WatchdogInstance   *watchdog_instance;
    IWDG_HandleTypeDef &handle;
    HAL_StatusTypeDef (*refresh_watchdog)(IWDG_HandleTypeDef *){};
    void (*timeout_callback)(WatchdogInstance *){};

    // Function to refresh hardware watchdog
    void refresh_hardware_watchdog() { refresh_watchdog(&handle); }

  public:
    constexpr explicit monitor(
        WatchdogInstance   *watchdog_instance_in,
        IWDG_HandleTypeDef &handle_in,
        HAL_StatusTypeDef (*refresh_watchdog_in)(IWDG_HandleTypeDef *),
        void (*timeout_callback_in)(WatchdogInstance *) = nullptr)
      : watchdog_instance(watchdog_instance_in),
        handle(handle_in),
        refresh_watchdog(refresh_watchdog_in),
        timeout_callback(timeout_callback_in){};

    void registerWatchdogInstance();

    /**
     * Check if any software watchdog has expired.
     * @note  If no software watchdog has expired, the hardware watchdog is
     *        refreshed. If any software watchdog has expired, the callback function
     *        is called and the hardware watchdog will no longer be refreshed.
     * @note  This function must be called periodically. It is good practice to call
     *        it from the system tick handler.
     */
    void checkForTimeouts();
};
} // namespace hw::watchdog
// namespace hw::watchdog

/*
 * NOT implemented yet
 * Callback function used to perform additional operations prior to the reset of the microcontroller.
 * For example, a message may be written to a log file.
 */
