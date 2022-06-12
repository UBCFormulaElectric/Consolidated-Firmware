#include "states/App_AllStates.h"
#include "states/App_PreChargeState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

// Number of cycles before the voltage measurements settle to a stable value
#define NUM_CYCLES_FOR_V_TO_SETTLE (3U)
#define TS_DISCHARGED_THRESHOLD_V (10.0f)

static void App_BroadcastInitialEnergyInPack(struct BmsCanTxInterface *can_tx, struct Accumulator *accumulator);

static void App_BroadcastInitialEnergyInPack(struct BmsCanTxInterface *can_tx, struct Accumulator *accumulator)
{
    static bool is_first_time = true;

    if (is_first_time)
    {
        // Init the cell state of charge
        for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS; curr_segment++)
        {
            for (uint8_t curr_cell = 0U; curr_cell < NUM_OF_CELLS_PER_SEGMENT; curr_cell++)
            {
                App_Soc_InitCellEnergy(
                    curr_segment, curr_cell,
                    App_Accumulator_GetIndividualCellVoltage(accumulator, curr_segment, curr_cell));
            }
        }

        // Broadcast energy remaining in the pack
        uint8_t segment = 0U;
        uint8_t cell    = 0U;
        App_CanTx_SetPeriodicSignal_MIN_ENERGY_REMAINING(can_tx, App_Soc_GetMinCellEnergyInPercent(&segment, &cell));
        App_CanTx_SetPeriodicSignal_MIN_ENERGY_CELL(can_tx, cell);
        App_CanTx_SetPeriodicSignal_MIN_ENERGY_SEGMENT(can_tx, segment);
        App_CanTx_SetPeriodicSignal_MAX_ENERGY_REMAINING(can_tx, App_Soc_GetMaxCellEnergyInPercent(&segment, &cell));
        App_CanTx_SetPeriodicSignal_MAX_ENERGY_CELL(can_tx, cell);
        App_CanTx_SetPeriodicSignal_MAX_ENERGY_SEGMENT(can_tx, segment);

        is_first_time = false;
    }
}

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Clock *            clock            = App_BmsWorld_GetClock(world);
    struct Accumulator *      accumulator      = App_BmsWorld_GetAccumulator(world);
    struct OkStatus *         bms_ok_status    = App_BmsWorld_GetBmsOkStatus(world);

    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE);
    App_SharedClock_SetPreviousTimeInMilliseconds(clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    App_Accumulator_InitRunOnEntry(accumulator);
    App_OkStatus_Enable(bms_ok_status);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);

    struct BmsWorld *         world       = App_SharedStateMachine_GetWorld(state_machine);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);

    static uint8_t num_cycles_counter = 0U;

    // Wait for NUM_CYCLES_FOR_V_TO_SETTLE before broadcasting the amount of energy remaining in the pack
    if (num_cycles_counter < NUM_CYCLES_FOR_V_TO_SETTLE)
    {
        num_cycles_counter++;
    }
    else
    {
        App_BroadcastInitialEnergyInPack(can_tx, accumulator);
    }
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *      world = App_SharedStateMachine_GetWorld(state_machine);
        struct TractiveSystem *ts    = App_BmsWorld_GetTractiveSystem(world);
        struct Airs *          airs  = App_BmsWorld_GetAirs(world);

#ifndef BSPD_DEMO_MODE
        // don't allow pre_charge if in BSPD_DEMO_MODE
        if (App_Airs_IsAirNegativeClosed(airs) && (App_TractiveSystem_GetVoltage(ts) < TS_DISCHARGED_THRESHOLD_V))
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetPreChargeState());
        }
#endif
    }
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState(void)
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
