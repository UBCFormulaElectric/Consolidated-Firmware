#include "app/segments.h"
#include "app/segments/segments_internal.h"
#include "app/timer.h"
#include "app/utils.h"
#include "io/log.h"
#include "io/ltc6813.h"
#include <app_canTx.h>
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

static TimerChannel   settle_timer;
static TimerChannel   balance_timer;
static bool           discharge_enabled[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static CellParam      discharge_leader;
static BalancingState state;

static void updateCellsToBalance(void)
{
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (segment == discharge_leader.segment && cell == discharge_leader.cell)
            {
                discharge_enabled[segment][cell] = false;
                continue;
            }

            if (IS_EXIT_ERR(cell_voltage_success[segment][cell]))
            {
                discharge_enabled[segment][cell] = false;
                continue;
            }

            const float delta = cell_voltages[segment][cell] - discharge_leader.value;
            if (delta >= DISCHARGE_THRESHOLD_V)
            {
                discharge_enabled[segment][cell] = true;
            }
        }
    }

    app_segments_setBalanceConfig((const bool(*)[CELLS_PER_SEGMENT])discharge_enabled);
}

static void disableBalance(void)
{
    memset(discharge_enabled, false, sizeof(discharge_enabled));
    app_segments_setBalanceConfig((const bool(*)[CELLS_PER_SEGMENT])discharge_enabled);
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
