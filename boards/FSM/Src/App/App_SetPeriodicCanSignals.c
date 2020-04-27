#include "App_SetPeriodicCanSignals.h"
#include "App_CanTx.h"
#include "App_FlowMeter.h"
#include "App_WheelSpeedSensor.h"

void App_SetPeriodicCanSignals_PrimaryFlowMeter(
    struct FsmCanTxInterface *can_tx,
    struct FlowMeter *        flow_meter)
{
    App_CanTx_SetPeriodicSignal_PRIMARY_FLOW_RATE(
        can_tx, App_FlowMeter_GetFlowRate(flow_meter));
}

void App_SetPeriodicCanSignals_SecondaryFlowMeter(
    struct FsmCanTxInterface *can_tx,
    struct FlowMeter *        flow_meter)
{
    App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_RATE(
        can_tx, App_FlowMeter_GetFlowRate(flow_meter));
}

void App_SetPeriodicCanSignals_LeftWheelSpeedSensor(
    struct FsmCanTxInterface *can_tx,
    struct WheelSpeedSensor * wheel_speed_sensor)
{
    App_CanTx_SetPeriodicSignal_LEFT_WHEEL_SPEED(
        can_tx, App_WheelSpeedSensor_GetWheelSpeed(wheel_speed_sensor));
}

void App_SetPeriodicCanSignals_RightWheelSpeedSensor(
    struct FsmCanTxInterface *can_tx,
    struct WheelSpeedSensor * wheel_speed_sensor)
{
    App_CanTx_SetPeriodicSignal_RIGHT_WHEEL_SPEED(
        can_tx, App_WheelSpeedSensor_GetWheelSpeed(wheel_speed_sensor));
}

void App_SetPeriodicCanSignals_WheelSpeedNonCriticalFault(
    struct FsmCanTxInterface *can_tx,
    struct WheelSpeedSensor * left_wheel_sensor,
    struct WheelSpeedSensor * right_wheel_sensor)
{
    const enum WheelSpeedStatus left_wheel_status =
        App_WheelSpeedSensor_GetStatus(left_wheel_sensor);
    const enum WheelSpeedStatus right_wheel_status =
        App_WheelSpeedSensor_GetStatus(right_wheel_sensor);

    App_CanTx_SetPeriodicSignal_WHEEL_SPEED_NON_CRITICAL_FAULT(
        can_tx, (left_wheel_status == WHEEL_SPEED_NON_CRITICAL_FAULT ||
                 right_wheel_status == WHEEL_SPEED_NON_CRITICAL_FAULT));
}
