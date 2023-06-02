#include "states/App_AllStates.h"
#include "states/App_DriveState.h"
#include "states/App_InitState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_DRIVE_STATE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world    = App_SharedStateMachine_GetWorld(state_machine);
    struct Odometer *odometer = App_BmsWorld_GetOdometer(world);

    App_AllStatesRunOnTick1Hz(state_machine);

    if (App_Odometer_TickCounter(odometer))
    {
        struct Eeprom *eeprom = App_BmsWorld_GetEeprom(world);
        App_Odometer_WriteValToEeprom(odometer, eeprom, ODOMETER_ADDRESS);
    }
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *world    = App_SharedStateMachine_GetWorld(state_machine);
        struct Imd *     imd      = App_BmsWorld_GetImd(world);
        struct Airs *    airs     = App_BmsWorld_GetAirs(world);
        struct Odometer *odometer = App_BmsWorld_GetOdometer(world);

        App_SetPeriodicCanSignals_Imd(imd);

        App_Odometer_UpdateReading(odometer);

        // if AIR- opens, go back to init state
        const bool air_negative_opened = !App_Airs_IsAirNegativeClosed(airs);
        if (air_negative_opened)
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
        }
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *world    = App_SharedStateMachine_GetWorld(state_machine);
    struct Airs *    airs     = App_BmsWorld_GetAirs(world);
    struct Eeprom *  eeprom   = App_BmsWorld_GetEeprom(world);
    struct Odometer *odometer = App_BmsWorld_GetOdometer(world);

    // AIR+ opens upon exiting drive state
    App_Airs_OpenAirPositive(airs);

    App_Odometer_WriteValToEeprom(odometer, eeprom, ODOMETER_ADDRESS);
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
