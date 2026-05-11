#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "io_adbms.hpp"
#include "app_timer.hpp"
#include "jobs.hpp"
#include <algorithm>
#include <cstring>
#include <cmath>

using namespace std;
using namespace app::can_tx;
using namespace app::can_utils;
using namespace app::segments;

static constexpr float DISCHARGE_THRESHOLD_V = 10e-3f;
static constexpr uint32_t SETTLE_TIME_MS  = 5 * 1000;
static constexpr uint32_t BALANCE_TIME_MS = 5 * 1000;

static BalancingState                                                 state;
static array<array<bool, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>    discharge_enabled;
static array<array<uint8_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> pwm_duty;
static app::Timer                                                          settle_timer(SETTLE_TIME_MS);
static app::Timer                                                          balance_timer(BALANCE_TIME_MS);

static void updateCellsToBalance()
{
    adbms_app_lock.take(io::MAX_TIMEOUT);
    memset(&discharge_enabled, 0, sizeof(discharge_enabled));
    memset(&pwm_duty, 0, sizeof(pwm_duty));

    for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            // Skip cells with failed voltage reads
            if (!cell_voltage_success[seg][cell])
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            // Never discharge the leader cell unless balancing to target voltage
            if (seg == min_cell_voltage.segment && cell == min_cell_voltage.cell && !app::can_rx::Debug_CellBalancing_OverrideValue_get())
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            // Never discharge below minimum allowed voltage
            if (cell_voltages[seg][cell] <= convertUVOVToFloat(VUV)) {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            const float delta = cell_voltages[seg][cell] - (app::can_rx::Debug_CellBalancing_OverrideValue_get() ? app::can_rx::Debug_CellBalancing_TargetValue_get() : min_cell_voltage.voltage);

            // Don't dischange below threshold
            if (delta < DISCHARGE_THRESHOLD_V)
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            const float raw_duty = app::can_rx::Debug_CellBalancing_OverrideDutyCycle_get() ? app::can_rx::Debug_CellBalancing_TargetDutyCycle_get() : 0.0f;

            pwm_duty[seg][cell] = (uint8_t) roundf(raw_duty / 100.0f * 15.0f);
            discharge_enabled[seg][cell] = true;
            
        }
    }

    app::segments::setBalanceConfig(discharge_enabled, true);
    app::segments::setPwmConfig(pwm_duty);
    adbms_app_lock.give();

    spi_bus_lock.take(io::MAX_TIMEOUT);
    app::segments::writeConfig();
    spi_bus_lock.give();
}

static void disableBalance()
{
    adbms_app_lock.take(io::MAX_TIMEOUT);
    memset(&discharge_enabled, 0, sizeof(discharge_enabled));
    memset(&pwm_duty, 0, sizeof(pwm_duty));
    app::segments::setBalanceConfig(discharge_enabled, false);
    app::segments::setPwmConfig(pwm_duty);
    adbms_app_lock.give();

    spi_bus_lock.take(io::MAX_TIMEOUT);
    app::segments::writeConfig();
    spi_bus_lock.give();
}

namespace app::segments
{
void balancingInit()
{
    disableBalance();
    state = BalancingState::BALANCING_DISABLED;
}

void balancingDisable() {
    disableBalance();
    state = BalancingState::BALANCING_DISABLED;
    LOG_INFO("Disabling");
}

void balancingEnable()
{
    switch (state)
    {
        case BalancingState::BALANCING_DISABLED:
        {
            settle_timer.restart();
            state = BalancingState::BALANCING_SETTLE;
            LOG_INFO("Settling");
            break;
        }
        case BalancingState::BALANCING_SETTLE:
        {
            if (settle_timer.updateAndGetState() == Timer::TimerState::EXPIRED)
            {
                updateCellsToBalance();
                io::adbms::sendBalanceCmd();
                balance_timer.restart();
                state = BalancingState::BALANCING_BALANCE;
                LOG_INFO("Balancing");
            }
            break;
        }
        case BalancingState::BALANCING_BALANCE:
        {
            if (balance_timer.updateAndGetState() == Timer::TimerState::EXPIRED)
            {
                io::adbms::sendStopBalanceCmd();
                settle_timer.restart();
                state = BalancingState::BALANCING_SETTLE;
                LOG_INFO("Settling");
            }
            break;
        }
        default:
            break;
    }
}
} // namespace app::segments
