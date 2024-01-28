#include "app_coolant.h"
#include <stdlib.h>
#include <assert.h>
#include "app_rangeCheck.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
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
    App_CanTx_FSM_CoolantTemperatureA_Set(io_coolant_getTemperatureA());
    App_CanTx_FSM_CoolantTemperatureB_Set(io_coolant_getTemperatureB());
    App_CanTx_FSM_CoolantPressureA_Set(io_coolant_getPressureA());
    App_CanTx_FSM_CoolantPressureB_Set(io_coolant_getPressureB());

    float            coolant_flow;
    RangeCheckStatus coolant_status = app_rangeCheck_getValue(&flow_rate_in_range_check, &coolant_flow);
    App_CanTx_FSM_CoolantFlowRate_Set(coolant_flow);
    App_CanAlerts_FSM_Warning_FlowRateOutOfRange_Set(coolant_status != VALUE_IN_RANGE);

    // motor shutdown in flow rate check
    const bool  in_drive_state             = App_CanRx_DCM_State_Get() == DCM_DRIVE_STATE;
    SignalState flow_in_range_signal_state = app_signal_getState(
        &flow_in_range_signal, coolant_status == VALUE_UNDERFLOW && in_drive_state,
        coolant_status == VALUE_IN_RANGE || !in_drive_state);
    App_CanAlerts_FSM_Fault_FlowMeterUnderflow_Set(flow_in_range_signal_state == SIGNAL_STATE_ACTIVE);
}
