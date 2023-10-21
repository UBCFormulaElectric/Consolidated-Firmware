#include "app_coolant.h"
#include <stdlib.h>
#include <assert.h>
#include "App_InRangeCheck.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_SharedSignal.h"
#include "io_coolant.h"

static struct InRangeCheck *flow_rate_in_range_check;
static struct Signal *      flow_in_range_signal;

void app_coolant_init(void)
{
    flow_rate_in_range_check =
        App_InRangeCheck_Create(io_coolant_getFlowRate, MIN_FLOW_RATE_L_PER_MIN, MAX_FLOW_RATE_L_PER_MIN);
    flow_in_range_signal = App_SharedSignal_Create(FLOW_METER_TIME_TO_FAULT, FLOW_METER_TIME_TO_CLEAR);
}

void app_coolant_destroy(void)
{
    App_InRangeCheck_Destroy(flow_rate_in_range_check);
    App_SharedSignal_Destroy(flow_in_range_signal);
}

void app_coolant_broadcast(void)
{
    App_CanTx_FSM_Coolant_TemperatureA_Set(io_coolant_getTemperatureA());
    App_CanTx_FSM_Coolant_TemperatureB_Set(io_coolant_getTemperatureB());
    App_CanTx_FSM_Coolant_PressureA_Set(io_coolant_getPressureA());
    App_CanTx_FSM_Coolant_PressureB_Set(io_coolant_getPressureB());

    float                    coolant_flow_clamped;
    enum InRangeCheck_Status coolant_status =
        App_InRangeCheck_GetValue(flow_rate_in_range_check, &coolant_flow_clamped);
    App_CanTx_FSM_Coolant_FlowRate_Set(coolant_flow_clamped);
    App_CanAlerts_SetWarning(FSM_WARNING_FLOW_RATE_OUT_OF_RANGE, coolant_status != VALUE_IN_RANGE);

    // motor shutdown in flow rate check
    float                    flow_rate;
    enum InRangeCheck_Status flow_rate_inRangeCheck_status =
        App_InRangeCheck_GetValue(flow_rate_in_range_check, &flow_rate);
    const bool  in_drive_state             = App_CanRx_DCM_Vitals_CurrentState_Get() == DCM_DRIVE_STATE;
    SignalState flow_in_range_signal_state = App_SharedSignal_Update(
        flow_in_range_signal, flow_rate_inRangeCheck_status == VALUE_UNDERFLOW && in_drive_state,
        flow_rate_inRangeCheck_status == VALUE_IN_RANGE || !in_drive_state);
    App_CanAlerts_SetFault(FSM_FAULT_FLOW_METER_HAS_UNDERFLOW, flow_in_range_signal_state == SIGNAL_STATE_ACTIVE);
}
