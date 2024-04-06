#include <math.h>
#include "states/app_initState.h"
#include "states/app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_powerManager.h"
#include <stddef.h>
#include "states/app_allStates.h"
#include "states/app_driveState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_buzzer.h"
#include "app_globals.h"
#include "app_faultCheck.h"

#include <app_pumpControl.h>

#define DEFAULT_FLOW_RATE 600 // 10 Liters/Hour

static void initStateRunOnEntry(void)
{
    app_allStates_runOnTick100Hz();
    app_canTx_VC_State_set(VC_INIT_STATE);
    app_powerManager_setState(POWER_MANAGER_SHUTDOWN);
    // Disable inverters and apply zero torque upon entering init state
    app_canTx_VC_LeftInverterEnable_set(false);
    app_canTx_VC_RightInverterEnable_set(false);
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);

    // Disable buzzer on transition to init.
    io_buzzer_enable(false);
    app_canTx_VC_BuzzerOn_set(false);
}

static void initStateRunOnTick100Hz(void)
{
    const bool any_board_has_fault = app_boardFaultCheck();
    const bool inverter_has_fault  = app_inverterFaultCheck();
    const bool all_states_ok       = !(any_board_has_fault || inverter_has_fault);

    bool is_bms_in_correct_state = app_canRx_BMS_State_get() == BMS_INVERTER_ON_STATE ||
                                   app_canRx_BMS_State_get() == BMS_PRECHARGE_STATE ||
                                   app_canRx_BMS_State_get() == BMS_DRIVE_STATE;

    if (is_bms_in_correct_state && all_states_ok)
    {
        app_stateMachine_setNextState(app_inverterOnState_get());
    }

    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    static bool prev_start_switch_pos = true;

    // TODO: Finish setting up DIM can set up once crit is done

    // const bool curr_start_switch_pos      = app_canRx_DIM_StartSwitch_get();
    // const bool was_start_switch_pulled_up = !prev_start_switch_pos && curr_start_switch_pos;
    // prev_start_switch_pos                 = curr_start_switch_pos;

    // const bool bms_in_drive_state = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    // const bool is_brake_actuated  = app_canRx_FSM_BrakeActuated_get();

    // if (bms_in_drive_state && is_brake_actuated && was_start_switch_pulled_up && all_states_ok)
    // {
    //     // Transition to drive state when start-up conditions are passed (see
    //     // EV.10.4.3):

    //     // TODO: Could not thoroughly validate VC refactor without a working BMS.
    //     // Thus, re-test IO, app, and vehicle dynamics before going HV up or driving again.
    //     app_stateMachine_setNextState(app_driveState_get());
    // }
}

static void initStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();

    if (app_canRx_Debug_SetCoolantPump_CustomEnable_get())
    {
        app_pumpControl_setFlowRate(app_canRx_Debug_SetCoolantPump_CustomVal_get());
    }
    else
    {
        app_pumpControl_setFlowRate(DEFAULT_FLOW_RATE);
    }
}

const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = initStateRunOnEntry,
        .run_on_tick_1Hz   = initStateRunOnTick1Hz,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &init_state;
}
