#include "app_states.h"
#include "app_powerManager.h"
#include "app_timer.h"
#include "app_warningHandling.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_log.h"

#define TIMEOUT 300u

static VCInverterFaults current_inverter_fault_state;
static TimerChannel     retry_timer;
static bool             pulse_high   = false; // are we currently holding ErrorReset=1?
static bool             cycle_active = false; // are we in a retry cycle window?

static const PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 } }
};

/*routine for resetting errors from the AMK datasheet*/
static void inverter_retry_routine(InverterWarningHandling handle)
{
    // start pulse if faulted
    if (handle.can_error_bit())
    {
        handle.can_invOn(false);
        handle.can_enable_inv(false);
        handle.error_reset(true);
        handle.can_inv_warning(true);
    }
}
static void end_retry_pulse(InverterWarningHandling handle)
{
    // drop reset unconditionally; harmless if it was low/falling edge
    handle.error_reset(false);
}

/*Lockout error just means don't retry you need to power cycle
add any new exceptions error codes here if you don't want retry*/
static bool is_lockout_code(uint32_t code)
{
    switch (code)
    {
        case 259u:
        case 1342u:
        case 2311u:
            return true;
        default:
            return false;
    }
}

static void InverterFaultHandlingStateRunOnEntry(void)
{
    app_powerManager_updateConfig(power_manager_state);
    app_canTx_VC_State_set(VC_INV_FAUTLED);
    app_canAlerts_VC_Info_InverterRetry_set(true);
    app_timer_init(&retry_timer, TIMEOUT);
    pulse_high                   = false;
    cycle_active                 = false;
    current_inverter_fault_state = INV_FAULT_RETRY;
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
                end_retry_pulse(inverter_handle_FL);
                end_retry_pulse(inverter_handle_FR);
                end_retry_pulse(inverter_handle_RL);
                end_retry_pulse(inverter_handle_RR);
                pulse_high   = false;
                cycle_active = false;

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

            // Start a new retry cycle (pulse ErrorReset=1) if none is active
            if (!cycle_active)
            {
                // Preconditions for AMK remove-error: Enable=0, InverterOn=0, setpoints=0 (you already zero torque)
                if (fl_fault)
                    inverter_retry_routine(inverter_handle_FL); // sets invOn=0, enable=0, error_reset=1
                if (fr_fault)
                    inverter_retry_routine(inverter_handle_FR);
                if (rl_fault)
                    inverter_retry_routine(inverter_handle_RL);
                if (rr_fault)
                    inverter_retry_routine(inverter_handle_RR);

                app_timer_restart(&retry_timer);
                pulse_high   = true; // holding ErrorReset=1 now
                cycle_active = true; // this attempt window is active
                break;               // give the pulse a tick to go out on CAN
            }

            // timing of a cycle
            const uint32_t dt = app_timer_getElapsedTime(&retry_timer);

            // After ~100 ms, drop ErrorReset=0 for all
            if (pulse_high && dt > 100u)
            {
                end_retry_pulse(inverter_handle_FL);
                end_retry_pulse(inverter_handle_FR);
                end_retry_pulse(inverter_handle_RL);
                end_retry_pulse(inverter_handle_RR);
                pulse_high = false;
            }

            // After TIMEOUT window check for recovery and retry if needed
            if (dt > TIMEOUT)
            {
                const bool fl_now = inverter_handle_FL.can_error_bit();
                const bool fr_now = inverter_handle_FR.can_error_bit();
                const bool rl_now = inverter_handle_RL.can_error_bit();
                const bool rr_now = inverter_handle_RR.can_error_bit();

                if (!fl_now && !fr_now && !rl_now && !rr_now)
                {
                    LOG_INFO("retry -> recovered after pulse window");
                    current_inverter_fault_state = INV_FAULT_RECOVERED;
                }
                else
                {
                    // Some inverter still faulted keep retrying
                    cycle_active = false;
                }
            }
            break;
        }

        case INV_FAULT_LOCKOUT:
        {
            // No retries needs hw reset
            app_canAlerts_VC_Info_InverterRetry_set(false);
            return;
        }

        case INV_FAULT_RECOVERED:
        {
            // Clear retry indicator and back to hvInit
            app_canAlerts_VC_Info_InverterRetry_set(false);
            app_stateMachine_setNextState(&hvInit_state);
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
    pulse_high   = false;
    cycle_active = false;
}

State inverter_fault_handling_state = { .name              = "Retry State",
                               .run_on_entry      = InverterFaultHandlingStateRunOnEntry,
                               .run_on_tick_100Hz = InverterFaultHandlingStateRunOnTick100Hz,
                               .run_on_exit       = InverterfaultHandlingStateRunOnExit };
