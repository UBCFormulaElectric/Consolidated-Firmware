#include <stdlib.h>
#include <math.h>
#include "App_SharedMacros.h"
#include "App_SharedDcmConstants.h"
#include "App_SharedConstants.h"
#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "App_SetPeriodicCanSignals.h"
#include "torquevectoring/App_TorqueVectoring.h"

#define EFFICIENCY_ESTIMATE (0.80f)

static bool torque_vectoring_switch_is_on;

void App_SetPeriodicCanSignals_TorqueRequests()
{
    const float bms_available_power   = App_CanRx_BMS_AvailablePower_Get();
    const float right_motor_speed_rpm = (float)App_CanRx_INVR_MotorSpeed_Get();
    const float left_motor_speed_rpm  = (float)App_CanRx_INVL_MotorSpeed_Get();
    float       bms_torque_limit      = MAX_TORQUE_REQUEST_NM;

    if ((right_motor_speed_rpm + left_motor_speed_rpm) > 0.0f)
    {
        // Estimate the maximum torque request to draw the maximum power available from the BMS
        const float available_output_power_w  = bms_available_power * EFFICIENCY_ESTIMATE;
        const float combined_motor_speed_rads = RPM_TO_RADS(right_motor_speed_rpm) + RPM_TO_RADS(left_motor_speed_rpm);
        bms_torque_limit = MIN(available_output_power_w / combined_motor_speed_rads, MAX_TORQUE_REQUEST_NM);
    }

    // Calculate the maximum torque request, according to the BMS available power
    const float apps_pedal_percentage  = 0.01f * App_CanRx_FSM_PappsMappedPedalPercentage_Get();
    const float max_bms_torque_request = apps_pedal_percentage * bms_torque_limit;

    // Calculate the actual torque request to transmit
    const float torque_request = MIN(max_bms_torque_request, MAX_TORQUE_REQUEST_NM);

    // Transmit torque command to both inverters
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(torque_request);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(torque_request);
}

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    struct Buzzer *buzzer = App_DcmWorld_GetBuzzer(world);
    App_Buzzer_TurnOn(buzzer);

    App_CanTx_DCM_State_Set(DCM_DRIVE_STATE);

    App_CanTx_DCM_LeftInverterEnable_Set(true);
    App_CanTx_DCM_RightInverterEnable_Set(true);

    // Set inverter directions.
    App_CanTx_DCM_LeftInverterDirectionCommand_Set(INVERTER_FORWARD_DIRECTION);
    App_CanTx_DCM_RightInverterDirectionCommand_Set(INVERTER_REVERSE_DIRECTION);

    // Read torque vectoring switch only when entering drive state, not during driving
    torque_vectoring_switch_is_on = App_IsTorqueVectoringSwitchOn();

    if (torque_vectoring_switch_is_on)
    {
        App_TorqueVectoring_Setup();
    }
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    // All states module checks for faults, and returns whether or not a fault was detected.
    const bool all_states_ok    = App_AllStatesRunOnTick100Hz(state_machine);
    const bool start_switch_off = !App_IsStartSwitchOn();
    const bool bms_not_in_drive = !App_IsBmsInDriveState();
    bool       exit_drive       = !all_states_ok || start_switch_off || bms_not_in_drive;

    if (all_states_ok)
    {
        if (torque_vectoring_switch_is_on)
        {
            App_TorqueVectoring_Run();
        }
        else
        {
            App_SetPeriodicCanSignals_TorqueRequests();
        }
    }
    if (exit_drive)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    // Disable inverters and apply zero torque upon exiting drive state
    App_CanTx_DCM_LeftInverterEnable_Set(false);
    App_CanTx_DCM_RightInverterEnable_Set(false);

    App_CanTx_DCM_LeftInverterTorqueCommand_Set(0.0f);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(0.0f);
}

const struct State *app_driveState_get(void)
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
