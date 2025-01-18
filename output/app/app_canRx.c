/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "app_canRx.h"

/* ------------------------------- Structs -------------------------------- */

/**
 * Struct for holding all messages received by  (i.e. the RX table).
 */
typedef struct
{
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    BMS_WarningsCounts_Signals BMS_WarningsCounts_signals;
    BMS_FaultsCounts_Signals BMS_FaultsCounts_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
    FSM_WarningsCounts_Signals FSM_WarningsCounts_signals;
    FSM_FaultsCounts_Signals FSM_FaultsCounts_signals;
    BMS_TractiveSystem_Signals BMS_TractiveSystem_signals;
    BMS_BMSShdnNodeStatus_Signals BMS_BMSShdnNodeStatus_signals;
    BMS_CellTemperatures_Signals BMS_CellTemperatures_signals;
    BMS_VoltageAndChargeStats_Signals BMS_VoltageAndChargeStats_signals;
    BMS_OkStatuses_Signals BMS_OkStatuses_signals;
    BMS_TractiveSystem_Signals BMS_TractiveSystem_signals;
    BMS_Vitals_Signals BMS_Vitals_signals;
} BMS_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static BMS_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canRx_init()
{
    memset(&rx_table, 0, sizeof(BMS_RxMsgs));
    // Initialize all signals to Start-up value
    app_canRx_BMS_Warning_StackWaterMarkHighTask1Hz_update(CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    app_canRx_BMS_Warning_StackWaterMarkHighTask100Hz_update(CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    app_canRx_BMS_Warning_StackWaterMarkHighTask1kHz_update(CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    app_canRx_BMS_Warning_StackWaterMarkHighTaskCanRx_update(CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    app_canRx_BMS_Warning_StackWaterMarkHighTaskCanTx_update(CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    app_canRx_BMS_Warning_WatchdogTimeout_update(CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    app_canRx_BMS_Warning_TxOverflow_update(CANSIG_BMS_WARNING_TX_OVERFLOW_START_VAL);
    app_canRx_BMS_Warning_RxOverflow_update(CANSIG_BMS_WARNING_RX_OVERFLOW_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheckFault_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheck_Segment0_GND_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheck_Segment1_GND_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheck_Segment2_GND_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheck_Segment3_GND_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheck_Segment4_GND_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_START_VAL);
    app_canRx_BMS_Warning_MissingVCHeartbeat_update(CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_START_VAL);
    app_canRx_BMS_Fault_StateMachine_update(CANSIG_BMS_FAULT_STATE_MACHINE_START_VAL);
    app_canRx_BMS_Fault_CellUndervoltage_update(CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_START_VAL);
    app_canRx_BMS_Fault_CellOvervoltage_update(CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_START_VAL);
    app_canRx_BMS_Fault_ModuleCommunicationError_update(CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_START_VAL);
    app_canRx_BMS_Fault_CellUndertemp_update(CANSIG_BMS_FAULT_CELL_UNDERTEMP_START_VAL);
    app_canRx_BMS_Fault_CellOvertemp_update(CANSIG_BMS_FAULT_CELL_OVERTEMP_START_VAL);
    app_canRx_BMS_Fault_ChargerReportedError_update(CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_START_VAL);
    app_canRx_BMS_Fault_ChargerDisconnectedDuringCharge_update(CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_START_VAL);
    app_canRx_BMS_Fault_ChargerShutdownLoopOpen_update(CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_START_VAL);
    app_canRx_BMS_Fault_TractiveSystemOvercurrent_update(CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_START_VAL);
    app_canRx_BMS_Fault_PrechargeFailure_update(CANSIG_BMS_FAULT_PRECHARGE_FAILURE_START_VAL);
    app_canRx_BMS_Warning_StackWaterMarkHighTask1HzCount_update(CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_START_VAL);
    app_canRx_BMS_Warning_StackWaterMarkHighTask100HzCount_update(CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_START_VAL);
    app_canRx_BMS_Warning_StackWaterMarkHighTask1kHzCount_update(CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_START_VAL);
    app_canRx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_update(CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_START_VAL);
    app_canRx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_update(CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_START_VAL);
    app_canRx_BMS_Warning_WatchdogTimeoutCount_update(CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_START_VAL);
    app_canRx_BMS_Warning_TxOverflowCount_update(CANSIG_BMS_WARNING_TX_OVERFLOW_COUNT_START_VAL);
    app_canRx_BMS_Warning_RxOverflowCount_update(CANSIG_BMS_WARNING_RX_OVERFLOW_COUNT_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheckFaultCount_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_START_VAL);
    app_canRx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_update(CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_START_VAL);
    app_canRx_BMS_Warning_MissingVCHeartbeatCount_update(CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_START_VAL);
    app_canRx_BMS_Fault_StateMachineCount_update(CANSIG_BMS_FAULT_STATE_MACHINE_COUNT_START_VAL);
    app_canRx_BMS_Fault_CellUndervoltageCount_update(CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_START_VAL);
    app_canRx_BMS_Fault_CellOvervoltageCount_update(CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_START_VAL);
    app_canRx_BMS_Fault_ModuleCommunicationErrorCount_update(CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_START_VAL);
    app_canRx_BMS_Fault_CellUndertempCount_update(CANSIG_BMS_FAULT_CELL_UNDERTEMP_COUNT_START_VAL);
    app_canRx_BMS_Fault_CellOvertempCount_update(CANSIG_BMS_FAULT_CELL_OVERTEMP_COUNT_START_VAL);
    app_canRx_BMS_Fault_ChargerReportedErrorCount_update(CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_START_VAL);
    app_canRx_BMS_Fault_ChargerDisconnectedDuringChargeCount_update(CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_START_VAL);
    app_canRx_BMS_Fault_ChargerShutdownLoopOpenCount_update(CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_START_VAL);
    app_canRx_BMS_Fault_TractiveSystemOvercurrentCount_update(CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_START_VAL);
    app_canRx_BMS_Fault_PrechargeFailureCount_update(CANSIG_BMS_FAULT_PRECHARGE_FAILURE_COUNT_START_VAL);
    app_canRx_FSM_Warning_WatchdogTimeout_update(CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    app_canRx_FSM_Warning_TxOverflow_update(CANSIG_FSM_WARNING_TX_OVERFLOW_START_VAL);
    app_canRx_FSM_Warning_RxOverflow_update(CANSIG_FSM_WARNING_RX_OVERFLOW_START_VAL);
    app_canRx_FSM_Warning_StackWaterMarkHighTask1Hz_update(CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    app_canRx_FSM_Warning_StackWaterMarkHighTask100Hz_update(CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    app_canRx_FSM_Warning_StackWaterMarkHighTask1kHz_update(CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    app_canRx_FSM_Warning_StackWaterMarkHighTaskCanRx_update(CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    app_canRx_FSM_Warning_StackWaterMarkHighTaskCanTx_update(CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    app_canRx_FSM_Warning_SteeringAngleOCSC_update(CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_START_VAL);
    app_canRx_FSM_Warning_SteeringAngleOutOfRange_update(CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_START_VAL);
    app_canRx_FSM_Warning_LeftWheelSpeedOutOfRange_update(CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    app_canRx_FSM_Warning_RightWheelSpeedOutOfRange_update(CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    app_canRx_FSM_Warning_AppsDisagreement_update(CANSIG_FSM_WARNING_APPS_DISAGREEMENT_START_VAL);
    app_canRx_FSM_Warning_FrontBrakePressureOutOfRange_update(CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL);
    app_canRx_FSM_Warning_RearBrakePressureOutOfRange_update(CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL);
    app_canRx_FSM_Warning_FrontBrakePressureOcSc_update(CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_START_VAL);
    app_canRx_FSM_Warning_RearBrakePressureOcSc_update(CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_START_VAL);
    app_canRx_FSM_Warning_BrakeOcScNotOk_update(CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_START_VAL);
    app_canRx_FSM_Fault_PappsOCSC_update(CANSIG_FSM_FAULT_PAPPS_OCSC_START_VAL);
    app_canRx_FSM_Fault_SappsOCSC_update(CANSIG_FSM_FAULT_SAPPS_OCSC_START_VAL);
    app_canRx_FSM_Warning_WatchdogTimeoutCount_update(CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_START_VAL);
    app_canRx_FSM_Warning_TxOverflowCount_update(CANSIG_FSM_WARNING_TX_OVERFLOW_COUNT_START_VAL);
    app_canRx_FSM_Warning_RxOverflowCount_update(CANSIG_FSM_WARNING_RX_OVERFLOW_COUNT_START_VAL);
    app_canRx_FSM_Warning_StackWaterMarkHighTask1HzCount_update(CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_START_VAL);
    app_canRx_FSM_Warning_StackWaterMarkHighTask100HzCount_update(CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_START_VAL);
    app_canRx_FSM_Warning_StackWaterMarkHighTask1kHzCount_update(CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_START_VAL);
    app_canRx_FSM_Warning_StackWaterMarkHighTaskCanRxCount_update(CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_START_VAL);
    app_canRx_FSM_Warning_StackWaterMarkHighTaskCanTxCount_update(CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_START_VAL);
    app_canRx_FSM_Warning_SteeringAngleOCSCCount_update(CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_START_VAL);
    app_canRx_FSM_Warning_SteeringAngleOutOfRangeCount_update(CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_START_VAL);
    app_canRx_FSM_Warning_LeftWheelSpeedOutOfRangeCount_update(CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_START_VAL);
    app_canRx_FSM_Warning_RightWheelSpeedOutOfRangeCount_update(CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_START_VAL);
    app_canRx_FSM_Warning_AppsDisagreementCount_update(CANSIG_FSM_WARNING_APPS_DISAGREEMENT_COUNT_START_VAL);
    app_canRx_FSM_Warning_FrontBrakePressureOutOfRangeCount_update(CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_START_VAL);
    app_canRx_FSM_Warning_RearBrakePressureOutOfRangeCount_update(CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_START_VAL);
    app_canRx_FSM_Warning_FrontBrakePressureOcScCount_update(CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_COUNT_START_VAL);
    app_canRx_FSM_Warning_RearBrakePressureOcScCount_update(CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_COUNT_START_VAL);
    app_canRx_FSM_Warning_BrakeOcScNotOkCount_update(CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_COUNT_START_VAL);
    app_canRx_FSM_Fault_PappsOCSCCount_update(CANSIG_FSM_FAULT_PAPPS_OCSC_COUNT_START_VAL);
    app_canRx_FSM_Fault_SappsOCSCCount_update(CANSIG_FSM_FAULT_SAPPS_OCSC_COUNT_START_VAL);
    app_canRx_BMS_TractiveSystemVoltage_update(CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_START_VAL);
    app_canRx_BMS_TractiveSystemCurrent_update(CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_START_VAL);
    app_canRx_BMS_TractiveSystemPower_update(CANSIG_BMS_TRACTIVE_SYSTEM_POWER_START_VAL);
    app_canRx_BMS_AvailablePower_update(CANSIG_BMS_AVAILABLE_POWER_START_VAL);
    app_canRx_BMS_BSPDCurrentThresholdExceeded_update(CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_START_VAL);
    app_canRx_BMS_HVDShdnOKStatus_update(CANSIG_BMS_HVD_SHDN_OK_STATUS_START_VAL);
    app_canRx_BMS_TSIlckOKStatus_update(CANSIG_BMS_TS_ILCK_OK_STATUS_START_VAL);
    app_canRx_BMS_MinCellTemperature_update(CANSIG_BMS_MIN_CELL_TEMPERATURE_START_VAL);
    app_canRx_BMS_MaxCellTemperature_update(CANSIG_BMS_MAX_CELL_TEMPERATURE_START_VAL);
    app_canRx_BMS_PackVoltage_update(CANSIG_BMS_PACK_VOLTAGE_START_VAL);
    app_canRx_BMS_Soc_update(CANSIG_BMS_SOC_START_VAL);
    app_canRx_BMS_SocCorrupt_update(CANSIG_BMS_SOC_CORRUPT_START_VAL);
    app_canRx_BMS_MinCellVoltage_update(CANSIG_BMS_MIN_CELL_VOLTAGE_START_VAL);
    app_canRx_BMS_MaxCellVoltage_update(CANSIG_BMS_MAX_CELL_VOLTAGE_START_VAL);
    app_canRx_BMS_BmsOk_update(CANSIG_BMS_BMS_OK_START_VAL);
    app_canRx_BMS_ImdOk_update(CANSIG_BMS_IMD_OK_START_VAL);
    app_canRx_BMS_BspdOk_update(CANSIG_BMS_BSPD_OK_START_VAL);
    app_canRx_BMS_BmsLatchedFault_update(CANSIG_BMS_BMS_LATCHED_FAULT_START_VAL);
    app_canRx_BMS_ImdLatchedFault_update(CANSIG_BMS_IMD_LATCHED_FAULT_START_VAL);
    app_canRx_BMS_BspdLatchedFault_update(CANSIG_BMS_BSPD_LATCHED_FAULT_START_VAL);
    app_canRx_BMS_TractiveSystemVoltage_update(CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_START_VAL);
    app_canRx_BMS_TractiveSystemCurrent_update(CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_START_VAL);
    app_canRx_BMS_TractiveSystemPower_update(CANSIG_BMS_TRACTIVE_SYSTEM_POWER_START_VAL);
    app_canRx_BMS_AvailablePower_update(CANSIG_BMS_AVAILABLE_POWER_START_VAL);
    app_canRx_BMS_BSPDCurrentThresholdExceeded_update(CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_START_VAL);
    app_canRx_BMS_Heartbeat_update(CANSIG_BMS_HEARTBEAT_START_VAL);
    app_canRx_BMS_State_update(CANSIG_BMS_STATE_START_VAL);
    
}

void app_canRx_BMS_Warning_StackWaterMarkHighTask1Hz_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1Hz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_StackWaterMarkHighTask100Hz_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask100Hz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_StackWaterMarkHighTask1kHz_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1kHz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_StackWaterMarkHighTaskCanRx_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanRx_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_StackWaterMarkHighTaskCanTx_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanTx_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_WatchdogTimeout_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_WatchdogTimeout_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_TxOverflow_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_TxOverflow_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_TX_OVERFLOW_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_TX_OVERFLOW_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_TX_OVERFLOW_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_TX_OVERFLOW_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_RxOverflow_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_RxOverflow_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_RX_OVERFLOW_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_RX_OVERFLOW_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_RX_OVERFLOW_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_RX_OVERFLOW_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheckFault_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheckFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheck_Segment0_GND_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment0_GND_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheck_Segment1_GND_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment1_GND_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheck_Segment2_GND_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment2_GND_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheck_Segment3_GND_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment3_GND_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheck_Segment4_GND_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment4_GND_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_MissingVCHeartbeat_update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_Warning_MissingVCHeartbeat_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_MIN_VAL;
    }
}

void app_canRx_BMS_Fault_StateMachine_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_StateMachine_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_STATE_MACHINE_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_STATE_MACHINE_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_STATE_MACHINE_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_STATE_MACHINE_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_CellUndervoltage_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellUndervoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_CellOvervoltage_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellOvervoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_ModuleCommunicationError_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_ModuleCommunicationError_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_CellUndertemp_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellUndertemp_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CELL_UNDERTEMP_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_UNDERTEMP_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CELL_UNDERTEMP_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_UNDERTEMP_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_CellOvertemp_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_CellOvertemp_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CELL_OVERTEMP_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_OVERTEMP_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CELL_OVERTEMP_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_OVERTEMP_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_ChargerReportedError_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_ChargerReportedError_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_ChargerDisconnectedDuringCharge_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_ChargerDisconnectedDuringCharge_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_ChargerShutdownLoopOpen_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_ChargerShutdownLoopOpen_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_TractiveSystemOvercurrent_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_TractiveSystemOvercurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_PrechargeFailure_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_PrechargeFailure_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_PRECHARGE_FAILURE_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_PRECHARGE_FAILURE_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_PRECHARGE_FAILURE_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_PRECHARGE_FAILURE_MIN_VAL;
    }
}

void app_canRx_BMS_Warning_StackWaterMarkHighTask1HzCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1HzCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_StackWaterMarkHighTask100HzCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask100HzCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_StackWaterMarkHighTask1kHzCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1kHzCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanRxCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanTxCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_WatchdogTimeoutCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_WatchdogTimeoutCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_TxOverflowCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_TxOverflowCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_TX_OVERFLOW_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_TX_OVERFLOW_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_TX_OVERFLOW_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_TX_OVERFLOW_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_RxOverflowCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_RxOverflowCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_RX_OVERFLOW_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_RX_OVERFLOW_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_RX_OVERFLOW_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_RX_OVERFLOW_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheckFaultCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheckFaultCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment0_GNDCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment1_GNDCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment2_GNDCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment3_GNDCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment4_GNDCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Warning_MissingVCHeartbeatCount_update(uint32_t value)
{
    rx_table.BMS_WarningsCounts_signals.BMS_Warning_MissingVCHeartbeatCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_MIN_VAL;
    }
}

void app_canRx_BMS_Fault_StateMachineCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_StateMachineCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_STATE_MACHINE_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_STATE_MACHINE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_STATE_MACHINE_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_STATE_MACHINE_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_CellUndervoltageCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndervoltageCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_CellOvervoltageCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvervoltageCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_ModuleCommunicationErrorCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_ModuleCommunicationErrorCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_CellUndertempCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndertempCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CELL_UNDERTEMP_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_UNDERTEMP_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CELL_UNDERTEMP_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_UNDERTEMP_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_CellOvertempCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvertempCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CELL_OVERTEMP_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_OVERTEMP_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CELL_OVERTEMP_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CELL_OVERTEMP_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_ChargerReportedErrorCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerReportedErrorCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_ChargerDisconnectedDuringChargeCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerDisconnectedDuringChargeCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_ChargerShutdownLoopOpenCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerShutdownLoopOpenCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_TractiveSystemOvercurrentCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_TractiveSystemOvercurrentCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_MIN_VAL;
    }
}
void app_canRx_BMS_Fault_PrechargeFailureCount_update(uint32_t value)
{
    rx_table.BMS_FaultsCounts_signals.BMS_Fault_PrechargeFailureCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_FAULT_PRECHARGE_FAILURE_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_FAULT_PRECHARGE_FAILURE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_FAULT_PRECHARGE_FAILURE_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_FAULT_PRECHARGE_FAILURE_COUNT_MIN_VAL;
    }
}

void app_canRx_FSM_Warning_WatchdogTimeout_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_WatchdogTimeout_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_TxOverflow_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_TxOverflow_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_TX_OVERFLOW_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_TX_OVERFLOW_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_TX_OVERFLOW_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_TX_OVERFLOW_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_RxOverflow_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_RxOverflow_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_RX_OVERFLOW_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_RX_OVERFLOW_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_RX_OVERFLOW_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_RX_OVERFLOW_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_StackWaterMarkHighTask1Hz_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1Hz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_StackWaterMarkHighTask100Hz_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask100Hz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_StackWaterMarkHighTask1kHz_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1kHz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_StackWaterMarkHighTaskCanRx_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanRx_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_StackWaterMarkHighTaskCanTx_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanTx_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_SteeringAngleOCSC_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOCSC_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_SteeringAngleOutOfRange_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOutOfRange_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_LeftWheelSpeedOutOfRange_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_LeftWheelSpeedOutOfRange_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_RightWheelSpeedOutOfRange_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_RightWheelSpeedOutOfRange_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_AppsDisagreement_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_AppsDisagreement_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_APPS_DISAGREEMENT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_APPS_DISAGREEMENT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_APPS_DISAGREEMENT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_APPS_DISAGREEMENT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_FrontBrakePressureOutOfRange_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_FrontBrakePressureOutOfRange_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_RearBrakePressureOutOfRange_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_RearBrakePressureOutOfRange_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_FrontBrakePressureOcSc_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_FrontBrakePressureOcSc_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_RearBrakePressureOcSc_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_RearBrakePressureOcSc_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_BrakeOcScNotOk_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_BrakeOcScNotOk_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_MIN_VAL;
    }
}

void app_canRx_FSM_Fault_PappsOCSC_update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_PappsOCSC_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_FAULT_PAPPS_OCSC_MAX_VAL)
    {
        value = CANSIG_FSM_FAULT_PAPPS_OCSC_MAX_VAL;
    }
    if (value < CANSIG_FSM_FAULT_PAPPS_OCSC_MIN_VAL)
    {
        value = CANSIG_FSM_FAULT_PAPPS_OCSC_MIN_VAL;
    }
}
void app_canRx_FSM_Fault_SappsOCSC_update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_SappsOCSC_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_FAULT_SAPPS_OCSC_MAX_VAL)
    {
        value = CANSIG_FSM_FAULT_SAPPS_OCSC_MAX_VAL;
    }
    if (value < CANSIG_FSM_FAULT_SAPPS_OCSC_MIN_VAL)
    {
        value = CANSIG_FSM_FAULT_SAPPS_OCSC_MIN_VAL;
    }
}

void app_canRx_FSM_Warning_WatchdogTimeoutCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_WatchdogTimeoutCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_TxOverflowCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_TxOverflowCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_TX_OVERFLOW_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_TX_OVERFLOW_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_TX_OVERFLOW_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_TX_OVERFLOW_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_RxOverflowCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_RxOverflowCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_RX_OVERFLOW_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_RX_OVERFLOW_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_RX_OVERFLOW_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_RX_OVERFLOW_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_StackWaterMarkHighTask1HzCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask1HzCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_StackWaterMarkHighTask100HzCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask100HzCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_StackWaterMarkHighTask1kHzCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask1kHzCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_StackWaterMarkHighTaskCanRxCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTaskCanRxCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_StackWaterMarkHighTaskCanTxCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTaskCanTxCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_SteeringAngleOCSCCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_SteeringAngleOCSCCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_SteeringAngleOutOfRangeCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_SteeringAngleOutOfRangeCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_LeftWheelSpeedOutOfRangeCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_LeftWheelSpeedOutOfRangeCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_RightWheelSpeedOutOfRangeCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_RightWheelSpeedOutOfRangeCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_AppsDisagreementCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_AppsDisagreementCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_APPS_DISAGREEMENT_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_APPS_DISAGREEMENT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_APPS_DISAGREEMENT_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_APPS_DISAGREEMENT_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_FrontBrakePressureOutOfRangeCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_FrontBrakePressureOutOfRangeCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_RearBrakePressureOutOfRangeCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_RearBrakePressureOutOfRangeCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_FrontBrakePressureOcScCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_FrontBrakePressureOcScCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_RearBrakePressureOcScCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_RearBrakePressureOcScCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Warning_BrakeOcScNotOkCount_update(uint32_t value)
{
    rx_table.FSM_WarningsCounts_signals.FSM_Warning_BrakeOcScNotOkCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_COUNT_MIN_VAL;
    }
}

void app_canRx_FSM_Fault_PappsOCSCCount_update(uint32_t value)
{
    rx_table.FSM_FaultsCounts_signals.FSM_Fault_PappsOCSCCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_FAULT_PAPPS_OCSC_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_FAULT_PAPPS_OCSC_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_FAULT_PAPPS_OCSC_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_FAULT_PAPPS_OCSC_COUNT_MIN_VAL;
    }
}
void app_canRx_FSM_Fault_SappsOCSCCount_update(uint32_t value)
{
    rx_table.FSM_FaultsCounts_signals.FSM_Fault_SappsOCSCCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_FSM_FAULT_SAPPS_OCSC_COUNT_MAX_VAL)
    {
        value = CANSIG_FSM_FAULT_SAPPS_OCSC_COUNT_MAX_VAL;
    }
    if (value < CANSIG_FSM_FAULT_SAPPS_OCSC_COUNT_MIN_VAL)
    {
        value = CANSIG_FSM_FAULT_SAPPS_OCSC_COUNT_MIN_VAL;
    }
}

void app_canRx_BMS_TractiveSystemVoltage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN_VAL;
    }
}
void app_canRx_BMS_TractiveSystemCurrent_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MIN_VAL;
    }
}
void app_canRx_BMS_TractiveSystemPower_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemPower_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MIN_VAL;
    }
}
void app_canRx_BMS_AvailablePower_update(uint32_t value)
{
    rx_table.BMS_TractiveSystem_signals.BMS_AvailablePower_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_AVAILABLE_POWER_MAX_VAL)
    {
        value = CANSIG_BMS_AVAILABLE_POWER_MAX_VAL;
    }
    if (value < CANSIG_BMS_AVAILABLE_POWER_MIN_VAL)
    {
        value = CANSIG_BMS_AVAILABLE_POWER_MIN_VAL;
    }
}
void app_canRx_BMS_BSPDCurrentThresholdExceeded_update(bool value)
{
    rx_table.BMS_TractiveSystem_signals.BMS_BSPDCurrentThresholdExceeded_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MAX_VAL)
    {
        value = CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MAX_VAL;
    }
    if (value < CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MIN_VAL)
    {
        value = CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MIN_VAL;
    }
}

void app_canRx_BMS_HVDShdnOKStatus_update(bool value)
{
    rx_table.BMS_BMSShdnNodeStatus_signals.BMS_HVDShdnOKStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_HVD_SHDN_OK_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_HVD_SHDN_OK_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_HVD_SHDN_OK_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_HVD_SHDN_OK_STATUS_MIN_VAL;
    }
}
void app_canRx_BMS_TSIlckOKStatus_update(bool value)
{
    rx_table.BMS_BMSShdnNodeStatus_signals.BMS_TSIlckOKStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TS_ILCK_OK_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_TS_ILCK_OK_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_TS_ILCK_OK_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_TS_ILCK_OK_STATUS_MIN_VAL;
    }
}

void app_canRx_BMS_MinCellTemperature_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_CellTemperatures_signals.BMS_MinCellTemperature_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MIN_CELL_TEMPERATURE_MAX_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_TEMPERATURE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MIN_CELL_TEMPERATURE_MIN_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_TEMPERATURE_MIN_VAL;
    }
}
void app_canRx_BMS_MaxCellTemperature_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_CellTemperatures_signals.BMS_MaxCellTemperature_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_CELL_TEMPERATURE_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_TEMPERATURE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_CELL_TEMPERATURE_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_TEMPERATURE_MIN_VAL;
    }
}

void app_canRx_BMS_PackVoltage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_VoltageAndChargeStats_signals.BMS_PackVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_PACK_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_PACK_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_PACK_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_PACK_VOLTAGE_MIN_VAL;
    }
}
void app_canRx_BMS_Soc_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_VoltageAndChargeStats_signals.BMS_Soc_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SOC_MAX_VAL)
    {
        value = CANSIG_BMS_SOC_MAX_VAL;
    }
    if (value < CANSIG_BMS_SOC_MIN_VAL)
    {
        value = CANSIG_BMS_SOC_MIN_VAL;
    }
}
void app_canRx_BMS_SocCorrupt_update(bool value)
{
    rx_table.BMS_VoltageAndChargeStats_signals.BMS_SocCorrupt_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SOC_CORRUPT_MAX_VAL)
    {
        value = CANSIG_BMS_SOC_CORRUPT_MAX_VAL;
    }
    if (value < CANSIG_BMS_SOC_CORRUPT_MIN_VAL)
    {
        value = CANSIG_BMS_SOC_CORRUPT_MIN_VAL;
    }
}
void app_canRx_BMS_MinCellVoltage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_VoltageAndChargeStats_signals.BMS_MinCellVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MIN_CELL_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MIN_CELL_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_VOLTAGE_MIN_VAL;
    }
}
void app_canRx_BMS_MaxCellVoltage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_VoltageAndChargeStats_signals.BMS_MaxCellVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_CELL_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_CELL_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_VOLTAGE_MIN_VAL;
    }
}

void app_canRx_BMS_BmsOk_update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_BmsOk_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BMS_OK_MAX_VAL)
    {
        value = CANSIG_BMS_BMS_OK_MAX_VAL;
    }
    if (value < CANSIG_BMS_BMS_OK_MIN_VAL)
    {
        value = CANSIG_BMS_BMS_OK_MIN_VAL;
    }
}
void app_canRx_BMS_ImdOk_update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_ImdOk_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_OK_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_OK_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_OK_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_OK_MIN_VAL;
    }
}
void app_canRx_BMS_BspdOk_update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_BspdOk_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BSPD_OK_MAX_VAL)
    {
        value = CANSIG_BMS_BSPD_OK_MAX_VAL;
    }
    if (value < CANSIG_BMS_BSPD_OK_MIN_VAL)
    {
        value = CANSIG_BMS_BSPD_OK_MIN_VAL;
    }
}
void app_canRx_BMS_BmsLatchedFault_update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BMS_LATCHED_FAULT_MAX_VAL)
    {
        value = CANSIG_BMS_BMS_LATCHED_FAULT_MAX_VAL;
    }
    if (value < CANSIG_BMS_BMS_LATCHED_FAULT_MIN_VAL)
    {
        value = CANSIG_BMS_BMS_LATCHED_FAULT_MIN_VAL;
    }
}
void app_canRx_BMS_ImdLatchedFault_update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_LATCHED_FAULT_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_LATCHED_FAULT_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_LATCHED_FAULT_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_LATCHED_FAULT_MIN_VAL;
    }
}
void app_canRx_BMS_BspdLatchedFault_update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BSPD_LATCHED_FAULT_MAX_VAL)
    {
        value = CANSIG_BMS_BSPD_LATCHED_FAULT_MAX_VAL;
    }
    if (value < CANSIG_BMS_BSPD_LATCHED_FAULT_MIN_VAL)
    {
        value = CANSIG_BMS_BSPD_LATCHED_FAULT_MIN_VAL;
    }
}

void app_canRx_BMS_TractiveSystemVoltage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN_VAL;
    }
}
void app_canRx_BMS_TractiveSystemCurrent_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MIN_VAL;
    }
}
void app_canRx_BMS_TractiveSystemPower_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemPower_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MIN_VAL;
    }
}
void app_canRx_BMS_AvailablePower_update(uint32_t value)
{
    rx_table.BMS_TractiveSystem_signals.BMS_AvailablePower_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_AVAILABLE_POWER_MAX_VAL)
    {
        value = CANSIG_BMS_AVAILABLE_POWER_MAX_VAL;
    }
    if (value < CANSIG_BMS_AVAILABLE_POWER_MIN_VAL)
    {
        value = CANSIG_BMS_AVAILABLE_POWER_MIN_VAL;
    }
}
void app_canRx_BMS_BSPDCurrentThresholdExceeded_update(bool value)
{
    rx_table.BMS_TractiveSystem_signals.BMS_BSPDCurrentThresholdExceeded_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MAX_VAL)
    {
        value = CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MAX_VAL;
    }
    if (value < CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MIN_VAL)
    {
        value = CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MIN_VAL;
    }
}

void app_canRx_BMS_Heartbeat_update(bool value)
{
    rx_table.BMS_Vitals_signals.BMS_Heartbeat_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_HEARTBEAT_MAX_VAL)
    {
        value = CANSIG_BMS_HEARTBEAT_MAX_VAL;
    }
    if (value < CANSIG_BMS_HEARTBEAT_MIN_VAL)
    {
        value = CANSIG_BMS_HEARTBEAT_MIN_VAL;
    }
}
void app_canRx_BMS_State_update(BmsState value)
{
    rx_table.BMS_Vitals_signals.BMS_State_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_STATE_MAX_VAL)
    {
        value = CANSIG_BMS_STATE_MAX_VAL;
    }
    if (value < CANSIG_BMS_STATE_MIN_VAL)
    {
        value = CANSIG_BMS_STATE_MIN_VAL;
    }
}




bool app_canRx_BMS_Warning_StackWaterMarkHighTask1Hz_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1Hz_value;
}
bool app_canRx_BMS_Warning_StackWaterMarkHighTask100Hz_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask100Hz_value;
}
bool app_canRx_BMS_Warning_StackWaterMarkHighTask1kHz_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1kHz_value;
}
bool app_canRx_BMS_Warning_StackWaterMarkHighTaskCanRx_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanRx_value;
}
bool app_canRx_BMS_Warning_StackWaterMarkHighTaskCanTx_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanTx_value;
}
bool app_canRx_BMS_Warning_WatchdogTimeout_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_WatchdogTimeout_value;
}
bool app_canRx_BMS_Warning_TxOverflow_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_TxOverflow_value;
}
bool app_canRx_BMS_Warning_RxOverflow_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_RxOverflow_value;
}
bool app_canRx_BMS_Warning_OpenWireCheckFault_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheckFault_value;
}
bool app_canRx_BMS_Warning_OpenWireCheck_Segment0_GND_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment0_GND_value;
}
bool app_canRx_BMS_Warning_OpenWireCheck_Segment1_GND_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment1_GND_value;
}
bool app_canRx_BMS_Warning_OpenWireCheck_Segment2_GND_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment2_GND_value;
}
bool app_canRx_BMS_Warning_OpenWireCheck_Segment3_GND_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment3_GND_value;
}
bool app_canRx_BMS_Warning_OpenWireCheck_Segment4_GND_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment4_GND_value;
}
bool app_canRx_BMS_Warning_MissingVCHeartbeat_get()
{
    return rx_table.BMS_Warnings_signals.BMS_Warning_MissingVCHeartbeat_value;
}

bool app_canRx_BMS_Fault_StateMachine_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_StateMachine_value;
}
bool app_canRx_BMS_Fault_CellUndervoltage_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellUndervoltage_value;
}
bool app_canRx_BMS_Fault_CellOvervoltage_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellOvervoltage_value;
}
bool app_canRx_BMS_Fault_ModuleCommunicationError_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_ModuleCommunicationError_value;
}
bool app_canRx_BMS_Fault_CellUndertemp_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellUndertemp_value;
}
bool app_canRx_BMS_Fault_CellOvertemp_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_CellOvertemp_value;
}
bool app_canRx_BMS_Fault_ChargerReportedError_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_ChargerReportedError_value;
}
bool app_canRx_BMS_Fault_ChargerDisconnectedDuringCharge_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_ChargerDisconnectedDuringCharge_value;
}
bool app_canRx_BMS_Fault_ChargerShutdownLoopOpen_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_ChargerShutdownLoopOpen_value;
}
bool app_canRx_BMS_Fault_TractiveSystemOvercurrent_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_TractiveSystemOvercurrent_value;
}
bool app_canRx_BMS_Fault_PrechargeFailure_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_PrechargeFailure_value;
}

uint32_t app_canRx_BMS_Warning_StackWaterMarkHighTask1HzCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1HzCount_value;
}
uint32_t app_canRx_BMS_Warning_StackWaterMarkHighTask100HzCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask100HzCount_value;
}
uint32_t app_canRx_BMS_Warning_StackWaterMarkHighTask1kHzCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1kHzCount_value;
}
uint32_t app_canRx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanRxCount_value;
}
uint32_t app_canRx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanTxCount_value;
}
uint32_t app_canRx_BMS_Warning_WatchdogTimeoutCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_WatchdogTimeoutCount_value;
}
uint32_t app_canRx_BMS_Warning_TxOverflowCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_TxOverflowCount_value;
}
uint32_t app_canRx_BMS_Warning_RxOverflowCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_RxOverflowCount_value;
}
uint32_t app_canRx_BMS_Warning_OpenWireCheckFaultCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheckFaultCount_value;
}
uint32_t app_canRx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment0_GNDCount_value;
}
uint32_t app_canRx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment1_GNDCount_value;
}
uint32_t app_canRx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment2_GNDCount_value;
}
uint32_t app_canRx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment3_GNDCount_value;
}
uint32_t app_canRx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment4_GNDCount_value;
}
uint32_t app_canRx_BMS_Warning_MissingVCHeartbeatCount_get()
{
    return rx_table.BMS_WarningsCounts_signals.BMS_Warning_MissingVCHeartbeatCount_value;
}

uint32_t app_canRx_BMS_Fault_StateMachineCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_StateMachineCount_value;
}
uint32_t app_canRx_BMS_Fault_CellUndervoltageCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndervoltageCount_value;
}
uint32_t app_canRx_BMS_Fault_CellOvervoltageCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvervoltageCount_value;
}
uint32_t app_canRx_BMS_Fault_ModuleCommunicationErrorCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_ModuleCommunicationErrorCount_value;
}
uint32_t app_canRx_BMS_Fault_CellUndertempCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndertempCount_value;
}
uint32_t app_canRx_BMS_Fault_CellOvertempCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvertempCount_value;
}
uint32_t app_canRx_BMS_Fault_ChargerReportedErrorCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerReportedErrorCount_value;
}
uint32_t app_canRx_BMS_Fault_ChargerDisconnectedDuringChargeCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerDisconnectedDuringChargeCount_value;
}
uint32_t app_canRx_BMS_Fault_ChargerShutdownLoopOpenCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerShutdownLoopOpenCount_value;
}
uint32_t app_canRx_BMS_Fault_TractiveSystemOvercurrentCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_TractiveSystemOvercurrentCount_value;
}
uint32_t app_canRx_BMS_Fault_PrechargeFailureCount_get()
{
    return rx_table.BMS_FaultsCounts_signals.BMS_Fault_PrechargeFailureCount_value;
}

bool app_canRx_FSM_Warning_WatchdogTimeout_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_WatchdogTimeout_value;
}
bool app_canRx_FSM_Warning_TxOverflow_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_TxOverflow_value;
}
bool app_canRx_FSM_Warning_RxOverflow_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_RxOverflow_value;
}
bool app_canRx_FSM_Warning_StackWaterMarkHighTask1Hz_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1Hz_value;
}
bool app_canRx_FSM_Warning_StackWaterMarkHighTask100Hz_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask100Hz_value;
}
bool app_canRx_FSM_Warning_StackWaterMarkHighTask1kHz_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1kHz_value;
}
bool app_canRx_FSM_Warning_StackWaterMarkHighTaskCanRx_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanRx_value;
}
bool app_canRx_FSM_Warning_StackWaterMarkHighTaskCanTx_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanTx_value;
}
bool app_canRx_FSM_Warning_SteeringAngleOCSC_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOCSC_value;
}
bool app_canRx_FSM_Warning_SteeringAngleOutOfRange_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOutOfRange_value;
}
bool app_canRx_FSM_Warning_LeftWheelSpeedOutOfRange_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_LeftWheelSpeedOutOfRange_value;
}
bool app_canRx_FSM_Warning_RightWheelSpeedOutOfRange_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_RightWheelSpeedOutOfRange_value;
}
bool app_canRx_FSM_Warning_AppsDisagreement_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_AppsDisagreement_value;
}
bool app_canRx_FSM_Warning_FrontBrakePressureOutOfRange_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_FrontBrakePressureOutOfRange_value;
}
bool app_canRx_FSM_Warning_RearBrakePressureOutOfRange_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_RearBrakePressureOutOfRange_value;
}
bool app_canRx_FSM_Warning_FrontBrakePressureOcSc_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_FrontBrakePressureOcSc_value;
}
bool app_canRx_FSM_Warning_RearBrakePressureOcSc_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_RearBrakePressureOcSc_value;
}
bool app_canRx_FSM_Warning_BrakeOcScNotOk_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_BrakeOcScNotOk_value;
}

bool app_canRx_FSM_Fault_PappsOCSC_get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_PappsOCSC_value;
}
bool app_canRx_FSM_Fault_SappsOCSC_get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_SappsOCSC_value;
}

uint32_t app_canRx_FSM_Warning_WatchdogTimeoutCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_WatchdogTimeoutCount_value;
}
uint32_t app_canRx_FSM_Warning_TxOverflowCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_TxOverflowCount_value;
}
uint32_t app_canRx_FSM_Warning_RxOverflowCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_RxOverflowCount_value;
}
uint32_t app_canRx_FSM_Warning_StackWaterMarkHighTask1HzCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask1HzCount_value;
}
uint32_t app_canRx_FSM_Warning_StackWaterMarkHighTask100HzCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask100HzCount_value;
}
uint32_t app_canRx_FSM_Warning_StackWaterMarkHighTask1kHzCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask1kHzCount_value;
}
uint32_t app_canRx_FSM_Warning_StackWaterMarkHighTaskCanRxCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTaskCanRxCount_value;
}
uint32_t app_canRx_FSM_Warning_StackWaterMarkHighTaskCanTxCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTaskCanTxCount_value;
}
uint32_t app_canRx_FSM_Warning_SteeringAngleOCSCCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_SteeringAngleOCSCCount_value;
}
uint32_t app_canRx_FSM_Warning_SteeringAngleOutOfRangeCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_SteeringAngleOutOfRangeCount_value;
}
uint32_t app_canRx_FSM_Warning_LeftWheelSpeedOutOfRangeCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_LeftWheelSpeedOutOfRangeCount_value;
}
uint32_t app_canRx_FSM_Warning_RightWheelSpeedOutOfRangeCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_RightWheelSpeedOutOfRangeCount_value;
}
uint32_t app_canRx_FSM_Warning_AppsDisagreementCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_AppsDisagreementCount_value;
}
uint32_t app_canRx_FSM_Warning_FrontBrakePressureOutOfRangeCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_FrontBrakePressureOutOfRangeCount_value;
}
uint32_t app_canRx_FSM_Warning_RearBrakePressureOutOfRangeCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_RearBrakePressureOutOfRangeCount_value;
}
uint32_t app_canRx_FSM_Warning_FrontBrakePressureOcScCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_FrontBrakePressureOcScCount_value;
}
uint32_t app_canRx_FSM_Warning_RearBrakePressureOcScCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_RearBrakePressureOcScCount_value;
}
uint32_t app_canRx_FSM_Warning_BrakeOcScNotOkCount_get()
{
    return rx_table.FSM_WarningsCounts_signals.FSM_Warning_BrakeOcScNotOkCount_value;
}

uint32_t app_canRx_FSM_Fault_PappsOCSCCount_get()
{
    return rx_table.FSM_FaultsCounts_signals.FSM_Fault_PappsOCSCCount_value;
}
uint32_t app_canRx_FSM_Fault_SappsOCSCCount_get()
{
    return rx_table.FSM_FaultsCounts_signals.FSM_Fault_SappsOCSCCount_value;
}

float app_canRx_BMS_TractiveSystemVoltage_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value;
}
float app_canRx_BMS_TractiveSystemCurrent_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value;
}
float app_canRx_BMS_TractiveSystemPower_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemPower_value;
}
uint32_t app_canRx_BMS_AvailablePower_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_AvailablePower_value;
}
bool app_canRx_BMS_BSPDCurrentThresholdExceeded_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_BSPDCurrentThresholdExceeded_value;
}

bool app_canRx_BMS_HVDShdnOKStatus_get()
{
    return rx_table.BMS_BMSShdnNodeStatus_signals.BMS_HVDShdnOKStatus_value;
}
bool app_canRx_BMS_TSIlckOKStatus_get()
{
    return rx_table.BMS_BMSShdnNodeStatus_signals.BMS_TSIlckOKStatus_value;
}

float app_canRx_BMS_MinCellTemperature_get()
{
    return rx_table.BMS_CellTemperatures_signals.BMS_MinCellTemperature_value;
}
float app_canRx_BMS_MaxCellTemperature_get()
{
    return rx_table.BMS_CellTemperatures_signals.BMS_MaxCellTemperature_value;
}

float app_canRx_BMS_PackVoltage_get()
{
    return rx_table.BMS_VoltageAndChargeStats_signals.BMS_PackVoltage_value;
}
float app_canRx_BMS_Soc_get()
{
    return rx_table.BMS_VoltageAndChargeStats_signals.BMS_Soc_value;
}
bool app_canRx_BMS_SocCorrupt_get()
{
    return rx_table.BMS_VoltageAndChargeStats_signals.BMS_SocCorrupt_value;
}
float app_canRx_BMS_MinCellVoltage_get()
{
    return rx_table.BMS_VoltageAndChargeStats_signals.BMS_MinCellVoltage_value;
}
float app_canRx_BMS_MaxCellVoltage_get()
{
    return rx_table.BMS_VoltageAndChargeStats_signals.BMS_MaxCellVoltage_value;
}

bool app_canRx_BMS_BmsOk_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BmsOk_value;
}
bool app_canRx_BMS_ImdOk_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_ImdOk_value;
}
bool app_canRx_BMS_BspdOk_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BspdOk_value;
}
bool app_canRx_BMS_BmsLatchedFault_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value;
}
bool app_canRx_BMS_ImdLatchedFault_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value;
}
bool app_canRx_BMS_BspdLatchedFault_get()
{
    return rx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value;
}

float app_canRx_BMS_TractiveSystemVoltage_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value;
}
float app_canRx_BMS_TractiveSystemCurrent_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value;
}
float app_canRx_BMS_TractiveSystemPower_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemPower_value;
}
uint32_t app_canRx_BMS_AvailablePower_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_AvailablePower_value;
}
bool app_canRx_BMS_BSPDCurrentThresholdExceeded_get()
{
    return rx_table.BMS_TractiveSystem_signals.BMS_BSPDCurrentThresholdExceeded_value;
}

bool app_canRx_BMS_Heartbeat_get()
{
    return rx_table.BMS_Vitals_signals.BMS_Heartbeat_value;
}
BmsState app_canRx_BMS_State_get()
{
    return rx_table.BMS_Vitals_signals.BMS_State_value;
}

