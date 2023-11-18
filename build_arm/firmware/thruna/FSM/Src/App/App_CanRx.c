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
 * Struct for holding all messages received by FSM (i.e. the RX table).
 */
typedef struct
{
    Debug_CanMode_Signals Debug_CanMode_signals;
    BMS_Vitals_Signals BMS_Vitals_signals;
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    DCM_Vitals_Signals DCM_Vitals_signals;
    DCM_LeftInverterCommand_Signals DCM_LeftInverterCommand_signals;
    DCM_RightInverterCommand_Signals DCM_RightInverterCommand_signals;
    DCM_Warnings_Signals DCM_Warnings_signals;
    DCM_Faults_Signals DCM_Faults_signals;
    PDM_Warnings_Signals PDM_Warnings_signals;
    PDM_Faults_Signals PDM_Faults_signals;
    DIM_Warnings_Signals DIM_Warnings_signals;
    DIM_Faults_Signals DIM_Faults_signals;
} FSM_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static FSM_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(FSM_RxMsgs));
    App_CanRx_Debug_EnableDebugMode_Update(CANSIG_DEBUG_CAN_MODE_DEBUG_ENABLE_DEBUG_MODE_START_VAL);
    App_CanRx_BMS_Heartbeat_Update(CANSIG_BMS_VITALS_BMS_HEARTBEAT_START_VAL);
    App_CanRx_BMS_State_Update(CANSIG_BMS_VITALS_BMS_STATE_START_VAL);
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
    App_CanRx_DCM_LeftInverterTorqueCommand_Update(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_START_VAL);
    App_CanRx_DCM_LeftInverterSpeedCommand_Update(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_START_VAL);
    App_CanRx_DCM_LeftInverterDirectionCommand_Update(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_DIRECTION_COMMAND_START_VAL);
    App_CanRx_DCM_LeftInverterEnable_Update(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_START_VAL);
    App_CanRx_DCM_LeftInverterEnableDischarge_Update(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_DISCHARGE_START_VAL);
    App_CanRx_DCM_LeftInverterEnableSpeedMode_Update(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_SPEED_MODE_START_VAL);
    App_CanRx_DCM_LeftInverterTorqueLimit_Update(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_START_VAL);
    App_CanRx_DCM_RightInverterTorqueCommand_Update(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_START_VAL);
    App_CanRx_DCM_RightInverterSpeedCommand_Update(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_START_VAL);
    App_CanRx_DCM_RightInverterDirectionCommand_Update(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_DIRECTION_COMMAND_START_VAL);
    App_CanRx_DCM_RightInverterEnable_Update(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_START_VAL);
    App_CanRx_DCM_RightInverterEnableDischarge_Update(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_DISCHARGE_START_VAL);
    App_CanRx_DCM_RightInverterEnableSpeedMode_Update(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_SPEED_MODE_START_VAL);
    App_CanRx_DCM_RightInverterTorqueLimit_Update(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_START_VAL);
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
    App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_PDM_WatchdogTimeoutWarning_Update(CANSIG_PDM_WARNINGS_PDM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_PDM_TxOverflowWarning_Update(CANSIG_PDM_WARNINGS_PDM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_PDM_RxOverflowWarning_Update(CANSIG_PDM_WARNINGS_PDM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_PDM_DummyFault_Update(CANSIG_PDM_FAULTS_PDM_DUMMY_FAULT_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_DIM_WatchdogTimeoutWarning_Update(CANSIG_DIM_WARNINGS_DIM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_DIM_TxOverflowWarning_Update(CANSIG_DIM_WARNINGS_DIM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_DIM_RxOverflowWarning_Update(CANSIG_DIM_WARNINGS_DIM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_DIM_MissingHeartbeatFault_Update(CANSIG_DIM_FAULTS_DIM_MISSING_HEARTBEAT_FAULT_START_VAL);
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

void App_CanRx_DCM_LeftInverterTorqueCommand_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_MIN ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_MIN : value;
    rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterTorqueCommand_value = tmp > CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_MAX ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_MAX : tmp;
}

void App_CanRx_DCM_LeftInverterSpeedCommand_Update(int value)
{
    const int tmp = value < CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_MIN ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_MIN : value;
    rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterSpeedCommand_value = tmp > CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_MAX ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_MAX : tmp;
}

void App_CanRx_DCM_LeftInverterDirectionCommand_Update(InverterDirection value)
{
    rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterDirectionCommand_value = (value > CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_DIRECTION_COMMAND_MAX) ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_DIRECTION_COMMAND_MAX : value;
}

void App_CanRx_DCM_LeftInverterEnable_Update(bool value)
{
    rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnable_value = value;
}

void App_CanRx_DCM_LeftInverterEnableDischarge_Update(bool value)
{
    rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnableDischarge_value = value;
}

void App_CanRx_DCM_LeftInverterEnableSpeedMode_Update(bool value)
{
    rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnableSpeedMode_value = value;
}

void App_CanRx_DCM_LeftInverterTorqueLimit_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_MIN ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_MIN : value;
    rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterTorqueLimit_value = tmp > CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_MAX ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_MAX : tmp;
}

void App_CanRx_DCM_RightInverterTorqueCommand_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_MIN ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_MIN : value;
    rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterTorqueCommand_value = tmp > CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_MAX ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_MAX : tmp;
}

void App_CanRx_DCM_RightInverterSpeedCommand_Update(int value)
{
    const int tmp = value < CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_MIN ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_MIN : value;
    rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterSpeedCommand_value = tmp > CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_MAX ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_MAX : tmp;
}

void App_CanRx_DCM_RightInverterDirectionCommand_Update(InverterDirection value)
{
    rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterDirectionCommand_value = (value > CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_DIRECTION_COMMAND_MAX) ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_DIRECTION_COMMAND_MAX : value;
}

void App_CanRx_DCM_RightInverterEnable_Update(bool value)
{
    rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnable_value = value;
}

void App_CanRx_DCM_RightInverterEnableDischarge_Update(bool value)
{
    rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnableDischarge_value = value;
}

void App_CanRx_DCM_RightInverterEnableSpeedMode_Update(bool value)
{
    rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnableSpeedMode_value = value;
}

void App_CanRx_DCM_RightInverterTorqueLimit_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_MIN ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_MIN : value;
    rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterTorqueLimit_value = tmp > CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_MAX ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_MAX : tmp;
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

float App_CanRx_DCM_LeftInverterTorqueCommand_Get()
{
    return rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterTorqueCommand_value;
}

int App_CanRx_DCM_LeftInverterSpeedCommand_Get()
{
    return rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterSpeedCommand_value;
}

InverterDirection App_CanRx_DCM_LeftInverterDirectionCommand_Get()
{
    return rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterDirectionCommand_value;
}

bool App_CanRx_DCM_LeftInverterEnable_Get()
{
    return rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnable_value;
}

bool App_CanRx_DCM_LeftInverterEnableDischarge_Get()
{
    return rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnableDischarge_value;
}

bool App_CanRx_DCM_LeftInverterEnableSpeedMode_Get()
{
    return rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnableSpeedMode_value;
}

float App_CanRx_DCM_LeftInverterTorqueLimit_Get()
{
    return rx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterTorqueLimit_value;
}

float App_CanRx_DCM_RightInverterTorqueCommand_Get()
{
    return rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterTorqueCommand_value;
}

int App_CanRx_DCM_RightInverterSpeedCommand_Get()
{
    return rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterSpeedCommand_value;
}

InverterDirection App_CanRx_DCM_RightInverterDirectionCommand_Get()
{
    return rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterDirectionCommand_value;
}

bool App_CanRx_DCM_RightInverterEnable_Get()
{
    return rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnable_value;
}

bool App_CanRx_DCM_RightInverterEnableDischarge_Get()
{
    return rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnableDischarge_value;
}

bool App_CanRx_DCM_RightInverterEnableSpeedMode_Get()
{
    return rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnableSpeedMode_value;
}

float App_CanRx_DCM_RightInverterTorqueLimit_Get()
{
    return rx_table.DCM_RightInverterCommand_signals.DCM_RightInverterTorqueLimit_value;
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

