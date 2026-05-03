#include "app_inverter.hpp"
#include "app_timer.h"
#include "states/app_states.hpp"
#include "app_timer.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "io_log.hpp"

static app::State *state_to_recover_after_fault;
static app::Timer retry_timer{1000u};
static constexpr uint8_t RETRY_LIMIT = 3;
static uint8_t retry_count = 0;


constexpr app::inverter::Handle inverter_handle_FL{
    app::can_tx::VC_INVFLbEnable_set,
    app::can_tx::VC_INVFLbInverterOn_set,
    app::can_tx::VC_INVFLbDcOn_set,
    app::can_rx::INVFL_ErrorInfo_get,
    app::can_tx::VC_INVFLbErrorReset_set,
    app::can_rx::INVFL_bError_get,
    app::can_alerts::warnings::FrontRightInverterFault_set,
};

constexpr app::inverter::Handle inverter_handle_FR{
    app::can_tx::VC_INVFRbEnable_set,
    app::can_tx::VC_INVFRbInverterOn_set,
    app::can_tx::VC_INVFRbDcOn_set,
    app::can_rx::INVFR_ErrorInfo_get,
    app::can_tx::VC_INVFRbErrorReset_set,
    app::can_rx::INVFR_bError_get, 
    app::can_alerts::warnings::FrontRightInverterFault_set,
};
constexpr app::inverter::Handle inverter_handle_RL{
    app::can_tx::VC_INVRLbEnable_set,
    app::can_tx::VC_INVRLbInverterOn_set,
    app::can_tx::VC_INVRLbDcOn_set,
    app::can_rx::INVRL_ErrorInfo_get,
    app::can_tx::VC_INVRLbErrorReset_set,
    app::can_rx::INVRL_bError_get,
    app::can_alerts::warnings::RearLeftInverterFault_set,
};

constexpr app::inverter::Handle inverter_handle_RR{
    app::can_tx::VC_INVRRbEnable_set,
    app::can_tx::VC_INVRRbInverterOn_set,
    app::can_tx::VC_INVRRbDcOn_set,
    app::can_rx::INVRR_ErrorInfo_get,
    app::can_tx::VC_INVRRbErrorReset_set,
    app::can_rx::INVRR_bError_get, 
    app::can_alerts::warnings::RearRightInverterFault_set,
};

static bool inverter_status(void)
{
    const bool invrr_error = inverter_handle_RR.can_error_bit() == true;
    const bool invrl_error = inverter_handle_RL.can_error_bit() == true;
    const bool invfl_error = inverter_handle_FL.can_error_bit() == true;
    const bool invfr_error = inverter_handle_FR.can_error_bit() == true;

    return invfl_error || invrl_error || invrr_error || invfr_error;
}

static bool is_lockout_code(uint32_t code)
{
    switch (code)
    {
        case 259u:  // Deallocation of MNU out of function
        case 1342u: // System run-up aborted
        case 2311u: // Controller enable (RF) is withdrawn internally (Encoder error)
        case 3871u: // Communication error with the supply
        case 1100u: //  Short-circuit / overload digital outputs
            return true;
        default:
            return false;
    }
}

/*routine for resetting errors from the AMK datasheet*/
static void inverter_start_retry_routine(app::inverter::Handle handle)
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

static bool lockout(void)
{
    // Lockout check, if ANY lockout, stop retrying and remain faulted.
    // This is the most safety critical check so it has highest priority
    const bool any_lockout = is_lockout_code(inverter_handle_FL.can_error_info()) ||
                                is_lockout_code(inverter_handle_FR.can_error_info()) ||
                                is_lockout_code(inverter_handle_RL.can_error_info()) ||
                                is_lockout_code(inverter_handle_RR.can_error_info());

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

app::inverter::FaultHandlerState app::inverter::FaultHandler(void)
{
    // If all inverters are clear, we’re done
    // Second priority because we want to recover as quickly as possible

    if (lockout())
    {
        return app::inverter::FaultHandlerState::INV_FAULT_LOCKOUT;
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
        return app::inverter::FaultHandlerState::INV_FAULT_RECOVERED;
    }

    // Now start a new retry cycle is fault persist after TIMEOUT = 1000 ms of retrying
    const app::Timer::TimerState state = retry_timer.runIfCondition(!inverter_fault);

    if (fl_fault)
        inverter_start_retry_routine(inverter_handle_FL);
    if (fr_fault)
        inverter_start_retry_routine(inverter_handle_FR);
    if (rl_fault)
        inverter_start_retry_routine(inverter_handle_RL);
    if (rr_fault)
        inverter_start_retry_routine(inverter_handle_RR);
    
    if (state == app::Timer::TimerState::EXPIRED)
    {
        // First end the retry cycle so we can reset it again
        inverter_handle_FL.error_reset(false);
        inverter_handle_FR.error_reset(false);
        inverter_handle_RL.error_reset(false);
        inverter_handle_RR.error_reset(false);
        if (retry_count == RETRY_LIMIT)
        {
            return app::inverter::FaultHandlerState::INV_FAULT_LOCKOUT;
        }
        retry_count++;
    }



    return app::inverter::FaultHandlerState::INV_FAULT_RETRY;
}


void app::inverter::FaultCheck(void)
{
    const app::State *curr = app::StateMachine::get_current_state();

    if (!inverter_status() || curr == &app::states::inverter_fault_handling_state)
    {
        return;
    }

    // We do not want to go back to HV or Drive while we haven't passed HV_INIT again
    if (curr == &app::states::hv_state || curr == &app::states::drive_state)
    {
        state_to_recover_after_fault = &app::states::hvInit_state;
    }
    else
    {
        state_to_recover_after_fault = const_cast<app::State *>(curr);
    }

    LOG_INFO("inverter state in appinv %s", state_to_recover_after_fault->name);
    app::StateMachine::set_next_state(&app::states::inverter_fault_handling_state);
}

app::State *app::inverter::recovery_state(void)
{
    return state_to_recover_after_fault;
}