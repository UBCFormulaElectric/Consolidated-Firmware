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
    DIM_Vitals_Signals DIM_Vitals_signals;
    DIM_Warnings_Signals DIM_Warnings_signals;
    DIM_Faults_Signals DIM_Faults_signals;
    DCM_Vitals_Signals DCM_Vitals_signals;
    DCM_Warnings_Signals DCM_Warnings_signals;
    DCM_Faults_Signals DCM_Faults_signals;
    PDM_Vitals_Signals PDM_Vitals_signals;
    PDM_Warnings_Signals PDM_Warnings_signals;
    PDM_Faults_Signals PDM_Faults_signals;
    Debug_CanMode_Signals Debug_CanMode_signals;
    Debug_Charging_Signals Debug_Charging_signals;
    Debug_CellBalancing_Signals Debug_CellBalancing_signals;
    FSM_Vitals_Signals FSM_Vitals_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
} BMS_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static BMS_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(BMS_RxMsgs));
    App_CanRx_DIM_Heartbeat_Update(CANSIG_DIM_VITALS_DIM_HEARTBEAT_START_VAL);
    App_CanRx_DIM_State_Update(CANSIG_DIM_VITALS_DIM_STATE_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_DIM_WatchdogTimeoutWarning_Update(CANSIG_DIM_WARNINGS_DIM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_DIM_TxOverflowWarning_Update(CANSIG_DIM_WARNINGS_DIM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_DIM_RxOverflowWarning_Update(CANSIG_DIM_WARNINGS_DIM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_DIM_MissingHeartbeatFault_Update(CANSIG_DIM_FAULTS_DIM_MISSING_HEARTBEAT_FAULT_START_VAL);
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
    App_CanRx_PDM_Heartbeat_Update(CANSIG_PDM_VITALS_PDM_HEARTBEAT_START_VAL);
    App_CanRx_PDM_State_Update(CANSIG_PDM_VITALS_PDM_STATE_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Update(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanRx_PDM_WatchdogTimeoutWarning_Update(CANSIG_PDM_WARNINGS_PDM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanRx_PDM_TxOverflowWarning_Update(CANSIG_PDM_WARNINGS_PDM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_PDM_RxOverflowWarning_Update(CANSIG_PDM_WARNINGS_PDM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanRx_PDM_DummyFault_Update(CANSIG_PDM_FAULTS_PDM_DUMMY_FAULT_START_VAL);
    App_CanRx_Debug_EnableDebugMode_Update(CANSIG_DEBUG_CAN_MODE_DEBUG_ENABLE_DEBUG_MODE_START_VAL);
    App_CanRx_Debug_StartCharging_Update(CANSIG_DEBUG_CHARGING_DEBUG_START_CHARGING_START_VAL);
    App_CanRx_Debug_CellBalancingRequest_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_REQUEST_START_VAL);
    App_CanRx_Debug_CellBalancingOverrideTarget_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_START_VAL);
    App_CanRx_Debug_CellBalancingOverrideTargetValue_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_VALUE_START_VAL);
    App_CanRx_Debug_CellBalancingOverridePWM_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_START_VAL);
    App_CanRx_Debug_CellBalancingOverridePWMFrequency_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_FREQUENCY_START_VAL);
    App_CanRx_Debug_CellBalancingOverridePWMDuty_Update(CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_DUTY_START_VAL);
    App_CanRx_FSM_Heartbeat_Update(CANSIG_FSM_VITALS_FSM_HEARTBEAT_START_VAL);
    App_CanRx_FSM_State_Update(CANSIG_FSM_VITALS_FSM_STATE_START_VAL);
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

void App_CanRx_DIM_Heartbeat_Update(bool value)
{
    rx_table.DIM_Vitals_signals.DIM_Heartbeat_value = value;
}

void App_CanRx_DIM_State_Update(DimState value)
{
    rx_table.DIM_Vitals_signals.DIM_State_value = (value > CANSIG_DIM_VITALS_DIM_STATE_MAX) ? CANSIG_DIM_VITALS_DIM_STATE_MAX : value;
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

void App_CanRx_PDM_Heartbeat_Update(bool value)
{
    rx_table.PDM_Vitals_signals.PDM_Heartbeat_value = value;
}

void App_CanRx_PDM_State_Update(PdmState value)
{
    rx_table.PDM_Vitals_signals.PDM_State_value = (value > CANSIG_PDM_VITALS_PDM_STATE_MAX) ? CANSIG_PDM_VITALS_PDM_STATE_MAX : value;
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

void App_CanRx_FSM_Heartbeat_Update(bool value)
{
    rx_table.FSM_Vitals_signals.FSM_Heartbeat_value = value;
}

void App_CanRx_FSM_State_Update(State_Machine value)
{
    rx_table.FSM_Vitals_signals.FSM_State_value = (value > CANSIG_FSM_VITALS_FSM_STATE_MAX) ? CANSIG_FSM_VITALS_FSM_STATE_MAX : value;
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

bool App_CanRx_DIM_Heartbeat_Get()
{
    return rx_table.DIM_Vitals_signals.DIM_Heartbeat_value;
}

DimState App_CanRx_DIM_State_Get()
{
    return rx_table.DIM_Vitals_signals.DIM_State_value;
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

bool App_CanRx_PDM_Heartbeat_Get()
{
    return rx_table.PDM_Vitals_signals.PDM_Heartbeat_value;
}

PdmState App_CanRx_PDM_State_Get()
{
    return rx_table.PDM_Vitals_signals.PDM_State_value;
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

bool App_CanRx_FSM_Heartbeat_Get()
{
    return rx_table.FSM_Vitals_signals.FSM_Heartbeat_value;
}

State_Machine App_CanRx_FSM_State_Get()
{
    return rx_table.FSM_Vitals_signals.FSM_State_value;
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

