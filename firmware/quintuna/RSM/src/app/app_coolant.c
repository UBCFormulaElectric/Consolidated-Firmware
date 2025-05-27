#include "app_coolant.h"
#include "app_rangeCheck.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "app_signal.h"
#include "io_coolants.h"

static const RangeCheck flow_rate_in_range_check = {
    .min_value = MIN_FLOW_RATE_L_PER_MIN,
    .max_value = MAX_FLOW_RATE_L_PER_MIN,
};
static Signal flow_in_range_signal;

void app_coolant_init(void)
{
    app_signal_init(&flow_in_range_signal, FLOW_METER_TIME_TO_FAULT, FLOW_METER_TIME_TO_CLEAR);
}

void app_coolant_broadcast(void)
{
    const float                    flow_val       = io_coolant_getFlowRate();
    const RangeCheckStatusMetaData coolant_status = app_rangeCheck_getValue(&flow_rate_in_range_check, flow_val);
    app_canTx_RSM_CoolantFlowRate_set(flow_val);
    app_canAlerts_RSM_Info_FlowRateOutOfRange_set(coolant_status.status != VALUE_IN_RANGE);

    // motor shutdown in flow rate check
    // const bool  in_drive_state             = app_canRx_VC_State_get() == VC_DRIVE_STATE;
    // SignalState flow_in_range_signal_state = app_signal_getState(
    //  &flow_in_range_signal, coolant_status.status == VALUE_UNDERFLOW && in_drive_state,
    // coolant_status.status == VALUE_IN_RANGE || !in_drive_state);

    // TODO: check if ts work, apparently it didn't work last year
    // app_canAlerts_RSM_Warning_FlowMeterUnderflow_set(flow_in_range_signal_state == SIGNAL_STATE_ACTIVE);
}
