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

static TimerChannel     stability_timer;
static bool             retry_on  = false;
static bool             recovered = false;
static VCInverterFaults current_inverter_fault_state;
static uint16_t         retry_counter = 0;

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
    handle.can_invOn(false);
    handle.can_dcOn(false);
    handle.can_enable_inv(false);
    handle.error_reset(true);
    handle.can_inv_warning(true);
    return;
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
    app_canTx_VC_State_set(VC_FAULT_STATE);
    app_canAlerts_VC_Info_InverterRetry_set(true);
    app_timer_init(&stability_timer, TIMEOUT);
    current_inverter_fault_state = INV_FAULT_RETRY;
}

static void InverterFaultHandlingStateRunOnTick100Hz(void)
{
    switch (current_inverter_fault_state)
    {
        case INV_FAULT_RETRY:
        {
            bool stable_recovery = false;
            retry_counter++;
            LOG_INFO("inverter is retrying, retry number: %u", retry_counter / 2600);
            bool inv_faulted = false;
            bool any_lockout = false;

            any_lockout |=
                (is_lockout_code(inverter_handle_FL.can_error_info()) ||
                 is_lockout_code(inverter_handle_FR.can_error_info()) ||
                 is_lockout_code(inverter_handle_RL.can_error_info()) ||
                 is_lockout_code(inverter_handle_RR.can_error_info()));
            if (any_lockout)
            {
                current_inverter_fault_state = INV_FAULT_LOCKOUT;
                break;
            }

            if (inverter_handle_FL.can_error_bit())
            {
                app_timer_restart(&stability_timer);
                inverter_retry_routine(inverter_handle_FL);
                if (app_timer_getElapsedTime(&stability_timer) > 100u)
                {
                    inv_faulted = inverter_handle_FL.can_error_bit;
                    if (!inv_faulted && app_timer_updateAndGetState(&stability_timer) == TIMER_STATE_EXPIRED)
                    {
                        current_inverter_fault_state = INV_FAULT_RECOVERED;
                        break;
                    }
                }
            }

            if (inverter_handle_FR.can_error_bit())
            {
                app_timer_restart(&stability_timer);
                inverter_retry_routine(inverter_handle_FR);
                if (app_timer_getElapsedTime(&stability_timer) > 100u)
                {
                    inv_faulted = inverter_handle_FR.can_error_bit;
                    if (!inv_faulted && app_timer_updateAndGetState(&stability_timer) == TIMER_STATE_EXPIRED)
                    {
                        current_inverter_fault_state = INV_FAULT_RECOVERED;
                        break;
                    }
                }
            }

            if (inverter_handle_RL.can_error_bit())
            {
                app_timer_restart(&stability_timer);
                inverter_retry_routine(inverter_handle_RL);
                if (app_timer_getElapsedTime(&stability_timer) > 100u)
                {
                    inv_faulted = inverter_handle_RL.can_error_bit;
                    if (!inv_faulted && app_timer_updateAndGetState(&stability_timer) == TIMER_STATE_EXPIRED)
                    {
                        current_inverter_fault_state = INV_FAULT_RECOVERED;
                        break;
                    }
                }
            }

            if (inverter_handle_RR.can_error_bit())
            {
                app_timer_restart(&stability_timer);
                inverter_retry_routine(inverter_handle_RR);
                if (app_timer_getElapsedTime(&stability_timer) > 100u)
                {
                    inv_faulted = inverter_handle_RR.can_error_bit;
                    if (!inv_faulted && app_timer_updateAndGetState(&stability_timer) == TIMER_STATE_EXPIRED)
                    {
                        current_inverter_fault_state = INV_FAULT_RECOVERED;
                        break;
                    }
                }
            }
            break;
        }

        case INV_FAULT_LOCKOUT:
        {
            // Do nothing here no retry by design; wait for manual action or power cycle also toggling off retry since
            // it doesn't make sense to retry here
            LOG_INFO("inverter is locked out need to power cycle");
            app_canAlerts_VC_Info_InverterRetry_set(false);
            return;
        }

        case INV_FAULT_RECOVERED:
        {
            LOG_INFO("fault recovered on retry number: %u", retry_counter);
            app_canAlerts_VC_Info_InverterRetry_set(false);

            // jumping back to Hvinit instead of first state DC is alrady on
            app_stateMachine_setNextState(&hvInit_state);
            break;
        }
        default:
            break;
    }
}

static void InverterfaultHandlingStateRunOnExit(void)
{
    // We are setting back this to zero meaning that we either succedded in reseting the inverters or out reset protocl
    // didnt work so we are going back to init
    app_canAlerts_VC_Info_InverterRetry_set(false);
    app_canTx_VC_INVFLbErrorReset_set(false);
    app_canTx_VC_INVFRbErrorReset_set(false);
    app_canTx_VC_INVRLbErrorReset_set(false);
    app_canTx_VC_INVRRbErrorReset_set(false);
    app_timer_stop(&stability_timer);
}

State inverter_retry_state = { .name              = "Handling State",
                               .run_on_entry      = InverterFaultHandlingStateRunOnEntry,
                               .run_on_tick_100Hz = InverterFaultHandlingStateRunOnTick100Hz,
                               .run_on_exit       = InverterfaultHandlingStateRunOnExit };
