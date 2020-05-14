#include "App_SetPeriodicCanSignals.h"
#include "App_InRangeCheck.h"

void App_SetPeriodicSignals_WheelSpeed(
    struct FsmCanTxInterface *can_tx,
    struct InRangeCheck *     wheel_speed_sensor,
    void (*wheel_speed_can_signal_setter)(struct FsmCanTxInterface *, float),
    void (*out_of_range_can_signal_setter)(struct FsmCanTxInterface *, uint8_t))
{
    float wheel_speed;
    out_of_range_can_signal_setter(
        can_tx,
        (uint8_t)App_InRangeCheck_GetValue(wheel_speed_sensor, &wheel_speed));
    wheel_speed_can_signal_setter(can_tx, wheel_speed);
}

void App_SetPeriodicSignals_FlowRate(
    struct FsmCanTxInterface *can_tx,
    struct InRangeCheck *     flow_meter,
    void (*flow_meter_can_signal_setter)(struct FsmCanTxInterface *, float),
    void (*out_of_range_can_signal_setter)(struct FsmCanTxInterface *, uint8_t))
{
    float flow_rate;
    out_of_range_can_signal_setter(
        can_tx, (uint8_t)App_InRangeCheck_GetValue(flow_meter, &flow_rate));
    flow_meter_can_signal_setter(can_tx, flow_rate);
}
