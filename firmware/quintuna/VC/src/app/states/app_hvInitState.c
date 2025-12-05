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
#include "app_vehicleDynamicsConstants.h"
#include "io_log.h"

#define INV_QUIT_TIMEOUT_MS (10 * 1000)
#define NO_TORQUE 0.0

static VCInverterState current_inverter_state;
static TimerChannel    start_up_timer;

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

static void reset_inverter_power_requests(void)
{
    app_canTx_VC_INVFLbInverterOn_set(false);
    app_canTx_VC_INVFRbInverterOn_set(false);
    app_canTx_VC_INVRRbInverterOn_set(false);
    app_canTx_VC_INVRLbInverterOn_set(false);
    app_canTx_VC_INVFLbEnable_set(false);
    app_canTx_VC_INVFRbEnable_set(false);
    app_canTx_VC_INVRLbEnable_set(false);
    app_canTx_VC_INVRRbEnable_set(false);
    app_canTx_VC_INVFLbDcOn_set(false);
    app_canTx_VC_INVFRbDcOn_set(false);
    app_canTx_VC_INVRRbDcOn_set(false);
    app_canTx_VC_INVRLbDcOn_set(false);
}

static void hvInitStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_HV_INIT_STATE);
    app_powerManager_updateConfig(power_manager_state);
    current_inverter_state = INV_SYSTEM_READY;
    app_timer_init(&start_up_timer, INV_QUIT_TIMEOUT_MS);

    reset_inverter_power_requests();

    app_canTx_VC_INVFRTorqueSetpoint_set(0);
    app_canTx_VC_INVRRTorqueSetpoint_set(0);
    app_canTx_VC_INVFLTorqueSetpoint_set(0);
    app_canTx_VC_INVRLTorqueSetpoint_set(0);

    app_canTx_VC_INVFLTorqueLimitPositive_set(0);
    app_canTx_VC_INVFRTorqueLimitPositive_set(0);
    app_canTx_VC_INVRLTorqueLimitPositive_set(0);
    app_canTx_VC_INVRRTorqueLimitPositive_set(0);

    app_canTx_VC_INVFLTorqueLimitNegative_set(0);
    app_canTx_VC_INVFRTorqueLimitNegative_set(0);
    app_canTx_VC_INVRLTorqueLimitNegative_set(0);
    app_canTx_VC_INVRRTorqueLimitNegative_set(0);
}

static void hvInitStateRunOnTick100Hz(void)
{
    const bool inv_systemReady = app_canRx_INVFL_bSystemReady_get() && app_canRx_INVFR_bSystemReady_get() &&
                                 app_canRx_INVRL_bSystemReady_get() && app_canRx_INVRR_bSystemReady_get();
    const bool inverter_dc_quit = app_canRx_INVRR_bQuitDcOn_get() && app_canRx_INVRL_bQuitDcOn_get() &&
                                  app_canRx_INVFR_bQuitDcOn_get() && app_canRx_INVFL_bQuitDcOn_get();
    const bool inverter_invOn_quit = app_canRx_INVRR_bQuitInverterOn_get() && app_canRx_INVRL_bQuitInverterOn_get() &&
                                     app_canRx_INVFR_bQuitInverterOn_get() && app_canRx_INVFL_bQuitInverterOn_get();

    for (bool retry_transition = true; retry_transition;)
    {
        retry_transition = false;

        switch (current_inverter_state) // state transitions
        {
            case INV_SYSTEM_READY:
            {
                if (inv_systemReady)
                {
                    LOG_INFO("inv_system_ready -> inv_dc_on");
                    current_inverter_state = INV_DC_ON;
                    retry_transition       = true;
                    app_timer_stop(&start_up_timer);

                    // Error reset should be set to false cause we were successful
                    app_canTx_VC_INVFLbErrorReset_set(false);
                    app_canTx_VC_INVFRbErrorReset_set(false);
                    app_canTx_VC_INVRLbErrorReset_set(false);
                    app_canTx_VC_INVRRbErrorReset_set(false);
                }
                else if (app_canAlerts_VC_Info_InverterRetry_get())
                {
                    app_warningHandling_inverterReset();
                }
                break;
            }
            case INV_DC_ON:
            {
                if (!inv_systemReady)
                {
                    current_inverter_state = INV_SYSTEM_READY;
                    retry_transition       = true;
                    break;
                }

                if (inverter_dc_quit)
                {
                    LOG_INFO("inv_dc_on -> inv_enable");
                    current_inverter_state = INV_ENABLE;
                    // retry_transition       = true; // TODO we need a tick in INV_ENABLE?
                    app_timer_stop(&start_up_timer);
                }
                else if (app_timer_runIfCondition(&start_up_timer, !inverter_dc_quit) == TIMER_STATE_EXPIRED)
                {
                    LOG_INFO("dc quit timeout");
                    current_inverter_state = INV_SYSTEM_READY;
                    retry_transition       = true;
                }

                break;
            }
            case INV_ENABLE:
            {
                if (!(inv_systemReady && inverter_dc_quit))
                {
                    current_inverter_state = INV_DC_ON;
                    retry_transition       = true;
                    break;
                }

                current_inverter_state = INV_INVERTER_ON;
                // retry_transition       = true; // TODO if we did this might as well consolidate the states
                break;
            }
            case INV_INVERTER_ON:
            {
                if (!(inv_systemReady && inverter_dc_quit))
                {
                    current_inverter_state = INV_ENABLE;
                    retry_transition       = true;
                    break;
                }

                if (inverter_invOn_quit)
                {
                    LOG_INFO("inv_on -> inv_ready_for_drive");
                    current_inverter_state = INV_READY_FOR_DRIVE;
                    retry_transition       = true;
                    app_timer_stop(&start_up_timer);
                }
                else if (app_timer_runIfCondition(&start_up_timer, !inverter_invOn_quit) == TIMER_STATE_EXPIRED)
                {
                    LOG_INFO("inv on quit timeout");
                    current_inverter_state = INV_SYSTEM_READY;
                    retry_transition       = true;
                }

                break;
            }
            case INV_READY_FOR_DRIVE: // locked in to drive
                if (!(inv_systemReady && inverter_dc_quit && inverter_invOn_quit))
                {
                    current_inverter_state = INV_INVERTER_ON;
                    retry_transition       = true;
                }
                break;
            case NUM_VC_INVERTER_STATE_CHOICES:
            default:
                break;
        }
    }

    app_canTx_VC_InverterState_set(current_inverter_state);

    reset_inverter_power_requests();
    switch (current_inverter_state)
    {
        case INV_INVERTER_ON:
            app_canTx_VC_INVFLbInverterOn_set(true);
            app_canTx_VC_INVFRbInverterOn_set(true);
            app_canTx_VC_INVRRbInverterOn_set(true);
            app_canTx_VC_INVRLbInverterOn_set(true);
            __attribute__((fallthrough));
        case INV_ENABLE:
            app_canTx_VC_INVFLbEnable_set(true);
            app_canTx_VC_INVFRbEnable_set(true);
            app_canTx_VC_INVRLbEnable_set(true);
            app_canTx_VC_INVRRbEnable_set(true);
            __attribute__((fallthrough));
        case INV_DC_ON:
            app_canTx_VC_INVFLbDcOn_set(true);
            app_canTx_VC_INVFRbDcOn_set(true);
            app_canTx_VC_INVRRbDcOn_set(true);
            app_canTx_VC_INVRLbDcOn_set(true);
            break;
        case INV_READY_FOR_DRIVE:
            if (app_canAlerts_VC_Info_InverterRetry_get())
            {
                app_warningHandling_inverterStatus();
                app_canAlerts_VC_Info_InverterRetry_set(false);
                app_stateMachine_setNextState(&drive_state);
            }
            else
            {
                app_stateMachine_setNextState(&hv_state);
            }
            break;
        default:
        case INV_SYSTEM_READY:
        case NUM_VC_INVERTER_STATE_CHOICES:
            break;
    }
}

static void hvInitStateRunOnExit(void)
{
    current_inverter_state = INV_SYSTEM_READY;
    app_timer_stop(&start_up_timer);
    app_canAlerts_VC_Info_InverterRetry_set(false);

    // We are setting back this to zero meaning that we either succedded in reseting the inverters or out reset protocl
    // didnt work so we are going back to init
    app_canTx_VC_INVFLbErrorReset_set(false);
    app_canTx_VC_INVFRbErrorReset_set(false);
    app_canTx_VC_INVRLbErrorReset_set(false);
    app_canTx_VC_INVRRbErrorReset_set(false);
}

State hvInit_state = { .name              = "HV INIT",
                       .run_on_entry      = hvInitStateRunOnEntry,
                       .run_on_tick_100Hz = hvInitStateRunOnTick100Hz,
                       .run_on_exit       = hvInitStateRunOnExit };
