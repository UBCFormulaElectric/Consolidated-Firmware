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
    VC_Vitals_Signals VC_Vitals_signals;
    VC_Warnings_Signals VC_Warnings_signals;
    VC_Faults_Signals VC_Faults_signals;
} BMS_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static BMS_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canRx_init()
{
    memset(&rx_table, 0, sizeof(BMS_RxMsgs));
    // Initialize all signals to Start-up value
    app_canRx_VC_Heartbeat_update(CANSIG_VC_HEARTBEAT_START_VAL);
    app_canRx_VC_State_update(CANSIG_VC_STATE_START_VAL);
    app_canRx_VC_BuzzerOn_update(CANSIG_VC_BUZZER_ON_START_VAL);
    app_canRx_VC_NumberOfCanDataLogs_update(CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_START_VAL);
    app_canRx_VC_CanLoggingRemainingErrors_update(CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_START_VAL);
    app_canRx_VC_Warning_StackWaterMarkHighTask1Hz_update(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    app_canRx_VC_Warning_StackWaterMarkHighTask100Hz_update(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    app_canRx_VC_Warning_StackWaterMarkHighTask1kHz_update(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    app_canRx_VC_Warning_StackWaterMarkHighTaskCanRx_update(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    app_canRx_VC_Warning_StackWaterMarkHighTaskCanTx_update(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    app_canRx_VC_Warning_WatchdogTimeout_update(CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    app_canRx_VC_Warning_TxOverflow_update(CANSIG_VC_WARNING_TX_OVERFLOW_START_VAL);
    app_canRx_VC_Warning_RxOverflow_update(CANSIG_VC_WARNING_RX_OVERFLOW_START_VAL);
    app_canRx_VC_Warning_RegenNotAvailable_update(CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_START_VAL);
    app_canRx_VC_Warning_ImuInitFailed_update(CANSIG_VC_WARNING_IMU_INIT_FAILED_START_VAL);
    app_canRx_VC_Warning_FlowRateInputOutOfRange_update(CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_START_VAL);
    app_canRx_VC_Warning_BatteryRailOvercurrentFault_update(CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_START_VAL);
    app_canRx_VC_Warning_AccumulatorRailOvercurrentFault_update(CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_START_VAL);
    app_canRx_VC_Warning_LvChargeFault_update(CANSIG_VC_WARNING_LV_CHARGE_FAULT_START_VAL);
    app_canRx_VC_Warning_BoostControllerFault_update(CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_START_VAL);
    app_canRx_VC_Warning_HighNumberOfCanDataLogs_update(CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_START_VAL);
    app_canRx_VC_Warning_CanLoggingSdCardNotPresent_update(CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_START_VAL);
    app_canRx_VC_Warning_CanLoggingErrored_update(CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_START_VAL);
    app_canRx_VC_Warning_BrakeAppsDisagreement_update(CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_START_VAL);
    app_canRx_VC_Warning_SbgInitFailed_update(CANSIG_VC_WARNING_SBG_INIT_FAILED_START_VAL);
    app_canRx_VC_Fault_MissingBMSHeartbeat_update(CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_START_VAL);
    app_canRx_VC_Fault_MissingFSMHeartbeat_update(CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_START_VAL);
    app_canRx_VC_Fault_MissingRSMHeartbeat_update(CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_START_VAL);
    app_canRx_VC_Fault_MissingCRITHeartbeat_update(CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_START_VAL);
    app_canRx_VC_Fault_LeftInverterFault_update(CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_START_VAL);
    app_canRx_VC_Fault_RightInverterFault_update(CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_START_VAL);
    
}

void app_canRx_BRUSA_IsConnected_update(bool value)
{
    rx_table.BRUSA_Vitals_signals.BRUSA_IsConnected_value = value;
}

void app_canRx_VC_Heartbeat_update(bool value)
{
    tx_table.VC_Vitals_signals.VC_Heartbeat_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_HEARTBEAT_MAX_VAL)
    {
        value = CANSIG_VC_HEARTBEAT_MAX_VAL;
    }
    if (value < CANSIG_VC_HEARTBEAT_MIN_VAL)
    {
        value = CANSIG_VC_HEARTBEAT_MIN_VAL;
    }
}
void app_canRx_VC_State_update(VcState value)
{
    tx_table.VC_Vitals_signals.VC_State_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_STATE_MAX_VAL)
    {
        value = CANSIG_VC_STATE_MAX_VAL;
    }
    if (value < CANSIG_VC_STATE_MIN_VAL)
    {
        value = CANSIG_VC_STATE_MIN_VAL;
    }
}
void app_canRx_VC_BuzzerOn_update(bool value)
{
    tx_table.VC_Vitals_signals.VC_BuzzerOn_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_BUZZER_ON_MAX_VAL)
    {
        value = CANSIG_VC_BUZZER_ON_MAX_VAL;
    }
    if (value < CANSIG_VC_BUZZER_ON_MIN_VAL)
    {
        value = CANSIG_VC_BUZZER_ON_MIN_VAL;
    }
}
void app_canRx_VC_NumberOfCanDataLogs_update(uint32_t value)
{
    tx_table.VC_Vitals_signals.VC_NumberOfCanDataLogs_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_MAX_VAL)
    {
        value = CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_MAX_VAL;
    }
    if (value < CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_MIN_VAL)
    {
        value = CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_MIN_VAL;
    }
}
void app_canRx_VC_CanLoggingRemainingErrors_update(uint32_t value)
{
    tx_table.VC_Vitals_signals.VC_CanLoggingRemainingErrors_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_MAX_VAL)
    {
        value = CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_MAX_VAL;
    }
    if (value < CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_MIN_VAL)
    {
        value = CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_MIN_VAL;
    }
}

void app_canRx_VC_Warning_StackWaterMarkHighTask1Hz_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTask1Hz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MIN_VAL;
    }
}
void app_canRx_VC_Warning_StackWaterMarkHighTask100Hz_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTask100Hz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MIN_VAL;
    }
}
void app_canRx_VC_Warning_StackWaterMarkHighTask1kHz_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTask1kHz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MIN_VAL;
    }
}
void app_canRx_VC_Warning_StackWaterMarkHighTaskCanRx_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTaskCanRx_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MIN_VAL;
    }
}
void app_canRx_VC_Warning_StackWaterMarkHighTaskCanTx_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTaskCanTx_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MIN_VAL;
    }
}
void app_canRx_VC_Warning_WatchdogTimeout_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_WatchdogTimeout_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_MIN_VAL;
    }
}
void app_canRx_VC_Warning_TxOverflow_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_TxOverflow_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_TX_OVERFLOW_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_TX_OVERFLOW_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_TX_OVERFLOW_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_TX_OVERFLOW_MIN_VAL;
    }
}
void app_canRx_VC_Warning_RxOverflow_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_RxOverflow_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_RX_OVERFLOW_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_RX_OVERFLOW_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_RX_OVERFLOW_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_RX_OVERFLOW_MIN_VAL;
    }
}
void app_canRx_VC_Warning_RegenNotAvailable_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_RegenNotAvailable_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_MIN_VAL;
    }
}
void app_canRx_VC_Warning_ImuInitFailed_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_ImuInitFailed_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_IMU_INIT_FAILED_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_IMU_INIT_FAILED_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_IMU_INIT_FAILED_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_IMU_INIT_FAILED_MIN_VAL;
    }
}
void app_canRx_VC_Warning_FlowRateInputOutOfRange_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_FlowRateInputOutOfRange_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_MIN_VAL;
    }
}
void app_canRx_VC_Warning_BatteryRailOvercurrentFault_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_BatteryRailOvercurrentFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_MIN_VAL;
    }
}
void app_canRx_VC_Warning_AccumulatorRailOvercurrentFault_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_AccumulatorRailOvercurrentFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_MIN_VAL;
    }
}
void app_canRx_VC_Warning_LvChargeFault_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_LvChargeFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_LV_CHARGE_FAULT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_LV_CHARGE_FAULT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_LV_CHARGE_FAULT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_LV_CHARGE_FAULT_MIN_VAL;
    }
}
void app_canRx_VC_Warning_BoostControllerFault_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_BoostControllerFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_MIN_VAL;
    }
}
void app_canRx_VC_Warning_HighNumberOfCanDataLogs_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_HighNumberOfCanDataLogs_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_MIN_VAL;
    }
}
void app_canRx_VC_Warning_CanLoggingSdCardNotPresent_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_CanLoggingSdCardNotPresent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_MIN_VAL;
    }
}
void app_canRx_VC_Warning_CanLoggingErrored_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_CanLoggingErrored_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_MIN_VAL;
    }
}
void app_canRx_VC_Warning_BrakeAppsDisagreement_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_BrakeAppsDisagreement_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_MIN_VAL;
    }
}
void app_canRx_VC_Warning_SbgInitFailed_update(bool value)
{
    tx_table.VC_Warnings_signals.VC_Warning_SbgInitFailed_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_SBG_INIT_FAILED_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_SBG_INIT_FAILED_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_SBG_INIT_FAILED_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_SBG_INIT_FAILED_MIN_VAL;
    }
}

void app_canRx_VC_Fault_MissingBMSHeartbeat_update(bool value)
{
    tx_table.VC_Faults_signals.VC_Fault_MissingBMSHeartbeat_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_MIN_VAL;
    }
}
void app_canRx_VC_Fault_MissingFSMHeartbeat_update(bool value)
{
    tx_table.VC_Faults_signals.VC_Fault_MissingFSMHeartbeat_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_MIN_VAL;
    }
}
void app_canRx_VC_Fault_MissingRSMHeartbeat_update(bool value)
{
    tx_table.VC_Faults_signals.VC_Fault_MissingRSMHeartbeat_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_MIN_VAL;
    }
}
void app_canRx_VC_Fault_MissingCRITHeartbeat_update(bool value)
{
    tx_table.VC_Faults_signals.VC_Fault_MissingCRITHeartbeat_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_MIN_VAL;
    }
}
void app_canRx_VC_Fault_LeftInverterFault_update(bool value)
{
    tx_table.VC_Faults_signals.VC_Fault_LeftInverterFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_MIN_VAL;
    }
}
void app_canRx_VC_Fault_RightInverterFault_update(bool value)
{
    tx_table.VC_Faults_signals.VC_Fault_RightInverterFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_MIN_VAL;
    }
}




bool app_canRx_VC_Heartbeat_get()
{
    return rx_table.VC_Vitals_signals.VC_Heartbeat_value;
}
VcState app_canRx_VC_State_get()
{
    return rx_table.VC_Vitals_signals.VC_State_value;
}
bool app_canRx_VC_BuzzerOn_get()
{
    return rx_table.VC_Vitals_signals.VC_BuzzerOn_value;
}
uint32_t app_canRx_VC_NumberOfCanDataLogs_get()
{
    return rx_table.VC_Vitals_signals.VC_NumberOfCanDataLogs_value;
}
uint32_t app_canRx_VC_CanLoggingRemainingErrors_get()
{
    return rx_table.VC_Vitals_signals.VC_CanLoggingRemainingErrors_value;
}

bool app_canRx_VC_Warning_StackWaterMarkHighTask1Hz_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTask1Hz_value;
}
bool app_canRx_VC_Warning_StackWaterMarkHighTask100Hz_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTask100Hz_value;
}
bool app_canRx_VC_Warning_StackWaterMarkHighTask1kHz_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTask1kHz_value;
}
bool app_canRx_VC_Warning_StackWaterMarkHighTaskCanRx_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTaskCanRx_value;
}
bool app_canRx_VC_Warning_StackWaterMarkHighTaskCanTx_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTaskCanTx_value;
}
bool app_canRx_VC_Warning_WatchdogTimeout_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_WatchdogTimeout_value;
}
bool app_canRx_VC_Warning_TxOverflow_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_TxOverflow_value;
}
bool app_canRx_VC_Warning_RxOverflow_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_RxOverflow_value;
}
bool app_canRx_VC_Warning_RegenNotAvailable_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_RegenNotAvailable_value;
}
bool app_canRx_VC_Warning_ImuInitFailed_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_ImuInitFailed_value;
}
bool app_canRx_VC_Warning_FlowRateInputOutOfRange_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_FlowRateInputOutOfRange_value;
}
bool app_canRx_VC_Warning_BatteryRailOvercurrentFault_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_BatteryRailOvercurrentFault_value;
}
bool app_canRx_VC_Warning_AccumulatorRailOvercurrentFault_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_AccumulatorRailOvercurrentFault_value;
}
bool app_canRx_VC_Warning_LvChargeFault_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_LvChargeFault_value;
}
bool app_canRx_VC_Warning_BoostControllerFault_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_BoostControllerFault_value;
}
bool app_canRx_VC_Warning_HighNumberOfCanDataLogs_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_HighNumberOfCanDataLogs_value;
}
bool app_canRx_VC_Warning_CanLoggingSdCardNotPresent_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_CanLoggingSdCardNotPresent_value;
}
bool app_canRx_VC_Warning_CanLoggingErrored_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_CanLoggingErrored_value;
}
bool app_canRx_VC_Warning_BrakeAppsDisagreement_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_BrakeAppsDisagreement_value;
}
bool app_canRx_VC_Warning_SbgInitFailed_get()
{
    return rx_table.VC_Warnings_signals.VC_Warning_SbgInitFailed_value;
}

bool app_canRx_VC_Fault_MissingBMSHeartbeat_get()
{
    return rx_table.VC_Faults_signals.VC_Fault_MissingBMSHeartbeat_value;
}
bool app_canRx_VC_Fault_MissingFSMHeartbeat_get()
{
    return rx_table.VC_Faults_signals.VC_Fault_MissingFSMHeartbeat_value;
}
bool app_canRx_VC_Fault_MissingRSMHeartbeat_get()
{
    return rx_table.VC_Faults_signals.VC_Fault_MissingRSMHeartbeat_value;
}
bool app_canRx_VC_Fault_MissingCRITHeartbeat_get()
{
    return rx_table.VC_Faults_signals.VC_Fault_MissingCRITHeartbeat_value;
}
bool app_canRx_VC_Fault_LeftInverterFault_get()
{
    return rx_table.VC_Faults_signals.VC_Fault_LeftInverterFault_value;
}
bool app_canRx_VC_Fault_RightInverterFault_get()
{
    return rx_table.VC_Faults_signals.VC_Fault_RightInverterFault_value;
}



