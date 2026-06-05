#include "app_segments_internal.hpp"
#include "app_segments.hpp"

#include "app_canTx.hpp"
#include "app_canRx.hpp"
// #include "app_canUtils.hpp"
#include "app_timer.hpp"

#include "io_adbms.hpp"
#include "io_semaphore.hpp"

#include <cstring>
#include <cmath>

namespace
{
constexpr float    DISCHARGE_THRESHOLD_V = 10e-3f;
constexpr uint32_t SETTLE_TIME_MS        = 0;
constexpr uint32_t BALANCE_TIME_MS       = 5 * 1000;

using io::adbms::Cells;

auto           balancing_state = app::can_utils::BalancingState::BALANCING_DISABLED;
Cells<bool>    discharge_enabled;
Cells<uint8_t> pwm_duty;
app::Timer     settle_timer(SETTLE_TIME_MS);
app::Timer     balance_timer(BALANCE_TIME_MS);

void updateCellsToBalance()
{
    const Cells<result<float>> cell_voltages                   = app::segments::shared::getLatestVoltages();
    const auto [min_cell_seg, min_cell_cell, min_cell_voltage] = app::segments::shared::getMinCellVoltage();
    const Cells<result<bool>> cell_owc                         = app::segments::shared::getLatestCellOwc();

    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            // Skip cells with failed voltage reads
            if (!cell_voltages[seg][cell])
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            // Skip cells with failed owc or owc flagged
            if (!cell_owc[seg][cell].value_or(false))
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            // Never discharge the leader cell unless balancing to target voltage
            if (seg == min_cell_seg && cell == min_cell_cell && !app::can_rx::Debug_CellBalancing_OverrideValue_get())
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            // Never discharge below minimum allowed voltage
            if (cell_voltages[seg][cell].value() <= app::segments::convertUVOVToFloat(VUV))
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            const float delta =
                cell_voltages[seg][cell].value() - (app::can_rx::Debug_CellBalancing_OverrideValue_get()
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
        // const io::unique_semaphore s{ spi_bus_lock };
        LOG_IF_ERR(io::adbms::command::stopBalance());
    }
    balancing_state = can_utils::BalancingState::BALANCING_DISABLED;
}

void tick()
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
                updateCellsToBalance();
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
                    // const io::unique_semaphore s{ spi_bus_lock };
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
