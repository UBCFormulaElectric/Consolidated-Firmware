#include "states/App_DriveState.h"

#include "App_Imd.h"
#include "App_SharedMacros.h"

static void App_DriveState_SetImdPeriodicCanSignals(
    struct BmsCanTxInterface *can_tx,
    struct Imd *              imd)
{
    App_CanTx_SetPeriodicSignal_SECONDS_SINCE_POWER_ON(
        can_tx, App_Imd_GetSecondsSincePowerOn(imd));
    App_CanTx_SetPeriodicSignal_FREQUENCY(can_tx, App_Imd_GetPwmFrequency(imd));
    App_CanTx_SetPeriodicSignal_DUTY_CYCLE(
        can_tx, App_Imd_GetPwmDutyCycle(imd));

    const enum Imd_Condition condition = App_Imd_GetCondition(imd);
    App_CanTx_SetPeriodicSignal_CONDITION(can_tx, condition);

    const struct Imd_PwmEncoding pwm_encoding = App_Imd_GetPwmEncoding(imd);

    App_CanTx_SetPeriodicSignal_VALID_DUTY_CYCLE(
        can_tx, pwm_encoding.valid_duty_cycle);

    switch (condition)
    {
        case IMD_SHORT_CIRCUIT:
        {
            App_CanTx_SetPeriodicSignal_DUMMY_0_HZ(can_tx, 0);
        }
        break;
        case IMD_NORMAL:
        {
            if (pwm_encoding.valid_duty_cycle == true)
            {
                App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_10_HZ(
                    can_tx, pwm_encoding.insulation_measurement_dcp_kohms);
            }
        }
        break;
        case IMD_UNDERVOLTAGE_DETECTED:
        {
            if (pwm_encoding.valid_duty_cycle == true)
            {
                App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_20_HZ(
                    can_tx, pwm_encoding.insulation_measurement_dcp_kohms);
            }
        }
        break;
        case IMD_SST:
        {
            App_CanTx_SetPeriodicSignal_SPEED_START_STATUS_30_HZ(
                can_tx, pwm_encoding.speed_start_status);
        }
        break;
        case IMD_DEVICE_ERROR:
        {
            App_CanTx_SetPeriodicSignal_DUMMY_40_HZ(can_tx, 0);
        }
        break;
        case IMD_EARTH_FAULT:
        {
            App_CanTx_SetPeriodicSignal_DUMMY_50_HZ(can_tx, 0);
        }
        break;
        case IMD_INVALID:
        {
            App_CanTx_SetPeriodicSignal_DUMMY_INVALID(can_tx, 0);
        }
        break;
    }
}

static void DriveStateRunOnEntry(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void DriveStateRunOnTick(struct StateMachine *state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct Imd *              imd    = App_BmsWorld_GetImd(world);

    App_DriveState_SetImdPeriodicCanSignals(can_tx, imd);
}

static void DriveStateRunOnExit(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState()
{
    static struct State drive_state = {
        .name         = "DRIVE",
        .run_on_enter = DriveStateRunOnEntry,
        .run_on_tick  = DriveStateRunOnTick,
        .run_on_exit  = DriveStateRunOnExit,
    };

    return &drive_state;
}
