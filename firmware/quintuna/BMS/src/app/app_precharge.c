#include "app_precharge.h"
#include "app_timer.h"
#include "app_tractiveSystem.h"
#include "io_irs.h"
#include "app_canAlerts.h"
#include "app_segments.h"

static TimerChannel lower_bound_timer;
static TimerChannel upper_bound_timer;
static bool         precharge_limit_exceeded;
static uint8_t      num_precharge_failures;
// static bool         precharge_for_charge;

static TimerChannel cooldown_timer;
#define PRECHARGE_COOLDOWN_TIME (1000U) // 1 second cooldown after precharge failure

void app_precharge_init(void)
{
    precharge_limit_exceeded = false;
    num_precharge_failures   = 0;
    app_timer_init(&lower_bound_timer, PRECHARGE_COMPLETION_LOWER_BOUND);
    app_timer_init(&upper_bound_timer, PRECHARGE_COMPLETION_UPPER_BOUND);
    app_timer_init(&cooldown_timer, PRECHARGE_COOLDOWN_TIME);
}

void app_precharge_restart(void)
{
    app_timer_init(&cooldown_timer, PRECHARGE_COOLDOWN_TIME);
    // Restart timers for checking if we're precharging too slow/quick.
    app_timer_restart(&lower_bound_timer);
    app_timer_restart(&upper_bound_timer);
}

PrechargeState app_precharge_poll(const bool precharge_for_charging)
{
    switch (app_timer_updateAndGetState(&cooldown_timer))
    {
        case TIMER_STATE_RUNNING:
            return PRECHARGE_STATE_COOLDOWN;
        case TIMER_STATE_EXPIRED:
            // restart precharge process
            app_precharge_restart();
            break;
        case TIMER_STATE_IDLE:
        default:
            break;
    }
    const float ts_voltage = app_tractiveSystem_getVoltage();
#ifdef TARGET_HV_SUPPLY
#define HV_SUPPLY_VOLTAGE (550.0f)
    const float threshold_voltage = HV_SUPPLY_VOLTAGE * PRECHARGE_ACC_V_THRESHOLD;
#else
    const float threshold_voltage = app_segments_getPackVoltage() * PRECHARGE_ACC_V_THRESHOLD;
#endif

    const bool is_air_negative_open = io_irs_negativeState() == CONTACTOR_STATE_OPEN;
    const bool is_ts_rising_slowly =
        (ts_voltage < threshold_voltage) && (app_timer_updateAndGetState(&upper_bound_timer) == TIMER_STATE_EXPIRED);
    const bool is_ts_rising_quickly =
        (ts_voltage > threshold_voltage) && (app_timer_updateAndGetState(&lower_bound_timer) == TIMER_STATE_RUNNING);

    bool has_precharge_fault =
        precharge_for_charging ? is_ts_rising_slowly : is_ts_rising_slowly | is_ts_rising_quickly;
    has_precharge_fault |= is_air_negative_open;

    precharge_limit_exceeded = num_precharge_failures % MAX_PRECHARGE_ATTEMPTS == MAX_PRECHARGE_ATTEMPTS - 1U;

    if (has_precharge_fault)
    {
        num_precharge_failures++;
    }

    precharge_limit_exceeded = num_precharge_failures >= MAX_PRECHARGE_ATTEMPTS;
    app_canAlerts_BMS_Info_CriticalPrechargeFailure_set(precharge_limit_exceeded);

    // If there is a pre-charge fault and there were no more than three previous pre-charge faults
    // Go back to Init State, add one to the pre-charge failed counter and set the CAN charging message to false
    // Else go to Fault State, reset the pre-charge failed counter and set the CAN charging message to false
    if (has_precharge_fault)
    {
        app_timer_restart(&cooldown_timer);
        return precharge_limit_exceeded ? PRECHARGE_STATE_FAILED_CRITICAL : PRECHARGE_STATE_FAILED;
    }

    // If there is no precharge fault and the charger is connected
    // Close the AIRs+, reset fault counter and go to Charge State
    // Else close the AIRs+, reset fault counter and go to Drive State
    if (ts_voltage >= threshold_voltage)
    {
        num_precharge_failures = 0U;
        return PRECHARGE_STATE_SUCCESS;
    }

    return PRECHARGE_STATE_RUNNING;
}

bool app_precharge_limitExceeded(void)
{
    return precharge_limit_exceeded;
}
