#pragma once

struct FsmCanTxInterface;
struct FlowMeter;
struct WheelSpeedSensor;

void App_SetPeriodicCanSignals_PrimaryFlowMeter(
    struct FsmCanTxInterface *can_tx,
    struct FlowMeter *        flow_meter);

void App_SetPeriodicCanSignals_SecondaryFlowMeter(
    struct FsmCanTxInterface *can_tx,
    struct FlowMeter *        flow_meter);

void App_SetPeriodicCanSignals_LeftWheelSpeedSensor(
    struct FsmCanTxInterface *can_tx,
    struct WheelSpeedSensor * wheel_speed_sensor);

void App_SetPeriodicCanSignals_RightWheelSpeedSensor(
    struct FsmCanTxInterface *can_tx,
    struct WheelSpeedSensor * wheel_speed_sensor);

void App_SetPeriodicCanSignals_WheelSpeedNonCriticalFault(
    struct FsmCanTxInterface *can_tx,
    struct WheelSpeedSensor * left_wheel_sensor,
    struct WheelSpeedSensor * right_wheel_sensor);
