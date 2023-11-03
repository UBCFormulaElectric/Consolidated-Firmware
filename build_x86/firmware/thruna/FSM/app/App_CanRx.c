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
    DCM_Vitals_Signals DCM_Vitals_signals;
    DCM_LeftInverterCommand_Signals DCM_LeftInverterCommand_signals;
    DCM_RightInverterCommand_Signals DCM_RightInverterCommand_signals;
    DCM_Warnings_Signals DCM_Warnings_signals;
    DCM_Faults_Signals DCM_Faults_signals;
    DIM_Warnings_Signals DIM_Warnings_signals;
    DIM_Faults_Signals DIM_Faults_signals;
    BMS_Vitals_Signals BMS_Vitals_signals;
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    Debug_CanMode_Signals Debug_CanMode_signals;
    PDM_Warnings_Signals PDM_Warnings_signals;
    PDM_Faults_Signals PDM_Faults_signals;
} FSM_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static FSM_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(FSM_RxMsgs));
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
    App_CanRx_Debug_EnableDebugMode_Update(CANSIG_DEBUG_CAN_MODE_DEBUG_ENABLE_DEBUG_MODE_START_VAL);
    App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanRx_PDM_Warning_WatchdogTimeout_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_PDM_Warning_TxOverflow_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_PDM_Warning_RxOverflow_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_PDM_Fault_DummyFault_Update(CANSIG_PDM_FAULTS_PDM_FAULT_DUMMY_FAULT_START_VAL);
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

void App_CanRx_Debug_EnableDebugMode_Update(bool value)
{
    rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value = value;
}

void App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanRx_PDM_Warning_WatchdogTimeout_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_WatchdogTimeout_value = value;
}

void App_CanRx_PDM_Warning_TxOverflow_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_TxOverflow_value = value;
}

void App_CanRx_PDM_Warning_RxOverflow_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_Warning_RxOverflow_value = value;
}

void App_CanRx_PDM_Fault_DummyFault_Update(bool value)
{
    rx_table.PDM_Faults_signals.PDM_Fault_DummyFault_value = value;
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

bool App_CanRx_Debug_EnableDebugMode_Get()
{
    return rx_table.Debug_CanMode_signals.Debug_EnableDebugMode_value;
}

bool App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanRx_PDM_Warning_WatchdogTimeout_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_WatchdogTimeout_value;
}

bool App_CanRx_PDM_Warning_TxOverflow_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_TxOverflow_value;
}

bool App_CanRx_PDM_Warning_RxOverflow_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_Warning_RxOverflow_value;
}

bool App_CanRx_PDM_Fault_DummyFault_Get()
{
    return rx_table.PDM_Faults_signals.PDM_Fault_DummyFault_value;
}

