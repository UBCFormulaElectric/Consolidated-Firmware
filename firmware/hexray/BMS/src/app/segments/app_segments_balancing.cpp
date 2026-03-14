#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "io_adbms.hpp"
#include "app_timer.hpp"
#include <algorithm>
#include <cstring>

using namespace std;
using namespace app::can_tx;
using namespace app::can_utils;
using app::Timer;

// Minimum voltage delta to begin discharging a cell (10 mV)
static constexpr float DISCHARGE_THRESHOLD_V = 10e-3f;
// Delta at which PWM reaches full duty cycle (100 mV → duty = 15)
static constexpr float BALANCE_FULL_RANGE_V = 100e-3f;

static constexpr uint32_t SETTLE_TIME_MS  = 30 * 1000;
static constexpr uint32_t BALANCE_TIME_MS = 5 * 60 * 1000;

static BalancingState                                                 state;
static array<array<bool, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>    discharge_enabled;
static array<array<uint8_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> pwm_duty;
static Timer                                                          settle_timer(SETTLE_TIME_MS);
static Timer                                                          balance_timer(BALANCE_TIME_MS);

static void updateCellsToBalance()
{
    memset(&discharge_enabled, 0, sizeof(discharge_enabled));
    memset(&pwm_duty, 0, sizeof(pwm_duty));

    for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            // Never discharge the leader cell
            if (seg == min_cell_voltage.segment && cell == min_cell_voltage.cell)
                continue;

            // Skip cells with failed voltage reads
            if (!cell_voltage_success[seg][cell])
                continue;

            const float delta = cell_voltages[seg][cell] - min_cell_voltage.voltage;
            if (delta < DISCHARGE_THRESHOLD_V)
                continue;

            // Proportional PWM: clamp duty 1–15 based on how far above the leader the cell is
            const float raw_duty         = (delta / BALANCE_FULL_RANGE_V) * 15.0f;
            discharge_enabled[seg][cell] = true;
            pwm_duty[seg][cell]          = static_cast<uint8_t>(clamp(raw_duty, 1.0f, 15.0f));
        }
    }

    app::segments::setBalanceConfig(discharge_enabled);
    app::segments::setPwmConfig(pwm_duty);
    app::segments::writeConfig();
}

static void disableBalance()
{
    memset(&discharge_enabled, 0, sizeof(discharge_enabled));
    memset(&pwm_duty, 0, sizeof(pwm_duty));
    app::segments::setBalanceConfig(discharge_enabled);
    app::segments::setPwmConfig(pwm_duty);
    app::segments::writeConfig();
}

namespace app::segments
{
void balancingInit()
{
    disableBalance();
    state = BalancingState::BALANCING_DISABLED;
}

void balancingTick(bool enable)
{

    switch (state)
    {
        case BalancingState::BALANCING_DISABLED:
        {
            if (enable)
            {
                settle_timer.restart();
                state = BalancingState::BALANCING_SETTLE;
            }
            break;
        }
        case BalancingState::BALANCING_SETTLE:
        {
            if (!enable)
            {
                disableBalance();
                state = BalancingState::BALANCING_DISABLED;
            }
            else if (settle_timer.updateAndGetState() == Timer::TimerState::EXPIRED)
            {
                updateCellsToBalance();
                io::adbms::sendBalanceCmd();
                balance_timer.restart();
                state = BalancingState::BALANCING_BALANCE;
            }
            break;
        }
        case BalancingState::BALANCING_BALANCE:
        {
            if (!enable)
            {
                disableBalance();
                state = BalancingState::BALANCING_DISABLED;
            }
            else if (balance_timer.updateAndGetState() == Timer::TimerState::EXPIRED)
            {
                io::adbms::sendStopBalanceCmd();
                settle_timer.restart();
                state = BalancingState::BALANCING_SETTLE;
            }
            break;
        }
        default:
            break;
    }
}
} // namespace app::segments
