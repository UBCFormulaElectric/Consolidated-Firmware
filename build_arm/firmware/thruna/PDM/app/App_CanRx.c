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
    DCM_Warnings_Signals DCM_Warnings_signals;
    DCM_Faults_Signals DCM_Faults_signals;
    DIM_Warnings_Signals DIM_Warnings_signals;
    DIM_Faults_Signals DIM_Faults_signals;
    BMS_Vitals_Signals BMS_Vitals_signals;
    BMS_Contactors_Signals BMS_Contactors_signals;
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
    Debug_CanMode_Signals Debug_CanMode_signals;
} PDM_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static PDM_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(PDM_RxMsgs));
    App_CanRx_DCM_Warning_StackWaterMarkHighTask1Hz_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanRx_DCM_Warning_StackWaterMarkHighTask100Hz_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanRx_DCM_Warning_StackWaterMarkHighTask1kHz_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanRx_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanTx_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanRx_DCM_Warning_WatchdogTimeout_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_DCM_Warning_TxOverflow_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_DCM_Warning_RxOverflow_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_DCM_Fault_MissingHeartbeat_Update(CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_DCM_Fault_LeftInverterFault_Update(CANSIG_DCM_FAULTS_DCM_FAULT_LEFT_INVERTER_FAULT_START_VAL);
    App_CanRx_DCM_Fault_RightInverterFault_Update(CANSIG_DCM_FAULTS_DCM_FAULT_RIGHT_INVERTER_FAULT_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanRx_DIM_Warning_WatchdogTimeout_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_DIM_Warning_TxOverflow_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_DIM_Warning_RxOverflow_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_DIM_Fault_MissingHeartbeat_Update(CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_BMS_Heartbeat_Update(CANSIG_BMS_VITALS_BMS_HEARTBEAT_START_VAL);
    App_CanRx_BMS_State_Update(CANSIG_BMS_VITALS_BMS_STATE_START_VAL);
    App_CanRx_BMS_AirPositive_Update(CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_START_VAL);
    App_CanRx_BMS_AirNegative_Update(CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_START_VAL);
    App_CanRx_BMS_PrechargeRelay_Update(CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_START_VAL);
    App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanRx_BMS_Warning_WatchdogTimeout_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_BMS_Warning_TxOverflow_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_BMS_Warning_RxOverflow_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_BMS_Fault_MissingHeartbeat_Update(CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_BMS_Fault_StateMachine_Update(CANSIG_BMS_FAULTS_BMS_FAULT_STATE_MACHINE_START_VAL);
    App_CanRx_BMS_Fault_CellUndervoltage_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_START_VAL);
    App_CanRx_BMS_Fault_CellOvervoltage_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_START_VAL);
    App_CanRx_BMS_Fault_ModuleCommunicationError_Update(CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_START_VAL);
    App_CanRx_BMS_Fault_CellUndertemp_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_START_VAL);
    App_CanRx_BMS_Fault_CellOvertemp_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_START_VAL);
    App_CanRx_BMS_Fault_Charger_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_START_VAL);
    App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_START_VAL);
    App_CanRx_BMS_Fault_ChargerExternalShutdown_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_START_VAL);
    App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Update(CANSIG_BMS_FAULTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_START_VAL);
    App_CanRx_BMS_Fault_PrechargeFailure_Update(CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_FAILURE_START_VAL);
    App_CanRx_FSM_Warning_StackWaterMarkHighTask1Hz_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanRx_FSM_Warning_StackWaterMarkHighTask100Hz_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanRx_FSM_Warning_StackWaterMarkHighTask1kHz_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanRx_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanTx_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanRx_FSM_Warning_WatchdogTimeout_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_FSM_Warning_TxOverflow_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_FSM_Warning_RxOverflow_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_FSM_Warning_LeftWheelSpeedOutOfRange_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warning_RightWheelSpeedOutOfRange_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OCSC_START_VAL);
    App_CanRx_FSM_Warning_SteeringAngleOutOfRange_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warning_FrontBrakePressureOutOfRange_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanRx_FSM_Warning_FlowRateOutOfRange_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warning_BrakeAppsDisagreement_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_START_VAL);
    App_CanRx_FSM_Warning_AppsDisagreement_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_APPS_DISAGREEMENT_START_VAL);
    App_CanRx_FSM_Fault_MissingHeartbeat_Update(CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_FSM_Fault_StateMachine_Update(CANSIG_FSM_FAULTS_FSM_FAULT_STATE_MACHINE_START_VAL);
    App_CanRx_FSM_Fault_PappsOCSC_Update(CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_OCSC_START_VAL);
    App_CanRx_FSM_Fault_SappsOCSCFault_Update(CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_OCSC_FAULT_START_VAL);
    App_CanRx_FSM_Fault_FlowMeterUnderflow_Update(CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_UNDERFLOW_START_VAL);
    App_CanRx_FSM_Fault_TorquePlausabilityFailed_Update(CANSIG_FSM_FAULTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_START_VAL);
    App_CanRx_Debug_EnableDebugMode_Update(CANSIG_DEBUG_CAN_MODE_DEBUG_ENABLE_DEBUG_MODE_START_VAL);
}

void App_CanRx_DCM_Warning_StackWaterMarkHighTask1Hz_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanRx_DCM_Warning_StackWaterMarkHighTask100Hz_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanRx_DCM_Warning_StackWaterMarkHighTask1kHz_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanRx_DCM_Warning_WatchdogTimeout_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_Warning_WatchdogTimeout_value = value;
}

void App_CanRx_DCM_Warning_TxOverflow_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_Warning_TxOverflow_value = value;
}

void App_CanRx_DCM_Warning_RxOverflow_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_Warning_RxOverflow_value = value;
}

void App_CanRx_DCM_Fault_MissingHeartbeat_Update(bool value)
{
    rx_table.DCM_Faults_signals.DCM_Fault_MissingHeartbeat_value = value;
}

void App_CanRx_DCM_Fault_LeftInverterFault_Update(bool value)
{
    rx_table.DCM_Faults_signals.DCM_Fault_LeftInverterFault_value = value;
}

void App_CanRx_DCM_Fault_RightInverterFault_Update(bool value)
{
    rx_table.DCM_Faults_signals.DCM_Fault_RightInverterFault_value = value;
}

void App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanRx_DIM_Warning_WatchdogTimeout_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_WatchdogTimeout_value = value;
}

void App_CanRx_DIM_Warning_TxOverflow_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_TxOverflow_value = value;
}

void App_CanRx_DIM_Warning_RxOverflow_Update(bool value)
{
    rx_table.DIM_Warnings_signals.DIM_Warning_RxOverflow_value = value;
}

void App_CanRx_DIM_Fault_MissingHeartbeat_Update(bool value)
{
    rx_table.DIM_Faults_signals.DIM_Fault_MissingHeartbeat_value = value;
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

void App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanRx_BMS_Warning_WatchdogTimeout_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_WatchdogTimeout_value = value;
}

void App_CanRx_BMS_Warning_TxOverflow_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_TxOverflow_value = value;
}

void App_CanRx_BMS_Warning_RxOverflow_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_RxOverflow_value = value;
}

void App_CanRx_BMS_Fault_MissingHeartbeat_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_MissingHeartbeat_value = value;
}

void App_CanRx_BMS_Fault_StateMachine_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_StateMachine_value = value;
}

void App_CanRx_BMS_Fault_CellUndervoltage_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellUndervoltage_value = value;
}

void App_CanRx_BMS_Fault_CellOvervoltage_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellOvervoltage_value = value;
}

void App_CanRx_BMS_Fault_ModuleCommunicationError_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_ModuleCommunicationError_value = value;
}

void App_CanRx_BMS_Fault_CellUndertemp_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellUndertemp_value = value;
}

void App_CanRx_BMS_Fault_CellOvertemp_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellOvertemp_value = value;
}

void App_CanRx_BMS_Fault_Charger_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_Charger_value = value;
}

void App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_ChargerDisconnectedDuringCharge_value = value;
}

void App_CanRx_BMS_Fault_ChargerExternalShutdown_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_ChargerExternalShutdown_value = value;
}

void App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_TractiveSystemOvercurrent_value = value;
}

void App_CanRx_BMS_Fault_PrechargeFailure_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_PrechargeFailure_value = value;
}

void App_CanRx_FSM_Warning_StackWaterMarkHighTask1Hz_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanRx_FSM_Warning_StackWaterMarkHighTask100Hz_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanRx_FSM_Warning_StackWaterMarkHighTask1kHz_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanRx_FSM_Warning_WatchdogTimeout_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_WatchdogTimeout_value = value;
}

void App_CanRx_FSM_Warning_TxOverflow_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_TxOverflow_value = value;
}

void App_CanRx_FSM_Warning_RxOverflow_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_RxOverflow_value = value;
}

void App_CanRx_FSM_Warning_LeftWheelSpeedOutOfRange_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_LeftWheelSpeedOutOfRange_value = value;
}

void App_CanRx_FSM_Warning_RightWheelSpeedOutOfRange_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_RightWheelSpeedOutOfRange_value = value;
}

void App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOCSC_value = value;
}

void App_CanRx_FSM_Warning_SteeringAngleOutOfRange_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOutOfRange_value = value;
}

void App_CanRx_FSM_Warning_FrontBrakePressureOutOfRange_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_FrontBrakePressureOutOfRange_value = value;
}

void App_CanRx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_RearBrakePressureOutOfRangeWarning_value = value;
}

void App_CanRx_FSM_Warning_FlowRateOutOfRange_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_FlowRateOutOfRange_value = value;
}

void App_CanRx_FSM_Warning_BrakeAppsDisagreement_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_BrakeAppsDisagreement_value = value;
}

void App_CanRx_FSM_Warning_AppsDisagreement_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_AppsDisagreement_value = value;
}

void App_CanRx_FSM_Fault_MissingHeartbeat_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_MissingHeartbeat_value = value;
}

void App_CanRx_FSM_Fault_StateMachine_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_StateMachine_value = value;
}

void App_CanRx_FSM_Fault_PappsOCSC_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_PappsOCSC_value = value;
}

void App_CanRx_FSM_Fault_SappsOCSCFault_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_SappsOCSCFault_value = value;
}

void App_CanRx_FSM_Fault_FlowMeterUnderflow_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_FlowMeterUnderflow_value = value;
}

void App_CanRx_FSM_Fault_TorquePlausabilityFailed_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_TorquePlausabilityFailed_value = value;
}

void App_CanRx_Debug_EnableDebugMode_Update(bool value)
{
    rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value = value;
}

bool App_CanRx_DCM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanRx_DCM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanRx_DCM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanRx_DCM_Warning_WatchdogTimeout_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_Warning_WatchdogTimeout_value;
}

bool App_CanRx_DCM_Warning_TxOverflow_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_Warning_TxOverflow_value;
}

bool App_CanRx_DCM_Warning_RxOverflow_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_Warning_RxOverflow_value;
}

bool App_CanRx_DCM_Fault_MissingHeartbeat_Get()
{
    return rx_table.DCM_Faults_signals.DCM_Fault_MissingHeartbeat_value;
}

bool App_CanRx_DCM_Fault_LeftInverterFault_Get()
{
    return rx_table.DCM_Faults_signals.DCM_Fault_LeftInverterFault_value;
}

bool App_CanRx_DCM_Fault_RightInverterFault_Get()
{
    return rx_table.DCM_Faults_signals.DCM_Fault_RightInverterFault_value;
}

bool App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanRx_DIM_Warning_WatchdogTimeout_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_WatchdogTimeout_value;
}

bool App_CanRx_DIM_Warning_TxOverflow_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_TxOverflow_value;
}

bool App_CanRx_DIM_Warning_RxOverflow_Get()
{
    return rx_table.DIM_Warnings_signals.DIM_Warning_RxOverflow_value;
}

bool App_CanRx_DIM_Fault_MissingHeartbeat_Get()
{
    return rx_table.DIM_Faults_signals.DIM_Fault_MissingHeartbeat_value;
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

bool App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanRx_BMS_Warning_WatchdogTimeout_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_WatchdogTimeout_value;
}

bool App_CanRx_BMS_Warning_TxOverflow_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_TxOverflow_value;
}

bool App_CanRx_BMS_Warning_RxOverflow_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_RxOverflow_value;
}

bool App_CanRx_BMS_Fault_MissingHeartbeat_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_MissingHeartbeat_value;
}

bool App_CanRx_BMS_Fault_StateMachine_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_StateMachine_value;
}

bool App_CanRx_BMS_Fault_CellUndervoltage_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellUndervoltage_value;
}

bool App_CanRx_BMS_Fault_CellOvervoltage_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellOvervoltage_value;
}

bool App_CanRx_BMS_Fault_ModuleCommunicationError_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_ModuleCommunicationError_value;
}

bool App_CanRx_BMS_Fault_CellUndertemp_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellUndertemp_value;
}

bool App_CanRx_BMS_Fault_CellOvertemp_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellOvertemp_value;
}

bool App_CanRx_BMS_Fault_Charger_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_Charger_value;
}

bool App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_ChargerDisconnectedDuringCharge_value;
}

bool App_CanRx_BMS_Fault_ChargerExternalShutdown_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_ChargerExternalShutdown_value;
}

bool App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_TractiveSystemOvercurrent_value;
}

bool App_CanRx_BMS_Fault_PrechargeFailure_Get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_PrechargeFailure_value;
}

bool App_CanRx_FSM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanRx_FSM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanRx_FSM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanRx_FSM_Warning_WatchdogTimeout_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_WatchdogTimeout_value;
}

bool App_CanRx_FSM_Warning_TxOverflow_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_TxOverflow_value;
}

bool App_CanRx_FSM_Warning_RxOverflow_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_RxOverflow_value;
}

bool App_CanRx_FSM_Warning_LeftWheelSpeedOutOfRange_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_LeftWheelSpeedOutOfRange_value;
}

bool App_CanRx_FSM_Warning_RightWheelSpeedOutOfRange_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_RightWheelSpeedOutOfRange_value;
}

bool App_CanRx_FSM_Warning_SteeringAngleOCSC_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOCSC_value;
}

bool App_CanRx_FSM_Warning_SteeringAngleOutOfRange_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOutOfRange_value;
}

bool App_CanRx_FSM_Warning_FrontBrakePressureOutOfRange_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_FrontBrakePressureOutOfRange_value;
}

bool App_CanRx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_RearBrakePressureOutOfRangeWarning_value;
}

bool App_CanRx_FSM_Warning_FlowRateOutOfRange_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_FlowRateOutOfRange_value;
}

bool App_CanRx_FSM_Warning_BrakeAppsDisagreement_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_BrakeAppsDisagreement_value;
}

bool App_CanRx_FSM_Warning_AppsDisagreement_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_AppsDisagreement_value;
}

bool App_CanRx_FSM_Fault_MissingHeartbeat_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_MissingHeartbeat_value;
}

bool App_CanRx_FSM_Fault_StateMachine_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_StateMachine_value;
}

bool App_CanRx_FSM_Fault_PappsOCSC_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_PappsOCSC_value;
}

bool App_CanRx_FSM_Fault_SappsOCSCFault_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_SappsOCSCFault_value;
}

bool App_CanRx_FSM_Fault_FlowMeterUnderflow_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_FlowMeterUnderflow_value;
}

bool App_CanRx_FSM_Fault_TorquePlausabilityFailed_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_TorquePlausabilityFailed_value;
}

bool App_CanRx_Debug_EnableDebugMode_Get()
{
    return rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value;
}

