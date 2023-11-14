/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "App_CanRx.h"

/* ------------------------------- Structs -------------------------------- */

/**
 * Struct for holding all messages received by PDM (i.e. the RX table).
 */
typedef struct
{
    DIM_Warnings_Signals DIM_Warnings_signals;
    DIM_Faults_Signals DIM_Faults_signals;
    DCM_Warnings_Signals DCM_Warnings_signals;
    DCM_Faults_Signals DCM_Faults_signals;
    BMS_Vitals_Signals BMS_Vitals_signals;
    BMS_Contactors_Signals BMS_Contactors_signals;
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    Debug_CanMode_Signals Debug_CanMode_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
} PDM_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static PDM_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(PDM_RxMsgs));
    App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_DIM_WatchdogTimeoutWarning_Update(CANSIG_DIM_WARNINGS_DIM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_DIM_TxOverflowWarning_Update(CANSIG_DIM_WARNINGS_DIM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_DIM_RxOverflowWarning_Update(CANSIG_DIM_WARNINGS_DIM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_DIM_MissingHeartbeatFault_Update(CANSIG_DIM_FAULTS_DIM_MISSING_HEARTBEAT_FAULT_START_VAL);
    App_CanRx_DCM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_DCM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_DCM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_DCM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_DCM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_DCM_WatchdogTimeoutWarning_Update(CANSIG_DCM_WARNINGS_DCM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_DCM_TxOverflowWarning_Update(CANSIG_DCM_WARNINGS_DCM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_DCM_RxOverflowWarning_Update(CANSIG_DCM_WARNINGS_DCM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_DCM_MissingHeartbeatFault_Update(CANSIG_DCM_FAULTS_DCM_MISSING_HEARTBEAT_FAULT_START_VAL);
    App_CanRx_DCM_LeftInverterFault_Update(CANSIG_DCM_FAULTS_DCM_LEFT_INVERTER_FAULT_START_VAL);
    App_CanRx_DCM_RightInverterFault_Update(CANSIG_DCM_FAULTS_DCM_RIGHT_INVERTER_FAULT_START_VAL);
    App_CanRx_BMS_Heartbeat_Update(CANSIG_BMS_VITALS_BMS_HEARTBEAT_START_VAL);
    App_CanRx_BMS_State_Update(CANSIG_BMS_VITALS_BMS_STATE_START_VAL);
    App_CanRx_BMS_AirPositive_Update(CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_START_VAL);
    App_CanRx_BMS_AirNegative_Update(CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_START_VAL);
    App_CanRx_BMS_PrechargeRelay_Update(CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_START_VAL);
    App_CanRx_BMS_StackWaterMarkHighTask1HzWarning_Update(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_BMS_StackWaterMarkHighTask100HzWarning_Update(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_BMS_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_BMS_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_BMS_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_BMS_WatchdogTimeoutWarning_Update(CANSIG_BMS_WARNINGS_BMS_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_BMS_TxOverflowWarning_Update(CANSIG_BMS_WARNINGS_BMS_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_BMS_RxOverflowWarning_Update(CANSIG_BMS_WARNINGS_BMS_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_BMS_MissingHeartbeatFault_Update(CANSIG_BMS_FAULTS_BMS_MISSING_HEARTBEAT_FAULT_START_VAL);
    App_CanRx_BMS_StateMachineFault_Update(CANSIG_BMS_FAULTS_BMS_STATE_MACHINE_FAULT_START_VAL);
    App_CanRx_BMS_CellUndervoltageFault_Update(CANSIG_BMS_FAULTS_BMS_CELL_UNDERVOLTAGE_FAULT_START_VAL);
    App_CanRx_BMS_CellOvervoltageFault_Update(CANSIG_BMS_FAULTS_BMS_CELL_OVERVOLTAGE_FAULT_START_VAL);
    App_CanRx_BMS_ModuleCommunicationFault_Update(CANSIG_BMS_FAULTS_BMS_MODULE_COMMUNICATION_FAULT_START_VAL);
    App_CanRx_BMS_CellUndertempFault_Update(CANSIG_BMS_FAULTS_BMS_CELL_UNDERTEMP_FAULT_START_VAL);
    App_CanRx_BMS_CellOvertempFault_Update(CANSIG_BMS_FAULTS_BMS_CELL_OVERTEMP_FAULT_START_VAL);
    App_CanRx_BMS_ChargerFault_Update(CANSIG_BMS_FAULTS_BMS_CHARGER_FAULT_START_VAL);
    App_CanRx_BMS_ChargerDisconnectedDuringChargeFault_Update(CANSIG_BMS_FAULTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_START_VAL);
    App_CanRx_BMS_ChargerExternalShutdownFault_Update(CANSIG_BMS_FAULTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_START_VAL);
    App_CanRx_BMS_TractiveSystemOvercurrentFault_Update(CANSIG_BMS_FAULTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_START_VAL);
    App_CanRx_BMS_PrechargeFailureFault_Update(CANSIG_BMS_FAULTS_BMS_PRECHARGE_FAILURE_FAULT_START_VAL);
    App_CanRx_Debug_EnableDebugMode_Update(CANSIG_DEBUG_CAN_MODE_DEBUG_ENABLE_DEBUG_MODE_START_VAL);
    App_CanRx_FSM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_FSM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_FSM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_FSM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_FSM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_FSM_WatchdogTimeoutWarning_Update(CANSIG_FSM_WARNINGS_FSM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_FSM_TxOverflowWarning_Update(CANSIG_FSM_WARNINGS_FSM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_FSM_RxOverflowWarning_Update(CANSIG_FSM_WARNINGS_FSM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_FSM_LeftWheelSpeedOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_RightWheelSpeedOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_SteeringAngleOCSCWarning_Update(CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OCSC_WARNING_START_VAL);
    App_CanRx_FSM_SteeringAngleOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_FrontBrakePressureOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_RearBrakePressureOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_FlowRateOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_BrakeAppsDisagreementWarning_Update(CANSIG_FSM_WARNINGS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_START_VAL);
    App_CanRx_FSM_AppsDisagreementWarning_Update(CANSIG_FSM_WARNINGS_FSM_APPS_DISAGREEMENT_WARNING_START_VAL);
    App_CanRx_FSM_MissingHeartbeatFault_Update(CANSIG_FSM_FAULTS_FSM_MISSING_HEARTBEAT_FAULT_START_VAL);
    App_CanRx_FSM_StateMachineFault_Update(CANSIG_FSM_FAULTS_FSM_STATE_MACHINE_FAULT_START_VAL);
    App_CanRx_FSM_PappsOCSCFault_Update(CANSIG_FSM_FAULTS_FSM_PAPPS_OCSC_FAULT_START_VAL);
    App_CanRx_FSM_SappsOCSCFault_Update(CANSIG_FSM_FAULTS_FSM_SAPPS_OCSC_FAULT_START_VAL);
    App_CanRx_FSM_FlowMeterUnderflowFault_Update(CANSIG_FSM_FAULTS_FSM_FLOW_METER_UNDERFLOW_FAULT_START_VAL);
    App_CanRx_FSM_TorquePlausabilityFailedFault_Update(CANSIG_FSM_FAULTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_START_VAL);
}

void App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanRx_DIM_WatchdogTimeoutWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_WatchdogTimeoutWarning_value = value;
}

void App_CanRx_DIM_TxOverflowWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_TxOverflowWarning_value = value;
}

void App_CanRx_DIM_RxOverflowWarning_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_RxOverflowWarning_value = value;
}

void App_CanRx_DIM_MissingHeartbeatFault_Update(bool value)
{
    rx_table.DIM_Faults_signals.DIM_MissingHeartbeatFault_value = value;
}

void App_CanRx_DCM_StackWaterMarkHighTask1HzWarning_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanRx_DCM_StackWaterMarkHighTask100HzWarning_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanRx_DCM_StackWaterMarkHighTask1kHzWarning_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanRx_DCM_StackWaterMarkHighTaskCanRxWarning_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanRx_DCM_StackWaterMarkHighTaskCanTxWarning_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanRx_DCM_WatchdogTimeoutWarning_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_WatchdogTimeoutWarning_value = value;
}

void App_CanRx_DCM_TxOverflowWarning_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_TxOverflowWarning_value = value;
}

void App_CanRx_DCM_RxOverflowWarning_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_RxOverflowWarning_value = value;
}

void App_CanRx_DCM_MissingHeartbeatFault_Update(bool value)
{
    rx_table.DCM_Faults_signals.DCM_MissingHeartbeatFault_value = value;
}

void App_CanRx_DCM_LeftInverterFault_Update(bool value)
{
    rx_table.DCM_Faults_signals.DCM_LeftInverterFault_value = value;
}

void App_CanRx_DCM_RightInverterFault_Update(bool value)
{
    rx_table.DCM_Faults_signals.DCM_RightInverterFault_value = value;
}

void App_CanRx_BMS_Heartbeat_Update(bool value)
{
    rx_table.BMS_Vitals_signals.BMS_Heartbeat_value = value;
}

void App_CanRx_BMS_State_Update(BmsState value)
{
    rx_table.BMS_Vitals_signals.BMS_State_value = (value > CANSIG_BMS_VITALS_BMS_STATE_MAX) ? CANSIG_BMS_VITALS_BMS_STATE_MAX : value;
}

void App_CanRx_BMS_AirPositive_Update(ContactorState value)
{
    rx_table.BMS_Contactors_signals.BMS_AirPositive_value = (value > CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_MAX) ? CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_MAX : value;
}

void App_CanRx_BMS_AirNegative_Update(ContactorState value)
{
    rx_table.BMS_Contactors_signals.BMS_AirNegative_value = (value > CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_MAX) ? CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_MAX : value;
}

void App_CanRx_BMS_PrechargeRelay_Update(ContactorState value)
{
    rx_table.BMS_Contactors_signals.BMS_PrechargeRelay_value = (value > CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_MAX) ? CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_MAX : value;
}

void App_CanRx_BMS_StackWaterMarkHighTask1HzWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanRx_BMS_StackWaterMarkHighTask100HzWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanRx_BMS_StackWaterMarkHighTask1kHzWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanRx_BMS_StackWaterMarkHighTaskCanRxWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanRx_BMS_StackWaterMarkHighTaskCanTxWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanRx_BMS_WatchdogTimeoutWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WatchdogTimeoutWarning_value = value;
}

void App_CanRx_BMS_TxOverflowWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_TxOverflowWarning_value = value;
}

void App_CanRx_BMS_RxOverflowWarning_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_RxOverflowWarning_value = value;
}

void App_CanRx_BMS_MissingHeartbeatFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_MissingHeartbeatFault_value = value;
}

void App_CanRx_BMS_StateMachineFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_StateMachineFault_value = value;
}

void App_CanRx_BMS_CellUndervoltageFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_CellUndervoltageFault_value = value;
}

void App_CanRx_BMS_CellOvervoltageFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_CellOvervoltageFault_value = value;
}

void App_CanRx_BMS_ModuleCommunicationFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_ModuleCommunicationFault_value = value;
}

void App_CanRx_BMS_CellUndertempFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_CellUndertempFault_value = value;
}

void App_CanRx_BMS_CellOvertempFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_CellOvertempFault_value = value;
}

void App_CanRx_BMS_ChargerFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_ChargerFault_value = value;
}

void App_CanRx_BMS_ChargerDisconnectedDuringChargeFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_ChargerDisconnectedDuringChargeFault_value = value;
}

void App_CanRx_BMS_ChargerExternalShutdownFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_ChargerExternalShutdownFault_value = value;
}

void App_CanRx_BMS_TractiveSystemOvercurrentFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_TractiveSystemOvercurrentFault_value = value;
}

void App_CanRx_BMS_PrechargeFailureFault_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_PrechargeFailureFault_value = value;
}

void App_CanRx_Debug_EnableDebugMode_Update(bool value)
{
    rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value = value;
}

void App_CanRx_FSM_StackWaterMarkHighTask1HzWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanRx_FSM_StackWaterMarkHighTask100HzWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanRx_FSM_StackWaterMarkHighTask1kHzWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanRx_FSM_StackWaterMarkHighTaskCanRxWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanRx_FSM_StackWaterMarkHighTaskCanTxWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanRx_FSM_WatchdogTimeoutWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WatchdogTimeoutWarning_value = value;
}

void App_CanRx_FSM_TxOverflowWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_TxOverflowWarning_value = value;
}

void App_CanRx_FSM_RxOverflowWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_RxOverflowWarning_value = value;
}

void App_CanRx_FSM_LeftWheelSpeedOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_LeftWheelSpeedOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_RightWheelSpeedOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_RightWheelSpeedOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_SteeringAngleOCSCWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_SteeringAngleOCSCWarning_value = value;
}

void App_CanRx_FSM_SteeringAngleOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_SteeringAngleOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_FrontBrakePressureOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_FrontBrakePressureOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_RearBrakePressureOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_RearBrakePressureOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_FlowRateOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_FlowRateOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_BrakeAppsDisagreementWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_BrakeAppsDisagreementWarning_value = value;
}

void App_CanRx_FSM_AppsDisagreementWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_AppsDisagreementWarning_value = value;
}

void App_CanRx_FSM_MissingHeartbeatFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_MissingHeartbeatFault_value = value;
}

void App_CanRx_FSM_StateMachineFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_StateMachineFault_value = value;
}

void App_CanRx_FSM_PappsOCSCFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_PappsOCSCFault_value = value;
}

void App_CanRx_FSM_SappsOCSCFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_SappsOCSCFault_value = value;
}

void App_CanRx_FSM_FlowMeterUnderflowFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FlowMeterUnderflowFault_value = value;
}

void App_CanRx_FSM_TorquePlausabilityFailedFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_TorquePlausabilityFailedFault_value = value;
}

bool App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanRx_DIM_WatchdogTimeoutWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_WatchdogTimeoutWarning_value;
}

bool App_CanRx_DIM_TxOverflowWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_TxOverflowWarning_value;
}

bool App_CanRx_DIM_RxOverflowWarning_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_RxOverflowWarning_value;
}

bool App_CanRx_DIM_MissingHeartbeatFault_Get()
{
    return rx_table.DIM_Faults_signals.DIM_MissingHeartbeatFault_value;
}

bool App_CanRx_DCM_StackWaterMarkHighTask1HzWarning_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanRx_DCM_StackWaterMarkHighTask100HzWarning_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanRx_DCM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanRx_DCM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanRx_DCM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanRx_DCM_WatchdogTimeoutWarning_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_WatchdogTimeoutWarning_value;
}

bool App_CanRx_DCM_TxOverflowWarning_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_TxOverflowWarning_value;
}

bool App_CanRx_DCM_RxOverflowWarning_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_RxOverflowWarning_value;
}

bool App_CanRx_DCM_MissingHeartbeatFault_Get()
{
    return rx_table.DCM_Faults_signals.DCM_MissingHeartbeatFault_value;
}

bool App_CanRx_DCM_LeftInverterFault_Get()
{
    return rx_table.DCM_Faults_signals.DCM_LeftInverterFault_value;
}

bool App_CanRx_DCM_RightInverterFault_Get()
{
    return rx_table.DCM_Faults_signals.DCM_RightInverterFault_value;
}

bool App_CanRx_BMS_Heartbeat_Get()
{
    return rx_table.BMS_Vitals_signals.BMS_Heartbeat_value;
}

BmsState App_CanRx_BMS_State_Get()
{
    return rx_table.BMS_Vitals_signals.BMS_State_value;
}

ContactorState App_CanRx_BMS_AirPositive_Get()
{
    return rx_table.BMS_Contactors_signals.BMS_AirPositive_value;
}

ContactorState App_CanRx_BMS_AirNegative_Get()
{
    return rx_table.BMS_Contactors_signals.BMS_AirNegative_value;
}

ContactorState App_CanRx_BMS_PrechargeRelay_Get()
{
    return rx_table.BMS_Contactors_signals.BMS_PrechargeRelay_value;
}

bool App_CanRx_BMS_StackWaterMarkHighTask1HzWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanRx_BMS_StackWaterMarkHighTask100HzWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanRx_BMS_StackWaterMarkHighTask1kHzWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanRx_BMS_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanRx_BMS_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanRx_BMS_WatchdogTimeoutWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WatchdogTimeoutWarning_value;
}

bool App_CanRx_BMS_TxOverflowWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_TxOverflowWarning_value;
}

bool App_CanRx_BMS_RxOverflowWarning_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_RxOverflowWarning_value;
}

bool App_CanRx_BMS_MissingHeartbeatFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_MissingHeartbeatFault_value;
}

bool App_CanRx_BMS_StateMachineFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_StateMachineFault_value;
}

bool App_CanRx_BMS_CellUndervoltageFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_CellUndervoltageFault_value;
}

bool App_CanRx_BMS_CellOvervoltageFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_CellOvervoltageFault_value;
}

bool App_CanRx_BMS_ModuleCommunicationFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_ModuleCommunicationFault_value;
}

bool App_CanRx_BMS_CellUndertempFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_CellUndertempFault_value;
}

bool App_CanRx_BMS_CellOvertempFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_CellOvertempFault_value;
}

bool App_CanRx_BMS_ChargerFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_ChargerFault_value;
}

bool App_CanRx_BMS_ChargerDisconnectedDuringChargeFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_ChargerDisconnectedDuringChargeFault_value;
}

bool App_CanRx_BMS_ChargerExternalShutdownFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_ChargerExternalShutdownFault_value;
}

bool App_CanRx_BMS_TractiveSystemOvercurrentFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_TractiveSystemOvercurrentFault_value;
}

bool App_CanRx_BMS_PrechargeFailureFault_Get()
{
    return rx_table.BMS_Faults_signals.BMS_PrechargeFailureFault_value;
}

bool App_CanRx_Debug_EnableDebugMode_Get()
{
    return rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value;
}

bool App_CanRx_FSM_StackWaterMarkHighTask1HzWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanRx_FSM_StackWaterMarkHighTask100HzWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanRx_FSM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanRx_FSM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanRx_FSM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanRx_FSM_WatchdogTimeoutWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WatchdogTimeoutWarning_value;
}

bool App_CanRx_FSM_TxOverflowWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_TxOverflowWarning_value;
}

bool App_CanRx_FSM_RxOverflowWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_RxOverflowWarning_value;
}

bool App_CanRx_FSM_LeftWheelSpeedOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_LeftWheelSpeedOutOfRangeWarning_value;
}

bool App_CanRx_FSM_RightWheelSpeedOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_RightWheelSpeedOutOfRangeWarning_value;
}

bool App_CanRx_FSM_SteeringAngleOCSCWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_SteeringAngleOCSCWarning_value;
}

bool App_CanRx_FSM_SteeringAngleOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_SteeringAngleOutOfRangeWarning_value;
}

bool App_CanRx_FSM_FrontBrakePressureOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_FrontBrakePressureOutOfRangeWarning_value;
}

bool App_CanRx_FSM_RearBrakePressureOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_RearBrakePressureOutOfRangeWarning_value;
}

bool App_CanRx_FSM_FlowRateOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_FlowRateOutOfRangeWarning_value;
}

bool App_CanRx_FSM_BrakeAppsDisagreementWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_BrakeAppsDisagreementWarning_value;
}

bool App_CanRx_FSM_AppsDisagreementWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_AppsDisagreementWarning_value;
}

bool App_CanRx_FSM_MissingHeartbeatFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_MissingHeartbeatFault_value;
}

bool App_CanRx_FSM_StateMachineFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_StateMachineFault_value;
}

bool App_CanRx_FSM_PappsOCSCFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_PappsOCSCFault_value;
}

bool App_CanRx_FSM_SappsOCSCFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_SappsOCSCFault_value;
}

bool App_CanRx_FSM_FlowMeterUnderflowFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FlowMeterUnderflowFault_value;
}

bool App_CanRx_FSM_TorquePlausabilityFailedFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_TorquePlausabilityFailedFault_value;
}

