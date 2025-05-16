#ifdef TARGET_EMBEDDED
#include "io_canTx.h"
#endif

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_units.h"
#include "app_utils.h"
#include "app_signal.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "app_regen.h"
#include "app_states.h"
#include "app_powerManager.h"

#define EFFICIENCY_ESTIMATE (0.80f)
#define BUZZER_ON_DURATION_MS 2000

static bool         torque_vectoring_switch_is_on;
static bool         regen_switch_is_on;
static TimerChannel buzzer_timer;

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = false, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = false, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_RR_PUMP] = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_F_PUMP]  = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_L_RAD]   = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = false, .timeout = 200, .max_retry = 5 } }
};

static void driveStateRunOnEntry(void)
{
    // Enable buzzer on transition to drive, and start 2s timer.
    app_timer_init(&buzzer_timer, BUZZER_ON_DURATION_MS);
    app_timer_restart(&buzzer_timer);
    app_canTx_VC_BuzzerOn_set(true);

    app_canTx_VC_State_set(VC_DRIVE_STATE);
    app_powerManager_updateConfig(power_manager_state);

    app_canTx_VC_LeftInverterEnable_set(true);
    app_canTx_VC_RightInverterEnable_set(true);
    // TODO: replace with new messages
    // app_canTx_VC_LeftInverterDirectionCommand_set(INVERTER_FORWARD_DIRECTION);
    // app_canTx_VC_RightInverterDirectionCommand_set(INVERTER_REVERSE_DIRECTION);
    // app_canTx_VC_LeftInverterTorqueLimit_set(MAX_TORQUE_REQUEST_NM);
    // app_canTx_VC_RightInverterTorqueLimit_set(MAX_TORQUE_REQUEST_NM);

    if (app_canRx_CRIT_TorqueVecSwitch_get() == SWITCH_ON)
    {
        app_torqueVectoring_init();
        torque_vectoring_switch_is_on = true;
    }

    if (app_canRx_CRIT_RegenSwitch_get() == SWITCH_ON)
    {
        app_regen_init();
        regen_switch_is_on = true;
    }
}

static void driveStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void driveStateRunOnTick100Hz(void)
{
    float apps_pedal_percentage  = app_canRx_FSM_PappsMappedPedalPercentage_get() * 0.01f;
    float sapps_pedal_percentage = app_canRx_FSM_SappsMappedPedalPercentage_get() * 0.01f;

    if (!driveStatePassPreCheck())
    {
        return;
    }

    // Disable drive buzzer after 2 seconds.
    if (app_timer_updateAndGetState(&buzzer_timer) == TIMER_STATE_EXPIRED)
    {
        io_efuse_setChannel(EFUSE_CHANNEL_BUZZER, false);
        app_canTx_VC_BuzzerOn_set(false);
    }

    // regen switched pedal percentage from [0, 100] to [0.0, 1.0] to [-0.3, 0.7] and then scaled to [-1,1]
    if (regen_switch_is_on)
    {
        apps_pedal_percentage  = app_regen_pedalRemapping(apps_pedal_percentage);
        sapps_pedal_percentage = app_regen_pedalRemapping(sapps_pedal_percentage);
    }

    app_canTx_VC_MappedPedalPercentage_set(apps_pedal_percentage);
    runDrivingAlgorithm(apps_pedal_percentage, sapps_pedal_percentage);
}

static void driveStateRunOnExit(void) {}

State drive_state = {
    .name              = "DRIVE",
    .run_on_entry      = driveStateRunOnEntry,
    .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
    .run_on_tick_100Hz = driveStateRunOnTick100Hz,
    .run_on_exit       = driveStateRunOnExit,
};

bool driveStatePassPreCheck()
{
    // All states module checks for faults, and returns whether or not a fault was detected.
    const bool any_board_has_fault = app_faultCheck_checkBoards();
    const bool inverter_has_fault  = app_faultCheck_checkInverters();
    const bool all_states_ok       = !(any_board_has_fault || inverter_has_fault);

    const bool start_switch_off          = app_canRx_CRIT_StartSwitch_get() == SWITCH_OFF;
    const bool bms_not_in_drive          = app_canRx_BMS_State_get() != BMS_DRIVE_STATE;
    bool       exit_drive_to_init        = bms_not_in_drive;
    bool       exit_drive_to_inverter_on = !all_states_ok || start_switch_off;

    // Make sure you can only turn on VD in init and not during drive, only able to turn off
    bool prev_regen_switch_val = regen_switch_is_on;
    regen_switch_is_on         = app_canRx_CRIT_RegenSwitch_get() == SWITCH_ON && prev_regen_switch_val;

    bool prev_torque_vectoring_switch_val = torque_vectoring_switch_is_on;
    regen_switch_is_on = app_canRx_CRIT_TorqueVecSwitch_get() == SWITCH_ON && prev_torque_vectoring_switch_val;

    /* TODO: Vehicle dyanmics people need to make sure to do a check if sensor init failed
        or not before using closed loop features */

    // Update Regen + TV LEDs
    if (!regen_switch_is_on)
    {
        app_canTx_VC_RegenEnabled_set(false);
        app_canTx_VC_Warning_RegenNotAvailable_set(true);
    }

    if (!torque_vectoring_switch_is_on)
    {
        app_canTx_VC_TorqueVectoringEnabled_set(true);
    }

    if (exit_drive_to_init)
    {
        app_stateMachine_setNextState(app_initState_get());
        return false;
    }
    else if (exit_drive_to_inverter_on)
    {
        app_stateMachine_setNextState(app_inverterOnState_get());
        return false;
    }

    return true;
}

void runDrivingAlgorithm(float apps_pedal_percentage, float sapps_pedal_percentage)
{
    if (app_faultCheck_checkSoftwareBspd(apps_pedal_percentage, sapps_pedal_percentage))
    {
        // If bspd warning is true, set torque to 0.0
        app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
        app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    }
    else if (apps_pedal_percentage < 0.0f && regen_switch_is_on)
    {
        app_regen_run(apps_pedal_percentage);
    }
    else if (torque_vectoring_switch_is_on)
    {
        app_torqueVectoring_run(apps_pedal_percentage);
    }
    else
    {
        app_regularDrive_run(apps_pedal_percentage);
    }
}

void app_regularDrive_run(float apps_pedal_percentage)
{
    // TODO: Use power limiting in regular drive
    const float bms_available_power         = (float)app_canRx_BMS_AvailablePower_get();
    const float right_front_motor_speed_rpm = (float)app_canRx_INVR_MotorSpeed_get();
    const float right_back_motor_speed_rpm  = (float)app_canRx_INVR_MotorSpeed_get();
    const float left_front_motor_speed_rpm  = (float)app_canRx_INVL_MotorSpeed_get();
    const float left_back_motor_speed_rpm   = (float)app_canRx_INVL_MotorSpeed_get();
    float       bms_torque_limit            = MAX_TORQUE_REQUEST_NM;

    if ((right_front_motor_speed_rpm + right_back_motor_speed_rpm + left_front_motor_speed_rpm +
         left_back_motor_speed_rpm) > 0.0f)
    {
        // Estimate the maximum torque request to draw the maximum power available from the BMS
        const float available_output_power_w = bms_available_power * EFFICIENCY_ESTIMATE;
        const float combined_motor_speed_rads =
            RPM_TO_RADS(right_front_motor_speed_rpm) + RPM_TO_RADS(right_back_motor_speed_rpm) +
            RPM_TO_RADS(left_front_motor_speed_rpm) + RPM_TO_RADS(left_back_motor_speed_rpm);
        bms_torque_limit = MIN(available_output_power_w / combined_motor_speed_rads, MAX_TORQUE_REQUEST_NM);
    }

    // Calculate the maximum torque request, according to the BMS available power
    const float max_bms_torque_request = apps_pedal_percentage * bms_torque_limit;

    // Calculate the actual torque request to transmit
    const float torque_request = MIN(max_bms_torque_request, MAX_TORQUE_REQUEST_NM);

    // Transmit torque command to both inverters
    app_canTx_VC_RightFrontInverterTorqueCommand_set(torque_request);
    app_canTx_VC_RightBackInverterTorqueCommand_set(torque_request);
    app_canTx_VC_LeftFrontInverterTorqueCommand_set(torque_request);
    app_canTx_VC_LeftBackInverterTorqueCommand_set(torque_request);
}