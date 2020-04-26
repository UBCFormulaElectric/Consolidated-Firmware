#include "App_SetPeriodicCanSignals.h"
#include "App_CanTx.h"
<<<<<<< HEAD
=======
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
>>>>>>> Add simple wheel speed tests
