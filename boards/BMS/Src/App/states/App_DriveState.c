#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void App_UpdateRemainingEnergyInPack(
    struct BmsCanTxInterface *can_tx,
    struct Accumulator *      accumulator,
    struct TractiveSystem *   ts);

static void App_UpdateRemainingEnergyInPack(
    struct BmsCanTxInterface *can_tx,
    struct Accumulator *      accumulator,
    struct TractiveSystem *   ts)
{
    // Update the energy level remaining within the cell
    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS; curr_segment++)
    {
        for (uint8_t curr_cell = 0U; curr_cell < NUM_OF_CELLS_PER_SEGMENT; curr_cell++)
        {
            App_Soc_UpdateCellEnergy(
                curr_segment, curr_cell, App_TractiveSystem_GetCurrent(ts),
                App_Accumulator_GetIndividualCellVoltage(accumulator, curr_segment, curr_cell), TASK_100HZ_PERIOD_S);
        }
    }

    uint8_t segment = 0U;
    uint8_t cell    = 0U;
    App_CanTx_SetPeriodicSignal_MIN_ENERGY_REMAINING(can_tx, App_Soc_GetMinCellEnergyInPercent(&segment, &cell));
    App_CanTx_SetPeriodicSignal_MIN_ENERGY_CELL(can_tx, cell);
    App_CanTx_SetPeriodicSignal_MIN_ENERGY_SEGMENT(can_tx, segment);
    App_CanTx_SetPeriodicSignal_MAX_ENERGY_REMAINING(can_tx, App_Soc_GetMaxCellEnergyInPercent(&segment, &cell));
    App_CanTx_SetPeriodicSignal_MAX_ENERGY_CELL(can_tx, cell);
    App_CanTx_SetPeriodicSignal_MAX_ENERGY_SEGMENT(can_tx, segment);
}

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *         world       = App_SharedStateMachine_GetWorld(state_machine);
        struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
        struct Imd *              imd         = App_BmsWorld_GetImd(world);
        struct Airs *             airs        = App_BmsWorld_GetAirs(world);
        struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
        struct TractiveSystem *   ts          = App_BmsWorld_GetTractiveSystem(world);

        App_SetPeriodicCanSignals_Imd(can_tx, imd);

        if (!App_Airs_IsAirNegativeClosed(airs))
        {
            // if AIR- opens, go back to fault state (AIR+ will be opened there)
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        }

        App_UpdateRemainingEnergyInPack(can_tx, accumulator, ts);
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
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
