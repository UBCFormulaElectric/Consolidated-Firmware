#include "app_steering.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include "app_rangeCheck.hpp"

#include "io_steering.hpp"

namespace app::steering
{
// TODO: angles need to be validated
static constexpr float MIN_STEERING_ANGLE_DEG = -95;
static constexpr float MAX_STEERING_ANGLE_DEG = 95;

static constexpr RangeCheck steering_angle_in_range_check = RangeCheck(MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG);

void broadcast()
{
    const float steering_angle       = io::steering::getAngleDegrees();
    const bool  steering_sensor_ocsc = io::steering::sensorOCSC();
    can_alerts::infos::SteeringAngleOCSC_set(steering_sensor_ocsc);
    can_tx::FSM_SteeringAngle_set(steering_angle_in_range_check.get_clamped_value(steering_angle));
    can_alerts::infos::SteeringAngleOutOfRange_set(
        steering_angle_in_range_check.get_status(steering_angle) != RangeCheck::Status::VALUE_IN_RANGE);
}
} // namespace app::steering