#include "app_brake.hpp"
#include "io_brake.hpp"
#include <cmath>
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include "app_rangeCheck.hpp"

static constexpr float MIN_BRAKE_PRESSURE_PSI       = 0.0f;
static constexpr float MAX_BRAKE_PRESSURE_PSI       = 1000.0f;
static constexpr float BRAKE_ACTUATED_THRESHOLD_PSI = 20.0f;

namespace app::brake
{
bool isActuated()
{
    return io::brake::getRearPressurePsi() > BRAKE_ACTUATED_THRESHOLD_PSI;
}

static constexpr RangeCheck brake_pressure_in_range_check = RangeCheck(MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI);

void broadcast()
{
    const float rear_pressure = io::brake::getRearPressurePsi();
    const bool  brake_pressed = isActuated();

    can_tx::RSM_BrakeActuated_set(brake_pressed);
    can_tx::RSM_RearBrakePressure_set(roundf(rear_pressure));

    can_alerts::infos::RearBrakePressureOutOfRange_set(
        brake_pressure_in_range_check.get_status(rear_pressure) != RangeCheck::Status::VALUE_IN_RANGE);
}
} // namespace app::brake