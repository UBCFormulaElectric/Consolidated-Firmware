#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SetPeriodicCanSignals.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(PdmCanTxInterface)

void App_SetPeriodicCanSignals_CurrentChecks(struct PdmWorld *world)
{
    struct PdmCanTxInterface *can_tx = App_PdmWorld_GetCanTx(world);

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
        can_tx, aux1_current_check,
        App_CanTx_SetPeriodicSignal_AUXILIARY1_CURRENT,
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_AUX_1,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AUX_1);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, aux2_current_check,
        App_CanTx_SetPeriodicSignal_AUXILIARY2_CURRENT,
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_AUX_2,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AUX_2);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, left_inverter_current_check,
        App_CanTx_SetPeriodicSignal_LEFT_INVERTER_CURRENT,
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_LEFT_INVERTER,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_LEFT_INVERTER);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, right_inverter_current_check,
        App_CanTx_SetPeriodicSignal_RIGHT_INVERTER_CURRENT,
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_RIGHT_INVERTER,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_RIGHT_INVERTER);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, energy_meter_current_check,
        App_CanTx_SetPeriodicSignal_ENERGY_METER_CURRENT,
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_ENERGY_METER,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_ENERGY_METER);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, can_current_check, App_CanTx_SetPeriodicSignal_GLV_CURRENT,
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_CAN,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_CAN);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, air_shutdown_current_check,
        App_CanTx_SetPeriodicSignal_AIR_SHUTDOWN_CURRENT,
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_AIR_SHUTDOWN,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AIR_SHUTDOWN);
}

void App_SetPeriodicCanSignals_VoltageChecks(struct PdmWorld *world)
{
    struct PdmCanTxInterface *can_tx = App_PdmWorld_GetCanTx(world);

    struct InRangeCheck *vbat_voltage_check =
        App_PdmWorld_GetVbatInRangeCheck(world);
    struct InRangeCheck *_24v_aux_voltage_check =
        App_PdmWorld_Get24vAuxInRangeCheck(world);
    struct InRangeCheck *_24v_acc_voltage_check =
        App_PdmWorld_Get24vAccInRangeCheck(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, vbat_voltage_check, App_CanTx_SetPeriodicSignal_VBAT,
        App_CanTx_SetPeriodicSignal_UNDERVOLTAGE_VBAT,
        App_CanTx_SetPeriodicSignal_OVERVOLTAGE_VBAT);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, _24v_aux_voltage_check, App_CanTx_SetPeriodicSignal__24_V_AUX,
        App_CanTx_SetPeriodicSignal_UNDERVOLTAGE_24_V_AUX,
        App_CanTx_SetPeriodicSignal_OVERVOLTAGE_24_V_AUX);
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, _24v_acc_voltage_check, App_CanTx_SetPeriodicSignal__24_V_ACC,
        App_CanTx_SetPeriodicSignal_UNDERVOLTAGE_24_V_ACC,
        App_CanTx_SetPeriodicSignal_OVERVOLTAGE_24_V_ACC);
}
