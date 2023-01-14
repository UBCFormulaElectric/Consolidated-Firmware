#include "App_FlowMeterSignals.h"

bool App_FlowMetersSignals_IsPrimaryFlowRateBelowThreshold(struct FsmWorld *const world)
{
    struct InRangeCheck *flow_rate_in_range_check = App_FsmWorld_GetFlowRateInRangeCheck(world);

    float flow_rate;
    return App_InRangeCheck_GetValue(flow_rate_in_range_check, &flow_rate) == VALUE_UNDERFLOW;
}

bool App_FlowMetersSignals_IsFlowRateInRange(struct FsmWorld *const world)
{
    struct InRangeCheck *flow_rate_in_range_check = App_FsmWorld_GetFlowRateInRangeCheck(world);

    float flow_rate;
    return App_InRangeCheck_GetValue(flow_rate_in_range_check, &flow_rate) == VALUE_IN_RANGE;
}

void App_FlowMetersSignals_FlowRateBelowThresholdCallback(struct FsmWorld *const world)
{
    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_FLOW_METER_HAS_UNDERFLOW(
    // can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FLOW_METER_HAS_UNDERFLOW_TRUE_CHOICE);
}
