#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SetPeriodicCanSignals.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)
STATIC_DEFINE_APP_SET_CAN_SIGNALS_BINARY_SWITCH(FsmCanTxInterface)

void App_SetPeriodicSignals_FlowRateInRangeChecks(const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    struct InRangeCheck *primary_flow_rate_in_range_check =
        App_FsmWorld_GetPrimaryFlowRateInRangeCheck(world);
    struct InRangeCheck *secondary_flow_rate_in_range_check =
        App_FsmWorld_GetSecondaryFlowRateInRangeCheck(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, primary_flow_rate_in_range_check,
        App_CanTx_SetPeriodicSignal_PRIMARY_FLOW_RATE,
        App_CanTx_SetPeriodicSignal_PRIMARY_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, secondary_flow_rate_in_range_check,
        App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_RATE,
        App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

void App_SetPeriodicSignals_WheelSpeedInRangeChecks(
    const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    struct InRangeCheck *left_wheel_speed_in_range_check =
        App_FsmWorld_GetLeftWheelSpeedInRangeCheck(world);
    struct InRangeCheck *right_wheel_speed_in_range_check =
        App_FsmWorld_GetRightWheelSpeedInRangeCheck(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, left_wheel_speed_in_range_check,
        App_CanTx_SetPeriodicSignal_LEFT_WHEEL_SPEED,
        App_CanTx_SetPeriodicSignal_LEFT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, right_wheel_speed_in_range_check,
        App_CanTx_SetPeriodicSignal_RIGHT_WHEEL_SPEED,
        App_CanTx_SetPeriodicSignal_RIGHT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

void App_SetPeriodicSignals_SteeringAngleInRangeCheck(
    const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    struct InRangeCheck *steering_angle_in_range_check =
        App_FsmWorld_GetSteeringAngleInRangeCheck(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, steering_angle_in_range_check,
        App_CanTx_SetPeriodicSignal_STEERING_ANGLE,
        App_CanTx_SetPeriodicSignal_STEERING_ANGLE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

void App_SetPeriodicSignals_BrakePressureInRangeCheck(
    const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    struct InRangeCheck *brake_pressure_in_range_check =
        App_FsmWorld_GetBrakePressureInRangeCheck(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, brake_pressure_in_range_check,
        App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE,
        App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

void App_SetPeriodicSignals_BrakeActuationStatus(const struct FsmWorld *world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    struct BinarySwitch *brake_actuation_status =
        App_FsmWorld_GetBrakeActuationStatus(world);

    App_SetPeriodicCanSignals_BinarySwitch(
        can_tx, brake_actuation_status,
        App_CanTx_SetPeriodicSignal_BRAKE_STATUS,
        CANMSGS_FSM_BRAKE_BRAKE_STATUS_ACTUATED_CHOICE,
        CANMSGS_FSM_BRAKE_BRAKE_STATUS_NOT_ACTUATED_CHOICE);
}
