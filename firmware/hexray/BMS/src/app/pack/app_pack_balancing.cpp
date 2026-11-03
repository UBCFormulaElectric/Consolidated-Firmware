#include <algorithm>
#include <cmath>
#include <limits>

#include "app_canRx.hpp"
#include "app_math.hpp"
#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "io_semaphore.hpp"

namespace
{
constexpr float   DISCHARGE_THRESHOLD_V = 10e-3f;
constexpr float   FULL_DUTY_DELTA_V     = 50e-3f;
constexpr uint8_t MAX_DUTY              = 0x0F;

uint32_t pwm_updated_ms = 0U;

io::adbms::PackCells<uint8_t>
    calculateDuty(const app::pack::BalancingBuffer &balance_buffer, const app::pack::PackCellFlags &cells_owc_ok)
{
    io::adbms::PackCells<uint8_t> duty{};

    // Min cell must have no open wire, invalid reading or undervoltage
    float min_cell_v = std::numeric_limits<float>::max();
    for (uint8_t seg = 0; seg < io::adbms::NUM_SEGMENTS; seg++)
    {
        if (!cells_owc_ok[seg][0])
            continue;

        for (uint8_t cell = 0; cell < io::adbms::CELLS_PER_SEGMENT; cell++)
        {
            const result<float> &volts = balance_buffer.voltages[seg][cell];
            if (volts && cells_owc_ok[seg][cell] && *volts > V_FAULT_UV)
                min_cell_v = std::min(min_cell_v, *volts);
        }
    }

    // Balance down to the min cell unless overridden
    const float target = app::can_rx::Debug_CellBalancing_MinCellVoltageOverrideEnable_get()
                             ? app::can_rx::Debug_CellBalancing_MinCellVoltageOverride_get()
                             : min_cell_v;

    for (uint8_t seg = 0; seg < io::adbms::NUM_SEGMENTS; seg++)
    {
        // Cell 0 open wire turns off balancing on entire segment
        if (!cells_owc_ok[seg][0])
            continue;

        for (uint8_t cell = 0; cell < io::adbms::CELLS_PER_SEGMENT; cell++)
        {
            const result<float> &volts = balance_buffer.voltages[seg][cell];

            // Invalid cell reading or cell open wire disables cell balancing
            if (!volts || !cells_owc_ok[seg][cell])
                continue;

            // Cell undervoltage disables balancing
            if (*volts <= V_FAULT_UV)
                continue;

            // Threshold value met disables balancing
            const float delta = *volts - target;
            if (delta < DISCHARGE_THRESHOLD_V)
                continue;

            // Duty derates linearly as cell approaches thershold
            duty[seg][cell] = static_cast<uint8_t>(
                std::lround(app::math::linearDerating(delta, MAX_DUTY, FULL_DUTY_DELTA_V, 0.0f, INCREASE_X)));
        }
    }
    return duty;
}
} // namespace

namespace app::pack::balancing
{

void init()
{
    stop();
}

void tick()
{
    const io::semaphore::unique_semaphore s{ buffer_lock };
    balance_buffer.balancing_enabled = true;

    // Recompute only when data is new
    if (balance_buffer.updated_ms == pwm_updated_ms)
        return;

    balance_buffer.pwm = calculateDuty(balance_buffer, owc_buffer.cells_owc_ok);
    pwm_updated_ms     = balance_buffer.updated_ms;
}

void stop()
{
    const io::semaphore::unique_semaphore s{ buffer_lock };
    balance_buffer = {};
}
} // namespace app::pack::balancing
