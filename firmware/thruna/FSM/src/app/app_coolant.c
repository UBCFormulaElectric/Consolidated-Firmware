#include "app_coolant.h"
#include <stdlib.h>
#include <assert.h>
#include "app_rangeCheck.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_signal.h"
#include "io_coolant.h"

static const RangeCheck flow_rate_in_range_check = {
    .get_value = io_coolant_getFlowRate,
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
    app_canTx_FSM_CoolantTemperatureA_set(io_coolant_getTemperatureA());
    app_canTx_FSM_CoolantTemperatureB_set(io_coolant_getTemperatureB());
    app_canTx_FSM_CoolantPressureA_set(io_coolant_getPressureA());
    app_canTx_FSM_CoolantPressureB_set(io_coolant_getPressureB());

    float            coolant_flow;
    RangeCheckStatus coolant_status = app_rangeCheck_getValue(&flow_rate_in_range_check, &coolant_flow);
    app_canTx_FSM_CoolantFlowRate_set(coolant_flow);
    // app_canAlerts_FSM_Warning_FlowRateOutOfRange_set(coolant_status != VALUE_IN_RANGE);
    app_canAlerts_FSM_Warning_FlowRateOutOfRange_set(true);

    // motor shutdown in flow rate check
    const bool  in_drive_state             = app_canRx_DCM_State_get() == DCM_DRIVE_STATE;
    SignalState flow_in_range_signal_state = app_signal_getState(
        &flow_in_range_signal, coolant_status == VALUE_UNDERFLOW && in_drive_state,
        coolant_status == VALUE_IN_RANGE || !in_drive_state);
    app_canAlerts_FSM_Fault_FlowMeterUnderflow_set(flow_in_range_signal_state == SIGNAL_STATE_ACTIVE);
}
