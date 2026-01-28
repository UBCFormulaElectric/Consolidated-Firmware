#include "app_steering.hpp"
extern "C"
{
#include "app_canTx.h"
#include "app_rangeCheck.h"
#include "app_canAlerts.h"
}

#include "io_steering.hpp"

namespace app::steering
{
// TODO: angles need to be validated
static constexpr float MIN_STEERING_ANGLE_DEG = -95;
static constexpr float MAX_STEERING_ANGLE_DEG = 95;

static const RangeCheck steering_angle_in_range_check = {
    .get_value = nullptr,
    .min_value = MIN_STEERING_ANGLE_DEG,
    .max_value = MAX_STEERING_ANGLE_DEG,
};

void broadcast(void)
{
    const float steering_angle       = io::steering::getAngleDegrees();
    const bool  steering_sensor_ocsc = io::steering::sensorOCSC();
    app_canTx_FSM_SteeringAngle_set(steering_angle);
    app_canAlerts_FSM_Info_SteeringAngleOCSC_set(steering_sensor_ocsc);
    const RangeCheckStatusMetaData steering_in_range =
        app_rangeCheck_getValue(&steering_angle_in_range_check, steering_angle);
    app_canTx_FSM_SteeringAngle_set(steering_angle);
    app_canAlerts_FSM_Info_SteeringAngleOutOfRange_set(steering_in_range.status != VALUE_IN_RANGE);
}
} // namespace app::steering