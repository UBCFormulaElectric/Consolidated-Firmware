#include "app_stateMachine.h"
#include "app_states.h"
#include "app_powerManager.h"
#include "app_timer.h"
#include "app_canAlerts.h"
#include "app_warningHandling.h"
#include "io_loadswitches.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canUtils.h"
#include <stdbool.h>
#include <stdint.h>
#include "io_log.h"

static VCInverterFaults   current_inverter_fault_state;
static uint16_t           retry_counter       = 0;
static PowerManagerConfig power_manager_state = {
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
static inline void inverter_retry_routine(InverterConfig inverter)
{
    inverter_reset_handle[inverter].can_invOn(false);
    inverter_reset_handle[inverter].can_dcOn(false);
    inverter_reset_handle[inverter].can_enable_inv(false);
    inverter_reset_handle[inverter].error_reset(true);
    return;
}

// Only retry on the faulted inverter to speed up the recovery process
static inline void inverter_fault_retry(InverterConfig inverter)
{
    switch (inverter)
    {
        case INVERTER_FR:
            /* cast then act on FR */
            app_canTx_VC_INVFRbErrorReset_set(true);
            app_canAlerts_VC_Warning_FrontRightInverterFault_set(true);
            inverter_retry_routine(INVERTER_FR);
            break;

        case INVERTER_FL:
            app_canTx_VC_INVFLbErrorReset_set(true);
            app_canAlerts_VC_Warning_FrontLeftInverterFault_set(true);
            inverter_retry_routine(INVERTER_FL);
            break;

        case INVERTER_RR:
            app_canTx_VC_INVRRbErrorReset_set(true);
            app_canAlerts_VC_Warning_RearRightInverterFault_set(true);
            inverter_retry_routine(INVERTER_RR);
            break;

        case INVERTER_RL:
            app_canTx_VC_INVRLbErrorReset_set(true);
            app_canAlerts_VC_Warning_RearLeftInverterFault_set(true);
            inverter_retry_routine(INVERTER_RL);
            break;

        default:
            break;
    }
}

/*Lockout error just means don't retry you need to power cycle
add any new exceptions error codes here if you don't want retry*/
static inline bool is_lockout_code(uint32_t code)
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

static inline bool inv_bError(InverterConfig inv)
{
    switch (inv)
    {
        case INVERTER_FL:
            return app_canRx_INVFL_bError_get();
        case INVERTER_FR:
            return app_canRx_INVFR_bError_get();
        case INVERTER_RL:
            return app_canRx_INVRL_bError_get();
        case INVERTER_RR:
            return app_canRx_INVRR_bError_get();
        default:
            return true;
    }
}

static void faultHandlingStateRunOnEntry(void)
{
    app_powerManager_updateConfig(power_manager_state);
    app_canTx_VC_State_set(VC_FAULT_STATE);
    app_canAlerts_VC_Info_InverterRetry_set(true);
    current_inverter_fault_state = INV_FAULT_RETRY;
    /*may not need this unless we wanna transition time out faults here too*/
    // app_timer_init(&start_up_timer, FAULT_TIMEOUT_MS);
}

static void FaultHandlingStateRunOnTick100Hz(void)
{
    switch (current_inverter_fault_state)
    {
        case INV_FAULT_RETRY:
        {
            retry_counter++;
            bool any_faulted = false;
            bool any_lockout = false;

            for (int i = 0; i < NUM_INVERTERS; ++i)
            {
                any_faulted |= inv_bError((InverterConfig)i);
                any_lockout |= is_lockout_code(inverter_reset_handle[(InverterConfig)i].can_error_info());
                if (any_lockout)
                {
                    current_inverter_fault_state = INV_FAULT_LOCKOUT;
                    break;
                }
                if (any_faulted)
                {
                    inverter_fault_retry((InverterConfig)i);
                    any_faulted = inv_bError((InverterConfig)i);
                }
            }

            if (!any_faulted)
            {
                current_inverter_fault_state = INV_FAULT_RECOVERED;
            }
            else
            {
                current_inverter_fault_state = INV_FAULT_RETRY;
            }
            break;
        }

        case INV_FAULT_LOCKOUT:
            // Do nothing here no retry by design; wait for manual action or power cycle also toggling off retry since
            // it doesn't make sense to retry here
            app_canAlerts_VC_Info_InverterRetry_set(false);
            return;

        case INV_FAULT_RECOVERED:
            app_canAlerts_VC_Info_InverterRetry_set(false);

            // jumping back to Hvinit instead of first state DC is alrady on
            app_stateMachine_setNextState(&hvInit_state);
            break;

        default:
            break;
    }
}

static void faultHandlingStateRunOnExit(void)
{
    // We are setting back this to zero meaning that we either succedded in reseting the inverters or out reset protocl
    // didnt work so we are going back to init
    app_canAlerts_VC_Info_InverterRetry_set(false);
    app_canTx_VC_INVFLbErrorReset_set(false);
    app_canTx_VC_INVFRbErrorReset_set(false);
    app_canTx_VC_INVRLbErrorReset_set(false);
    app_canTx_VC_INVRRbErrorReset_set(false);
    // app_timer_stop(&start_up_timer);
}

State inverter_retry_state = { .name              = "Handling State",
                               .run_on_entry      = faultHandlingStateRunOnEntry,
                               .run_on_tick_100Hz = FaultHandlingStateRunOnTick100Hz,
                               .run_on_exit       = faultHandlingStateRunOnExit };
