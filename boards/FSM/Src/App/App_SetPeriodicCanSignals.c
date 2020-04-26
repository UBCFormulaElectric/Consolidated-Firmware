#include "App_SetPeriodicCanSignals.h"
#include "App_CanTx.h"
#include "App_FlowMeter.h"

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
