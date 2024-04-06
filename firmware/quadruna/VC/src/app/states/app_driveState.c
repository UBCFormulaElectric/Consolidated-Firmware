#include <stdlib.h>
#include <math.h>
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_vehicleDynamicsConstants.h"
#include "states/app_allStates.h"
#include "app_powerManager.h"
#include "states/app_initState.h"
#include "states/app_driveState.h"
#include "app_globals.h"
#include "app_torqueVectoring.h"
#include "app_faultCheck.h"
#include "app_regen.h"
#include "app_units.h"

#define EFFICIENCY_ESTIMATE (0.80f)
#define PEDAL_SCALE 0.3f
#define MAX_PEDAL_PERCENT 1.0f
#define BUZZER_ON_DURATION_MS 2000

static bool         torque_vectoring_switch_is_on;
static TimerChannel buzzer_timer;

void transmitTorqueRequests(float apps_pedal_percentage)
{
    const float bms_available_power   = app_canRx_BMS_AvailablePower_get();
    const float right_motor_speed_rpm = (float)app_canRx_INVR_MotorSpeed_get();
    const float left_motor_speed_rpm  = (float)app_canRx_INVL_MotorSpeed_get();
    float       bms_torque_limit      = MAX_TORQUE_REQUEST_NM;

    if ((right_motor_speed_rpm + left_motor_speed_rpm) > 0.0f)
    {
        // Estimate the maximum torque request to draw the maximum power available from the BMS
        const float available_output_power_w  = bms_available_power * EFFICIENCY_ESTIMATE;
        const float combined_motor_speed_rads = RPM_TO_RADS(right_motor_speed_rpm) + RPM_TO_RADS(left_motor_speed_rpm);
        bms_torque_limit = MIN(available_output_power_w / combined_motor_speed_rads, MAX_TORQUE_REQUEST_NM);
    }

    // Calculate the maximum torque request, according to the BMS available power
    const float max_bms_torque_request = apps_pedal_percentage * bms_torque_limit;

    // Calculate the actual torque requcest to transmit
    const float torque_request = MIN(max_bms_torque_request, MAX_TORQUE_REQUEST_NM);

    // Transmit torque command to both inverters
    app_canTx_VC_LeftInverterTorqueCommand_set(torque_request);
    app_canTx_VC_RightInverterTorqueCommand_set(torque_request);
}

static void driveStateRunOnEntry(void)
{
    app_timer_init(&buzzer_timer, BUZZER_ON_DURATION_MS);
    // Enable buzzer on transition to drive, and start 2s timer.
    io_buzzer_enable(true);
    app_canTx_VC_BuzzerOn_set(true);
    app_timer_restart(&buzzer_timer);

    app_canTx_VC_State_set(VC_DRIVE_STATE);
    app_powerManager_setState(POWER_MANAGER_DRIVE);

    app_canTx_VC_LeftInverterEnable_set(true);
    app_canTx_VC_RightInverterEnable_set(true);

    // Set inverter directions.
    app_canTx_VC_LeftInverterDirectionCommand_set(INVERTER_FORWARD_DIRECTION);
    app_canTx_VC_RightInverterDirectionCommand_set(INVERTER_REVERSE_DIRECTION);

    // Read torque vectoring switch only when entering drive state, not during driving

    // TODO: Finish setting up CRIT can set up once crit is done

    torque_vectoring_switch_is_on = app_canRx_CRIT_AuxSwitch_get() == SWITCH_ON;

    if (torque_vectoring_switch_is_on)
    {
        app_torqueVectoring_init();
    }
}

static void driveStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void driveStateRunOnTick100Hz(void)
{
    // All states module checks for faults, and returns whether or not a fault was detected.
    const bool any_board_has_fault = app_boardFaultCheck();
    const bool inverter_has_fault  = app_inverterFaultCheck();
    const bool all_states_ok       = !(any_board_has_fault || inverter_has_fault);

    const bool start_switch_off      = app_canRx_CRIT_StartSwitch_get() == SWITCH_OFF;
    const bool bms_not_in_drive      = app_canRx_BMS_State_get() != BMS_DRIVE_STATE;
    bool       exit_drive            = !all_states_ok || bms_not_in_drive || start_switch_off;
    bool       regen_switch_enabled  = app_canRx_CRIT_AuxSwitch_get() == SWITCH_ON;
    float      apps_pedal_percentage = app_canRx_FSM_PappsMappedPedalPercentage_get() * 0.01f;

    // Disable drive buzzer after 2 seconds.
    if (app_timer_updateAndGetState(&buzzer_timer) == TIMER_STATE_EXPIRED)
    {
        io_buzzer_enable(false);
        app_canTx_VC_BuzzerOn_set(false);
    }

    // regen switched pedal percentage from [0, 100] to [0.0, 1.0] to [-0.3, 0.7] and then scaled to [-1,1]

    if (regen_switch_enabled)
    {
        apps_pedal_percentage = (apps_pedal_percentage - PEDAL_SCALE) * MAX_PEDAL_PERCENT;
        apps_pedal_percentage = apps_pedal_percentage < 0.0f
                                    ? apps_pedal_percentage / PEDAL_SCALE
                                    : apps_pedal_percentage / (MAX_PEDAL_PERCENT - PEDAL_SCALE);
    }

    if (exit_drive)
    {
        app_stateMachine_setNextState(app_initState_get());
        return;
    }

    if (apps_pedal_percentage < 0.0f)
    {
        app_regen_run(apps_pedal_percentage);
    }
    else if (torque_vectoring_switch_is_on)
    {
        app_torqueVectoring_run(apps_pedal_percentage);
    }
    else
    {
        transmitTorqueRequests(apps_pedal_percentage);
    }
}

static void driveStateRunOnExit(void)
{
    // Disable inverters and apply zero torque upon exiting drive state
    app_canTx_VC_LeftInverterEnable_set(false);
    app_canTx_VC_RightInverterEnable_set(false);

    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);

    // Disable buzzer on exit drive.
    io_buzzer_enable(false);
    app_canTx_VC_BuzzerOn_set(false);
}

const State *app_driveState_get()
{
    static State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = driveStateRunOnExit,
    };

    return &drive_state;
}
