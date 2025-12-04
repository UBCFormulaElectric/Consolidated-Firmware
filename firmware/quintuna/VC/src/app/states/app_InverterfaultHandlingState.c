#include "app_states.h"
#include "app_powerManager.h"
#include "app_timer.h"
#include "app_inverter.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_log.h"

// Retry window time
#define TIMEOUT 1000u

static VCInverterFaults current_inverter_fault_state;
static TimerChannel     retry_timer;
static bool             retry_cycle_active = false;
static const State     *state_recovering_to;

/*routine for resetting errors from the AMK datasheet*/
static void inverter_start_retry_routine(InverterHandle handle)
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

/*turns the retry cycle off*/
static void end_retry_cycle(InverterHandle handle)
{
    handle.error_reset(false);
}

/*Lockout error just means don't retry you need to power cycle
add any new exceptions error codes here if you don't want retry*/
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

static void InverterFaultHandlingStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_INV_FAUTLED);
    app_canAlerts_VC_Info_InverterRetry_set(true);
    app_timer_init(&retry_timer, TIMEOUT);
    retry_cycle_active           = false;
    current_inverter_fault_state = INV_FAULT_RETRY;
    state_recovering_to          = state_to_recover_after_fault;
}
static void InverterFaultHandlingStateRunOnTick100Hz(void)
{
    switch (current_inverter_fault_state)
    {
        case INV_FAULT_RETRY:
        {
            // Lockout check, if ANY lockout, stop retrying and remain faulted
            const bool any_lockout = is_lockout_code(inverter_handle_FL.can_error_info()) ||
                                     is_lockout_code(inverter_handle_FR.can_error_info()) ||
                                     is_lockout_code(inverter_handle_RL.can_error_info()) ||
                                     is_lockout_code(inverter_handle_RR.can_error_info());

            if (any_lockout)
            {
                end_retry_cycle(inverter_handle_FL);
                end_retry_cycle(inverter_handle_FR);
                end_retry_cycle(inverter_handle_RL);
                end_retry_cycle(inverter_handle_RR);
                retry_cycle_active = false;

                LOG_INFO("retry -> lockout detected; holding in fault (no retries)");
                current_inverter_fault_state = INV_FAULT_LOCKOUT;
                break;
            }

            // If all inverters are clear, we’re done
            const bool fl_fault = inverter_handle_FL.can_error_bit();
            const bool fr_fault = inverter_handle_FR.can_error_bit();
            const bool rl_fault = inverter_handle_RL.can_error_bit();
            const bool rr_fault = inverter_handle_RR.can_error_bit();

            if (!fl_fault && !fr_fault && !rl_fault && !rr_fault)
            {
                LOG_INFO("retry -> all inverter errors cleared");
                current_inverter_fault_state = INV_FAULT_RECOVERED;
                break;
            }

            // Start a new retry cycle is fault persist after 1000 ms of retrying
            if (!retry_cycle_active)
            {
                if (fl_fault)
                    inverter_start_retry_routine(inverter_handle_FL);
                if (fr_fault)
                    inverter_start_retry_routine(inverter_handle_FR);
                if (rl_fault)
                    inverter_start_retry_routine(inverter_handle_RL);
                if (rr_fault)
                    inverter_start_retry_routine(inverter_handle_RR);

                app_timer_restart(&retry_timer);
                retry_cycle_active = true;
                break;
            }

            // timing of a cycle
            const TimerState timer_state = app_timer_updateAndGetState(&retry_timer);

            if (retry_cycle_active && timer_state == TIMER_STATE_EXPIRED)
            {
                // First end the retry cycle
                end_retry_cycle(inverter_handle_FL);
                end_retry_cycle(inverter_handle_FR);
                end_retry_cycle(inverter_handle_RL);
                end_retry_cycle(inverter_handle_RR);

                // Check the error bits
                const bool fl_now = inverter_handle_FL.can_error_bit();
                const bool fr_now = inverter_handle_FR.can_error_bit();
                const bool rl_now = inverter_handle_RL.can_error_bit();
                const bool rr_now = inverter_handle_RR.can_error_bit();

                // If there are faults wait to
                if (!fl_now && !fr_now && !rl_now && !rr_now)
                {
                    LOG_INFO("retry -> recovered after pulse window");
                    current_inverter_fault_state = INV_FAULT_RECOVERED;
                }
                else
                {
                    // Some inverter still faulted keep retrying in next cycle
                    retry_cycle_active = false;
                }
            }
            break;
        }

        case INV_FAULT_LOCKOUT:
        {
            // No retries needs hw reset
            app_canAlerts_VC_Info_InverterRetry_set(false);
            // Ensuring we are in a hard fault state so the BMS contactors open
            app_canTx_VC_State_set(VC_FAULT_STATE);
            return;
        }

        case INV_FAULT_RECOVERED:
        {
            // Clear retry indicator and back to the state it came from
            app_canAlerts_VC_Info_InverterRetry_set(false);
            app_stateMachine_setNextState(state_recovering_to);
            break;
        }

        default:
            break;
    }
}

static void InverterfaultHandlingStateRunOnExit(void)
{
    app_canAlerts_VC_Info_InverterRetry_set(false);
    app_canTx_VC_INVFLbErrorReset_set(false);
    app_canTx_VC_INVFRbErrorReset_set(false);
    app_canTx_VC_INVRLbErrorReset_set(false);
    app_canTx_VC_INVRRbErrorReset_set(false);
    app_timer_stop(&retry_timer);
    retry_cycle_active = false;
}

State inverter_fault_handling_state = { .name              = "Retry State",
                                        .run_on_entry      = InverterFaultHandlingStateRunOnEntry,
                                        .run_on_tick_100Hz = InverterFaultHandlingStateRunOnTick100Hz,
                                        .run_on_exit       = InverterfaultHandlingStateRunOnExit };
