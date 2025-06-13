#include "app_stateMachine.h"
#include "app_states.h"
#include "app_powerManager.h"
#include "app_timer.h"
#include "app_canAlerts.h"
#include "app_warningHanding.h"
#include "io_loadswitches.h"
#include <app_canTx.h>
#include <app_canRx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <stdint.h>
#include "app_warningHanding.h"
#include "app_vehicleDynamicsConstants.h"
#include "io_log.h"

#define NO_TORQUE 0.0

typedef enum
{
    INV_SYSTEM_READY    = 0,
    INV_DC_ON           = 1,
    INV_ENABLE          = 2,
    INV_INVERTER_ON     = 3,
    INV_READY_FOR_DRIVE = 4
} INVERTER_STATES;

static INVERTER_STATES current_inverter_state;
static TimerChannel    start_up_timer;

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_RR_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_F_PUMP]  = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_L_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 } }
};

// static bool power_sequencing_done = false;
// static bool ready_for_drive       = false;

static void hvInitStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_HV_INIT_STATE);
    app_powerManager_updateConfig(power_manager_state);
    current_inverter_state = INV_SYSTEM_READY;
    app_timer_init(&start_up_timer, 30000);

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
    switch (current_inverter_state)
    {
        case INV_SYSTEM_READY:
        {
            const bool inv_systemReady = app_canRx_INVFL_bSystemReady_get() && app_canRx_INVFR_bSystemReady_get() &&
                                         app_canRx_INVRL_bSystemReady_get() && app_canRx_INVRR_bSystemReady_get();

            if (inv_systemReady)
            {
                current_inverter_state = INV_DC_ON;
                app_timer_restart(&start_up_timer);
            }
            break;
        }
        case INV_DC_ON:
        {
            app_canTx_VC_INVFLbDcOn_set(true);
            app_canTx_VC_INVFRbDcOn_set(true);
            app_canTx_VC_INVRRbDcOn_set(true);
            app_canTx_VC_INVRLbDcOn_set(true);

            const bool inverter_dc_quit = app_canRx_INVRR_bQuitDcOn_get() && app_canRx_INVRL_bQuitDcOn_get() &&
                                          app_canRx_INVFR_bQuitDcOn_get() && app_canRx_INVFL_bQuitDcOn_get();

            if (inverter_dc_quit)
            {
                current_inverter_state = INV_ENABLE;
                app_timer_restart(&start_up_timer);
            }

            if (app_timer_runIfCondition(&start_up_timer, !inverter_dc_quit) == TIMER_STATE_EXPIRED)
            {
                app_stateMachine_setNextState(&init_state);
            }

            break;
        }
        case INV_ENABLE:
        {
            app_canTx_VC_INVFLbEnable_set(true);
            app_canTx_VC_INVFRbEnable_set(true);
            app_canTx_VC_INVRLbEnable_set(true);
            app_canTx_VC_INVRRbEnable_set(true);
            current_inverter_state = INV_INVERTER_ON;
            break;
        }
        case INV_INVERTER_ON:
        {
            app_canTx_VC_INVFLbInverterOn_set(true);
            app_canTx_VC_INVFRbInverterOn_set(true);
            app_canTx_VC_INVRRbInverterOn_set(true);
            app_canTx_VC_INVRLbInverterOn_set(true);

            const bool inverter_invOn_quit =
                app_canRx_INVRR_bQuitInverterOn_get() && app_canRx_INVRL_bQuitInverterOn_get() &&
                app_canRx_INVFR_bQuitInverterOn_get() && app_canRx_INVFL_bQuitInverterOn_get();

            if (inverter_invOn_quit)
            {
                current_inverter_state = INV_READY_FOR_DRIVE;
                app_timer_restart(&start_up_timer);
            }

            if (app_timer_runIfCondition(&start_up_timer, !inverter_invOn_quit) == TIMER_STATE_EXPIRED)
            {
                app_stateMachine_setNextState(&init_state);
            }

            break;
        }
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
    }
}
static void hvInitStateRunOnExit(void)
{
    current_inverter_state = INV_SYSTEM_READY;
    app_timer_stop(&start_up_timer);
    app_canAlerts_VC_Info_InverterRetry_set(false);
}

State hvInit_state = { .name              = "HV INIT",
                       .run_on_entry      = hvInitStateRunOnEntry,
                       .run_on_tick_100Hz = hvInitStateRunOnTick100Hz,
                       .run_on_exit       = hvInitStateRunOnExit };
