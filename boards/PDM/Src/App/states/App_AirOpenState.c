#include "states/App_AirOpenState.h"

#include "App_SharedMacros.h"

static void AirOpenStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx_interface = App_PdmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_PDM_STATE_MACHINE_STATE_AIR_OPEN_CHOICE);
}

static void AirOpenStateRunOnTick(struct StateMachine *const state_machine)
{
    struct PdmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    struct PdmCanTxInterface *can_tx = App_PdmWorld_GetCanTx(world);
    struct InRangeCheck *     vbat_voltage_check =
        App_PdmWorld_GetVbatInRangeCheck(world);
    struct InRangeCheck *_24v_aux_voltage_check =
        App_PdmWorld_Get24vAuxInRangeCheck(world);
    struct InRangeCheck *_24v_acc_voltage_check =
        App_PdmWorld_Get24vAccInRangeCheck(world);
    struct InRangeCheck *aux1_current_check =
        App_PdmWorld_GetAux1CurrentCheck(world);
    struct InRangeCheck *aux2_current_check =
        App_PdmWorld_GetAux2CurrentCheck(world);
    struct InRangeCheck *left_inverter_current_check =
        App_PdmWorld_GetLeftInverterCurrentCheck(world);
    struct InRangeCheck *right_inverter_current_check =
        App_PdmWorld_GetRightInverterCurrentCheck(world);
    struct InRangeCheck *energy_meter_current_check =
        App_PdmWorld_GetEnergyMeterCurrentCheck(world);
    struct InRangeCheck *can_current_check =
        App_PdmWorld_GetCanCurrentCheck(world);
    struct InRangeCheck *air_shutdown_current_check =
        App_PdmWorld_GetAirShutdownCurrentCheck(world);

    float voltage_buffer;

    App_InRangeCheck_GetValue(vbat_voltage_check, &voltage_buffer);
    App_CanTx_SetPeriodicSignal_VBAT(can_tx, voltage_buffer);

    App_InRangeCheck_GetValue(_24v_aux_voltage_check, &voltage_buffer);
    App_CanTx_SetPeriodicSignal__24_V_AUX(can_tx, voltage_buffer);

    App_InRangeCheck_GetValue(_24v_acc_voltage_check, &voltage_buffer);
    App_CanTx_SetPeriodicSignal__24_V_ACC(can_tx, voltage_buffer);

    float current_buffer;

    App_InRangeCheck_GetValue(aux1_current_check, &current_buffer);
    App_CanTx_SetPeriodicSignal_AUXILIARY1_CURRENT(can_tx, current_buffer);

    App_InRangeCheck_GetValue(aux2_current_check, &current_buffer);
    App_CanTx_SetPeriodicSignal_AUXILIARY2_CURRENT(can_tx, current_buffer);

    App_InRangeCheck_GetValue(left_inverter_current_check, &current_buffer);
    App_CanTx_SetPeriodicSignal_LEFT_INVERTER_CURRENT(can_tx, current_buffer);

    App_InRangeCheck_GetValue(right_inverter_current_check, &current_buffer);
    App_CanTx_SetPeriodicSignal_RIGHT_INVERTER_CURRENT(can_tx, current_buffer);

    App_InRangeCheck_GetValue(energy_meter_current_check, &current_buffer);
    App_CanTx_SetPeriodicSignal_ENERGY_METER_CURRENT(can_tx, current_buffer);

    App_InRangeCheck_GetValue(can_current_check, &current_buffer);
    App_CanTx_SetPeriodicSignal_GLV_CURRENT(can_tx, current_buffer);

    App_InRangeCheck_GetValue(air_shutdown_current_check, &current_buffer);
    App_CanTx_SetPeriodicSignal_AIR_SHUTDOWN_CURRENT(can_tx, current_buffer);
}

static void AirOpenStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetAirOpenState(void)
{
    static struct State air_open_state = {
        .name         = "AIR OPEN",
        .run_on_entry = AirOpenStateRunOnEntry,
        .run_on_tick  = AirOpenStateRunOnTick,
        .run_on_exit  = AirOpenStateRunOnExit,
    };

    return &air_open_state;
}
