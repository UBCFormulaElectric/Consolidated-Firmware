#include <stdlib.h>
#include <math.h>
#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "App_SetPeriodicCanSignals.h"
#include "torquevectoring/App_TorqueVectoring.h"

#define RPM_TO_RADS(rpm) ((rpm) * (float)M_PI / 30.0f)
#define EFFICIENCY_ESTIMATE (0.80f)

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    struct Buzzer *buzzer = App_DcmWorld_GetBuzzer(world);
    App_Buzzer_TurnOn(buzzer);

    App_CanTx_DCM_Vitals_CurrentState_Set(DCM_DRIVE_STATE);

    App_CanTx_DCM_LeftInverterCommand_EnableInverter_Set(true);
    App_CanTx_DCM_RightInverterCommand_EnableInverter_Set(true);

    // Set inverter directions.
    App_CanTx_DCM_LeftInverterCommand_DirectionCommand_Set(INVERTER_FORWARD_DIRECTION);
    App_CanTx_DCM_RightInverterCommand_DirectionCommand_Set(INVERTER_REVERSE_DIRECTION);

    static bool torque_vectoring_switch_is_on = App_IsTorqueVectoringSwitch_On() 
    if (torque_vectoring_switch_is_on) {
        App_TorqueVectoring_Setup();
    } 
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        if (torque_vectoring_switch_is_on)
        {
            App_TorqueVectoring_Setup();
        }
        else
        { 
            App_SetPeriodicCanSignals_TorqueRequests();
        }

        if (!App_IsStartSwitchOn() || !App_IsBmsInDriveState())
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
        }
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    // Disable inverters and apply zero torque upon exiting drive state
    App_CanTx_DCM_LeftInverterCommand_EnableInverter_Set(false);
    App_CanTx_DCM_RightInverterCommand_EnableInverter_Set(false);

    App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(0.0f);
    App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(0.0f);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = DriveStateRunOnEntry,
        .run_on_tick_1Hz   = DriveStateRunOnTick1Hz,
        .run_on_tick_100Hz = DriveStateRunOnTick100Hz,
        .run_on_exit       = DriveStateRunOnExit,
    };

    return &drive_state;
}
