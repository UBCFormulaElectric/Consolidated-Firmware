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

#define DISCHARGE_THRESHOLD_V (10 * 1e-3f)            // 10mV
#define SETTLE_PERIOD_MS (30 * 1000)                  // 30s
#define BALANCE_PERIOD_MS (5 * 60 * 1000)             // 5min
#define PWM_TICK_MS (500U)                            // PWM resolution (500ms) is limited by tasks_runLtcVoltages
#define PWM_DEFAULT_FREQUENCY_HZ (0.1F)               // 0.1Hz
#define PWM_DEFAULT_DUTY_PC (50U)                     // 50%
#define PWM_MAXIMUM_FREQUENCY_HZ (1000 / PWM_TICK_MS) // 2Hz

static TimerChannel settle_timer;
static TimerChannel balance_timer;
static TimerChannel period_timer;
static TimerChannel pwm_duty_timer;

static bool           discharge_candidates[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static BalancingState state;
static float          target_voltage;

static void balanceConfig(void)
{
    const bool override_pwm = app_canRx_Debug_CellBalancingOverridePWM_get();
    float freq_hz = override_pwm ? app_canRx_Debug_CellBalancingOverridePWMFrequency_get() : PWM_DEFAULT_FREQUENCY_HZ;
    uint32_t duty_pc = override_pwm ? app_canRx_Debug_CellBalancingOverridePWMDuty_get() : PWM_DEFAULT_DUTY_PC;

    const bool override_target = app_canRx_Debug_CellBalancingOverrideTarget_get();
    target_voltage = override_target ? app_canRx_Debug_CellBalancingOverrideTarget_get() : min_cell_voltage.value;

    uint32_t period_ms  = (uint32_t)(1000.0f / freq_hz);
    uint32_t on_time_ms = (uint32_t)(((float)duty_pc / 100.0f) * (float)period_ms);

    app_timer_init(&period_timer, period_ms);
    app_timer_init(&pwm_duty_timer, on_time_ms);
}

static void updateCellsToBalance(void)
{
    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            bool should_balance = false;
            if (segment == min_cell_voltage.segment && cell == min_cell_voltage.cell)
            {
                should_balance = false;
            }
            else if (IS_EXIT_ERR(cell_voltage_success[segment][cell]))
            {
                should_balance = false;
            }
            else
            {
                const float delta = cell_voltages[segment][cell] - target_voltage;
                should_balance    = (delta >= DISCHARGE_THRESHOLD_V);
            }
            discharge_candidates[segment][cell] = should_balance;
        }
    }
}

static void disableBalance(void)
{
    bool discharge_off[NUM_SEGMENTS][CELLS_PER_SEGMENT] = { false };
    app_segments_setBalanceConfig((const bool(*)[CELLS_PER_SEGMENT])discharge_off);
}

static void enableBalance(void)
{
    app_segments_setBalanceConfig((const bool(*)[CELLS_PER_SEGMENT])discharge_candidates);
}

void app_segments_balancingInit(void)
{
    app_timer_init(&settle_timer, SETTLE_PERIOD_MS);
    app_timer_init(&balance_timer, BALANCE_PERIOD_MS);

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
                app_canTx_BMS_BalancingLeaderSegment_set(min_cell_voltage.segment);
                app_canTx_BMS_BalancingLeaderCellIdx_set(min_cell_voltage.cell);

                balanceConfig();
                updateCellsToBalance();
                app_timer_restart(&pwm_duty_timer);
                app_timer_restart(&period_timer);
                app_timer_restart(&balance_timer);
                enableBalance();
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
            else if (app_timer_updateAndGetState(&period_timer) == TIMER_STATE_EXPIRED)
            {
                balanceConfig();
                enableBalance();
                app_timer_restart(&pwm_duty_timer);
                app_timer_restart(&period_timer);
            }
            else if (app_timer_updateAndGetState(&pwm_duty_timer) == TIMER_STATE_EXPIRED)
            {
                disableBalance();
            }

            break;
        }
        case NUM_BALANCING_STATE_CHOICES:
        default:
        {
            // Should never get here!
            break;
        }
    }

    app_canTx_BMS_BalancingState_set(state);
    app_canTx_BMS_ImbalanceMagnitude_set(max_cell_voltage.value - min_cell_voltage.value);
}