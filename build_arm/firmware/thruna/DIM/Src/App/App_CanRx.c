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
 * Struct for holding all messages received by DIM (i.e. the RX table).
 */
typedef struct
{
    Debug_CanMode_Signals Debug_CanMode_signals;
    BMS_Vitals_Signals BMS_Vitals_signals;
    BMS_OkStatuses_Signals BMS_OkStatuses_signals;
    BMS_Contactors_Signals BMS_Contactors_signals;
    BMS_CellVoltages_Signals BMS_CellVoltages_signals;
    BMS_TractiveSystem_Signals BMS_TractiveSystem_signals;
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    DCM_Vitals_Signals DCM_Vitals_signals;
    DCM_Warnings_Signals DCM_Warnings_signals;
    DCM_Faults_Signals DCM_Faults_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
    PDM_Warnings_Signals PDM_Warnings_signals;
    PDM_Faults_Signals PDM_Faults_signals;
    INVR_Temperatures1_Signals INVR_Temperatures1_signals;
    INVR_MotorPositionInfo_Signals INVR_MotorPositionInfo_signals;
    INVL_MotorPositionInfo_Signals INVL_MotorPositionInfo_signals;
} DIM_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static DIM_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(DIM_RxMsgs));
    App_CanRx_Debug_EnableDebugMode_Update(CANSIG_DEBUG_CAN_MODE_DEBUG_ENABLE_DEBUG_MODE_START_VAL);
    App_CanRx_BMS_Heartbeat_Update(CANSIG_BMS_VITALS_BMS_HEARTBEAT_START_VAL);
    App_CanRx_BMS_State_Update(CANSIG_BMS_VITALS_BMS_STATE_START_VAL);
    App_CanRx_BMS_BmsOk_Update(CANSIG_BMS_OK_STATUSES_BMS_BMS_OK_START_VAL);
    App_CanRx_BMS_ImdOk_Update(CANSIG_BMS_OK_STATUSES_BMS_IMD_OK_START_VAL);
    App_CanRx_BMS_BspdOk_Update(CANSIG_BMS_OK_STATUSES_BMS_BSPD_OK_START_VAL);
    App_CanRx_BMS_BmsLatchedFault_Update(CANSIG_BMS_OK_STATUSES_BMS_BMS_LATCHED_FAULT_START_VAL);
    App_CanRx_BMS_ImdLatchedFault_Update(CANSIG_BMS_OK_STATUSES_BMS_IMD_LATCHED_FAULT_START_VAL);
    App_CanRx_BMS_BspdLatchedFault_Update(CANSIG_BMS_OK_STATUSES_BMS_BSPD_LATCHED_FAULT_START_VAL);
    App_CanRx_BMS_AirPositive_Update(CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_START_VAL);
    App_CanRx_BMS_AirNegative_Update(CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_START_VAL);
    App_CanRx_BMS_PrechargeRelay_Update(CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_START_VAL);
    App_CanRx_BMS_MinCellVoltage_Update(CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_START_VAL);
    App_CanRx_BMS_MaxCellVoltage_Update(CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_START_VAL);
    App_CanRx_BMS_TractiveSystemVoltage_Update(CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_START_VAL);
    App_CanRx_BMS_TractiveSystemCurrent_Update(CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_START_VAL);
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
    App_CanRx_DCM_Heartbeat_Update(CANSIG_DCM_VITALS_DCM_HEARTBEAT_START_VAL);
    App_CanRx_DCM_State_Update(CANSIG_DCM_VITALS_DCM_STATE_START_VAL);
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
    App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_PDM_WatchdogTimeoutWarning_Update(CANSIG_PDM_WARNINGS_PDM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_PDM_TxOverflowWarning_Update(CANSIG_PDM_WARNINGS_PDM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_PDM_RxOverflowWarning_Update(CANSIG_PDM_WARNINGS_PDM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_PDM_DummyFault_Update(CANSIG_PDM_FAULTS_PDM_DUMMY_FAULT_START_VAL);
    App_CanRx_INVR_ModuleATemperature_Update(CANSIG_INVR_TEMPERATURES1_INVR_MODULE_A_TEMPERATURE_START_VAL);
    App_CanRx_INVR_ModuleBTemperature_Update(CANSIG_INVR_TEMPERATURES1_INVR_MODULE_B_TEMPERATURE_START_VAL);
    App_CanRx_INVR_ModuleCTemperature_Update(CANSIG_INVR_TEMPERATURES1_INVR_MODULE_C_TEMPERATURE_START_VAL);
    App_CanRx_INVR_GateDriverBoardTemperature_Update(CANSIG_INVR_TEMPERATURES1_INVR_GATE_DRIVER_BOARD_TEMPERATURE_START_VAL);
    App_CanRx_INVR_MotorAngle_Update(CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_START_VAL);
    App_CanRx_INVR_MotorSpeed_Update(CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_START_VAL);
    App_CanRx_INVR_ElectricalOutputFrequency_Update(CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_START_VAL);
    App_CanRx_INVR_DeltaResolverFiltered_Update(CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_START_VAL);
    App_CanRx_INVL_MotorAngle_Update(CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_START_VAL);
    App_CanRx_INVL_MotorSpeed_Update(CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_START_VAL);
    App_CanRx_INVL_ElectricalOutputFrequency_Update(CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_START_VAL);
    App_CanRx_INVL_DeltaResolverFiltered_Update(CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_START_VAL);
}

void App_CanRx_Debug_EnableDebugMode_Update(bool value)
{
    rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value = value;
}

void App_CanRx_BMS_Heartbeat_Update(bool value)
{
    rx_table.BMS_Vitals_signals.BMS_Heartbeat_value = value;
}

void App_CanRx_BMS_State_Update(BmsState value)
{
    rx_table.BMS_Vitals_signals.BMS_State_value = (value > CANSIG_BMS_VITALS_BMS_STATE_MAX) ? CANSIG_BMS_VITALS_BMS_STATE_MAX : value;
}

void App_CanRx_BMS_BmsOk_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_BmsOk_value = value;
}

void App_CanRx_BMS_ImdOk_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_ImdOk_value = value;
}

void App_CanRx_BMS_BspdOk_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_BspdOk_value = value;
}

void App_CanRx_BMS_BmsLatchedFault_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value = value;
}

void App_CanRx_BMS_ImdLatchedFault_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value = value;
}

void App_CanRx_BMS_BspdLatchedFault_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value = value;
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

void App_CanRx_BMS_MinCellVoltage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_MIN ? CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_MIN : value;
    rx_table.BMS_CellVoltages_signals.BMS_MinCellVoltage_value = tmp > CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_MAX ? CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_MAX : tmp;
}

void App_CanRx_BMS_MaxCellVoltage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_MIN ? CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_MIN : value;
    rx_table.BMS_CellVoltages_signals.BMS_MaxCellVoltage_value = tmp > CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_MAX ? CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_MAX : tmp;
}

void App_CanRx_BMS_TractiveSystemVoltage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN : value;
    rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value = tmp > CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX : tmp;
}

void App_CanRx_BMS_TractiveSystemCurrent_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MIN ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MIN : value;
    rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value = tmp > CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MAX ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MAX : tmp;
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

void App_CanRx_DCM_Heartbeat_Update(bool value)
{
    rx_table.DCM_Vitals_signals.DCM_Heartbeat_value = value;
}

void App_CanRx_DCM_State_Update(DcmState value)
{
    rx_table.DCM_Vitals_signals.DCM_State_value = (value > CANSIG_DCM_VITALS_DCM_STATE_MAX) ? CANSIG_DCM_VITALS_DCM_STATE_MAX : value;
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

void App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanRx_PDM_WatchdogTimeoutWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WatchdogTimeoutWarning_value = value;
}

void App_CanRx_PDM_TxOverflowWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_TxOverflowWarning_value = value;
}

void App_CanRx_PDM_RxOverflowWarning_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_RxOverflowWarning_value = value;
}

void App_CanRx_PDM_DummyFault_Update(bool value)
{
    rx_table.PDM_Faults_signals.PDM_DummyFault_value = value;
}

void App_CanRx_INVR_ModuleATemperature_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_TEMPERATURES1_INVR_MODULE_A_TEMPERATURE_MIN ? CANSIG_INVR_TEMPERATURES1_INVR_MODULE_A_TEMPERATURE_MIN : value;
    rx_table.INVR_Temperatures1_signals.INVR_ModuleATemperature_value = tmp > CANSIG_INVR_TEMPERATURES1_INVR_MODULE_A_TEMPERATURE_MAX ? CANSIG_INVR_TEMPERATURES1_INVR_MODULE_A_TEMPERATURE_MAX : tmp;
}

void App_CanRx_INVR_ModuleBTemperature_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_TEMPERATURES1_INVR_MODULE_B_TEMPERATURE_MIN ? CANSIG_INVR_TEMPERATURES1_INVR_MODULE_B_TEMPERATURE_MIN : value;
    rx_table.INVR_Temperatures1_signals.INVR_ModuleBTemperature_value = tmp > CANSIG_INVR_TEMPERATURES1_INVR_MODULE_B_TEMPERATURE_MAX ? CANSIG_INVR_TEMPERATURES1_INVR_MODULE_B_TEMPERATURE_MAX : tmp;
}

void App_CanRx_INVR_ModuleCTemperature_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_TEMPERATURES1_INVR_MODULE_C_TEMPERATURE_MIN ? CANSIG_INVR_TEMPERATURES1_INVR_MODULE_C_TEMPERATURE_MIN : value;
    rx_table.INVR_Temperatures1_signals.INVR_ModuleCTemperature_value = tmp > CANSIG_INVR_TEMPERATURES1_INVR_MODULE_C_TEMPERATURE_MAX ? CANSIG_INVR_TEMPERATURES1_INVR_MODULE_C_TEMPERATURE_MAX : tmp;
}

void App_CanRx_INVR_GateDriverBoardTemperature_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_TEMPERATURES1_INVR_GATE_DRIVER_BOARD_TEMPERATURE_MIN ? CANSIG_INVR_TEMPERATURES1_INVR_GATE_DRIVER_BOARD_TEMPERATURE_MIN : value;
    rx_table.INVR_Temperatures1_signals.INVR_GateDriverBoardTemperature_value = tmp > CANSIG_INVR_TEMPERATURES1_INVR_GATE_DRIVER_BOARD_TEMPERATURE_MAX ? CANSIG_INVR_TEMPERATURES1_INVR_GATE_DRIVER_BOARD_TEMPERATURE_MAX : tmp;
}

void App_CanRx_INVR_MotorAngle_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.INVR_MotorAngle_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_MAX : tmp;
}

void App_CanRx_INVR_MotorSpeed_Update(int value)
{
    const int tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.INVR_MotorSpeed_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_MAX : tmp;
}

void App_CanRx_INVR_ElectricalOutputFrequency_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.INVR_ElectricalOutputFrequency_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_MAX : tmp;
}

void App_CanRx_INVR_DeltaResolverFiltered_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.INVR_DeltaResolverFiltered_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_MAX : tmp;
}

void App_CanRx_INVL_MotorAngle_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.INVL_MotorAngle_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_MAX : tmp;
}

void App_CanRx_INVL_MotorSpeed_Update(int value)
{
    const int tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.INVL_MotorSpeed_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_MAX : tmp;
}

void App_CanRx_INVL_ElectricalOutputFrequency_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.INVL_ElectricalOutputFrequency_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_MAX : tmp;
}

void App_CanRx_INVL_DeltaResolverFiltered_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.INVL_DeltaResolverFiltered_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_MAX : tmp;
}

bool App_CanRx_Debug_EnableDebugMode_Get()
{
    return rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value;
}

bool App_CanRx_BMS_Heartbeat_Get()
{
    return rx_table.BMS_Vitals_signals.BMS_Heartbeat_value;
}

BmsState App_CanRx_BMS_State_Get()
{
    return rx_table.BMS_Vitals_signals.BMS_State_value;
}

bool App_CanRx_BMS_BmsOk_Get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BmsOk_value;
}

bool App_CanRx_BMS_ImdOk_Get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_ImdOk_value;
}

bool App_CanRx_BMS_BspdOk_Get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BspdOk_value;
}

bool App_CanRx_BMS_BmsLatchedFault_Get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value;
}

bool App_CanRx_BMS_ImdLatchedFault_Get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value;
}

bool App_CanRx_BMS_BspdLatchedFault_Get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value;
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

float App_CanRx_BMS_MinCellVoltage_Get()
{
    return rx_table.BMS_CellVoltages_signals.BMS_MinCellVoltage_value;
}

float App_CanRx_BMS_MaxCellVoltage_Get()
{
    return rx_table.BMS_CellVoltages_signals.BMS_MaxCellVoltage_value;
}

float App_CanRx_BMS_TractiveSystemVoltage_Get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value;
}

float App_CanRx_BMS_TractiveSystemCurrent_Get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value;
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

bool App_CanRx_DCM_Heartbeat_Get()
{
    return rx_table.DCM_Vitals_signals.DCM_Heartbeat_value;
}

DcmState App_CanRx_DCM_State_Get()
{
    return rx_table.DCM_Vitals_signals.DCM_State_value;
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

bool App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanRx_PDM_WatchdogTimeoutWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WatchdogTimeoutWarning_value;
}

bool App_CanRx_PDM_TxOverflowWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_TxOverflowWarning_value;
}

bool App_CanRx_PDM_RxOverflowWarning_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_RxOverflowWarning_value;
}

bool App_CanRx_PDM_DummyFault_Get()
{
    return rx_table.PDM_Faults_signals.PDM_DummyFault_value;
}

float App_CanRx_INVR_ModuleATemperature_Get()
{
    return rx_table.INVR_Temperatures1_signals.INVR_ModuleATemperature_value;
}

float App_CanRx_INVR_ModuleBTemperature_Get()
{
    return rx_table.INVR_Temperatures1_signals.INVR_ModuleBTemperature_value;
}

float App_CanRx_INVR_ModuleCTemperature_Get()
{
    return rx_table.INVR_Temperatures1_signals.INVR_ModuleCTemperature_value;
}

float App_CanRx_INVR_GateDriverBoardTemperature_Get()
{
    return rx_table.INVR_Temperatures1_signals.INVR_GateDriverBoardTemperature_value;
}

float App_CanRx_INVR_MotorAngle_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.INVR_MotorAngle_value;
}

int App_CanRx_INVR_MotorSpeed_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.INVR_MotorSpeed_value;
}

float App_CanRx_INVR_ElectricalOutputFrequency_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.INVR_ElectricalOutputFrequency_value;
}

float App_CanRx_INVR_DeltaResolverFiltered_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.INVR_DeltaResolverFiltered_value;
}

float App_CanRx_INVL_MotorAngle_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.INVL_MotorAngle_value;
}

int App_CanRx_INVL_MotorSpeed_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.INVL_MotorSpeed_value;
}

float App_CanRx_INVL_ElectricalOutputFrequency_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.INVL_ElectricalOutputFrequency_value;
}

float App_CanRx_INVL_DeltaResolverFiltered_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.INVL_DeltaResolverFiltered_value;
}

