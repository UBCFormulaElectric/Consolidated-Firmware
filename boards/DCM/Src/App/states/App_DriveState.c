#include <stdlib.h>
#include <math.h>
#include "App_SharedMacros.h"
#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedConstants.h"

#define EFFICIENCY_ESTIMATE (0.80f)

#define MAX_RACE_POWER_W (20e3f)

void App_SetPeriodicCanSignals_TorqueRequests()
{
    const float bms_available_power   = App_CanRx_BMS_AvailablePower_AvailablePower_Get();
    const float right_motor_speed_rpm = (float)App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get();
    const float left_motor_speed_rpm  = (float)App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Get();
    float       bms_torque_limit      = MAX_TORQUE_REQUEST_NM;
    float       race_torque_limit     = MAX_TORQUE_REQUEST_NM;

    if ((right_motor_speed_rpm + left_motor_speed_rpm) > 0.0f)
    {
        // Estimate the maximum torque request to draw the maximum power available from the BMS
        const float available_output_power_w  = bms_available_power * EFFICIENCY_ESTIMATE;
        const float combined_motor_speed_rads = RPM_TO_RADS(right_motor_speed_rpm) + RPM_TO_RADS(left_motor_speed_rpm);
        bms_torque_limit = MIN(available_output_power_w / combined_motor_speed_rads, MAX_TORQUE_REQUEST_NM);

        // Race mode = hard ceiling on power consumed
        // P = T * w, so T = P/w
        if (App_CanRx_DIM_Switches_AuxSwitch_Get() == SWITCH_ON)
        {
            race_torque_limit = MIN(MAX_RACE_POWER_W / combined_motor_speed_rads, MAX_TORQUE_REQUEST_NM);
        }
    }

    // Calculate the maximum torque request, according to the BMS available power
    const float apps_pedal_percentage  = 0.01f * App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Get();
    const float max_bms_torque_request = apps_pedal_percentage * bms_torque_limit;

    // Get the maximum torque request, according to the FSM
    const float max_fsm_torque_request = App_CanRx_FSM_Apps_TorqueLimit_Get();

    // Calculate the actual torque request to transmit
    const float torque_request      = MIN3(max_bms_torque_request, max_fsm_torque_request, MAX_TORQUE_REQUEST_NM);
    const float race_torque_request = MIN(torque_request, race_torque_limit);

    // Transmit torque command to both inverters
    App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(race_torque_request);
    App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(race_torque_request);
}

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    struct Buzzer *buzzer = App_DcmWorld_GetBuzzer(world);
    App_Buzzer_TurnOn(buzzer);

    App_CanTx_DCM_Vitals_CurrentState_Set(DCM_DRIVE_STATE);

    // Enable inverters upon entering drive state.
    App_CanTx_DCM_LeftInverterCommand_EnableInverter_Set(true);
    App_CanTx_DCM_RightInverterCommand_EnableInverter_Set(true);

    // Set inverter directions.
    App_CanTx_DCM_LeftInverterCommand_DirectionCommand_Set(INVERTER_FORWARD_DIRECTION);
    App_CanTx_DCM_RightInverterCommand_DirectionCommand_Set(INVERTER_REVERSE_DIRECTION);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        App_SetPeriodicCanSignals_TorqueRequests();

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
