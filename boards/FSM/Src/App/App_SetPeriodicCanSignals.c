#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SetPeriodicCanSignals.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)

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
        CANMSGS_FSM_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, secondary_flow_rate_in_range_check,
        App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_RATE,
        App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
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
        CANMSGS_FSM_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, right_wheel_speed_in_range_check,
        App_CanTx_SetPeriodicSignal_RIGHT_WHEEL_SPEED,
        App_CanTx_SetPeriodicSignal_RIGHT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
