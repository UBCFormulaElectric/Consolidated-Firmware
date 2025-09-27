#include "app_states.h"

#include "app_canTx.h"
#include "app_canRx.h"

#include <app_utils.h>
#include "app_canAlerts.h"
#include "app_powerManager.h"
#include "app_warningHandling.h"
#include "app_inverterFaultHandling.h"

#include "app_regen.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "app_vehicleDynamics.h"
#include "app_torqueDistribution.h"
#include "app_driveHandling.h"
#include "app_startSwitch.h"
#include "app_inverterFaultHandling.h"


#define OFF 0
#define 

static bool                    launch_control_switch_is_on;
static bool                    regen_switch_is_on;
static TorqueAllocationOutputs torqueOutputToMotors;
static VCInverterFaults current_inverter_fault_state;


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


typedef enum { INVERTER_FL, INVERTER_FR, INVERTER_RL, INVERTER_RR, NUM_INVERTERS } InverterConfig;
extern InverterWarningHandling inverter_reset_handle[NUM_INVERTERS];
typedef enum {
    259 = 0,
    1342 = 1,
    2311 = 2,
} uint32_t LockoutErrorCodes;


{
    void (*can_enable_inv)(bool);
    void (*can_invOn)(bool);
    void (*can_dcOn)(bool);
    uint32_t (*can_error_info)(void);
    void (*error_reset)(bool);
} InverterWarningHandling;
static inline bool inv_bError(InverterConfig i)
{
    switch (i) {
    case INVERTER_FL: return app_canRx_INVFL_bError_get();
    case INVERTER_FR: return app_canRx_INVFR_bError_get();
    case INVERTER_RL: return app_canRx_INVRL_bError_get();
    case INVERTER_RR: return app_canRx_INVRR_bError_get();
    default:          return true;
    }
}


static void faultHandlingStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_FAULT_STATE);
    app_canAlerts_VC_Info_InverterRetry_set(true);
    app_powerManager_updateConfig(power_manager_state);
    current_inverter_fault_state = INV_FAULT_RETRY;
    //app_timer_init(&start_up_timer, FAULT_TIMEOUT_MS);
}

static void FaultHandlingStateRunOnTick100Hz(void)
{
    switch (current_inverter_fault_state)
    {
        case INV_FAULT_RETRY:
            inv_error_handling();
            app_warningHandling_inverterReset();
            current_inverter_fault_state = HW_RESET;
            break;

        case INV_FAULT_LOCKOUT:
            // Stay in this state until a manual reset is done
            break;

        case INV_FAULT_RECOVERED:
        {
            app_canAlerts_VC_info_InverterRetry_set(false);
            app_stateMachine_setNextState(&hvInit_state);
            break;
        }
        default:
            break;

    app_canTx_VC_InverterState_set(current_inverter_state);
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
    app_timer_stop(&start_up_timer);
}

State faultHandling_state = { .name       = "Handling State",
                       .run_on_entry      = faultHandlingStateRunOnEntry,
                       .run_on_tick_100Hz = FaultHandlingStateRunOnTick100Hz,
                       .run_on_exit       = faultHandlingStateRunOnExit };


