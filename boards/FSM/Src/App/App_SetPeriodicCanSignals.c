#include "App_SetPeriodicCanSignals.h"
#include "App_WheelSpeedSensor.h"

void App_SetPeriodicSignals_WheelSpeed(
    struct FsmCanTxInterface *can_tx,
    struct WheelSpeedSensor * wheel_speed_sensor,
    void (*wheel_speed_can_signal_setter)(struct FsmCanTxInterface *, float),
    void (*out_of_range_can_signal_setter)(struct FsmCanTxInterface *, uint8_t))
{
    const float wheel_speed =
        App_WheelSpeedSensor_GetWheelSpeed(wheel_speed_sensor);
    wheel_speed_can_signal_setter(can_tx, wheel_speed);

    // 150 km/h is the upper wheel speed threshold
    if (wheel_speed > 150.0f)
    {
        out_of_range_can_signal_setter(can_tx, true);
    }
    else
    {
        out_of_range_can_signal_setter(can_tx, false);
    }
}
