#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SetPeriodicCanSignals.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(PdmCanTxInterface)

void App_SetPeriodicCanSignals_CurrentInRangeChecks(const struct PdmWorld *world)
{
    struct PdmCanTxInterface *can_tx = App_PdmWorld_GetCanTx(world);

    struct InRangeCheck *aux1_current_in_range_check          = App_PdmWorld_GetAux1CurrentInRangeCheck(world);
    struct InRangeCheck *aux2_current_in_range_check          = App_PdmWorld_GetAux2CurrentInRangeCheck(world);
    struct InRangeCheck *left_inverter_current_in_range_check = App_PdmWorld_GetLeftInverterCurrentInRangeCheck(world);
    struct InRangeCheck *right_inverter_current_in_range_check =
        App_PdmWorld_GetRightInverterCurrentInRangeCheck(world);
    struct InRangeCheck *energy_meter_current_in_range_check = App_PdmWorld_GetEnergyMeterCurrentInRangeCheck(world);
    struct InRangeCheck *can_current_in_range_check          = App_PdmWorld_GetCanCurrentInRangeCheck(world);
    struct InRangeCheck *air_shutdown_current_in_range_check = App_PdmWorld_GetAirShutdownCurrentInRangeCheck(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, aux1_current_in_range_check, App_CanTx_SetPeriodicSignal_AUXILIARY1_CURRENT,
        App_CanTx_SetPeriodicSignal_AUX1_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, aux2_current_in_range_check, App_CanTx_SetPeriodicSignal_AUXILIARY2_CURRENT,
        App_CanTx_SetPeriodicSignal_AUX2_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, left_inverter_current_in_range_check, App_CanTx_SetPeriodicSignal_LEFT_INVERTER_CURRENT,
        App_CanTx_SetPeriodicSignal_LEFT_INVERTER_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, right_inverter_current_in_range_check, App_CanTx_SetPeriodicSignal_RIGHT_INVERTER_CURRENT,
        App_CanTx_SetPeriodicSignal_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, energy_meter_current_in_range_check, App_CanTx_SetPeriodicSignal_ENERGY_METER_CURRENT,
        App_CanTx_SetPeriodicSignal_ENERGY_METER_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, can_current_in_range_check, App_CanTx_SetPeriodicSignal_CAN_CURRENT,
        App_CanTx_SetPeriodicSignal_CAN_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_CAN_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_CAN_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_CAN_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, air_shutdown_current_in_range_check, App_CanTx_SetPeriodicSignal_AIR_SHUTDOWN_CURRENT,
        App_CanTx_SetPeriodicSignal_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

void App_SetPeriodicCanSignals_VoltageInRangeChecks(const struct PdmWorld *world)
{
    struct PdmCanTxInterface *can_tx = App_PdmWorld_GetCanTx(world);

    struct InRangeCheck *vbat_voltage_in_range_check     = App_PdmWorld_GetVbatVoltageInRangeCheck(world);
    struct InRangeCheck *_24v_aux_voltage_in_range_check = App_PdmWorld_Get24vAuxVoltageInRangeCheck(world);
    struct InRangeCheck *_24v_acc_voltage_in_range_check = App_PdmWorld_Get24vAccVoltageInRangeCheck(world);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, vbat_voltage_in_range_check, App_CanTx_SetPeriodicSignal_VBAT,
        App_CanTx_SetPeriodicSignal_VBAT_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_VBAT_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_VBAT_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_VBAT_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, _24v_aux_voltage_in_range_check, App_CanTx_SetPeriodicSignal__24_V_AUX,
        App_CanTx_SetPeriodicSignal__24_V_AUX_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_AUX_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_AUX_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_AUX_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, _24v_acc_voltage_in_range_check, App_CanTx_SetPeriodicSignal__24_V_ACC,
        App_CanTx_SetPeriodicSignal__24_V_ACC_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_ACC_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_ACC_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_ACC_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
