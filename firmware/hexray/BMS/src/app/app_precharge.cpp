// app_precharge.cpp
#include <cstdint>

#include "app_precharge.hpp"
#include "app_timer.hpp"
#include "app_tractiveSystem.hpp"
#include "io_irs.hpp"
// #include "app_segments.hpp"

extern "C"
{
#include "app_canAlerts.h"
}

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
    // Restart timers for checking if we're precharging too slow/quick.
    lower_bound_timer.restart();
    upper_bound_timer.restart();
}

State poll(bool precharge_for_charging)
{
    // Handle cooldown timer first
    switch (cooldown_timer.updateAndGetState())
    {
        case Timer::TimerState::RUNNING:
            return State::COOLDOWN;

        case Timer::TimerState::EXPIRED:
            // Restart precharge process after cooldown
            restart();
            break;

        case Timer::TimerState::IDLE:
        default:
            break;
    }

    const float ts_voltage = app::ts::getVoltage();

#ifdef TARGET_HV_SUPPLY
#define HV_SUPPLY_VOLTAGE (588.0f)
    const float threshold_voltage = HV_SUPPLY_VOLTAGE * PRECHARGE_ACC_V_THRESHOLD;
#else
    const float threshold_voltage = app::segments::getPackVoltage() * PRECHARGE_ACC_V_THRESHOLD;
#endif

    const bool is_air_negative_open = (io::irs::negativeState() == CONTACTOR_STATE_OPEN);

    const bool is_ts_rising_slowly =
        (ts_voltage < threshold_voltage) && (upper_bound_timer.updateAndGetState() == Timer::TimerState::EXPIRED);

    const bool is_ts_rising_quickly =
        (ts_voltage > threshold_voltage) && (lower_bound_timer.updateAndGetState() == Timer::TimerState::RUNNING);

    // For charging we only consider "rising slowly" as a fault; otherwise consider both.
    bool has_precharge_fault =
        precharge_for_charging ? is_ts_rising_slowly : (is_ts_rising_slowly || is_ts_rising_quickly);

    has_precharge_fault = has_precharge_fault || is_air_negative_open;

    if (has_precharge_fault)
    {
        ++num_precharge_failures;
    }

    precharge_limit_exceeded = (num_precharge_failures >= MAX_PRECHARGE_ATTEMPTS);
    app::canAlerts::BMS_Info_CriticalPrechargeFailure_set(precharge_limit_exceeded);

    // Fault handling
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

    return State::RUNNING;
}

bool limitExceeded()
{
    return precharge_limit_exceeded;
}

} // namespace app::precharge