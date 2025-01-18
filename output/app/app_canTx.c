/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "app_canTx.h"


/* ------------------------------- Structs -------------------------------- */

/**
 * Struct for holding all messages transmitted by BMS (i.e. the TX table).
 */
typedef struct
{
    VC_Vitals_Signals VC_Vitals_signals;
    VC_LeftInverterCommand_Signals VC_LeftInverterCommand_signals;
    VC_INVL_ReadWriteParamCommand_Signals VC_INVL_ReadWriteParamCommand_signals;
    VC_RightInverterCommand_Signals VC_RightInverterCommand_signals;
    VC_INVR_ReadWriteParamCommand_Signals VC_INVR_ReadWriteParamCommand_signals;
    VC_AlertsContext_Signals VC_AlertsContext_signals;
    VC_EllipseStatus_Signals VC_EllipseStatus_signals;
    VC_EllipseTime_Signals VC_EllipseTime_signals;
    VC_EllipseEulerAngles_Signals VC_EllipseEulerAngles_signals;
    VC_CommitInfo_Signals VC_CommitInfo_signals;
    VC_PowerEstimate_Signals VC_PowerEstimate_signals;
    VC_PowerEstimatePID_Signals VC_PowerEstimatePID_signals;
    VC_PIDSlipRatio_Signals VC_PIDSlipRatio_signals;
    VC_SlipRatios_Signals VC_SlipRatios_signals;
    VC_ActiveDiffOutputs_Signals VC_ActiveDiffOutputs_signals;
    VC_PowerLimit_Signals VC_PowerLimit_signals;
    VC_VehicleDynamics_Signals VC_VehicleDynamics_signals;
    VC_EllipseEkfNavVelocity_Signals VC_EllipseEkfNavVelocity_signals;
    VC_Voltages_Signals VC_Voltages_signals;
    VC_CurrentSensing_Signals VC_CurrentSensing_signals;
    VC_Efuse_Signals VC_Efuse_signals;
    VC_ImuLinearAcceleration_Signals VC_ImuLinearAcceleration_signals;
    VC_CoolantPumpFlowRate_Signals VC_CoolantPumpFlowRate_signals;
    VC_VCShdnNodeStatus_Signals VC_VCShdnNodeStatus_signals;
    VC_GlobalShdnNodeStatus_Signals VC_GlobalShdnNodeStatus_signals;
    VC_PedalPercentage_Signals VC_PedalPercentage_signals;
    VC_Warnings_Signals VC_Warnings_signals;
    VC_Faults_Signals VC_Faults_signals;
    VC_WarningsCounts_Signals VC_WarningsCounts_signals;
    VC_FaultsCounts_Signals VC_FaultsCounts_signals;
} VC_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static VC_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canTx_init()
{
    memset(&tx_table, 0, sizeof(VC_TxMsgs));
    app_canTx_VC_Heartbeat_set(CANSIG_VC_HEARTBEAT_START_VAL);
    app_canTx_VC_State_set(CANSIG_VC_STATE_START_VAL);
    app_canTx_VC_BuzzerOn_set(CANSIG_VC_BUZZER_ON_START_VAL);
    app_canTx_VC_NumberOfCanDataLogs_set(CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_START_VAL);
    app_canTx_VC_CanLoggingRemainingErrors_set(CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_START_VAL);
    app_canTx_VC_LeftInverterTorqueCommand_set(CANSIG_VC_LEFT_INVERTER_TORQUE_COMMAND_START_VAL);
    app_canTx_VC_LeftInverterSpeedCommand_set(CANSIG_VC_LEFT_INVERTER_SPEED_COMMAND_START_VAL);
    app_canTx_VC_LeftInverterDirectionCommand_set(CANSIG_VC_LEFT_INVERTER_DIRECTION_COMMAND_START_VAL);
    app_canTx_VC_LeftInverterEnable_set(CANSIG_VC_LEFT_INVERTER_ENABLE_START_VAL);
    app_canTx_VC_LeftInverterEnableDischarge_set(CANSIG_VC_LEFT_INVERTER_ENABLE_DISCHARGE_START_VAL);
    app_canTx_VC_LeftInverterEnableSpeedMode_set(CANSIG_VC_LEFT_INVERTER_ENABLE_SPEED_MODE_START_VAL);
    app_canTx_VC_LeftInverterTorqueLimit_set(CANSIG_VC_LEFT_INVERTER_TORQUE_LIMIT_START_VAL);
    app_canTx_VC_INVL_CommandParameterAddress_set(CANSIG_VC_INVL_COMMAND_PARAMETER_ADDRESS_START_VAL);
    app_canTx_VC_INVL_CommandReadWrite_set(CANSIG_VC_INVL_COMMAND_READ_WRITE_START_VAL);
    app_canTx_VC_INVL_CommandData_set(CANSIG_VC_INVL_COMMAND_DATA_START_VAL);
    app_canTx_VC_RightInverterTorqueCommand_set(CANSIG_VC_RIGHT_INVERTER_TORQUE_COMMAND_START_VAL);
    app_canTx_VC_RightInverterSpeedCommand_set(CANSIG_VC_RIGHT_INVERTER_SPEED_COMMAND_START_VAL);
    app_canTx_VC_RightInverterDirectionCommand_set(CANSIG_VC_RIGHT_INVERTER_DIRECTION_COMMAND_START_VAL);
    app_canTx_VC_RightInverterEnable_set(CANSIG_VC_RIGHT_INVERTER_ENABLE_START_VAL);
    app_canTx_VC_RightInverterEnableDischarge_set(CANSIG_VC_RIGHT_INVERTER_ENABLE_DISCHARGE_START_VAL);
    app_canTx_VC_RightInverterEnableSpeedMode_set(CANSIG_VC_RIGHT_INVERTER_ENABLE_SPEED_MODE_START_VAL);
    app_canTx_VC_RightInverterTorqueLimit_set(CANSIG_VC_RIGHT_INVERTER_TORQUE_LIMIT_START_VAL);
    app_canTx_VC_INVR_CommandParameterAddress_set(CANSIG_VC_INVR_COMMAND_PARAMETER_ADDRESS_START_VAL);
    app_canTx_VC_INVR_CommandReadWrite_set(CANSIG_VC_INVR_COMMAND_READ_WRITE_START_VAL);
    app_canTx_VC_INVR_CommandData_set(CANSIG_VC_INVR_COMMAND_DATA_START_VAL);
    app_canTx_VC_WatchdogTimeout_set(CANSIG_VC_WATCHDOG_TIMEOUT_START_VAL);
    app_canTx_VC_WatchdogTimeoutTaskName_set(CANSIG_VC_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    app_canTx_VC_TxOverflowCount_set(CANSIG_VC_TX_OVERFLOW_COUNT_START_VAL);
    app_canTx_VC_RxOverflowCount_set(CANSIG_VC_RX_OVERFLOW_COUNT_START_VAL);
    app_canTx_VC_EllipseGeneralStatusBitmask_set(CANSIG_VC_ELLIPSE_GENERAL_STATUS_BITMASK_START_VAL);
    app_canTx_VC_EllipseComStatusBitmask_set(CANSIG_VC_ELLIPSE_COM_STATUS_BITMASK_START_VAL);
    app_canTx_VC_EkfSolutionMode_set(CANSIG_VC_EKF_SOLUTION_MODE_START_VAL);
    app_canTx_VC_EllipseTimestamp_set(CANSIG_VC_ELLIPSE_TIMESTAMP_START_VAL);
    app_canTx_VC_EulerAnglesRoll_set(CANSIG_VC_EULER_ANGLES_ROLL_START_VAL);
    app_canTx_VC_EulerAnglesPitch_set(CANSIG_VC_EULER_ANGLES_PITCH_START_VAL);
    app_canTx_VC_EulerAnglesYaw_set(CANSIG_VC_EULER_ANGLES_YAW_START_VAL);
    app_canTx_VC_Hash_set(CANSIG_VC_HASH_START_VAL);
    app_canTx_VC_Clean_set(CANSIG_VC_CLEAN_START_VAL);
    app_canTx_VC_PowerEstimateValue_set(CANSIG_VC_POWER_ESTIMATE_VALUE_START_VAL);
    app_canTx_VC_PowerEstimateMeasuredValue_set(CANSIG_VC_POWER_ESTIMATE_MEASURED_VALUE_START_VAL);
    app_canTx_VC_PIDPowerEstimateOutput_set(CANSIG_VC_PID_POWER_ESTIMATE_OUTPUT_START_VAL);
    app_canTx_VC_PIDPowerEstimateError_set(CANSIG_VC_PID_POWER_ESTIMATE_ERROR_START_VAL);
    app_canTx_VC_PIDPowerEstimateDerivative_set(CANSIG_VC_PID_POWER_ESTIMATE_DERIVATIVE_START_VAL);
    app_canTx_VC_PIDPowerEstimateIntegral_set(CANSIG_VC_PID_POWER_ESTIMATE_INTEGRAL_START_VAL);
    app_canTx_VC_PIDSlipRatioOutput_set(CANSIG_VC_PID_SLIP_RATIO_OUTPUT_START_VAL);
    app_canTx_VC_PIDSlipRatioError_set(CANSIG_VC_PID_SLIP_RATIO_ERROR_START_VAL);
    app_canTx_VC_PIDSlipRatioDerivative_set(CANSIG_VC_PID_SLIP_RATIO_DERIVATIVE_START_VAL);
    app_canTx_VC_PIDSlipRatioIntegral_set(CANSIG_VC_PID_SLIP_RATIO_INTEGRAL_START_VAL);
    app_canTx_VC_SlipRatioLeft_set(CANSIG_VC_SLIP_RATIO_LEFT_START_VAL);
    app_canTx_VC_SlipRatioRight_set(CANSIG_VC_SLIP_RATIO_RIGHT_START_VAL);
    app_canTx_VC_ActiveDiffTorqueLeft_set(CANSIG_VC_ACTIVE_DIFF_TORQUE_LEFT_START_VAL);
    app_canTx_VC_ActiveDiffTorqueRight_set(CANSIG_VC_ACTIVE_DIFF_TORQUE_RIGHT_START_VAL);
    app_canTx_VC_PowerLimitValue_set(CANSIG_VC_POWER_LIMIT_VALUE_START_VAL);
    app_canTx_VC_PowerLimitValueAtFullThrottle_set(CANSIG_VC_POWER_LIMIT_VALUE_AT_FULL_THROTTLE_START_VAL);
    app_canTx_VC_RegenEnabled_set(CANSIG_VC_REGEN_ENABLED_START_VAL);
    app_canTx_VC_TorqueVectoringEnabled_set(CANSIG_VC_TORQUE_VECTORING_ENABLED_START_VAL);
    app_canTx_VC_VehicleVelocity_set(CANSIG_VC_VEHICLE_VELOCITY_START_VAL);
    app_canTx_VC_VelocityNorth_set(CANSIG_VC_VELOCITY_NORTH_START_VAL);
    app_canTx_VC_VelocityNorthAccuracy_set(CANSIG_VC_VELOCITY_NORTH_ACCURACY_START_VAL);
    app_canTx_VC_VelocityEast_set(CANSIG_VC_VELOCITY_EAST_START_VAL);
    app_canTx_VC_VelocityEastAccuracy_set(CANSIG_VC_VELOCITY_EAST_ACCURACY_START_VAL);
    app_canTx_VC_VelocityDown_set(CANSIG_VC_VELOCITY_DOWN_START_VAL);
    app_canTx_VC_VelocityDownAccuracy_set(CANSIG_VC_VELOCITY_DOWN_ACCURACY_START_VAL);
    app_canTx_VC_BatVoltage_set(CANSIG_VC_BAT_VOLTAGE_START_VAL);
    app_canTx_VC_BoostVoltage_set(CANSIG_VC_BOOST_VOLTAGE_START_VAL);
    app_canTx_VC_AccVoltage_set(CANSIG_VC_ACC_VOLTAGE_START_VAL);
    app_canTx_VC_AccCurrent_set(CANSIG_VC_ACC_CURRENT_START_VAL);
    app_canTx_VC_BatCurrent_set(CANSIG_VC_BAT_CURRENT_START_VAL);
    app_canTx_VC_ShdnCurrent_set(CANSIG_VC_SHDN_CURRENT_START_VAL);
    app_canTx_VC_ShdnStatus_set(CANSIG_VC_SHDN_STATUS_START_VAL);
    app_canTx_VC_LvCurrent_set(CANSIG_VC_LV_CURRENT_START_VAL);
    app_canTx_VC_LvStatus_set(CANSIG_VC_LV_STATUS_START_VAL);
    app_canTx_VC_PumpCurrent_set(CANSIG_VC_PUMP_CURRENT_START_VAL);
    app_canTx_VC_PumpStatus_set(CANSIG_VC_PUMP_STATUS_START_VAL);
    app_canTx_VC_AuxCurrent_set(CANSIG_VC_AUX_CURRENT_START_VAL);
    app_canTx_VC_AuxStatus_set(CANSIG_VC_AUX_STATUS_START_VAL);
    app_canTx_VC_InvRCurrent_set(CANSIG_VC_INV_R_CURRENT_START_VAL);
    app_canTx_VC_InvRStatus_set(CANSIG_VC_INV_R_STATUS_START_VAL);
    app_canTx_VC_InvLCurrent_set(CANSIG_VC_INV_L_CURRENT_START_VAL);
    app_canTx_VC_InvLStatus_set(CANSIG_VC_INV_L_STATUS_START_VAL);
    app_canTx_VC_ImuAccelerationX_set(CANSIG_VC_IMU_ACCELERATION_X_START_VAL);
    app_canTx_VC_ImuAccelerationY_set(CANSIG_VC_IMU_ACCELERATION_Y_START_VAL);
    app_canTx_VC_ImuAccelerationZ_set(CANSIG_VC_IMU_ACCELERATION_Z_START_VAL);
    app_canTx_VC_FlowRate_set(CANSIG_VC_FLOW_RATE_START_VAL);
    app_canTx_VC_LEStopOKStatus_set(CANSIG_VC_LE_STOP_OK_STATUS_START_VAL);
    app_canTx_VC_REStopOKStatus_set(CANSIG_VC_RE_STOP_OK_STATUS_START_VAL);
    app_canTx_VC_TSMSOKStatus_set(CANSIG_VC_TSMSOK_STATUS_START_VAL);
    app_canTx_VC_SplitterBoxInterlockOKStatus_set(CANSIG_VC_SPLITTER_BOX_INTERLOCK_OK_STATUS_START_VAL);
    app_canTx_VC_FirstFaultNode_set(CANSIG_VC_FIRST_FAULT_NODE_START_VAL);
    app_canTx_VC_MappedPedalPercentage_set(CANSIG_VC_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    app_canTx_VC_Warning_StackWaterMarkHighTask1Hz_set(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    app_canTx_VC_Warning_StackWaterMarkHighTask100Hz_set(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    app_canTx_VC_Warning_StackWaterMarkHighTask1kHz_set(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    app_canTx_VC_Warning_StackWaterMarkHighTaskCanRx_set(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    app_canTx_VC_Warning_StackWaterMarkHighTaskCanTx_set(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    app_canTx_VC_Warning_WatchdogTimeout_set(CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    app_canTx_VC_Warning_TxOverflow_set(CANSIG_VC_WARNING_TX_OVERFLOW_START_VAL);
    app_canTx_VC_Warning_RxOverflow_set(CANSIG_VC_WARNING_RX_OVERFLOW_START_VAL);
    app_canTx_VC_Warning_RegenNotAvailable_set(CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_START_VAL);
    app_canTx_VC_Warning_ImuInitFailed_set(CANSIG_VC_WARNING_IMU_INIT_FAILED_START_VAL);
    app_canTx_VC_Warning_FlowRateInputOutOfRange_set(CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_START_VAL);
    app_canTx_VC_Warning_BatteryRailOvercurrentFault_set(CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_START_VAL);
    app_canTx_VC_Warning_AccumulatorRailOvercurrentFault_set(CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_START_VAL);
    app_canTx_VC_Warning_LvChargeFault_set(CANSIG_VC_WARNING_LV_CHARGE_FAULT_START_VAL);
    app_canTx_VC_Warning_BoostControllerFault_set(CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_START_VAL);
    app_canTx_VC_Warning_HighNumberOfCanDataLogs_set(CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_START_VAL);
    app_canTx_VC_Warning_CanLoggingSdCardNotPresent_set(CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_START_VAL);
    app_canTx_VC_Warning_CanLoggingErrored_set(CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_START_VAL);
    app_canTx_VC_Warning_BrakeAppsDisagreement_set(CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_START_VAL);
    app_canTx_VC_Warning_SbgInitFailed_set(CANSIG_VC_WARNING_SBG_INIT_FAILED_START_VAL);
    app_canTx_VC_Fault_MissingBMSHeartbeat_set(CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_START_VAL);
    app_canTx_VC_Fault_MissingFSMHeartbeat_set(CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_START_VAL);
    app_canTx_VC_Fault_MissingRSMHeartbeat_set(CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_START_VAL);
    app_canTx_VC_Fault_MissingCRITHeartbeat_set(CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_START_VAL);
    app_canTx_VC_Fault_LeftInverterFault_set(CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_START_VAL);
    app_canTx_VC_Fault_RightInverterFault_set(CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_START_VAL);
    app_canTx_VC_Warning_StackWaterMarkHighTask1HzCount_set(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_START_VAL);
    app_canTx_VC_Warning_StackWaterMarkHighTask100HzCount_set(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_START_VAL);
    app_canTx_VC_Warning_StackWaterMarkHighTask1kHzCount_set(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_START_VAL);
    app_canTx_VC_Warning_StackWaterMarkHighTaskCanRxCount_set(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_START_VAL);
    app_canTx_VC_Warning_StackWaterMarkHighTaskCanTxCount_set(CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_START_VAL);
    app_canTx_VC_Warning_WatchdogTimeoutCount_set(CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_COUNT_START_VAL);
    app_canTx_VC_Warning_TxOverflowCount_set(CANSIG_VC_WARNING_TX_OVERFLOW_COUNT_START_VAL);
    app_canTx_VC_Warning_RxOverflowCount_set(CANSIG_VC_WARNING_RX_OVERFLOW_COUNT_START_VAL);
    app_canTx_VC_Warning_RegenNotAvailableCount_set(CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_COUNT_START_VAL);
    app_canTx_VC_Warning_ImuInitFailedCount_set(CANSIG_VC_WARNING_IMU_INIT_FAILED_COUNT_START_VAL);
    app_canTx_VC_Warning_FlowRateInputOutOfRangeCount_set(CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_COUNT_START_VAL);
    app_canTx_VC_Warning_BatteryRailOvercurrentFaultCount_set(CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_COUNT_START_VAL);
    app_canTx_VC_Warning_AccumulatorRailOvercurrentFaultCount_set(CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_COUNT_START_VAL);
    app_canTx_VC_Warning_LvChargeFaultCount_set(CANSIG_VC_WARNING_LV_CHARGE_FAULT_COUNT_START_VAL);
    app_canTx_VC_Warning_BoostControllerFaultCount_set(CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_COUNT_START_VAL);
    app_canTx_VC_Warning_HighNumberOfCanDataLogsCount_set(CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_COUNT_START_VAL);
    app_canTx_VC_Warning_CanLoggingSdCardNotPresentCount_set(CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_COUNT_START_VAL);
    app_canTx_VC_Warning_CanLoggingErroredCount_set(CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_COUNT_START_VAL);
    app_canTx_VC_Warning_BrakeAppsDisagreementCount_set(CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_START_VAL);
    app_canTx_VC_Warning_SbgInitFailedCount_set(CANSIG_VC_WARNING_SBG_INIT_FAILED_COUNT_START_VAL);
    app_canTx_VC_Fault_MissingBMSHeartbeatCount_set(CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_COUNT_START_VAL);
    app_canTx_VC_Fault_MissingFSMHeartbeatCount_set(CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_COUNT_START_VAL);
    app_canTx_VC_Fault_MissingRSMHeartbeatCount_set(CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_COUNT_START_VAL);
    app_canTx_VC_Fault_MissingCRITHeartbeatCount_set(CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_COUNT_START_VAL);
    app_canTx_VC_Fault_LeftInverterFaultCount_set(CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_COUNT_START_VAL);
    app_canTx_VC_Fault_RightInverterFaultCount_set(CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_COUNT_START_VAL);
}
void app_canTx_VC_Heartbeat_set(bool value)
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
void app_canTx_VC_State_set(VcState value)
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
void app_canTx_VC_BuzzerOn_set(bool value)
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
void app_canTx_VC_NumberOfCanDataLogs_set(uint32_t value)
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
void app_canTx_VC_CanLoggingRemainingErrors_set(uint32_t value)
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
void app_canTx_VC_LeftInverterTorqueCommand_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterTorqueCommand_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_LEFT_INVERTER_TORQUE_COMMAND_MAX_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_TORQUE_COMMAND_MAX_VAL;
    }
    if (value < CANSIG_VC_LEFT_INVERTER_TORQUE_COMMAND_MIN_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_TORQUE_COMMAND_MIN_VAL;
    }
}
void app_canTx_VC_LeftInverterSpeedCommand_set(int value)
{
    tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterSpeedCommand_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_LEFT_INVERTER_SPEED_COMMAND_MAX_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_SPEED_COMMAND_MAX_VAL;
    }
    if (value < CANSIG_VC_LEFT_INVERTER_SPEED_COMMAND_MIN_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_SPEED_COMMAND_MIN_VAL;
    }
}
void app_canTx_VC_LeftInverterDirectionCommand_set(InverterDirection value)
{
    tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterDirectionCommand_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_LEFT_INVERTER_DIRECTION_COMMAND_MAX_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_DIRECTION_COMMAND_MAX_VAL;
    }
    if (value < CANSIG_VC_LEFT_INVERTER_DIRECTION_COMMAND_MIN_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_DIRECTION_COMMAND_MIN_VAL;
    }
}
void app_canTx_VC_LeftInverterEnable_set(bool value)
{
    tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterEnable_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_LEFT_INVERTER_ENABLE_MAX_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_ENABLE_MAX_VAL;
    }
    if (value < CANSIG_VC_LEFT_INVERTER_ENABLE_MIN_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_ENABLE_MIN_VAL;
    }
}
void app_canTx_VC_LeftInverterEnableDischarge_set(bool value)
{
    tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterEnableDischarge_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_LEFT_INVERTER_ENABLE_DISCHARGE_MAX_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_ENABLE_DISCHARGE_MAX_VAL;
    }
    if (value < CANSIG_VC_LEFT_INVERTER_ENABLE_DISCHARGE_MIN_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_ENABLE_DISCHARGE_MIN_VAL;
    }
}
void app_canTx_VC_LeftInverterEnableSpeedMode_set(bool value)
{
    tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterEnableSpeedMode_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_LEFT_INVERTER_ENABLE_SPEED_MODE_MAX_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_ENABLE_SPEED_MODE_MAX_VAL;
    }
    if (value < CANSIG_VC_LEFT_INVERTER_ENABLE_SPEED_MODE_MIN_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_ENABLE_SPEED_MODE_MIN_VAL;
    }
}
void app_canTx_VC_LeftInverterTorqueLimit_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterTorqueLimit_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_LEFT_INVERTER_TORQUE_LIMIT_MAX_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_TORQUE_LIMIT_MAX_VAL;
    }
    if (value < CANSIG_VC_LEFT_INVERTER_TORQUE_LIMIT_MIN_VAL)
    {
        value = CANSIG_VC_LEFT_INVERTER_TORQUE_LIMIT_MIN_VAL;
    }
}
void app_canTx_VC_INVL_CommandParameterAddress_set(uint32_t value)
{
    tx_table.VC_INVL_ReadWriteParamCommand_signals.VC_INVL_CommandParameterAddress_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_INVL_COMMAND_PARAMETER_ADDRESS_MAX_VAL)
    {
        value = CANSIG_VC_INVL_COMMAND_PARAMETER_ADDRESS_MAX_VAL;
    }
    if (value < CANSIG_VC_INVL_COMMAND_PARAMETER_ADDRESS_MIN_VAL)
    {
        value = CANSIG_VC_INVL_COMMAND_PARAMETER_ADDRESS_MIN_VAL;
    }
}
void app_canTx_VC_INVL_CommandReadWrite_set(InverterReadWriteCommand value)
{
    tx_table.VC_INVL_ReadWriteParamCommand_signals.VC_INVL_CommandReadWrite_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_INVL_COMMAND_READ_WRITE_MAX_VAL)
    {
        value = CANSIG_VC_INVL_COMMAND_READ_WRITE_MAX_VAL;
    }
    if (value < CANSIG_VC_INVL_COMMAND_READ_WRITE_MIN_VAL)
    {
        value = CANSIG_VC_INVL_COMMAND_READ_WRITE_MIN_VAL;
    }
}
void app_canTx_VC_INVL_CommandData_set(uint32_t value)
{
    tx_table.VC_INVL_ReadWriteParamCommand_signals.VC_INVL_CommandData_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_INVL_COMMAND_DATA_MAX_VAL)
    {
        value = CANSIG_VC_INVL_COMMAND_DATA_MAX_VAL;
    }
    if (value < CANSIG_VC_INVL_COMMAND_DATA_MIN_VAL)
    {
        value = CANSIG_VC_INVL_COMMAND_DATA_MIN_VAL;
    }
}
void app_canTx_VC_RightInverterTorqueCommand_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_RightInverterCommand_signals.VC_RightInverterTorqueCommand_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_RIGHT_INVERTER_TORQUE_COMMAND_MAX_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_TORQUE_COMMAND_MAX_VAL;
    }
    if (value < CANSIG_VC_RIGHT_INVERTER_TORQUE_COMMAND_MIN_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_TORQUE_COMMAND_MIN_VAL;
    }
}
void app_canTx_VC_RightInverterSpeedCommand_set(int value)
{
    tx_table.VC_RightInverterCommand_signals.VC_RightInverterSpeedCommand_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_RIGHT_INVERTER_SPEED_COMMAND_MAX_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_SPEED_COMMAND_MAX_VAL;
    }
    if (value < CANSIG_VC_RIGHT_INVERTER_SPEED_COMMAND_MIN_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_SPEED_COMMAND_MIN_VAL;
    }
}
void app_canTx_VC_RightInverterDirectionCommand_set(InverterDirection value)
{
    tx_table.VC_RightInverterCommand_signals.VC_RightInverterDirectionCommand_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_RIGHT_INVERTER_DIRECTION_COMMAND_MAX_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_DIRECTION_COMMAND_MAX_VAL;
    }
    if (value < CANSIG_VC_RIGHT_INVERTER_DIRECTION_COMMAND_MIN_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_DIRECTION_COMMAND_MIN_VAL;
    }
}
void app_canTx_VC_RightInverterEnable_set(bool value)
{
    tx_table.VC_RightInverterCommand_signals.VC_RightInverterEnable_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_RIGHT_INVERTER_ENABLE_MAX_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_ENABLE_MAX_VAL;
    }
    if (value < CANSIG_VC_RIGHT_INVERTER_ENABLE_MIN_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_ENABLE_MIN_VAL;
    }
}
void app_canTx_VC_RightInverterEnableDischarge_set(bool value)
{
    tx_table.VC_RightInverterCommand_signals.VC_RightInverterEnableDischarge_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_RIGHT_INVERTER_ENABLE_DISCHARGE_MAX_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_ENABLE_DISCHARGE_MAX_VAL;
    }
    if (value < CANSIG_VC_RIGHT_INVERTER_ENABLE_DISCHARGE_MIN_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_ENABLE_DISCHARGE_MIN_VAL;
    }
}
void app_canTx_VC_RightInverterEnableSpeedMode_set(bool value)
{
    tx_table.VC_RightInverterCommand_signals.VC_RightInverterEnableSpeedMode_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_RIGHT_INVERTER_ENABLE_SPEED_MODE_MAX_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_ENABLE_SPEED_MODE_MAX_VAL;
    }
    if (value < CANSIG_VC_RIGHT_INVERTER_ENABLE_SPEED_MODE_MIN_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_ENABLE_SPEED_MODE_MIN_VAL;
    }
}
void app_canTx_VC_RightInverterTorqueLimit_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_RightInverterCommand_signals.VC_RightInverterTorqueLimit_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_RIGHT_INVERTER_TORQUE_LIMIT_MAX_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_TORQUE_LIMIT_MAX_VAL;
    }
    if (value < CANSIG_VC_RIGHT_INVERTER_TORQUE_LIMIT_MIN_VAL)
    {
        value = CANSIG_VC_RIGHT_INVERTER_TORQUE_LIMIT_MIN_VAL;
    }
}
void app_canTx_VC_INVR_CommandParameterAddress_set(uint32_t value)
{
    tx_table.VC_INVR_ReadWriteParamCommand_signals.VC_INVR_CommandParameterAddress_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_INVR_COMMAND_PARAMETER_ADDRESS_MAX_VAL)
    {
        value = CANSIG_VC_INVR_COMMAND_PARAMETER_ADDRESS_MAX_VAL;
    }
    if (value < CANSIG_VC_INVR_COMMAND_PARAMETER_ADDRESS_MIN_VAL)
    {
        value = CANSIG_VC_INVR_COMMAND_PARAMETER_ADDRESS_MIN_VAL;
    }
}
void app_canTx_VC_INVR_CommandReadWrite_set(InverterReadWriteCommand value)
{
    tx_table.VC_INVR_ReadWriteParamCommand_signals.VC_INVR_CommandReadWrite_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_INVR_COMMAND_READ_WRITE_MAX_VAL)
    {
        value = CANSIG_VC_INVR_COMMAND_READ_WRITE_MAX_VAL;
    }
    if (value < CANSIG_VC_INVR_COMMAND_READ_WRITE_MIN_VAL)
    {
        value = CANSIG_VC_INVR_COMMAND_READ_WRITE_MIN_VAL;
    }
}
void app_canTx_VC_INVR_CommandData_set(uint32_t value)
{
    tx_table.VC_INVR_ReadWriteParamCommand_signals.VC_INVR_CommandData_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_INVR_COMMAND_DATA_MAX_VAL)
    {
        value = CANSIG_VC_INVR_COMMAND_DATA_MAX_VAL;
    }
    if (value < CANSIG_VC_INVR_COMMAND_DATA_MIN_VAL)
    {
        value = CANSIG_VC_INVR_COMMAND_DATA_MIN_VAL;
    }
}
void app_canTx_VC_WatchdogTimeout_set(bool value)
{
    tx_table.VC_AlertsContext_signals.VC_WatchdogTimeout_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WATCHDOG_TIMEOUT_MAX_VAL)
    {
        value = CANSIG_VC_WATCHDOG_TIMEOUT_MAX_VAL;
    }
    if (value < CANSIG_VC_WATCHDOG_TIMEOUT_MIN_VAL)
    {
        value = CANSIG_VC_WATCHDOG_TIMEOUT_MIN_VAL;
    }
}
void app_canTx_VC_WatchdogTimeoutTaskName_set(RtosTaskName value)
{
    tx_table.VC_AlertsContext_signals.VC_WatchdogTimeoutTaskName_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WATCHDOG_TIMEOUT_TASK_NAME_MAX_VAL)
    {
        value = CANSIG_VC_WATCHDOG_TIMEOUT_TASK_NAME_MAX_VAL;
    }
    if (value < CANSIG_VC_WATCHDOG_TIMEOUT_TASK_NAME_MIN_VAL)
    {
        value = CANSIG_VC_WATCHDOG_TIMEOUT_TASK_NAME_MIN_VAL;
    }
}
void app_canTx_VC_TxOverflowCount_set(uint32_t value)
{
    tx_table.VC_AlertsContext_signals.VC_TxOverflowCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_TX_OVERFLOW_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_TX_OVERFLOW_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_TX_OVERFLOW_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_TX_OVERFLOW_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_RxOverflowCount_set(uint32_t value)
{
    tx_table.VC_AlertsContext_signals.VC_RxOverflowCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_RX_OVERFLOW_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_RX_OVERFLOW_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_RX_OVERFLOW_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_RX_OVERFLOW_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_EllipseGeneralStatusBitmask_set(uint32_t value)
{
    tx_table.VC_EllipseStatus_signals.VC_EllipseGeneralStatusBitmask_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_ELLIPSE_GENERAL_STATUS_BITMASK_MAX_VAL)
    {
        value = CANSIG_VC_ELLIPSE_GENERAL_STATUS_BITMASK_MAX_VAL;
    }
    if (value < CANSIG_VC_ELLIPSE_GENERAL_STATUS_BITMASK_MIN_VAL)
    {
        value = CANSIG_VC_ELLIPSE_GENERAL_STATUS_BITMASK_MIN_VAL;
    }
}
void app_canTx_VC_EllipseComStatusBitmask_set(uint32_t value)
{
    tx_table.VC_EllipseStatus_signals.VC_EllipseComStatusBitmask_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_ELLIPSE_COM_STATUS_BITMASK_MAX_VAL)
    {
        value = CANSIG_VC_ELLIPSE_COM_STATUS_BITMASK_MAX_VAL;
    }
    if (value < CANSIG_VC_ELLIPSE_COM_STATUS_BITMASK_MIN_VAL)
    {
        value = CANSIG_VC_ELLIPSE_COM_STATUS_BITMASK_MIN_VAL;
    }
}
void app_canTx_VC_EkfSolutionMode_set(VcEkfStatus value)
{
    tx_table.VC_EllipseStatus_signals.VC_EkfSolutionMode_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_EKF_SOLUTION_MODE_MAX_VAL)
    {
        value = CANSIG_VC_EKF_SOLUTION_MODE_MAX_VAL;
    }
    if (value < CANSIG_VC_EKF_SOLUTION_MODE_MIN_VAL)
    {
        value = CANSIG_VC_EKF_SOLUTION_MODE_MIN_VAL;
    }
}
void app_canTx_VC_EllipseTimestamp_set(uint32_t value)
{
    tx_table.VC_EllipseTime_signals.VC_EllipseTimestamp_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_ELLIPSE_TIMESTAMP_MAX_VAL)
    {
        value = CANSIG_VC_ELLIPSE_TIMESTAMP_MAX_VAL;
    }
    if (value < CANSIG_VC_ELLIPSE_TIMESTAMP_MIN_VAL)
    {
        value = CANSIG_VC_ELLIPSE_TIMESTAMP_MIN_VAL;
    }
}
void app_canTx_VC_EulerAnglesRoll_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_EllipseEulerAngles_signals.VC_EulerAnglesRoll_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_EULER_ANGLES_ROLL_MAX_VAL)
    {
        value = CANSIG_VC_EULER_ANGLES_ROLL_MAX_VAL;
    }
    if (value < CANSIG_VC_EULER_ANGLES_ROLL_MIN_VAL)
    {
        value = CANSIG_VC_EULER_ANGLES_ROLL_MIN_VAL;
    }
}
void app_canTx_VC_EulerAnglesPitch_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_EllipseEulerAngles_signals.VC_EulerAnglesPitch_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_EULER_ANGLES_PITCH_MAX_VAL)
    {
        value = CANSIG_VC_EULER_ANGLES_PITCH_MAX_VAL;
    }
    if (value < CANSIG_VC_EULER_ANGLES_PITCH_MIN_VAL)
    {
        value = CANSIG_VC_EULER_ANGLES_PITCH_MIN_VAL;
    }
}
void app_canTx_VC_EulerAnglesYaw_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_EllipseEulerAngles_signals.VC_EulerAnglesYaw_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_EULER_ANGLES_YAW_MAX_VAL)
    {
        value = CANSIG_VC_EULER_ANGLES_YAW_MAX_VAL;
    }
    if (value < CANSIG_VC_EULER_ANGLES_YAW_MIN_VAL)
    {
        value = CANSIG_VC_EULER_ANGLES_YAW_MIN_VAL;
    }
}
void app_canTx_VC_Hash_set(uint32_t value)
{
    tx_table.VC_CommitInfo_signals.VC_Hash_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_HASH_MAX_VAL)
    {
        value = CANSIG_VC_HASH_MAX_VAL;
    }
    if (value < CANSIG_VC_HASH_MIN_VAL)
    {
        value = CANSIG_VC_HASH_MIN_VAL;
    }
}
void app_canTx_VC_Clean_set(bool value)
{
    tx_table.VC_CommitInfo_signals.VC_Clean_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_CLEAN_MAX_VAL)
    {
        value = CANSIG_VC_CLEAN_MAX_VAL;
    }
    if (value < CANSIG_VC_CLEAN_MIN_VAL)
    {
        value = CANSIG_VC_CLEAN_MIN_VAL;
    }
}
void app_canTx_VC_PowerEstimateValue_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PowerEstimate_signals.VC_PowerEstimateValue_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_POWER_ESTIMATE_VALUE_MAX_VAL)
    {
        value = CANSIG_VC_POWER_ESTIMATE_VALUE_MAX_VAL;
    }
    if (value < CANSIG_VC_POWER_ESTIMATE_VALUE_MIN_VAL)
    {
        value = CANSIG_VC_POWER_ESTIMATE_VALUE_MIN_VAL;
    }
}
void app_canTx_VC_PowerEstimateMeasuredValue_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PowerEstimate_signals.VC_PowerEstimateMeasuredValue_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_POWER_ESTIMATE_MEASURED_VALUE_MAX_VAL)
    {
        value = CANSIG_VC_POWER_ESTIMATE_MEASURED_VALUE_MAX_VAL;
    }
    if (value < CANSIG_VC_POWER_ESTIMATE_MEASURED_VALUE_MIN_VAL)
    {
        value = CANSIG_VC_POWER_ESTIMATE_MEASURED_VALUE_MIN_VAL;
    }
}
void app_canTx_VC_PIDPowerEstimateOutput_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PowerEstimatePID_signals.VC_PIDPowerEstimateOutput_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_PID_POWER_ESTIMATE_OUTPUT_MAX_VAL)
    {
        value = CANSIG_VC_PID_POWER_ESTIMATE_OUTPUT_MAX_VAL;
    }
    if (value < CANSIG_VC_PID_POWER_ESTIMATE_OUTPUT_MIN_VAL)
    {
        value = CANSIG_VC_PID_POWER_ESTIMATE_OUTPUT_MIN_VAL;
    }
}
void app_canTx_VC_PIDPowerEstimateError_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PowerEstimatePID_signals.VC_PIDPowerEstimateError_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_PID_POWER_ESTIMATE_ERROR_MAX_VAL)
    {
        value = CANSIG_VC_PID_POWER_ESTIMATE_ERROR_MAX_VAL;
    }
    if (value < CANSIG_VC_PID_POWER_ESTIMATE_ERROR_MIN_VAL)
    {
        value = CANSIG_VC_PID_POWER_ESTIMATE_ERROR_MIN_VAL;
    }
}
void app_canTx_VC_PIDPowerEstimateDerivative_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PowerEstimatePID_signals.VC_PIDPowerEstimateDerivative_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_PID_POWER_ESTIMATE_DERIVATIVE_MAX_VAL)
    {
        value = CANSIG_VC_PID_POWER_ESTIMATE_DERIVATIVE_MAX_VAL;
    }
    if (value < CANSIG_VC_PID_POWER_ESTIMATE_DERIVATIVE_MIN_VAL)
    {
        value = CANSIG_VC_PID_POWER_ESTIMATE_DERIVATIVE_MIN_VAL;
    }
}
void app_canTx_VC_PIDPowerEstimateIntegral_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PowerEstimatePID_signals.VC_PIDPowerEstimateIntegral_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_PID_POWER_ESTIMATE_INTEGRAL_MAX_VAL)
    {
        value = CANSIG_VC_PID_POWER_ESTIMATE_INTEGRAL_MAX_VAL;
    }
    if (value < CANSIG_VC_PID_POWER_ESTIMATE_INTEGRAL_MIN_VAL)
    {
        value = CANSIG_VC_PID_POWER_ESTIMATE_INTEGRAL_MIN_VAL;
    }
}
void app_canTx_VC_PIDSlipRatioOutput_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PIDSlipRatio_signals.VC_PIDSlipRatioOutput_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_PID_SLIP_RATIO_OUTPUT_MAX_VAL)
    {
        value = CANSIG_VC_PID_SLIP_RATIO_OUTPUT_MAX_VAL;
    }
    if (value < CANSIG_VC_PID_SLIP_RATIO_OUTPUT_MIN_VAL)
    {
        value = CANSIG_VC_PID_SLIP_RATIO_OUTPUT_MIN_VAL;
    }
}
void app_canTx_VC_PIDSlipRatioError_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PIDSlipRatio_signals.VC_PIDSlipRatioError_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_PID_SLIP_RATIO_ERROR_MAX_VAL)
    {
        value = CANSIG_VC_PID_SLIP_RATIO_ERROR_MAX_VAL;
    }
    if (value < CANSIG_VC_PID_SLIP_RATIO_ERROR_MIN_VAL)
    {
        value = CANSIG_VC_PID_SLIP_RATIO_ERROR_MIN_VAL;
    }
}
void app_canTx_VC_PIDSlipRatioDerivative_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PIDSlipRatio_signals.VC_PIDSlipRatioDerivative_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_PID_SLIP_RATIO_DERIVATIVE_MAX_VAL)
    {
        value = CANSIG_VC_PID_SLIP_RATIO_DERIVATIVE_MAX_VAL;
    }
    if (value < CANSIG_VC_PID_SLIP_RATIO_DERIVATIVE_MIN_VAL)
    {
        value = CANSIG_VC_PID_SLIP_RATIO_DERIVATIVE_MIN_VAL;
    }
}
void app_canTx_VC_PIDSlipRatioIntegral_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PIDSlipRatio_signals.VC_PIDSlipRatioIntegral_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_PID_SLIP_RATIO_INTEGRAL_MAX_VAL)
    {
        value = CANSIG_VC_PID_SLIP_RATIO_INTEGRAL_MAX_VAL;
    }
    if (value < CANSIG_VC_PID_SLIP_RATIO_INTEGRAL_MIN_VAL)
    {
        value = CANSIG_VC_PID_SLIP_RATIO_INTEGRAL_MIN_VAL;
    }
}
void app_canTx_VC_SlipRatioLeft_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_SlipRatios_signals.VC_SlipRatioLeft_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_SLIP_RATIO_LEFT_MAX_VAL)
    {
        value = CANSIG_VC_SLIP_RATIO_LEFT_MAX_VAL;
    }
    if (value < CANSIG_VC_SLIP_RATIO_LEFT_MIN_VAL)
    {
        value = CANSIG_VC_SLIP_RATIO_LEFT_MIN_VAL;
    }
}
void app_canTx_VC_SlipRatioRight_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_SlipRatios_signals.VC_SlipRatioRight_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_SLIP_RATIO_RIGHT_MAX_VAL)
    {
        value = CANSIG_VC_SLIP_RATIO_RIGHT_MAX_VAL;
    }
    if (value < CANSIG_VC_SLIP_RATIO_RIGHT_MIN_VAL)
    {
        value = CANSIG_VC_SLIP_RATIO_RIGHT_MIN_VAL;
    }
}
void app_canTx_VC_ActiveDiffTorqueLeft_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_ActiveDiffOutputs_signals.VC_ActiveDiffTorqueLeft_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_ACTIVE_DIFF_TORQUE_LEFT_MAX_VAL)
    {
        value = CANSIG_VC_ACTIVE_DIFF_TORQUE_LEFT_MAX_VAL;
    }
    if (value < CANSIG_VC_ACTIVE_DIFF_TORQUE_LEFT_MIN_VAL)
    {
        value = CANSIG_VC_ACTIVE_DIFF_TORQUE_LEFT_MIN_VAL;
    }
}
void app_canTx_VC_ActiveDiffTorqueRight_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_ActiveDiffOutputs_signals.VC_ActiveDiffTorqueRight_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_ACTIVE_DIFF_TORQUE_RIGHT_MAX_VAL)
    {
        value = CANSIG_VC_ACTIVE_DIFF_TORQUE_RIGHT_MAX_VAL;
    }
    if (value < CANSIG_VC_ACTIVE_DIFF_TORQUE_RIGHT_MIN_VAL)
    {
        value = CANSIG_VC_ACTIVE_DIFF_TORQUE_RIGHT_MIN_VAL;
    }
}
void app_canTx_VC_PowerLimitValue_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PowerLimit_signals.VC_PowerLimitValue_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_POWER_LIMIT_VALUE_MAX_VAL)
    {
        value = CANSIG_VC_POWER_LIMIT_VALUE_MAX_VAL;
    }
    if (value < CANSIG_VC_POWER_LIMIT_VALUE_MIN_VAL)
    {
        value = CANSIG_VC_POWER_LIMIT_VALUE_MIN_VAL;
    }
}
void app_canTx_VC_PowerLimitValueAtFullThrottle_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PowerLimit_signals.VC_PowerLimitValueAtFullThrottle_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_POWER_LIMIT_VALUE_AT_FULL_THROTTLE_MAX_VAL)
    {
        value = CANSIG_VC_POWER_LIMIT_VALUE_AT_FULL_THROTTLE_MAX_VAL;
    }
    if (value < CANSIG_VC_POWER_LIMIT_VALUE_AT_FULL_THROTTLE_MIN_VAL)
    {
        value = CANSIG_VC_POWER_LIMIT_VALUE_AT_FULL_THROTTLE_MIN_VAL;
    }
}
void app_canTx_VC_RegenEnabled_set(bool value)
{
    tx_table.VC_VehicleDynamics_signals.VC_RegenEnabled_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_REGEN_ENABLED_MAX_VAL)
    {
        value = CANSIG_VC_REGEN_ENABLED_MAX_VAL;
    }
    if (value < CANSIG_VC_REGEN_ENABLED_MIN_VAL)
    {
        value = CANSIG_VC_REGEN_ENABLED_MIN_VAL;
    }
}
void app_canTx_VC_TorqueVectoringEnabled_set(bool value)
{
    tx_table.VC_VehicleDynamics_signals.VC_TorqueVectoringEnabled_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_TORQUE_VECTORING_ENABLED_MAX_VAL)
    {
        value = CANSIG_VC_TORQUE_VECTORING_ENABLED_MAX_VAL;
    }
    if (value < CANSIG_VC_TORQUE_VECTORING_ENABLED_MIN_VAL)
    {
        value = CANSIG_VC_TORQUE_VECTORING_ENABLED_MIN_VAL;
    }
}
void app_canTx_VC_VehicleVelocity_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_VehicleDynamics_signals.VC_VehicleVelocity_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_VEHICLE_VELOCITY_MAX_VAL)
    {
        value = CANSIG_VC_VEHICLE_VELOCITY_MAX_VAL;
    }
    if (value < CANSIG_VC_VEHICLE_VELOCITY_MIN_VAL)
    {
        value = CANSIG_VC_VEHICLE_VELOCITY_MIN_VAL;
    }
}
void app_canTx_VC_VelocityNorth_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityNorth_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_VELOCITY_NORTH_MAX_VAL)
    {
        value = CANSIG_VC_VELOCITY_NORTH_MAX_VAL;
    }
    if (value < CANSIG_VC_VELOCITY_NORTH_MIN_VAL)
    {
        value = CANSIG_VC_VELOCITY_NORTH_MIN_VAL;
    }
}
void app_canTx_VC_VelocityNorthAccuracy_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityNorthAccuracy_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_VELOCITY_NORTH_ACCURACY_MAX_VAL)
    {
        value = CANSIG_VC_VELOCITY_NORTH_ACCURACY_MAX_VAL;
    }
    if (value < CANSIG_VC_VELOCITY_NORTH_ACCURACY_MIN_VAL)
    {
        value = CANSIG_VC_VELOCITY_NORTH_ACCURACY_MIN_VAL;
    }
}
void app_canTx_VC_VelocityEast_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityEast_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_VELOCITY_EAST_MAX_VAL)
    {
        value = CANSIG_VC_VELOCITY_EAST_MAX_VAL;
    }
    if (value < CANSIG_VC_VELOCITY_EAST_MIN_VAL)
    {
        value = CANSIG_VC_VELOCITY_EAST_MIN_VAL;
    }
}
void app_canTx_VC_VelocityEastAccuracy_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityEastAccuracy_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_VELOCITY_EAST_ACCURACY_MAX_VAL)
    {
        value = CANSIG_VC_VELOCITY_EAST_ACCURACY_MAX_VAL;
    }
    if (value < CANSIG_VC_VELOCITY_EAST_ACCURACY_MIN_VAL)
    {
        value = CANSIG_VC_VELOCITY_EAST_ACCURACY_MIN_VAL;
    }
}
void app_canTx_VC_VelocityDown_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityDown_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_VELOCITY_DOWN_MAX_VAL)
    {
        value = CANSIG_VC_VELOCITY_DOWN_MAX_VAL;
    }
    if (value < CANSIG_VC_VELOCITY_DOWN_MIN_VAL)
    {
        value = CANSIG_VC_VELOCITY_DOWN_MIN_VAL;
    }
}
void app_canTx_VC_VelocityDownAccuracy_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityDownAccuracy_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_VELOCITY_DOWN_ACCURACY_MAX_VAL)
    {
        value = CANSIG_VC_VELOCITY_DOWN_ACCURACY_MAX_VAL;
    }
    if (value < CANSIG_VC_VELOCITY_DOWN_ACCURACY_MIN_VAL)
    {
        value = CANSIG_VC_VELOCITY_DOWN_ACCURACY_MIN_VAL;
    }
}
void app_canTx_VC_BatVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_Voltages_signals.VC_BatVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_BAT_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_VC_BAT_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_VC_BAT_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_VC_BAT_VOLTAGE_MIN_VAL;
    }
}
void app_canTx_VC_BoostVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_Voltages_signals.VC_BoostVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_BOOST_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_VC_BOOST_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_VC_BOOST_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_VC_BOOST_VOLTAGE_MIN_VAL;
    }
}
void app_canTx_VC_AccVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_Voltages_signals.VC_AccVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_ACC_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_VC_ACC_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_VC_ACC_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_VC_ACC_VOLTAGE_MIN_VAL;
    }
}
void app_canTx_VC_AccCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_CurrentSensing_signals.VC_AccCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_ACC_CURRENT_MAX_VAL)
    {
        value = CANSIG_VC_ACC_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_VC_ACC_CURRENT_MIN_VAL)
    {
        value = CANSIG_VC_ACC_CURRENT_MIN_VAL;
    }
}
void app_canTx_VC_BatCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_CurrentSensing_signals.VC_BatCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_BAT_CURRENT_MAX_VAL)
    {
        value = CANSIG_VC_BAT_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_VC_BAT_CURRENT_MIN_VAL)
    {
        value = CANSIG_VC_BAT_CURRENT_MIN_VAL;
    }
}
void app_canTx_VC_ShdnCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_Efuse_signals.VC_ShdnCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_SHDN_CURRENT_MAX_VAL)
    {
        value = CANSIG_VC_SHDN_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_VC_SHDN_CURRENT_MIN_VAL)
    {
        value = CANSIG_VC_SHDN_CURRENT_MIN_VAL;
    }
}
void app_canTx_VC_ShdnStatus_set(bool value)
{
    tx_table.VC_Efuse_signals.VC_ShdnStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_SHDN_STATUS_MAX_VAL)
    {
        value = CANSIG_VC_SHDN_STATUS_MAX_VAL;
    }
    if (value < CANSIG_VC_SHDN_STATUS_MIN_VAL)
    {
        value = CANSIG_VC_SHDN_STATUS_MIN_VAL;
    }
}
void app_canTx_VC_LvCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_Efuse_signals.VC_LvCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_LV_CURRENT_MAX_VAL)
    {
        value = CANSIG_VC_LV_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_VC_LV_CURRENT_MIN_VAL)
    {
        value = CANSIG_VC_LV_CURRENT_MIN_VAL;
    }
}
void app_canTx_VC_LvStatus_set(bool value)
{
    tx_table.VC_Efuse_signals.VC_LvStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_LV_STATUS_MAX_VAL)
    {
        value = CANSIG_VC_LV_STATUS_MAX_VAL;
    }
    if (value < CANSIG_VC_LV_STATUS_MIN_VAL)
    {
        value = CANSIG_VC_LV_STATUS_MIN_VAL;
    }
}
void app_canTx_VC_PumpCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_Efuse_signals.VC_PumpCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_PUMP_CURRENT_MAX_VAL)
    {
        value = CANSIG_VC_PUMP_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_VC_PUMP_CURRENT_MIN_VAL)
    {
        value = CANSIG_VC_PUMP_CURRENT_MIN_VAL;
    }
}
void app_canTx_VC_PumpStatus_set(bool value)
{
    tx_table.VC_Efuse_signals.VC_PumpStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_PUMP_STATUS_MAX_VAL)
    {
        value = CANSIG_VC_PUMP_STATUS_MAX_VAL;
    }
    if (value < CANSIG_VC_PUMP_STATUS_MIN_VAL)
    {
        value = CANSIG_VC_PUMP_STATUS_MIN_VAL;
    }
}
void app_canTx_VC_AuxCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_Efuse_signals.VC_AuxCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_AUX_CURRENT_MAX_VAL)
    {
        value = CANSIG_VC_AUX_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_VC_AUX_CURRENT_MIN_VAL)
    {
        value = CANSIG_VC_AUX_CURRENT_MIN_VAL;
    }
}
void app_canTx_VC_AuxStatus_set(bool value)
{
    tx_table.VC_Efuse_signals.VC_AuxStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_AUX_STATUS_MAX_VAL)
    {
        value = CANSIG_VC_AUX_STATUS_MAX_VAL;
    }
    if (value < CANSIG_VC_AUX_STATUS_MIN_VAL)
    {
        value = CANSIG_VC_AUX_STATUS_MIN_VAL;
    }
}
void app_canTx_VC_InvRCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_Efuse_signals.VC_InvRCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_INV_R_CURRENT_MAX_VAL)
    {
        value = CANSIG_VC_INV_R_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_VC_INV_R_CURRENT_MIN_VAL)
    {
        value = CANSIG_VC_INV_R_CURRENT_MIN_VAL;
    }
}
void app_canTx_VC_InvRStatus_set(bool value)
{
    tx_table.VC_Efuse_signals.VC_InvRStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_INV_R_STATUS_MAX_VAL)
    {
        value = CANSIG_VC_INV_R_STATUS_MAX_VAL;
    }
    if (value < CANSIG_VC_INV_R_STATUS_MIN_VAL)
    {
        value = CANSIG_VC_INV_R_STATUS_MIN_VAL;
    }
}
void app_canTx_VC_InvLCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_Efuse_signals.VC_InvLCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_INV_L_CURRENT_MAX_VAL)
    {
        value = CANSIG_VC_INV_L_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_VC_INV_L_CURRENT_MIN_VAL)
    {
        value = CANSIG_VC_INV_L_CURRENT_MIN_VAL;
    }
}
void app_canTx_VC_InvLStatus_set(bool value)
{
    tx_table.VC_Efuse_signals.VC_InvLStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_INV_L_STATUS_MAX_VAL)
    {
        value = CANSIG_VC_INV_L_STATUS_MAX_VAL;
    }
    if (value < CANSIG_VC_INV_L_STATUS_MIN_VAL)
    {
        value = CANSIG_VC_INV_L_STATUS_MIN_VAL;
    }
}
void app_canTx_VC_ImuAccelerationX_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_ImuLinearAcceleration_signals.VC_ImuAccelerationX_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_IMU_ACCELERATION_X_MAX_VAL)
    {
        value = CANSIG_VC_IMU_ACCELERATION_X_MAX_VAL;
    }
    if (value < CANSIG_VC_IMU_ACCELERATION_X_MIN_VAL)
    {
        value = CANSIG_VC_IMU_ACCELERATION_X_MIN_VAL;
    }
}
void app_canTx_VC_ImuAccelerationY_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_ImuLinearAcceleration_signals.VC_ImuAccelerationY_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_IMU_ACCELERATION_Y_MAX_VAL)
    {
        value = CANSIG_VC_IMU_ACCELERATION_Y_MAX_VAL;
    }
    if (value < CANSIG_VC_IMU_ACCELERATION_Y_MIN_VAL)
    {
        value = CANSIG_VC_IMU_ACCELERATION_Y_MIN_VAL;
    }
}
void app_canTx_VC_ImuAccelerationZ_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_ImuLinearAcceleration_signals.VC_ImuAccelerationZ_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_IMU_ACCELERATION_Z_MAX_VAL)
    {
        value = CANSIG_VC_IMU_ACCELERATION_Z_MAX_VAL;
    }
    if (value < CANSIG_VC_IMU_ACCELERATION_Z_MIN_VAL)
    {
        value = CANSIG_VC_IMU_ACCELERATION_Z_MIN_VAL;
    }
}
void app_canTx_VC_FlowRate_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_CoolantPumpFlowRate_signals.VC_FlowRate_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FLOW_RATE_MAX_VAL)
    {
        value = CANSIG_VC_FLOW_RATE_MAX_VAL;
    }
    if (value < CANSIG_VC_FLOW_RATE_MIN_VAL)
    {
        value = CANSIG_VC_FLOW_RATE_MIN_VAL;
    }
}
void app_canTx_VC_LEStopOKStatus_set(bool value)
{
    tx_table.VC_VCShdnNodeStatus_signals.VC_LEStopOKStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_LE_STOP_OK_STATUS_MAX_VAL)
    {
        value = CANSIG_VC_LE_STOP_OK_STATUS_MAX_VAL;
    }
    if (value < CANSIG_VC_LE_STOP_OK_STATUS_MIN_VAL)
    {
        value = CANSIG_VC_LE_STOP_OK_STATUS_MIN_VAL;
    }
}
void app_canTx_VC_REStopOKStatus_set(bool value)
{
    tx_table.VC_VCShdnNodeStatus_signals.VC_REStopOKStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_RE_STOP_OK_STATUS_MAX_VAL)
    {
        value = CANSIG_VC_RE_STOP_OK_STATUS_MAX_VAL;
    }
    if (value < CANSIG_VC_RE_STOP_OK_STATUS_MIN_VAL)
    {
        value = CANSIG_VC_RE_STOP_OK_STATUS_MIN_VAL;
    }
}
void app_canTx_VC_TSMSOKStatus_set(bool value)
{
    tx_table.VC_VCShdnNodeStatus_signals.VC_TSMSOKStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_TSMSOK_STATUS_MAX_VAL)
    {
        value = CANSIG_VC_TSMSOK_STATUS_MAX_VAL;
    }
    if (value < CANSIG_VC_TSMSOK_STATUS_MIN_VAL)
    {
        value = CANSIG_VC_TSMSOK_STATUS_MIN_VAL;
    }
}
void app_canTx_VC_SplitterBoxInterlockOKStatus_set(bool value)
{
    tx_table.VC_VCShdnNodeStatus_signals.VC_SplitterBoxInterlockOKStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_SPLITTER_BOX_INTERLOCK_OK_STATUS_MAX_VAL)
    {
        value = CANSIG_VC_SPLITTER_BOX_INTERLOCK_OK_STATUS_MAX_VAL;
    }
    if (value < CANSIG_VC_SPLITTER_BOX_INTERLOCK_OK_STATUS_MIN_VAL)
    {
        value = CANSIG_VC_SPLITTER_BOX_INTERLOCK_OK_STATUS_MIN_VAL;
    }
}
void app_canTx_VC_FirstFaultNode_set(ShutdownNode value)
{
    tx_table.VC_GlobalShdnNodeStatus_signals.VC_FirstFaultNode_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FIRST_FAULT_NODE_MAX_VAL)
    {
        value = CANSIG_VC_FIRST_FAULT_NODE_MAX_VAL;
    }
    if (value < CANSIG_VC_FIRST_FAULT_NODE_MIN_VAL)
    {
        value = CANSIG_VC_FIRST_FAULT_NODE_MIN_VAL;
    }
}
void app_canTx_VC_MappedPedalPercentage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.VC_PedalPercentage_signals.VC_MappedPedalPercentage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_MAPPED_PEDAL_PERCENTAGE_MAX_VAL)
    {
        value = CANSIG_VC_MAPPED_PEDAL_PERCENTAGE_MAX_VAL;
    }
    if (value < CANSIG_VC_MAPPED_PEDAL_PERCENTAGE_MIN_VAL)
    {
        value = CANSIG_VC_MAPPED_PEDAL_PERCENTAGE_MIN_VAL;
    }
}
void app_canTx_VC_Warning_StackWaterMarkHighTask1Hz_set(bool value)
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
void app_canTx_VC_Warning_StackWaterMarkHighTask100Hz_set(bool value)
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
void app_canTx_VC_Warning_StackWaterMarkHighTask1kHz_set(bool value)
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
void app_canTx_VC_Warning_StackWaterMarkHighTaskCanRx_set(bool value)
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
void app_canTx_VC_Warning_StackWaterMarkHighTaskCanTx_set(bool value)
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
void app_canTx_VC_Warning_WatchdogTimeout_set(bool value)
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
void app_canTx_VC_Warning_TxOverflow_set(bool value)
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
void app_canTx_VC_Warning_RxOverflow_set(bool value)
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
void app_canTx_VC_Warning_RegenNotAvailable_set(bool value)
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
void app_canTx_VC_Warning_ImuInitFailed_set(bool value)
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
void app_canTx_VC_Warning_FlowRateInputOutOfRange_set(bool value)
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
void app_canTx_VC_Warning_BatteryRailOvercurrentFault_set(bool value)
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
void app_canTx_VC_Warning_AccumulatorRailOvercurrentFault_set(bool value)
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
void app_canTx_VC_Warning_LvChargeFault_set(bool value)
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
void app_canTx_VC_Warning_BoostControllerFault_set(bool value)
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
void app_canTx_VC_Warning_HighNumberOfCanDataLogs_set(bool value)
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
void app_canTx_VC_Warning_CanLoggingSdCardNotPresent_set(bool value)
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
void app_canTx_VC_Warning_CanLoggingErrored_set(bool value)
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
void app_canTx_VC_Warning_BrakeAppsDisagreement_set(bool value)
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
void app_canTx_VC_Warning_SbgInitFailed_set(bool value)
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
void app_canTx_VC_Fault_MissingBMSHeartbeat_set(bool value)
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
void app_canTx_VC_Fault_MissingFSMHeartbeat_set(bool value)
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
void app_canTx_VC_Fault_MissingRSMHeartbeat_set(bool value)
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
void app_canTx_VC_Fault_MissingCRITHeartbeat_set(bool value)
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
void app_canTx_VC_Fault_LeftInverterFault_set(bool value)
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
void app_canTx_VC_Fault_RightInverterFault_set(bool value)
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
void app_canTx_VC_Warning_StackWaterMarkHighTask1HzCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_StackWaterMarkHighTask1HzCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_StackWaterMarkHighTask100HzCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_StackWaterMarkHighTask100HzCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_StackWaterMarkHighTask1kHzCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_StackWaterMarkHighTask1kHzCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_StackWaterMarkHighTaskCanRxCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_StackWaterMarkHighTaskCanRxCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_StackWaterMarkHighTaskCanTxCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_StackWaterMarkHighTaskCanTxCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_WatchdogTimeoutCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_WatchdogTimeoutCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_TxOverflowCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_TxOverflowCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_TX_OVERFLOW_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_TX_OVERFLOW_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_TX_OVERFLOW_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_TX_OVERFLOW_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_RxOverflowCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_RxOverflowCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_RX_OVERFLOW_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_RX_OVERFLOW_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_RX_OVERFLOW_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_RX_OVERFLOW_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_RegenNotAvailableCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_RegenNotAvailableCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_ImuInitFailedCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_ImuInitFailedCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_IMU_INIT_FAILED_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_IMU_INIT_FAILED_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_IMU_INIT_FAILED_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_IMU_INIT_FAILED_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_FlowRateInputOutOfRangeCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_FlowRateInputOutOfRangeCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_BatteryRailOvercurrentFaultCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_BatteryRailOvercurrentFaultCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_AccumulatorRailOvercurrentFaultCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_AccumulatorRailOvercurrentFaultCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_LvChargeFaultCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_LvChargeFaultCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_LV_CHARGE_FAULT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_LV_CHARGE_FAULT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_LV_CHARGE_FAULT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_LV_CHARGE_FAULT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_BoostControllerFaultCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_BoostControllerFaultCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_HighNumberOfCanDataLogsCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_HighNumberOfCanDataLogsCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_CanLoggingSdCardNotPresentCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_CanLoggingSdCardNotPresentCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_CanLoggingErroredCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_CanLoggingErroredCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_BrakeAppsDisagreementCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_BrakeAppsDisagreementCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Warning_SbgInitFailedCount_set(uint32_t value)
{
    tx_table.VC_WarningsCounts_signals.VC_Warning_SbgInitFailedCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_WARNING_SBG_INIT_FAILED_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_WARNING_SBG_INIT_FAILED_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_WARNING_SBG_INIT_FAILED_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_WARNING_SBG_INIT_FAILED_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Fault_MissingBMSHeartbeatCount_set(uint32_t value)
{
    tx_table.VC_FaultsCounts_signals.VC_Fault_MissingBMSHeartbeatCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Fault_MissingFSMHeartbeatCount_set(uint32_t value)
{
    tx_table.VC_FaultsCounts_signals.VC_Fault_MissingFSMHeartbeatCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Fault_MissingRSMHeartbeatCount_set(uint32_t value)
{
    tx_table.VC_FaultsCounts_signals.VC_Fault_MissingRSMHeartbeatCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Fault_MissingCRITHeartbeatCount_set(uint32_t value)
{
    tx_table.VC_FaultsCounts_signals.VC_Fault_MissingCRITHeartbeatCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Fault_LeftInverterFaultCount_set(uint32_t value)
{
    tx_table.VC_FaultsCounts_signals.VC_Fault_LeftInverterFaultCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_COUNT_MIN_VAL;
    }
}
void app_canTx_VC_Fault_RightInverterFaultCount_set(uint32_t value)
{
    tx_table.VC_FaultsCounts_signals.VC_Fault_RightInverterFaultCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_COUNT_MAX_VAL)
    {
        value = CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_COUNT_MAX_VAL;
    }
    if (value < CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_COUNT_MIN_VAL)
    {
        value = CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_COUNT_MIN_VAL;
    }
}
bool app_canTx_VC_Heartbeat_get(void)
{
    return tx_table.VC_Vitals_signals.VC_Heartbeat_value;
}
VcState app_canTx_VC_State_get(void)
{
    return tx_table.VC_Vitals_signals.VC_State_value;
}
bool app_canTx_VC_BuzzerOn_get(void)
{
    return tx_table.VC_Vitals_signals.VC_BuzzerOn_value;
}
uint32_t app_canTx_VC_NumberOfCanDataLogs_get(void)
{
    return tx_table.VC_Vitals_signals.VC_NumberOfCanDataLogs_value;
}
uint32_t app_canTx_VC_CanLoggingRemainingErrors_get(void)
{
    return tx_table.VC_Vitals_signals.VC_CanLoggingRemainingErrors_value;
}
float app_canTx_VC_LeftInverterTorqueCommand_get(void)
{
    return tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterTorqueCommand_value;
}
int app_canTx_VC_LeftInverterSpeedCommand_get(void)
{
    return tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterSpeedCommand_value;
}
InverterDirection app_canTx_VC_LeftInverterDirectionCommand_get(void)
{
    return tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterDirectionCommand_value;
}
bool app_canTx_VC_LeftInverterEnable_get(void)
{
    return tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterEnable_value;
}
bool app_canTx_VC_LeftInverterEnableDischarge_get(void)
{
    return tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterEnableDischarge_value;
}
bool app_canTx_VC_LeftInverterEnableSpeedMode_get(void)
{
    return tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterEnableSpeedMode_value;
}
float app_canTx_VC_LeftInverterTorqueLimit_get(void)
{
    return tx_table.VC_LeftInverterCommand_signals.VC_LeftInverterTorqueLimit_value;
}
uint32_t app_canTx_VC_INVL_CommandParameterAddress_get(void)
{
    return tx_table.VC_INVL_ReadWriteParamCommand_signals.VC_INVL_CommandParameterAddress_value;
}
InverterReadWriteCommand app_canTx_VC_INVL_CommandReadWrite_get(void)
{
    return tx_table.VC_INVL_ReadWriteParamCommand_signals.VC_INVL_CommandReadWrite_value;
}
uint32_t app_canTx_VC_INVL_CommandData_get(void)
{
    return tx_table.VC_INVL_ReadWriteParamCommand_signals.VC_INVL_CommandData_value;
}
float app_canTx_VC_RightInverterTorqueCommand_get(void)
{
    return tx_table.VC_RightInverterCommand_signals.VC_RightInverterTorqueCommand_value;
}
int app_canTx_VC_RightInverterSpeedCommand_get(void)
{
    return tx_table.VC_RightInverterCommand_signals.VC_RightInverterSpeedCommand_value;
}
InverterDirection app_canTx_VC_RightInverterDirectionCommand_get(void)
{
    return tx_table.VC_RightInverterCommand_signals.VC_RightInverterDirectionCommand_value;
}
bool app_canTx_VC_RightInverterEnable_get(void)
{
    return tx_table.VC_RightInverterCommand_signals.VC_RightInverterEnable_value;
}
bool app_canTx_VC_RightInverterEnableDischarge_get(void)
{
    return tx_table.VC_RightInverterCommand_signals.VC_RightInverterEnableDischarge_value;
}
bool app_canTx_VC_RightInverterEnableSpeedMode_get(void)
{
    return tx_table.VC_RightInverterCommand_signals.VC_RightInverterEnableSpeedMode_value;
}
float app_canTx_VC_RightInverterTorqueLimit_get(void)
{
    return tx_table.VC_RightInverterCommand_signals.VC_RightInverterTorqueLimit_value;
}
uint32_t app_canTx_VC_INVR_CommandParameterAddress_get(void)
{
    return tx_table.VC_INVR_ReadWriteParamCommand_signals.VC_INVR_CommandParameterAddress_value;
}
InverterReadWriteCommand app_canTx_VC_INVR_CommandReadWrite_get(void)
{
    return tx_table.VC_INVR_ReadWriteParamCommand_signals.VC_INVR_CommandReadWrite_value;
}
uint32_t app_canTx_VC_INVR_CommandData_get(void)
{
    return tx_table.VC_INVR_ReadWriteParamCommand_signals.VC_INVR_CommandData_value;
}
bool app_canTx_VC_WatchdogTimeout_get(void)
{
    return tx_table.VC_AlertsContext_signals.VC_WatchdogTimeout_value;
}
RtosTaskName app_canTx_VC_WatchdogTimeoutTaskName_get(void)
{
    return tx_table.VC_AlertsContext_signals.VC_WatchdogTimeoutTaskName_value;
}
uint32_t app_canTx_VC_TxOverflowCount_get(void)
{
    return tx_table.VC_AlertsContext_signals.VC_TxOverflowCount_value;
}
uint32_t app_canTx_VC_RxOverflowCount_get(void)
{
    return tx_table.VC_AlertsContext_signals.VC_RxOverflowCount_value;
}
uint32_t app_canTx_VC_EllipseGeneralStatusBitmask_get(void)
{
    return tx_table.VC_EllipseStatus_signals.VC_EllipseGeneralStatusBitmask_value;
}
uint32_t app_canTx_VC_EllipseComStatusBitmask_get(void)
{
    return tx_table.VC_EllipseStatus_signals.VC_EllipseComStatusBitmask_value;
}
VcEkfStatus app_canTx_VC_EkfSolutionMode_get(void)
{
    return tx_table.VC_EllipseStatus_signals.VC_EkfSolutionMode_value;
}
uint32_t app_canTx_VC_EllipseTimestamp_get(void)
{
    return tx_table.VC_EllipseTime_signals.VC_EllipseTimestamp_value;
}
float app_canTx_VC_EulerAnglesRoll_get(void)
{
    return tx_table.VC_EllipseEulerAngles_signals.VC_EulerAnglesRoll_value;
}
float app_canTx_VC_EulerAnglesPitch_get(void)
{
    return tx_table.VC_EllipseEulerAngles_signals.VC_EulerAnglesPitch_value;
}
float app_canTx_VC_EulerAnglesYaw_get(void)
{
    return tx_table.VC_EllipseEulerAngles_signals.VC_EulerAnglesYaw_value;
}
uint32_t app_canTx_VC_Hash_get(void)
{
    return tx_table.VC_CommitInfo_signals.VC_Hash_value;
}
bool app_canTx_VC_Clean_get(void)
{
    return tx_table.VC_CommitInfo_signals.VC_Clean_value;
}
float app_canTx_VC_PowerEstimateValue_get(void)
{
    return tx_table.VC_PowerEstimate_signals.VC_PowerEstimateValue_value;
}
float app_canTx_VC_PowerEstimateMeasuredValue_get(void)
{
    return tx_table.VC_PowerEstimate_signals.VC_PowerEstimateMeasuredValue_value;
}
float app_canTx_VC_PIDPowerEstimateOutput_get(void)
{
    return tx_table.VC_PowerEstimatePID_signals.VC_PIDPowerEstimateOutput_value;
}
float app_canTx_VC_PIDPowerEstimateError_get(void)
{
    return tx_table.VC_PowerEstimatePID_signals.VC_PIDPowerEstimateError_value;
}
float app_canTx_VC_PIDPowerEstimateDerivative_get(void)
{
    return tx_table.VC_PowerEstimatePID_signals.VC_PIDPowerEstimateDerivative_value;
}
float app_canTx_VC_PIDPowerEstimateIntegral_get(void)
{
    return tx_table.VC_PowerEstimatePID_signals.VC_PIDPowerEstimateIntegral_value;
}
float app_canTx_VC_PIDSlipRatioOutput_get(void)
{
    return tx_table.VC_PIDSlipRatio_signals.VC_PIDSlipRatioOutput_value;
}
float app_canTx_VC_PIDSlipRatioError_get(void)
{
    return tx_table.VC_PIDSlipRatio_signals.VC_PIDSlipRatioError_value;
}
float app_canTx_VC_PIDSlipRatioDerivative_get(void)
{
    return tx_table.VC_PIDSlipRatio_signals.VC_PIDSlipRatioDerivative_value;
}
float app_canTx_VC_PIDSlipRatioIntegral_get(void)
{
    return tx_table.VC_PIDSlipRatio_signals.VC_PIDSlipRatioIntegral_value;
}
float app_canTx_VC_SlipRatioLeft_get(void)
{
    return tx_table.VC_SlipRatios_signals.VC_SlipRatioLeft_value;
}
float app_canTx_VC_SlipRatioRight_get(void)
{
    return tx_table.VC_SlipRatios_signals.VC_SlipRatioRight_value;
}
float app_canTx_VC_ActiveDiffTorqueLeft_get(void)
{
    return tx_table.VC_ActiveDiffOutputs_signals.VC_ActiveDiffTorqueLeft_value;
}
float app_canTx_VC_ActiveDiffTorqueRight_get(void)
{
    return tx_table.VC_ActiveDiffOutputs_signals.VC_ActiveDiffTorqueRight_value;
}
float app_canTx_VC_PowerLimitValue_get(void)
{
    return tx_table.VC_PowerLimit_signals.VC_PowerLimitValue_value;
}
float app_canTx_VC_PowerLimitValueAtFullThrottle_get(void)
{
    return tx_table.VC_PowerLimit_signals.VC_PowerLimitValueAtFullThrottle_value;
}
bool app_canTx_VC_RegenEnabled_get(void)
{
    return tx_table.VC_VehicleDynamics_signals.VC_RegenEnabled_value;
}
bool app_canTx_VC_TorqueVectoringEnabled_get(void)
{
    return tx_table.VC_VehicleDynamics_signals.VC_TorqueVectoringEnabled_value;
}
float app_canTx_VC_VehicleVelocity_get(void)
{
    return tx_table.VC_VehicleDynamics_signals.VC_VehicleVelocity_value;
}
float app_canTx_VC_VelocityNorth_get(void)
{
    return tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityNorth_value;
}
float app_canTx_VC_VelocityNorthAccuracy_get(void)
{
    return tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityNorthAccuracy_value;
}
float app_canTx_VC_VelocityEast_get(void)
{
    return tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityEast_value;
}
float app_canTx_VC_VelocityEastAccuracy_get(void)
{
    return tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityEastAccuracy_value;
}
float app_canTx_VC_VelocityDown_get(void)
{
    return tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityDown_value;
}
float app_canTx_VC_VelocityDownAccuracy_get(void)
{
    return tx_table.VC_EllipseEkfNavVelocity_signals.VC_VelocityDownAccuracy_value;
}
float app_canTx_VC_BatVoltage_get(void)
{
    return tx_table.VC_Voltages_signals.VC_BatVoltage_value;
}
float app_canTx_VC_BoostVoltage_get(void)
{
    return tx_table.VC_Voltages_signals.VC_BoostVoltage_value;
}
float app_canTx_VC_AccVoltage_get(void)
{
    return tx_table.VC_Voltages_signals.VC_AccVoltage_value;
}
float app_canTx_VC_AccCurrent_get(void)
{
    return tx_table.VC_CurrentSensing_signals.VC_AccCurrent_value;
}
float app_canTx_VC_BatCurrent_get(void)
{
    return tx_table.VC_CurrentSensing_signals.VC_BatCurrent_value;
}
float app_canTx_VC_ShdnCurrent_get(void)
{
    return tx_table.VC_Efuse_signals.VC_ShdnCurrent_value;
}
bool app_canTx_VC_ShdnStatus_get(void)
{
    return tx_table.VC_Efuse_signals.VC_ShdnStatus_value;
}
float app_canTx_VC_LvCurrent_get(void)
{
    return tx_table.VC_Efuse_signals.VC_LvCurrent_value;
}
bool app_canTx_VC_LvStatus_get(void)
{
    return tx_table.VC_Efuse_signals.VC_LvStatus_value;
}
float app_canTx_VC_PumpCurrent_get(void)
{
    return tx_table.VC_Efuse_signals.VC_PumpCurrent_value;
}
bool app_canTx_VC_PumpStatus_get(void)
{
    return tx_table.VC_Efuse_signals.VC_PumpStatus_value;
}
float app_canTx_VC_AuxCurrent_get(void)
{
    return tx_table.VC_Efuse_signals.VC_AuxCurrent_value;
}
bool app_canTx_VC_AuxStatus_get(void)
{
    return tx_table.VC_Efuse_signals.VC_AuxStatus_value;
}
float app_canTx_VC_InvRCurrent_get(void)
{
    return tx_table.VC_Efuse_signals.VC_InvRCurrent_value;
}
bool app_canTx_VC_InvRStatus_get(void)
{
    return tx_table.VC_Efuse_signals.VC_InvRStatus_value;
}
float app_canTx_VC_InvLCurrent_get(void)
{
    return tx_table.VC_Efuse_signals.VC_InvLCurrent_value;
}
bool app_canTx_VC_InvLStatus_get(void)
{
    return tx_table.VC_Efuse_signals.VC_InvLStatus_value;
}
float app_canTx_VC_ImuAccelerationX_get(void)
{
    return tx_table.VC_ImuLinearAcceleration_signals.VC_ImuAccelerationX_value;
}
float app_canTx_VC_ImuAccelerationY_get(void)
{
    return tx_table.VC_ImuLinearAcceleration_signals.VC_ImuAccelerationY_value;
}
float app_canTx_VC_ImuAccelerationZ_get(void)
{
    return tx_table.VC_ImuLinearAcceleration_signals.VC_ImuAccelerationZ_value;
}
float app_canTx_VC_FlowRate_get(void)
{
    return tx_table.VC_CoolantPumpFlowRate_signals.VC_FlowRate_value;
}
bool app_canTx_VC_LEStopOKStatus_get(void)
{
    return tx_table.VC_VCShdnNodeStatus_signals.VC_LEStopOKStatus_value;
}
bool app_canTx_VC_REStopOKStatus_get(void)
{
    return tx_table.VC_VCShdnNodeStatus_signals.VC_REStopOKStatus_value;
}
bool app_canTx_VC_TSMSOKStatus_get(void)
{
    return tx_table.VC_VCShdnNodeStatus_signals.VC_TSMSOKStatus_value;
}
bool app_canTx_VC_SplitterBoxInterlockOKStatus_get(void)
{
    return tx_table.VC_VCShdnNodeStatus_signals.VC_SplitterBoxInterlockOKStatus_value;
}
ShutdownNode app_canTx_VC_FirstFaultNode_get(void)
{
    return tx_table.VC_GlobalShdnNodeStatus_signals.VC_FirstFaultNode_value;
}
float app_canTx_VC_MappedPedalPercentage_get(void)
{
    return tx_table.VC_PedalPercentage_signals.VC_MappedPedalPercentage_value;
}
bool app_canTx_VC_Warning_StackWaterMarkHighTask1Hz_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTask1Hz_value;
}
bool app_canTx_VC_Warning_StackWaterMarkHighTask100Hz_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTask100Hz_value;
}
bool app_canTx_VC_Warning_StackWaterMarkHighTask1kHz_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTask1kHz_value;
}
bool app_canTx_VC_Warning_StackWaterMarkHighTaskCanRx_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTaskCanRx_value;
}
bool app_canTx_VC_Warning_StackWaterMarkHighTaskCanTx_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_StackWaterMarkHighTaskCanTx_value;
}
bool app_canTx_VC_Warning_WatchdogTimeout_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_WatchdogTimeout_value;
}
bool app_canTx_VC_Warning_TxOverflow_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_TxOverflow_value;
}
bool app_canTx_VC_Warning_RxOverflow_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_RxOverflow_value;
}
bool app_canTx_VC_Warning_RegenNotAvailable_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_RegenNotAvailable_value;
}
bool app_canTx_VC_Warning_ImuInitFailed_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_ImuInitFailed_value;
}
bool app_canTx_VC_Warning_FlowRateInputOutOfRange_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_FlowRateInputOutOfRange_value;
}
bool app_canTx_VC_Warning_BatteryRailOvercurrentFault_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_BatteryRailOvercurrentFault_value;
}
bool app_canTx_VC_Warning_AccumulatorRailOvercurrentFault_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_AccumulatorRailOvercurrentFault_value;
}
bool app_canTx_VC_Warning_LvChargeFault_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_LvChargeFault_value;
}
bool app_canTx_VC_Warning_BoostControllerFault_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_BoostControllerFault_value;
}
bool app_canTx_VC_Warning_HighNumberOfCanDataLogs_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_HighNumberOfCanDataLogs_value;
}
bool app_canTx_VC_Warning_CanLoggingSdCardNotPresent_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_CanLoggingSdCardNotPresent_value;
}
bool app_canTx_VC_Warning_CanLoggingErrored_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_CanLoggingErrored_value;
}
bool app_canTx_VC_Warning_BrakeAppsDisagreement_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_BrakeAppsDisagreement_value;
}
bool app_canTx_VC_Warning_SbgInitFailed_get(void)
{
    return tx_table.VC_Warnings_signals.VC_Warning_SbgInitFailed_value;
}
bool app_canTx_VC_Fault_MissingBMSHeartbeat_get(void)
{
    return tx_table.VC_Faults_signals.VC_Fault_MissingBMSHeartbeat_value;
}
bool app_canTx_VC_Fault_MissingFSMHeartbeat_get(void)
{
    return tx_table.VC_Faults_signals.VC_Fault_MissingFSMHeartbeat_value;
}
bool app_canTx_VC_Fault_MissingRSMHeartbeat_get(void)
{
    return tx_table.VC_Faults_signals.VC_Fault_MissingRSMHeartbeat_value;
}
bool app_canTx_VC_Fault_MissingCRITHeartbeat_get(void)
{
    return tx_table.VC_Faults_signals.VC_Fault_MissingCRITHeartbeat_value;
}
bool app_canTx_VC_Fault_LeftInverterFault_get(void)
{
    return tx_table.VC_Faults_signals.VC_Fault_LeftInverterFault_value;
}
bool app_canTx_VC_Fault_RightInverterFault_get(void)
{
    return tx_table.VC_Faults_signals.VC_Fault_RightInverterFault_value;
}
uint32_t app_canTx_VC_Warning_StackWaterMarkHighTask1HzCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_StackWaterMarkHighTask1HzCount_value;
}
uint32_t app_canTx_VC_Warning_StackWaterMarkHighTask100HzCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_StackWaterMarkHighTask100HzCount_value;
}
uint32_t app_canTx_VC_Warning_StackWaterMarkHighTask1kHzCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_StackWaterMarkHighTask1kHzCount_value;
}
uint32_t app_canTx_VC_Warning_StackWaterMarkHighTaskCanRxCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_StackWaterMarkHighTaskCanRxCount_value;
}
uint32_t app_canTx_VC_Warning_StackWaterMarkHighTaskCanTxCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_StackWaterMarkHighTaskCanTxCount_value;
}
uint32_t app_canTx_VC_Warning_WatchdogTimeoutCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_WatchdogTimeoutCount_value;
}
uint32_t app_canTx_VC_Warning_TxOverflowCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_TxOverflowCount_value;
}
uint32_t app_canTx_VC_Warning_RxOverflowCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_RxOverflowCount_value;
}
uint32_t app_canTx_VC_Warning_RegenNotAvailableCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_RegenNotAvailableCount_value;
}
uint32_t app_canTx_VC_Warning_ImuInitFailedCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_ImuInitFailedCount_value;
}
uint32_t app_canTx_VC_Warning_FlowRateInputOutOfRangeCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_FlowRateInputOutOfRangeCount_value;
}
uint32_t app_canTx_VC_Warning_BatteryRailOvercurrentFaultCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_BatteryRailOvercurrentFaultCount_value;
}
uint32_t app_canTx_VC_Warning_AccumulatorRailOvercurrentFaultCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_AccumulatorRailOvercurrentFaultCount_value;
}
uint32_t app_canTx_VC_Warning_LvChargeFaultCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_LvChargeFaultCount_value;
}
uint32_t app_canTx_VC_Warning_BoostControllerFaultCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_BoostControllerFaultCount_value;
}
uint32_t app_canTx_VC_Warning_HighNumberOfCanDataLogsCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_HighNumberOfCanDataLogsCount_value;
}
uint32_t app_canTx_VC_Warning_CanLoggingSdCardNotPresentCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_CanLoggingSdCardNotPresentCount_value;
}
uint32_t app_canTx_VC_Warning_CanLoggingErroredCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_CanLoggingErroredCount_value;
}
uint32_t app_canTx_VC_Warning_BrakeAppsDisagreementCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_BrakeAppsDisagreementCount_value;
}
uint32_t app_canTx_VC_Warning_SbgInitFailedCount_get(void)
{
    return tx_table.VC_WarningsCounts_signals.VC_Warning_SbgInitFailedCount_value;
}
uint32_t app_canTx_VC_Fault_MissingBMSHeartbeatCount_get(void)
{
    return tx_table.VC_FaultsCounts_signals.VC_Fault_MissingBMSHeartbeatCount_value;
}
uint32_t app_canTx_VC_Fault_MissingFSMHeartbeatCount_get(void)
{
    return tx_table.VC_FaultsCounts_signals.VC_Fault_MissingFSMHeartbeatCount_value;
}
uint32_t app_canTx_VC_Fault_MissingRSMHeartbeatCount_get(void)
{
    return tx_table.VC_FaultsCounts_signals.VC_Fault_MissingRSMHeartbeatCount_value;
}
uint32_t app_canTx_VC_Fault_MissingCRITHeartbeatCount_get(void)
{
    return tx_table.VC_FaultsCounts_signals.VC_Fault_MissingCRITHeartbeatCount_value;
}
uint32_t app_canTx_VC_Fault_LeftInverterFaultCount_get(void)
{
    return tx_table.VC_FaultsCounts_signals.VC_Fault_LeftInverterFaultCount_value;
}
uint32_t app_canTx_VC_Fault_RightInverterFaultCount_get(void)
{
    return tx_table.VC_FaultsCounts_signals.VC_Fault_RightInverterFaultCount_value;
}
VC_Vitals_Signals* app_canTx_VC_Vitals_getData()
{
    return &tx_table.VC_Vitals_signals;
}
VC_LeftInverterCommand_Signals* app_canTx_VC_LeftInverterCommand_getData()
{
    return &tx_table.VC_LeftInverterCommand_signals;
}
VC_INVL_ReadWriteParamCommand_Signals* app_canTx_VC_INVL_ReadWriteParamCommand_getData()
{
    return &tx_table.VC_INVL_ReadWriteParamCommand_signals;
}
VC_RightInverterCommand_Signals* app_canTx_VC_RightInverterCommand_getData()
{
    return &tx_table.VC_RightInverterCommand_signals;
}
VC_INVR_ReadWriteParamCommand_Signals* app_canTx_VC_INVR_ReadWriteParamCommand_getData()
{
    return &tx_table.VC_INVR_ReadWriteParamCommand_signals;
}
VC_AlertsContext_Signals* app_canTx_VC_AlertsContext_getData()
{
    return &tx_table.VC_AlertsContext_signals;
}
VC_EllipseStatus_Signals* app_canTx_VC_EllipseStatus_getData()
{
    return &tx_table.VC_EllipseStatus_signals;
}
VC_EllipseTime_Signals* app_canTx_VC_EllipseTime_getData()
{
    return &tx_table.VC_EllipseTime_signals;
}
VC_EllipseEulerAngles_Signals* app_canTx_VC_EllipseEulerAngles_getData()
{
    return &tx_table.VC_EllipseEulerAngles_signals;
}
VC_CommitInfo_Signals* app_canTx_VC_CommitInfo_getData()
{
    return &tx_table.VC_CommitInfo_signals;
}
VC_PowerEstimate_Signals* app_canTx_VC_PowerEstimate_getData()
{
    return &tx_table.VC_PowerEstimate_signals;
}
VC_PowerEstimatePID_Signals* app_canTx_VC_PowerEstimatePID_getData()
{
    return &tx_table.VC_PowerEstimatePID_signals;
}
VC_PIDSlipRatio_Signals* app_canTx_VC_PIDSlipRatio_getData()
{
    return &tx_table.VC_PIDSlipRatio_signals;
}
VC_SlipRatios_Signals* app_canTx_VC_SlipRatios_getData()
{
    return &tx_table.VC_SlipRatios_signals;
}
VC_ActiveDiffOutputs_Signals* app_canTx_VC_ActiveDiffOutputs_getData()
{
    return &tx_table.VC_ActiveDiffOutputs_signals;
}
VC_PowerLimit_Signals* app_canTx_VC_PowerLimit_getData()
{
    return &tx_table.VC_PowerLimit_signals;
}
VC_VehicleDynamics_Signals* app_canTx_VC_VehicleDynamics_getData()
{
    return &tx_table.VC_VehicleDynamics_signals;
}
VC_EllipseEkfNavVelocity_Signals* app_canTx_VC_EllipseEkfNavVelocity_getData()
{
    return &tx_table.VC_EllipseEkfNavVelocity_signals;
}
VC_Voltages_Signals* app_canTx_VC_Voltages_getData()
{
    return &tx_table.VC_Voltages_signals;
}
VC_CurrentSensing_Signals* app_canTx_VC_CurrentSensing_getData()
{
    return &tx_table.VC_CurrentSensing_signals;
}
VC_Efuse_Signals* app_canTx_VC_Efuse_getData()
{
    return &tx_table.VC_Efuse_signals;
}
VC_ImuLinearAcceleration_Signals* app_canTx_VC_ImuLinearAcceleration_getData()
{
    return &tx_table.VC_ImuLinearAcceleration_signals;
}
VC_CoolantPumpFlowRate_Signals* app_canTx_VC_CoolantPumpFlowRate_getData()
{
    return &tx_table.VC_CoolantPumpFlowRate_signals;
}
VC_VCShdnNodeStatus_Signals* app_canTx_VC_VCShdnNodeStatus_getData()
{
    return &tx_table.VC_VCShdnNodeStatus_signals;
}
VC_GlobalShdnNodeStatus_Signals* app_canTx_VC_GlobalShdnNodeStatus_getData()
{
    return &tx_table.VC_GlobalShdnNodeStatus_signals;
}
VC_PedalPercentage_Signals* app_canTx_VC_PedalPercentage_getData()
{
    return &tx_table.VC_PedalPercentage_signals;
}
VC_Warnings_Signals* app_canTx_VC_Warnings_getData()
{
    return &tx_table.VC_Warnings_signals;
}
VC_Faults_Signals* app_canTx_VC_Faults_getData()
{
    return &tx_table.VC_Faults_signals;
}
VC_WarningsCounts_Signals* app_canTx_VC_WarningsCounts_getData()
{
    return &tx_table.VC_WarningsCounts_signals;
}
VC_FaultsCounts_Signals* app_canTx_VC_FaultsCounts_getData()
{
    return &tx_table.VC_FaultsCounts_signals;
}