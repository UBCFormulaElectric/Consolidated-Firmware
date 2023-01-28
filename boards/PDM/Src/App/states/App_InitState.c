#include <math.h>

#include "states/App_AllStates.h"
#include "states/App_InitState.h"

#include "App_SharedMacros.h"
#include "App_SetPeriodicCanSignals.h"

#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_AllStates.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_DriveState.h"
#include "App_SharedMacros.h"

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *const          world  = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *const can_tx = App_PdmWorld_GetCanTx(world);
}

void Rail_Voltage_Checks_CANTX(struct PdmCanTxInterface *can_tx, struct RailMonitoring *rail_monitor)
{
    App_CanTx_SetPeriodicSignal_RAIL_22_V_AUX_VOLTAGE_LOW(
        can_tx, App_RailMonitoring__22V_AUX_VoltageLowCheck(rail_monitor));
    App_CanTx_SetPeriodicSignal_RAIL_VBAT_VOLTAGE_LOW(can_tx, App_RailMonitoring_VBAT_VoltageLowCheck(rail_monitor));
}

void Efuse_Enable_Disable_Channels(
    struct Efuse *efuse1,
    struct Efuse *efuse2,
    struct Efuse *efuse3,
    struct Efuse *efuse4)
{
    App_Efuse_EnableChannel0(efuse1);
    App_Efuse_EnableChannel1(efuse1);
    App_Efuse_EnableChannel0(efuse2);
    App_Efuse_DisableChannel1(efuse2);
    App_Efuse_EnableChannel0(efuse3);
    App_Efuse_EnableChannel1(efuse3);
    App_Efuse_DisableChannel0(efuse4);
    App_Efuse_DisableChannel1(efuse4);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world        = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx       = App_PdmWorld_GetCanTx(world);
    struct PdmCanRxInterface *can_rx       = App_PdmWorld_GetCanRx(world);
    struct RailMonitoring *   rail_monitor = App_PdmWorld_GetRailMonitoring(world);
    struct Efuse *            efuse1       = App_PdmWorld_GetEfuse1(world);
    struct Efuse *            efuse2       = App_PdmWorld_GetEfuse2(world);
    struct Efuse *            efuse3       = App_PdmWorld_GetEfuse3(world);
    struct Efuse *            efuse4       = App_PdmWorld_GetEfuse4(world);

    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        Efuse_Enable_Disable_Channels(efuse1, efuse2, efuse3, efuse4);

        Rail_Voltage_Checks_CANTX(can_tx, rail_monitor);

        if (App_RailMonitoring_VBAT_VoltageCriticalCheck(rail_monitor) ||
            App_RailMonitoring__22V_AUX_VoltageCriticalCheck(rail_monitor))
        {
            // HW SHUTDOWN?
        }
    }

    if (App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx) == CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE &&
        App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx) == CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
    }
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState(void)
{
    static struct State init_state = {
        .name              = "FAULT",
        .run_on_entry      = InitStateRunOnEntry,
        .run_on_tick_1Hz   = InitStateRunOnTick1Hz,
        .run_on_tick_100Hz = InitStateRunOnTick100Hz,
        .run_on_exit       = InitStateRunOnExit,
    };

    return &init_state;
}

/*
static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState()
{
    static struct State init_state = {
        .name              = "INIT",
        .run_on_entry      = InitStateRunOnEntry,
        .run_on_tick_1Hz   = InitStateRunOnTick1Hz,
        .run_on_tick_100Hz = InitStateRunOnTick100Hz,
        .run_on_exit       = InitStateRunOnExit,
    };

    return &init_state;
}
*/