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
 * Struct for holding all messages received by BMS (i.e. the RX table).
 */
typedef struct
{
    DCM_Vitals_Signals DCM_Vitals_signals;
    DCM_Warnings_Signals DCM_Warnings_signals;
    DCM_Faults_Signals DCM_Faults_signals;
    DIM_Vitals_Signals DIM_Vitals_signals;
    DIM_Warnings_Signals DIM_Warnings_signals;
    DIM_Faults_Signals DIM_Faults_signals;
    FSM_Vitals_Signals FSM_Vitals_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
    Debug_CanMode_Signals Debug_CanMode_signals;
    Debug_Charging_Signals Debug_Charging_signals;
    Debug_CellBalancing_Signals Debug_CellBalancing_signals;
    PDM_Vitals_Signals PDM_Vitals_signals;
    PDM_Warnings_Signals PDM_Warnings_signals;
    PDM_Faults_Signals PDM_Faults_signals;
} BMS_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static BMS_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(BMS_RxMsgs));
    App_CanRx_DCM_Heartbeat_Update(CANSIG_DCM_VITALS_DCM_HEARTBEAT_START_VAL);
    App_CanRx_DCM_State_Update(CANSIG_DCM_VITALS_DCM_STATE_START_VAL);
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
    App_CanRx_DIM_Heartbeat_Update(CANSIG_DIM_VITALS_DIM_HEARTBEAT_START_VAL);
    App_CanRx_DIM_State_Update(CANSIG_DIM_VITALS_DIM_STATE_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanRx_DIM_Warning_WatchdogTimeout_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_DIM_Warning_TxOverflow_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_DIM_Warning_RxOverflow_Update(CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_DIM_Fault_MissingHeartbeat_Update(CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_FSM_Heartbeat_Update(CANSIG_FSM_VITALS_FSM_HEARTBEAT_START_VAL);
    App_CanRx_FSM_State_Update(CANSIG_FSM_VITALS_FSM_STATE_START_VAL);
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
    App_CanRx_Debug_StartCharging_Update(CANSIG_DEBUG_CHARGING_DEBUG_START_CHARGING_START_VAL);
    App_CanRx_Debug_CellBalancingRequest_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_REQUEST_START_VAL);
    App_CanRx_Debug_CellBalancingOverrideTarget_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_START_VAL);
    App_CanRx_Debug_CellBalancingOverrideTargetValue_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_VALUE_START_VAL);
    App_CanRx_Debug_CellBalancingOverridePWM_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_START_VAL);
    App_CanRx_Debug_CellBalancingOverridePWMFrequency_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_FREQUENCY_START_VAL);
    App_CanRx_Debug_CellBalancingOverridePWMDuty_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_DUTY_START_VAL);
    App_CanRx_PDM_Heartbeat_Update(CANSIG_PDM_VITALS_PDM_HEARTBEAT_START_VAL);
    App_CanRx_PDM_State_Update(CANSIG_PDM_VITALS_PDM_STATE_START_VAL);
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

void App_CanRx_DIM_Heartbeat_Update(bool value)
{
    rx_table.DIM_Vitals_signals.DIM_Heartbeat_value = value;
}

void App_CanRx_DIM_State_Update(DimState value)
{
    rx_table.DIM_Vitals_signals.DIM_State_value = (value > CANSIG_DIM_VITALS_DIM_STATE_MAX) ? CANSIG_DIM_VITALS_DIM_STATE_MAX : value;
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

void App_CanRx_FSM_Heartbeat_Update(bool value)
{
    rx_table.FSM_Vitals_signals.FSM_Heartbeat_value = value;
}

void App_CanRx_FSM_State_Update(State_Machine value)
{
    rx_table.FSM_Vitals_signals.FSM_State_value = (value > CANSIG_FSM_VITALS_FSM_STATE_MAX) ? CANSIG_FSM_VITALS_FSM_STATE_MAX : value;
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

void App_CanRx_Debug_StartCharging_Update(bool value)
{
    rx_table.Debug_Charging_signals.Debug_StartCharging_value = value;
}

void App_CanRx_Debug_CellBalancingRequest_Update(bool value)
{
    rx_table.Debug_CellBalancing_signals.Debug_CellBalancingRequest_value = value;
}

void App_CanRx_Debug_CellBalancingOverrideTarget_Update(bool value)
{
    rx_table.Debug_CellBalancing_signals.Debug_CellBalancingOverrideTarget_value = value;
}

void App_CanRx_Debug_CellBalancingOverrideTargetValue_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_VALUE_MIN ? CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_VALUE_MIN : value;
    rx_table.Debug_CellBalancing_signals.Debug_CellBalancingOverrideTargetValue_value = tmp > CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_VALUE_MAX ? CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_VALUE_MAX : tmp;
}

void App_CanRx_Debug_CellBalancingOverridePWM_Update(bool value)
{
    rx_table.Debug_CellBalancing_signals.Debug_CellBalancingOverridePWM_value = value;
}

void App_CanRx_Debug_CellBalancingOverridePWMFrequency_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_FREQUENCY_MIN ? CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_FREQUENCY_MIN : value;
    rx_table.Debug_CellBalancing_signals.Debug_CellBalancingOverridePWMFrequency_value = tmp > CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_FREQUENCY_MAX ? CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_FREQUENCY_MAX : tmp;
}

void App_CanRx_Debug_CellBalancingOverridePWMDuty_Update(uint32_t value)
{
    rx_table.Debug_CellBalancing_signals.Debug_CellBalancingOverridePWMDuty_value = (value > CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_DUTY_MAX) ? CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_DUTY_MAX : value;
}

void App_CanRx_PDM_Heartbeat_Update(bool value)
{
    rx_table.PDM_Vitals_signals.PDM_Heartbeat_value = value;
}

void App_CanRx_PDM_State_Update(PdmState value)
{
    rx_table.PDM_Vitals_signals.PDM_State_value = (value > CANSIG_PDM_VITALS_PDM_STATE_MAX) ? CANSIG_PDM_VITALS_PDM_STATE_MAX : value;
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

bool App_CanRx_DIM_Heartbeat_Get()
{
    return rx_table.DIM_Vitals_signals.DIM_Heartbeat_value;
}

DimState App_CanRx_DIM_State_Get()
{
    return rx_table.DIM_Vitals_signals.DIM_State_value;
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

bool App_CanRx_FSM_Heartbeat_Get()
{
    return rx_table.FSM_Vitals_signals.FSM_Heartbeat_value;
}

State_Machine App_CanRx_FSM_State_Get()
{
    return rx_table.FSM_Vitals_signals.FSM_State_value;
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

bool App_CanRx_Debug_StartCharging_Get()
{
    return rx_table.Debug_Charging_signals.Debug_StartCharging_value;
}

bool App_CanRx_Debug_CellBalancingRequest_Get()
{
    return rx_table.Debug_CellBalancing_signals.Debug_CellBalancingRequest_value;
}

bool App_CanRx_Debug_CellBalancingOverrideTarget_Get()
{
    return rx_table.Debug_CellBalancing_signals.Debug_CellBalancingOverrideTarget_value;
}

float App_CanRx_Debug_CellBalancingOverrideTargetValue_Get()
{
    return rx_table.Debug_CellBalancing_signals.Debug_CellBalancingOverrideTargetValue_value;
}

bool App_CanRx_Debug_CellBalancingOverridePWM_Get()
{
    return rx_table.Debug_CellBalancing_signals.Debug_CellBalancingOverridePWM_value;
}

float App_CanRx_Debug_CellBalancingOverridePWMFrequency_Get()
{
    return rx_table.Debug_CellBalancing_signals.Debug_CellBalancingOverridePWMFrequency_value;
}

uint32_t App_CanRx_Debug_CellBalancingOverridePWMDuty_Get()
{
    return rx_table.Debug_CellBalancing_signals.Debug_CellBalancingOverridePWMDuty_value;
}

bool App_CanRx_PDM_Heartbeat_Get()
{
    return rx_table.PDM_Vitals_signals.PDM_Heartbeat_value;
}

PdmState App_CanRx_PDM_State_Get()
{
    return rx_table.PDM_Vitals_signals.PDM_State_value;
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

