#pragma once
#include <stdbool.h>
#include <stdint.h>

struct FsmCanTxInterface;
struct WheelSpeedSensor;

void App_SetPeriodicSignals_WheelSpeed(
    struct FsmCanTxInterface *can_tx,
    struct WheelSpeedSensor * wheel_speed_sensor,
    void (*wheel_speed_can_signal_setter)(struct FsmCanTxInterface *, float),
    void (
        *out_of_range_can_signal_setter)(struct FsmCanTxInterface *, uint8_t));
