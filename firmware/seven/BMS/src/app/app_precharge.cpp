#include <cstdint>

#include "app_precharge.hpp"
#include "app_timer.hpp"
#include "app_tractiveSystem.hpp"
#include "app_segments.hpp"
#include "app_canAlerts.hpp"
#include "io_irs.hpp"
#include "io_semaphore.hpp"


namespace app::precharge
{
namespace
{
    Timer lower_bound_timer(PRECHARGE_COMPLETION_LOWER_BOUND);
    Timer upper_bound_timer(PRECHARGE_COMPLETION_UPPER_BOUND);
    Timer cooldown_timer(PRECHARGE_COOLDOWN_MS);

    bool    precharge_limit_exceeded = false;
    uint8_t num_precharge_failures   = 0U;
} // namespace

void init()
{
    precharge_limit_exceeded = false;
    num_precharge_failures   = 0U;
}

void restart()
{
    // Reset failure retry timer to idle state
    cooldown_timer.stop();

    // Restart timers for checking if we're precharging too fast / slow
    lower_bound_timer.restart();
    upper_bound_timer.restart();
}

State poll(bool precharge_for_charging)
{
    // Cooldown timer cases first
    switch (cooldown_timer.updateAndGetState())
    {
        // If timer running, precharge in cooldown from attempt
        case Timer::TimerState::RUNNING:
            return State::COOLDOWN;

        // restart precharge process after cooldown
        case Timer::TimerState::EXPIRED:
            restart();
            break;

        // Idle, dont poll
        case Timer::TimerState::IDLE:
        default:
            break;
    }

    const float ts_voltage = app::ts::getVoltage();

#ifdef TARGET_HV_SUPPLY
#define HV_SUPPLY_VOLTAGE (588.0f) // Allows us to build without having to read pack voltage
    const float threshold_voltage = HV_SUPPLY_VOLTAGE * PRECHARGE_ACC_V_THRESHOLD;
#else // take semaphore to get voltage and calculate threshold V
    float threshold_voltage;
    {
        io::unique_semaphore s{ shared_lock }; // why not semaphore give / take?
        threshold_voltage = app::segments::shared::getPackVoltage().value() * PRECHARGE_ACC_V_THRESHOLD;
    }
#endif

    // check if air negative open
    const bool is_air_negative_open = (io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);

    // checks if timer has expired (and is still running), and updates
    const Timer::TimerState upper_bound_state = upper_bound_timer.updateAndGetState();
    const Timer::TimerState lower_bound_state = lower_bound_timer.updateAndGetState();

    // if timer expires and Vts still < Vthreshold
    const bool is_ts_rising_slowly =
        (ts_voltage < threshold_voltage) && (upper_bound_state == Timer::TimerState::EXPIRED);

    // timer on lower bound running and Vts > Vthreshold
    const bool is_ts_rising_quickly =
        (ts_voltage > threshold_voltage) && (lower_bound_state == Timer::TimerState::RUNNING);

    // For charging we only consider "rising slowly" as a fault; otherwise consider both.
    bool has_precharge_fault =
        precharge_for_charging ? is_ts_rising_slowly : (is_ts_rising_slowly || is_ts_rising_quickly);

    has_precharge_fault = has_precharge_fault || is_air_negative_open;

    if (has_precharge_fault)
    {
        num_precharge_failures++;
    }

    // if limit exceeded, CAN alert
    precharge_limit_exceeded = (num_precharge_failures >= MAX_PRECHARGE_ATTEMPTS);
    app::can_alerts::infos::CriticalPrechargeFailure_set(precharge_limit_exceeded);

    // Fault handle
    if (has_precharge_fault)
    {
        cooldown_timer.restart();
        return precharge_limit_exceeded ? State::FAILED_CRITICAL : State::FAILED;
    }

    // Success path
    if (ts_voltage >= threshold_voltage)
    {
        num_precharge_failures = 0U;
        return State::SUCCESS;
    }

    return State::RUNNING
}

// check if limit exceeded
bool limitExceeded()
{
    return precharge_limit_exceeded;
}

// check if reset limit exceeded
void resetLimitExceeded()
{
    precharge_limit_exceeded = false;
    num_precharge_failures   = 0U;
}

} // namespace app::precharge