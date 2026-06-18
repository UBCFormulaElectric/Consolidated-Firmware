#include "app_inverter.hpp"
#include "app_timer.h"
#include "states/app_states.hpp"
#include "app_timer.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "torque_vectoring/datatypes/torque_limits.hpp"
#include "io_log.hpp"

static app::State       *state_to_recover_after_fault;
static app::Timer        retry_timer{ 1000u };
static constexpr uint8_t RETRY_LIMIT = 3;
static uint8_t           retry_count = 0;

constexpr app::inverter::Handle inverter_handle_FL{
    app::can_tx::VC_INVFLbEnable_set,
    app::can_tx::VC_INVFLbInverterOn_set,
    app::can_tx::VC_INVFLbDcOn_set,
    app::can_tx::VC_INVFLTorqueSetpoint_set,
    app::can_tx::VC_INVFLTorqueLimitNegative_set,
    app::can_tx::VC_INVFLTorqueLimitPositive_set,
    app::can_rx::INVFL_ErrorInfo_get,
    app::can_tx::VC_INVFLbErrorReset_set,
    app::can_rx::INVFL_bError_get,
    app::can_alerts::warnings::FrontRightInverterFault_set,
};

constexpr app::inverter::Handle inverter_handle_FR{
    app::can_tx::VC_INVFRbEnable_set,
    app::can_tx::VC_INVFRbInverterOn_set,
    app::can_tx::VC_INVFRbDcOn_set,
    app::can_tx::VC_INVFRTorqueSetpoint_set,
    app::can_tx::VC_INVFRTorqueLimitNegative_set,
    app::can_tx::VC_INVFRTorqueLimitPositive_set,
    app::can_rx::INVFR_ErrorInfo_get,
    app::can_tx::VC_INVFRbErrorReset_set,
    app::can_rx::INVFR_bError_get,
    app::can_alerts::warnings::FrontRightInverterFault_set,
};
constexpr app::inverter::Handle inverter_handle_RL{
    app::can_tx::VC_INVRLbEnable_set,
    app::can_tx::VC_INVRLbInverterOn_set,
    app::can_tx::VC_INVRLbDcOn_set,
    app::can_tx::VC_INVRLTorqueSetpoint_set,
    app::can_tx::VC_INVRLTorqueLimitNegative_set,
    app::can_tx::VC_INVRLTorqueLimitPositive_set,
    app::can_rx::INVRL_ErrorInfo_get,
    app::can_tx::VC_INVRLbErrorReset_set,
    app::can_rx::INVRL_bError_get,
    app::can_alerts::warnings::RearLeftInverterFault_set,
};

constexpr app::inverter::Handle inverter_handle_RR{
    app::can_tx::VC_INVRRbEnable_set,
    app::can_tx::VC_INVRRbInverterOn_set,
    app::can_tx::VC_INVRRbDcOn_set,
    app::can_tx::VC_INVRRTorqueSetpoint_set,
    app::can_tx::VC_INVRRTorqueLimitNegative_set,
    app::can_tx::VC_INVRRTorqueLimitPositive_set,
    app::can_rx::INVRR_ErrorInfo_get,
    app::can_tx::VC_INVRRbErrorReset_set,
    app::can_rx::INVRR_bError_get,
    app::can_alerts::warnings::RearRightInverterFault_set,
};

void app::inverter::inverter_enable_toggle(const bool fl, const bool fr, const bool rl, const bool rr)
{
    inverter_handle_FL.can_enable_inv(fl);
    inverter_handle_FR.can_enable_inv(fr);
    inverter_handle_RL.can_enable_inv(rl);
    inverter_handle_RR.can_enable_inv(rr);
}

void app::inverter::set_torque_limit_negative(
    const float fl_Nm,
    const float fr_Nm,
    const float rl_Nm,
    const float rr_Nm)
{
    using tv::datatypes::torque_limits::TORQUE_REQUEST;

    inverter_handle_FL.can_torque_limit_negative(TORQUE_REQUEST(fl_Nm));
    inverter_handle_FR.can_torque_limit_negative(TORQUE_REQUEST(fr_Nm));
    inverter_handle_RL.can_torque_limit_negative(TORQUE_REQUEST(rl_Nm));
    inverter_handle_RR.can_torque_limit_negative(TORQUE_REQUEST(rr_Nm));
}

void app::inverter::set_torque_limit_positive(
    const float fl_Nm,
    const float fr_Nm,
    const float rl_Nm,
    const float rr_Nm)
{
    using tv::datatypes::torque_limits::TORQUE_REQUEST;

    inverter_handle_FL.can_torque_limit_positive(TORQUE_REQUEST(fl_Nm));
    inverter_handle_FR.can_torque_limit_positive(TORQUE_REQUEST(fr_Nm));
    inverter_handle_RL.can_torque_limit_positive(TORQUE_REQUEST(rl_Nm));
    inverter_handle_RR.can_torque_limit_positive(TORQUE_REQUEST(rr_Nm));
}

void app::inverter::send_torque(const float fl_Nm, const float fr_Nm, const float rl_Nm, const float rr_Nm)
{
    using tv::datatypes::torque_limits::TORQUE_REQUEST;

    inverter_handle_FL.can_torque_setpoint(TORQUE_REQUEST(fl_Nm));
    inverter_handle_FR.can_torque_setpoint(TORQUE_REQUEST(fr_Nm));
    inverter_handle_RL.can_torque_setpoint(TORQUE_REQUEST(rl_Nm));
    inverter_handle_RR.can_torque_setpoint(TORQUE_REQUEST(rr_Nm));
}

static bool inverter_status()
{
    const bool invrr_error = inverter_handle_RR.can_error_bit() == true;
    const bool invrl_error = inverter_handle_RL.can_error_bit() == true;
    const bool invfl_error = inverter_handle_FL.can_error_bit() == true;
    const bool invfr_error = inverter_handle_FR.can_error_bit() == true;

    return invfl_error || invrl_error || invrr_error || invfr_error;
}

static bool is_lockout_code(const uint32_t code)
{
    switch (code)
    {
        case 259u:  // Deallocation of MNU out of function
        case 1342u: // System run-up aborted
        case 2311u: // Controller enable (RF) is withdrawn internally (Encoder error)
        case 3871u: // Communication error with the supply
        case 1100u: // Short-circuit / overload digital outputs
            return true;
        default:
            return false;
    }
}

/*routine for resetting errors from the AMK datasheet*/
static void inverter_start_retry_routine(const app::inverter::Handle &handle)
{
    // Start retry cycle if faulted
    if (handle.can_error_bit())
    {
        handle.can_invOn(false);
        handle.can_enable_inv(false);
        handle.can_inv_warning(true);
        handle.error_reset(true);
    }
}

static void inverter_stop_retry_routine(const app::inverter::Handle &handle)
{
    // Start retry cycle if faulted
    if (handle.can_error_bit())
    {
        handle.can_invOn(true);
        handle.can_enable_inv(true);
        handle.can_inv_warning(false);
        handle.error_reset(false);
    }
}

static bool lockout()
{
    // Lockout check, if ANY lockout, stop retrying and remain faulted.
    // This is the most safety critical check so it has highest priority
    const bool any_lockout =
        is_lockout_code(inverter_handle_FL.can_error_info()) || is_lockout_code(inverter_handle_FR.can_error_info()) ||
        is_lockout_code(inverter_handle_RL.can_error_info()) || is_lockout_code(inverter_handle_RR.can_error_info());

    if (any_lockout)
    {
        inverter_handle_FL.error_reset(false);
        inverter_handle_FR.error_reset(false);
        inverter_handle_RL.error_reset(false);
        inverter_handle_RR.error_reset(false);

        LOG_INFO("retry -> lockout detected; holding in fault (no retries)");

        return true;
    }

    return false;
}

app::inverter::FaultHandlerState app::inverter::FaultHandler()
{
    // If all inverters are clear, we’re done
    // Second priority because we want to recover as quickly as possible

    if (lockout())
    {
        return INV_FAULT_LOCKOUT;
    }

    const bool fl_fault = inverter_handle_FL.can_error_bit();
    const bool fr_fault = inverter_handle_FR.can_error_bit();
    const bool rl_fault = inverter_handle_RL.can_error_bit();
    const bool rr_fault = inverter_handle_RR.can_error_bit();

    const bool inverter_fault = fl_fault || fr_fault || rl_fault || rr_fault;

    if (!inverter_fault)
    {
        LOG_INFO("retry -> all inverter errors cleared");
        retry_count = 0u;
        inverter_stop_retry_routine(inverter_handle_FL);
        inverter_stop_retry_routine(inverter_handle_FR);
        inverter_stop_retry_routine(inverter_handle_RL);
        inverter_stop_retry_routine(inverter_handle_RR);
        return INV_FAULT_RECOVERED;
    }
    // Now start a new retry cycle is fault persist after TIMEOUT = 1000 ms of retrying
    const Timer::TimerState state = retry_timer.runIfCondition(!inverter_fault);

    if (fl_fault)
        inverter_start_retry_routine(inverter_handle_FL);
    if (fr_fault)
        inverter_start_retry_routine(inverter_handle_FR);
    if (rl_fault)
        inverter_start_retry_routine(inverter_handle_RL);
    if (rr_fault)
        inverter_start_retry_routine(inverter_handle_RR);

    if (state == Timer::TimerState::EXPIRED)
    {
        // First end the retry cycle so we can reset it again
        inverter_handle_FL.error_reset(false);
        inverter_handle_FR.error_reset(false);
        inverter_handle_RL.error_reset(false);
        inverter_handle_RR.error_reset(false);
        if (retry_count == RETRY_LIMIT)
        {
            return INV_FAULT_LOCKOUT;
        }
        retry_count++;
    }

    return INV_FAULT_RETRY;
}

void app::inverter::FaultCheck()
{
    const State *curr = StateMachine::get_current_state();

    if (!lockout())
    {
        can_tx::VC_Fault_InvLockoutFault_set(false);
    }

    if (!inverter_status() || curr == &states::inverter_fault_handling_state)
    {
        return;
    }

    // We do not want to go back to HV or Drive while we haven't passed HV_INIT again
    if (curr == &states::hv_state || curr == &states::drive_state)
    {
        state_to_recover_after_fault = &states::hvInit_state;
    }
    else
    {
        state_to_recover_after_fault = const_cast<State *>(curr);
    }

    // LOG_INFO("inverter state in appinv %s", state_to_recover_after_fault->name);
    StateMachine::set_next_state(&states::inverter_fault_handling_state);
}

app::State *app::inverter::recovery_state()
{
    return state_to_recover_after_fault;
}
