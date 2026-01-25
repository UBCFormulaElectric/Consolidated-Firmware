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

// NORMAL MODE  (override PWM = false)
//   30s settle → 5min balancing → repeat
//
// PWM MODE     (override PWM = true)
//   NO settling at all
//   Balance with PWM forever until all cells reach target voltage

#define DISCHARGE_THRESHOLD_V (10 * 1e-3f)
#define SETTLE_PERIOD_MS      (30 * 1000)
#define BALANCE_PERIOD_MS     (5 * 60 * 1000)
#define PWM_TICK_MS           (500U)
#define PWM_DEFAULT_FREQUENCY_HZ (0.1f)
#define PWM_DEFAULT_DUTY_PC       (50U)
#define PWM_MAXIMUM_FREQUENCY_HZ  (1000 / PWM_TICK_MS)

typedef struct {
    TimerChannel settle;
    TimerChannel balance;
    TimerChannel period;
    TimerChannel duty;
} BalTimers;

static BalTimers timers;
static bool discharge_candidates[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static BalancingState state;
static float target_voltage;
static bool pwm_enabled;

static inline bool timer_expired(TimerChannel *t)
{
    return app_timer_updateAndGetState(t) == TIMER_STATE_EXPIRED;
}

static void updateCellsToBalance(void)
{
    for (uint8_t s = 0; s < NUM_SEGMENTS; s++)
    {
        for (uint8_t c = 0; c < CELLS_PER_SEGMENT; c++)
        {
            bool should_balance = false;

            if (s == min_cell_voltage.segment && c == min_cell_voltage.cell)
            {
                should_balance = false;
            }
            else if (IS_EXIT_ERR(cell_voltage_success[s][c]))
            {
                should_balance = false;
            }
            else
            {
                const float delta = cell_voltages[s][c] - target_voltage;
                should_balance = (delta >= DISCHARGE_THRESHOLD_V);
            }

            discharge_candidates[s][c] = should_balance;
        }
    }
}

static void disableBalance(void)
{
    bool off[NUM_SEGMENTS][CELLS_PER_SEGMENT] = { false };
    app_segments_setBalanceConfig((const bool(*)[CELLS_PER_SEGMENT])off);
}

static void enableBalance(void)
{
    app_segments_setBalanceConfig((const bool(*)[CELLS_PER_SEGMENT])discharge_candidates);
}

static void updateBalancingConfig(void)
{
    pwm_enabled = app_canRx_Debug_CellBalancingOverridePWM_get();

    const bool override_target = app_canRx_Debug_CellBalancingOverrideTarget_get();
    target_voltage = override_target ?
        app_canRx_Debug_CellBalancingOverrideTarget_get() :
        min_cell_voltage.value;

    if (!pwm_enabled)
        return;

    float freq_hz  = app_canRx_Debug_CellBalancingOverridePWMFrequency_get();
    uint32_t duty  = app_canRx_Debug_CellBalancingOverridePWMDuty_get();

    if (freq_hz <= 0.f) freq_hz = PWM_DEFAULT_FREQUENCY_HZ;
    if (freq_hz > PWM_MAXIMUM_FREQUENCY_HZ) freq_hz = PWM_MAXIMUM_FREQUENCY_HZ;

    if (duty > 100U) duty = 100U;

    uint32_t period_ms = (uint32_t)(1000.0f / freq_hz);
    if (period_ms < PWM_TICK_MS) period_ms = PWM_TICK_MS;

    uint32_t on_time_ms = (uint32_t)(((float)duty / 100.0f) * (float)period_ms);

    app_timer_init(&timers.period, period_ms);
    app_timer_init(&timers.duty, on_time_ms);
}

static void balancingRunConstant(void)
{
    // Nothing needed: continuous ON
}

static void balancingRunPWM(void)
{
    if (timer_expired(&timers.period))
    {
        updateBalancingConfig();
        if (!pwm_enabled)
            return;

        updateCellsToBalance();
        enableBalance();
        app_timer_restart(&timers.period);
        app_timer_restart(&timers.duty);
    }
    else if (timer_expired(&timers.duty))
    {
        disableBalance();
    }
}

typedef void (*StateHandler)(bool enable);
static void handleDisabled(bool enable);
static void handleSettling(bool enable);
static void handleBalancing(bool enable);

static StateHandler handlers[NUM_BALANCING_STATE_CHOICES] = {
    [BALANCING_DISABLED] = handleDisabled,
    [BALANCING_SETTLE]   = handleSettling,
    [BALANCING_BALANCE]  = handleBalancing,
};

static void handleDisabled(bool enable)
{
    if (!enable)
        return;

    disableBalance();
    updateBalancingConfig();

    if (pwm_enabled)
    {
        app_canTx_BMS_BalancingLeaderSegment_set(min_cell_voltage.segment);
        app_canTx_BMS_BalancingLeaderCellIdx_set(min_cell_voltage.cell);

        updateCellsToBalance();
        enableBalance();

        app_timer_restart(&timers.period);
        app_timer_restart(&timers.duty);

        state = BALANCING_BALANCE;
        LOG_INFO("PWM: starting immediately, no settling");
    }
    else
    {
        app_timer_restart(&timers.settle);
        state = BALANCING_SETTLE;
        LOG_INFO("Balancing: settle 30s");
    }
}

static void handleSettling(bool enable)
{
    if (!enable)
    {
        disableBalance();
        state = BALANCING_DISABLED;
        app_canTx_BMS_BalancingLeaderSegment_set(0);
        app_canTx_BMS_BalancingLeaderCellIdx_set(0);
        return;
    }

    updateBalancingConfig();
    if (pwm_enabled)
    {
        app_canTx_BMS_BalancingLeaderSegment_set(min_cell_voltage.segment);
        app_canTx_BMS_BalancingLeaderCellIdx_set(min_cell_voltage.cell);

        updateCellsToBalance();
        enableBalance();

        app_timer_restart(&timers.period);
        app_timer_restart(&timers.duty);

        state = BALANCING_BALANCE;
        LOG_INFO("PWM: skipping settling");
        return;
    }

    if (!timer_expired(&timers.settle))
        return;

    app_canTx_BMS_BalancingLeaderSegment_set(min_cell_voltage.segment);
    app_canTx_BMS_BalancingLeaderCellIdx_set(min_cell_voltage.cell);

    updateCellsToBalance();
    enableBalance();

    app_timer_restart(&timers.balance);

    state = BALANCING_BALANCE;
    LOG_INFO("Normal: settled → balancing 5min");
}

static void handleBalancing(bool enable)
{
    if (!enable)
    {
        disableBalance();
        state = BALANCING_DISABLED;
        app_canTx_BMS_BalancingLeaderSegment_set(0);
        app_canTx_BMS_BalancingLeaderCellIdx_set(0);
        return;
    }

    if (!pwm_enabled)
    {
        if (timer_expired(&timers.balance))
        {
            disableBalance();
            app_timer_restart(&timers.settle);
            state = BALANCING_SETTLE;
            LOG_INFO("Normal: 5min done → settle 30s");
            return;
        }
        balancingRunConstant();
        return;
    }

    updateCellsToBalance();

    bool done = true;
    for (uint8_t s = 0; s < NUM_SEGMENTS && done; s++)
        for (uint8_t c = 0; c < CELLS_PER_SEGMENT; c++)
            if (discharge_candidates[s][c])
                done = false;

    if (done)
    {
        disableBalance();
        app_canTx_BMS_BalancingLeaderSegment_set(0);
        app_canTx_BMS_BalancingLeaderCellIdx_set(0);
        state = BALANCING_DISABLED;
        LOG_INFO("PWM: all cells reached target");
        return;
    }

    balancingRunPWM();
}

void app_segments_balancingInit(void)
{
    app_timer_init(&timers.settle,  SETTLE_PERIOD_MS);
    app_timer_init(&timers.balance, BALANCE_PERIOD_MS);

    disableBalance();
    pwm_enabled = false;
    target_voltage = 0.f;
    state = BALANCING_DISABLED;

    app_canTx_BMS_BalancingLeaderSegment_set(0);
    app_canTx_BMS_BalancingLeaderCellIdx_set(0);
}

void app_segments_balancingTick(bool enable)
{
    if (state < NUM_BALANCING_STATE_CHOICES && handlers[state] != NULL)
        handlers[state](enable);
    else
    {
        state = BALANCING_DISABLED;
        disableBalance();
    }

    app_canTx_BMS_BalancingState_set(state);
    app_canTx_BMS_ImbalanceMagnitude_set(max_cell_voltage.value - min_cell_voltage.value);
}
