#include "app_segments_internal.hpp"
#include "app_segments.hpp"

#include "jobs.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_timer.hpp"

#include "io_adbms.hpp"
#include "io_semaphore.hpp"

#include <cstring>
#include <cmath>

namespace
{
using io::adbms::Cells;

constexpr float    DISCHARGE_THRESHOLD_V = 10e-3f;
constexpr uint32_t SETTLE_TIME_MS        = 5 * 1000;
constexpr uint32_t BALANCE_TIME_MS       = 5 * 1000;

auto balancing_state = app::can_utils::BalancingState::BALANCING_DISABLED;
std::array<std::array<bool, CELLS_PER_SEGMENT>, NUM_SEGMENTS>    discharge_enabled;
std::array<std::array<uint8_t, CELLS_PER_SEGMENT>, NUM_SEGMENTS> pwm_duty;
app::Timer                                                       settle_timer(SETTLE_TIME_MS);
app::Timer                                                       balance_timer(BALANCE_TIME_MS);

void updateCellsToBalance(
    const Cells<std::expected<float, ErrorCode>> &cell_voltages,
    const app::segments::CellParam<float>        &min_cell_voltage)
{
    memset(&discharge_enabled, 0, sizeof(discharge_enabled));
    memset(&pwm_duty, 0, sizeof(pwm_duty));

    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            // Skip cells with failed voltage reads
            if (not cell_voltages[seg][cell])
            {
                discharge_enabled[seg][cell] = false;
                continue;
            }

            // Never discharge the leader cell unless balancing to target voltage
            if (seg == min_cell_voltage.segment && cell == min_cell_voltage.cell &&
                !app::can_rx::Debug_CellBalancing_OverrideValue_get())
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
                                                        : min_cell_voltage.value);

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
    balancing_state = can_utils::BalancingState::BALANCING_DISABLED;
    LOG_INFO("Disabling");
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
                updateCellsToBalance(state::getLatestVoltages(), state::getMinCellVoltage());
                {
                    const io::unique_semaphore s{ spi_bus_lock };
                    LOG_IF_ERR(io::adbms::sendBalanceCmd());
                }
                balance_timer.restart();
                balancing_state = can_utils::BalancingState::BALANCING_BALANCE;
                LOG_INFO("Balancing");
            }
            break;
        }
        case can_utils::BalancingState::BALANCING_BALANCE:
        {
            if (balance_timer.updateAndGetState() == Timer::TimerState::EXPIRED)
            {
                {
                    const io::unique_semaphore s{ spi_bus_lock };
                    LOG_IF_ERR(io::adbms::sendStopBalanceCmd());
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
