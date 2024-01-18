#include <stdlib.h>
#include <math.h>
#include "App_SharedMacros.h"
#include "torquevectoring/app_torqueVectoringConstants.h"
#include "App_SharedConstants.h"
#include "states/app_allStates.h"
#include "states/app_initState.h"
#include "torquevectoring/app_torqueVectoring.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "app_globals.h"
#include "App_Timer.h"

#define EFFICIENCY_ESTIMATE (0.80f)

void transmitTorqueRequests()
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

static void driveStateRunOnEntry(struct StateMachine *const state_machine)
{
    // Enable buzzer on transition to drive, and start 2s timer.
    io_buzzer_enable(globals->config->buzzer, true);
    App_CanTx_DCM_BuzzerOn_Set(true);
    App_Timer_Restart(&globals->buzzer_timer);

    App_CanTx_DCM_State_Set(DCM_DRIVE_STATE);

    App_CanTx_DCM_LeftInverterEnable_Set(true);
    App_CanTx_DCM_RightInverterEnable_Set(true);

    // Set inverter directions.
    App_CanTx_DCM_LeftInverterDirectionCommand_Set(INVERTER_FORWARD_DIRECTION);
    App_CanTx_DCM_RightInverterDirectionCommand_Set(INVERTER_REVERSE_DIRECTION);

    // Read torque vectoring switch only when entering drive state, not during driving
    globals->torque_vectoring_switch_is_on = App_CanRx_DIM_AuxSwitch_Get() == SWITCH_ON;

    if (globals->torque_vectoring_switch_is_on)
    {
        app_torqueVectoring_setup();
    }
}

static void driveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void driveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    // All states module checks for faults, and returns whether or not a fault was detected.
    const bool all_states_ok    = app_allStates_runOnTick100Hz(state_machine);
    const bool start_switch_off = App_CanRx_DIM_StartSwitch_Get() == SWITCH_OFF;
    const bool bms_not_in_drive = App_CanRx_BMS_State_Get() != BMS_DRIVE_STATE;
    bool       exit_drive       = !all_states_ok || start_switch_off || bms_not_in_drive;

    // Disable drive buzzer after 2 seconds.
    if (App_Timer_UpdateAndGetState(&globals->buzzer_timer) == TIMER_STATE_EXPIRED)
    {
        io_buzzer_enable(globals->config->buzzer, false);
        App_CanTx_DCM_BuzzerOn_Set(false);
    }

    if (all_states_ok)
    {
        if (globals->torque_vectoring_switch_is_on)
        {
            app_torqueVectoring_run();
        }
        else
        {
            transmitTorqueRequests();
        }
    }

    if (exit_drive)
    {
        App_SharedStateMachine_SetNextState(state_machine, app_initState_get());
    }
}

static void driveStateRunOnExit(struct StateMachine *const state_machine)
{
    // Disable inverters and apply zero torque upon exiting drive state
    App_CanTx_DCM_LeftInverterEnable_Set(false);
    App_CanTx_DCM_RightInverterEnable_Set(false);

    App_CanTx_DCM_LeftInverterTorqueCommand_Set(0.0f);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(0.0f);

    // Disable buzzer on exit drive.
    io_buzzer_enable(globals->config->buzzer, false);
    App_CanTx_DCM_BuzzerOn_Set(false);
}

const struct State *app_driveState_get(void)
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = driveStateRunOnExit,
    };

    return &drive_state;
}
