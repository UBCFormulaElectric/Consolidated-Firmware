#include "app_inverter.h"
#include "app_signal.h"
#include "app_canAlerts.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "states/app_states.h"
#include "io_log.h"

const State *state_to_recover_after_fault;
bool state_before_fault_locked = false; // internal flag so the variable can be mutable until we actually have a faults

const InverterHandle inverter_handle_FL = {
    .can_enable_inv  = app_canTx_VC_INVFLbEnable_set,
    .can_invOn       = app_canTx_VC_INVFLbInverterOn_set,
    .can_dcOn        = app_canTx_VC_INVFLbDcOn_set,
    .can_error_info  = app_canRx_INVFL_ErrorInfo_get,
    .error_reset     = app_canTx_VC_INVFLbErrorReset_set,
    .can_error_bit   = app_canRx_INVFL_bError_get,
    .can_inv_warning = app_canAlerts_VC_Warning_FrontLeftInverterFault_set,
};
const InverterHandle inverter_handle_FR = {
    .can_enable_inv  = app_canTx_VC_INVFRbEnable_set,
    .can_invOn       = app_canTx_VC_INVFRbInverterOn_set,
    .can_dcOn        = app_canTx_VC_INVFRbDcOn_set,
    .can_error_info  = app_canRx_INVFR_ErrorInfo_get,
    .error_reset     = app_canTx_VC_INVFRbErrorReset_set,
    .can_error_bit   = app_canRx_INVFR_bError_get,
    .can_inv_warning = app_canAlerts_VC_Warning_FrontRightInverterFault_set,
};
const InverterHandle inverter_handle_RL = {
    .can_enable_inv  = app_canTx_VC_INVRLbEnable_set,
    .can_invOn       = app_canTx_VC_INVRLbInverterOn_set,
    .can_dcOn        = app_canTx_VC_INVRLbDcOn_set,
    .can_error_info  = app_canRx_INVRL_ErrorInfo_get,
    .error_reset     = app_canTx_VC_INVRLbErrorReset_set,
    .can_error_bit   = app_canRx_INVRL_bError_get,
    .can_inv_warning = app_canAlerts_VC_Warning_RearLeftInverterFault_set,
};
const InverterHandle inverter_handle_RR = {
    .can_enable_inv  = app_canTx_VC_INVRRbEnable_set,
    .can_invOn       = app_canTx_VC_INVRRbInverterOn_set,
    .can_dcOn        = app_canTx_VC_INVRRbDcOn_set,
    .can_error_info  = app_canRx_INVRR_ErrorInfo_get,
    .error_reset     = app_canTx_VC_INVRRbErrorReset_set,
    .can_error_bit   = app_canRx_INVRR_bError_get,
    .can_inv_warning = app_canAlerts_VC_Warning_RearRightInverterFault_set,
};

bool app_inverter_inverterStatus(void)
{
    const bool invrr_error = app_canRx_INVRR_bError_get() == true;
    const bool invrl_error = app_canRx_INVRL_bError_get() == true;
    const bool invfl_error = app_canRx_INVFL_bError_get() == true;
    const bool invfr_error = app_canRx_INVFR_bError_get() == true;

    app_canAlerts_VC_Warning_RearLeftInverterFault_set(invrl_error);
    app_canAlerts_VC_Warning_FrontRightInverterFault_set(invfr_error);
    app_canAlerts_VC_Warning_FrontLeftInverterFault_set(invfl_error);
    app_canAlerts_VC_Warning_RearRightInverterFault_set(invrr_error);

    return invfl_error || invrl_error || invrr_error || invfr_error;
}

void app_stateMachine_inverterFaultHandling(void)
{
    // No fault dont do anything and keep resetting the lock to false
    if (!app_inverter_inverterStatus())
    {
        state_before_fault_locked = false;
        return;
    }

    // First time we see a fault while in some other state
    const State *curr = app_stateMachine_getCurrentState();

    // We do not want to go back to HV or Drive while we haven't passed HV_INIT again
    if (curr == &hv_state || curr == &drive_state)
    {
        state_to_recover_after_fault = &hvInit_state;
    }
    else
    {
        state_to_recover_after_fault = curr;
    }

    // We have a fault so we can lock in the  state we want to recover to
    state_before_fault_locked = true;

    LOG_INFO("inverter state in appinv %s", state_to_recover_after_fault->name);
    app_stateMachine_setNextState(&inverter_fault_handling_state);
}
