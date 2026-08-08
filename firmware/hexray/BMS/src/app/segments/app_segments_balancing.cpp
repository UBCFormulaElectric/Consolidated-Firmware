#include "app_segments_internal.hpp"
#include "app_segments.hpp"

#include "app_canTx.hpp"
#include "app_canRx.hpp"
// #include "app_canUtils.hpp"
#include "app_pack.hpp"
#include "app_timer.hpp"

#include "io_adbms.hpp"
#include "io_semaphore.hpp"

#include <cstring>
#include <cmath>

namespace
{
constexpr float    DISCHARGE_THRESHOLD_V = 10e-3f;
constexpr uint32_t SETTLE_TIME_MS        = 1 * 1000;
constexpr uint32_t BALANCE_TIME_MS       = 5 * 1000;

using io::adbms::Cells;

auto           balancing_state = app::can_utils::BalancingState::BALANCING_DISABLED;
Cells<bool>    discharge_enabled;
Cells<uint8_t> pwm_duty;
app::Timer     settle_timer(SETTLE_TIME_MS);
app::Timer     balance_timer(BALANCE_TIME_MS);

void updateCellsToBalance(const app::pack::VoltStats &volt_stats, const app::pack::OwcStats &owc_stats)
{
    const Cells<float> &cell_voltages = volt_stats.voltages;

    const float   min_cell_voltage = volt_stats.min.value;
    const uint8_t min_cell_seg     = volt_stats.min.segment;
    const uint8_t min_cell_cell    = volt_stats.min.index;

    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        // If any cell in the segment has a failed voltage read or a failed/flagged
        // owc, disable balancing for the entire segment
        const bool segment_ok = volt_stats.valid[seg].all() && owc_stats.cells_ok[seg].all();

        if (!segment_ok)
        {
            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            {
                discharge_enabled[seg][cell] = false;
            }
            continue;
        }

        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            // Never discharge the leader cell unless balancing to target voltage
            if (seg == min_cell_seg && cell == min_cell_cell && !app::can_rx::Debug_CellBalancing_OverrideValue_get())
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            // Never discharge below minimum allowed voltage
            if (cell_voltages[seg][cell] <= app::segments::convertUVOVToFloat(VUV))
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            const float delta = cell_voltages[seg][cell] - (app::can_rx::Debug_CellBalancing_OverrideValue_get()
                                                                ? app::can_rx::Debug_CellBalancing_TargetValue_get()
                                                                : min_cell_voltage);

            // Don't dischange below threshold
            if (delta < DISCHARGE_THRESHOLD_V)
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            const float raw_duty = app::can_rx::Debug_CellBalancing_OverrideDutyCycle_get()
                                       ? app::can_rx::Debug_CellBalancing_TargetDutyCycle_get()
                                       : 0.0f;

            pwm_duty[seg][cell]          = static_cast<uint8_t>(roundf(raw_duty / 100.0f * 15.0f));
            discharge_enabled[seg][cell] = true;
        }
    }
    app::segments::config::setBalanceConfig(discharge_enabled, pwm_duty, true);
    app::segments::broadcast::debug::balancing(discharge_enabled, pwm_duty);
}
} // namespace

namespace app::segments::balancing
{
void init()
{
    discharge_enabled.fill({});
    pwm_duty.fill({});
    settle_timer.restart();
    balance_timer.restart();
    balancing_state = can_utils::BalancingState::BALANCING_DISABLED;
}

void disable()
{
    discharge_enabled.fill({});
    pwm_duty.fill({});
    config::setBalanceConfig(discharge_enabled, pwm_duty, false);
    {
        LOG_IF_ERR(io::adbms::command::stopBalance());
    }
    balancing_state = can_utils::BalancingState::BALANCING_DISABLED;
}

void tick(const app::pack::VoltStats &volts, const app::pack::OwcStats &owc)
{
    switch (balancing_state)
    {
        case can_utils::BalancingState::BALANCING_DISABLED:
        {
            settle_timer.restart();
            balancing_state = can_utils::BalancingState::BALANCING_SETTLE;
            LOG_INFO("Settling");
            break;
        }
        case can_utils::BalancingState::BALANCING_SETTLE:
        {
            if (settle_timer.updateAndGetState() == Timer::TimerState::EXPIRED)
            {
                updateCellsToBalance(volts, owc);
                {
                    // const io::unique_semaphore s{ spi_bus_lock };
                    if (const auto r = io::adbms::command::startBalance(); r)
                    {
                        balance_timer.restart();
                        balancing_state = can_utils::BalancingState::BALANCING_BALANCE;
                        LOG_INFO("Balancing");
                    }
                    else
                    {
                        LOG_IF_ERR(r);
                    }
                }
            }
            break;
        }
        case can_utils::BalancingState::BALANCING_BALANCE:
        {
            if (balance_timer.updateAndGetState() == Timer::TimerState::EXPIRED)
            {
                {
                    LOG_IF_ERR(io::adbms::command::stopBalance());
                }
                settle_timer.restart();
                balancing_state = can_utils::BalancingState::BALANCING_SETTLE;
                LOG_INFO("Settling");
            }
            break;
        }
        default:
            break;
    }
}
} // namespace app::segments::balancing
