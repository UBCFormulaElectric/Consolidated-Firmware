#include "states/App_AllStates.h"

#include "App_SharedMacros.h"
#include "App_SetPeriodicCanSignals.h"

void AllStatesRunOnEntry(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

void AllStatesRunOnTick(struct StateMachine *const state_machine)
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

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, vbat_voltage_check, App_CanTx_SetPeriodicSignal_VBAT);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, _24v_aux_voltage_check, App_CanTx_SetPeriodicSignal__24_V_AUX);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, _24v_acc_voltage_check, App_CanTx_SetPeriodicSignal__24_V_ACC);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, aux1_current_check,
        App_CanTx_SetPeriodicSignal_AUXILIARY1_CURRENT);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, aux2_current_check,
        App_CanTx_SetPeriodicSignal_AUXILIARY2_CURRENT);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, left_inverter_current_check,
        App_CanTx_SetPeriodicSignal_LEFT_INVERTER_CURRENT);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, right_inverter_current_check,
        App_CanTx_SetPeriodicSignal_RIGHT_INVERTER_CURRENT);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, energy_meter_current_check,
        App_CanTx_SetPeriodicSignal_ENERGY_METER_CURRENT);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, can_current_check, App_CanTx_SetPeriodicSignal_GLV_CURRENT);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, air_shutdown_current_check,
        App_CanTx_SetPeriodicSignal_AIR_SHUTDOWN_CURRENT);
}

void AllStatesRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}
