#include "app_steering.hpp"
#include "app_canTx.hpp"
extern "C"
{
#include "app_rangeCheck.h"
}

#include "io_steering.hpp"

namespace app::steering
{
// TODO: angles need to be validated
static constexpr float MIN_STEERING_ANGLE_DEG = -95;
static constexpr float MAX_STEERING_ANGLE_DEG = 95;

static constexpr RangeCheck steering_angle_in_range_check = {
    .get_value = nullptr,
    .min_value = MIN_STEERING_ANGLE_DEG,
    .max_value = MAX_STEERING_ANGLE_DEG,
};

void broadcast()
{
    const float steering_angle       = io::steering::getAngleDegrees();
    const bool  steering_sensor_ocsc = io::steering::sensorOCSC();
    can_tx::FSM_SteeringAngle_set(steering_angle);
    can_tx::FSM_Info_SteeringAngleOCSC_set(steering_sensor_ocsc);
    const auto [status, clamped_value] = app_rangeCheck_getValue(&steering_angle_in_range_check, steering_angle);
    can_tx::FSM_SteeringAngle_set(steering_angle);
    can_tx::FSM_Info_SteeringAngleOutOfRange_set(status != VALUE_IN_RANGE);
}
} // namespace app::steering