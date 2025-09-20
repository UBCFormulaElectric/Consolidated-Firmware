#include "app_segments.h"
#include "app_segments_internal.h"
#include "app_timer.h"
#include "app_utils.h"
#include "io_log.h"
#include "io_ltc6813.h"
#include <app_canTx.h>
#include <app_canRx.h>
#include <string.h>
#include <app_canUtils.h>

// Super-simple balancing algo:
// 1. Let cell voltages settle for 30 seconds
// 2. Find min cell voltage
// 3. Discharge all cells >10mV above the min cell for 5mins
// 4. Repeat!

#define DISCHARGE_THRESHOLD_V (10 * 1e-3f) // 10mV
#define SETTLE_PERIOD_MS (30 * 1000)       // 1min
#define BALANCE_PERIOD_MS (5 * 60 * 1000)  // 5min
#define PWM_TICK_MS (500U) // PWM resolution (500ms) is limited by tasks_runLtcVoltages
#define PWM_DEFAULT_FREQUENCY_HZ (0.1F) // 0.1Hz
#define PWM_DEFAULT_DUTY_PC (50U) // 50%
#define PWM_MAXIMUM_FREQUENCY_HZ (1000 / PWM_TICK_MS) // 2Hz

static TimerChannel   settle_timer;
static TimerChannel   balance_timer;
static TimerChannel   pwm_timer;


static bool           discharge_candidates[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static bool           discharge_config[NUM_SEGMENTS][CELLS_PER_SEGMENT];  
static CellParam      discharge_leader;
static BalancingState state;
static bool pwm_high;
static bool pwm_config_initialized;
static uint32_t pwm_period_ticks;
static uint32_t pwm_tick_index;
static uint32_t pwm_on_ticks;
static bool pwm_has_candidates;
static bool pwm_phase_active;

// Computes the ticks in a period for a given frequency
static uint32_t computePeriodTicks(const float frequency_hz) {
    
    if (frequency_hz <= 0.0f) {
        return 0U;
    }

    const float period_ms = 1000.0f/frequency_hz;
    const float ticks_per_period = period_ms / (float) PWM_TICK_MS;

    return (uint32_t) MAX(1.0f, roundf(ticks_per_period));
}

// Computes the on ticks in a period for a given duty cycle
static uint32_t computeOnTicks(const uint32_t period_ticks, const uint8_t duty_pc) {
    
    if (period_ticks == 0U || duty_pc == 0U) {
        return 0U;
    }

    if (duty_pc >= 100U) {
        return period_ticks;
    }

    const float duty_ratio = (float) duty_pc / 100.0f;
    const uint32_t raw_on_ticks = (uint32_t) roundf(duty_ratio * period_ticks);
    
    return CLAMP (raw_on_ticks, 1U, period_ticks);
}


static void computeDuties() {
    bool any_candidate = false;
    bool any_active = false;

    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++) {
        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++) {
            const bool candidate = discharge_candidates[segment][cell];
            if (candidate) {
                any_candidate = true;
            }

            discharge_config[segment][cell] = pwm_high && candidate;

            if (discharge_config[segment][cell]) {
                any_active = true;
            }
        }
    }

    pwm_has_candidates = any_candidate;
    pwm_phase_active = any_active;
    app_segments_setBalanceConfig((const bool(*)[CELLS_PER_SEGMENT])discharge_config);
}


static void computePWMBalance(void) {
    const bool override = app_canRx_Debug_CellBalancingOverridePWM_get();
    float freq_hz = override ? app_canRx_Debug_CellBalancingOverridePWMFrequency_get() : PWM_DEFAULT_FREQUENCY_HZ;
    uint8_t duty_pc = override ? app_canRx_Debug_CellBalancingOverridePWMDuty_get() : PWM_DEFAULT_DUTY_PC;   

    //Frequency of 0Hz means static enable based on duty cycle only
    if (freq_hz <= 0.0f) {
        pwm_period_ticks = 1U;
        pwm_on_ticks = duty_pc > 0 ? 1U : 0U;
        pwm_tick_index = 0U;
        pwm_high = duty_pc > 0;
        pwm_config_initialized = true;

        app_timer_stop(&pwm_timer);
        computeDuties();
        return;
    }

    // Compute desired period and on ticks
    const uint32_t desired_period_ticks = computePeriodTicks(freq_hz);
    const uint32_t desired_on_ticks = computeOnTicks(desired_period_ticks, duty_pc);

    // Check if config changed
    const bool config_changed = !pwm_config_initialized || desired_period_ticks != pwm_period_ticks || desired_on_ticks != pwm_on_ticks;
    pwm_config_initialized = true;

    // Handle special cases
    if (desired_on_ticks == 0U) {
        pwm_period_ticks = desired_period_ticks;
        pwm_on_ticks = 0U;
        pwm_tick_index = 0U;
        pwm_high = false;

        app_timer_stop(&pwm_timer);
        computeDuties();
        return;
    }

    if (desired_on_ticks >= desired_period_ticks) {
        pwm_period_ticks = MAX(1U, desired_period_ticks);
        pwm_on_ticks = pwm_period_ticks;
        pwm_tick_index = 0U;
        pwm_high = true;

        app_timer_stop(&pwm_timer);
        computeDuties();
        return;
    }

    // If config changed, reset the PWM cycle
    if (config_changed) {
        pwm_period_ticks = MAX(1U, desired_period_ticks);
        pwm_on_ticks = CLAMP(desired_on_ticks, 1U, pwm_period_ticks) ;
        pwm_tick_index = 0U;
        pwm_high = true;

        app_timer_stop(&pwm_timer);
        computeDuties();
        return;
    }

    const TimerState pwm_timer_state = app_timer_updateAndGetState(&pwm_timer);

    if(pwm_timer_state == TIMER_STATE_IDLE) {
        app_timer_restart(&pwm_timer);
        computeDuties();
        return;
    } 

    if (pwm_timer_state == TIMER_STATE_RUNNING) {
        computeDuties();
        return;
    }

    app_timer_restart(&pwm_timer);

    pwm_high = pwm_tick_index < pwm_on_ticks;
    pwm_tick_index = (pwm_tick_index + 1U) % pwm_period_ticks;
    computeDuties();
}

static void updateCellsToBalance(void)
{
    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            bool should_balance = false;

            if (segment == discharge_leader.segment && cell == discharge_leader.cell)
            {
                should_balance = false;
            } else if (IS_EXIT_ERR(cell_voltage_success[segment][cell])) {
                should_balance = false;
            } else {
                const float delta = cell_voltages[segment][cell] - discharge_leader.value;
                should_balance = (delta >= DISCHARGE_THRESHOLD_V);
            }

            discharge_candidates[segment][cell] = should_balance;
        
        }
    }
    computeDuties();
}

static void disableBalance(void)
{

    memset(discharge_candidates, false, sizeof(discharge_candidates));
    memset(discharge_config, false, sizeof(discharge_config));
    pwm_period_ticks = 1U;
    pwm_on_ticks =  0U;
    pwm_tick_index = 0U;
    pwm_high = false;
    pwm_config_initialized = false;
    pwm_has_candidates = false;
    pwm_phase_active = false;

    app_timer_stop(&pwm_timer);
    app_segments_setBalanceConfig((const bool(*)[CELLS_PER_SEGMENT])discharge_config);
}

bool app_segments_balancingHasCandidates(void) {
    return pwm_has_candidates;
}

bool app_segments_balancing_isPhaseActive(void) {
    return pwm_phase_active;
}

void app_segments_balancingInit(void)
{
    app_timer_init(&settle_timer, SETTLE_PERIOD_MS);
    app_timer_init(&balance_timer, BALANCE_PERIOD_MS);
    app_timer_init(&pwm_timer, PWM_TICK_MS);

    disableBalance();
    state = BALANCING_DISABLED;
}

void app_segments_balancingTick(bool enable)
{
    switch (state)
    {
        case BALANCING_DISABLED:
        {
            if (enable)
            {
                LOG_INFO("Balancing: Starting, letting cells settle for 30s");
                app_timer_restart(&settle_timer);
                state = BALANCING_SETTLE;
            }
            break;
        }
        case BALANCING_SETTLE:
        {
            if (!enable)
            {
                state = BALANCING_DISABLED;
                LOG_INFO("Balancing: Disabled");
            }
            else if (app_timer_updateAndGetState(&settle_timer) == TIMER_STATE_EXPIRED)
            {
                discharge_leader = min_cell_voltage;
                app_canTx_BMS_BalancingLeaderSegment_set(min_cell_voltage.segment);
                app_canTx_BMS_BalancingLeaderCellIdx_set(min_cell_voltage.cell);

                updateCellsToBalance();
                app_timer_restart(&balance_timer);
                state = BALANCING_BALANCE;

                LOG_INFO("Balancing: Cells settled, starting discharge for 5mins");
            }

            break;
        }
        case BALANCING_BALANCE:
        {
            if (!enable)
            {
                app_canTx_BMS_BalancingLeaderSegment_set(0);
                app_canTx_BMS_BalancingLeaderCellIdx_set(0);

                disableBalance();
                state = BALANCING_DISABLED;

                LOG_INFO("Balancing: Disabled");
            }
            else if (app_timer_updateAndGetState(&balance_timer) == TIMER_STATE_EXPIRED)
            {
                disableBalance();
                app_timer_restart(&settle_timer);
                state = BALANCING_SETTLE;
                LOG_INFO("Balancing: Discharged, letting cells settle for 30s");
            }

            computePWMBalance();
            break;
        }
        default:
        {
            // Should never get here!
            break;
        }
    }

    app_canTx_BMS_BalancingState_set(state);
    app_canTx_BMS_ImbalanceMagnitude_set(max_cell_voltage.value - min_cell_voltage.value);
}
