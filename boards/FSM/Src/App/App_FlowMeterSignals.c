#include "App_FlowMeterSignals.h"

bool App_FlowMetersSignals_IsPrimaryFlowRateBelowThreshold(struct FsmWorld *const world)
{
    struct InRangeCheck *primary_flow_rate_in_range_check = App_FsmWorld_GetPrimaryFlowRateInRangeCheck(world);

    float primary_flow_rate;
    return App_InRangeCheck_GetValue(primary_flow_rate_in_range_check, &primary_flow_rate) == VALUE_UNDERFLOW;
}

bool App_FlowMetersSignals_IsPrimaryFlowRateInRange(struct FsmWorld *const world)
{
    struct InRangeCheck *primary_flow_rate_in_range_check = App_FsmWorld_GetPrimaryFlowRateInRangeCheck(world);

    float primary_flow_rate;
    return App_InRangeCheck_GetValue(primary_flow_rate_in_range_check, &primary_flow_rate) == VALUE_IN_RANGE;
}

bool App_FlowMetersSignals_IsSecondaryFlowRateBelowThreshold(struct FsmWorld *const world)
{
    struct InRangeCheck *secondary_flow_rate_in_range_check = App_FsmWorld_GetSecondaryFlowRateInRangeCheck(world);

    float secondary_flow_rate;
    return App_InRangeCheck_GetValue(secondary_flow_rate_in_range_check, &secondary_flow_rate) == VALUE_UNDERFLOW;
}

bool App_FlowMetersSignals_IsSecondaryFlowRateInRange(struct FsmWorld *const world)
{
    struct InRangeCheck *primary_flow_rate_in_range_check = App_FsmWorld_GetSecondaryFlowRateInRangeCheck(world);

    float secondary_flow_rate;
    return App_InRangeCheck_GetValue(primary_flow_rate_in_range_check, &secondary_flow_rate) == VALUE_IN_RANGE;
}

void App_FlowMetersSignals_PrimaryFlowRateBelowThresholdCallback(struct FsmWorld *const world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_PRIMARY_FLOW_RATE_HAS_UNDERFLOW(
        can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PRIMARY_FLOW_RATE_HAS_UNDERFLOW_TRUE_CHOICE);
}

void App_FlowMetersSignals_SecondaryFlowRateBelowThresholdCallback(struct FsmWorld *const world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_RATE_HAS_UNDERFLOW(
        can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SECONDARY_FLOW_RATE_HAS_UNDERFLOW_TRUE_CHOICE);
}
