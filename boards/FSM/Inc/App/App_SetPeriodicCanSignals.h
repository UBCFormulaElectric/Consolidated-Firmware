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
