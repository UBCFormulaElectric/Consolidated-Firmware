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
    Debug_CanModes_Signals Debug_CanModes_signals;
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
    App_CanRx_Debug_CanModes_EnableDebugMode_Update(CANSIG_DEBUG_CAN_MODES_ENABLE_DEBUG_MODE_START_VAL);
    App_CanRx_BMS_Vitals_Heartbeat_Update(CANSIG_BMS_VITALS_HEARTBEAT_START_VAL);
    App_CanRx_BMS_Vitals_CurrentState_Update(CANSIG_BMS_VITALS_CURRENT_STATE_START_VAL);
    App_CanRx_BMS_Vitals_StateOfCharge_Update(CANSIG_BMS_VITALS_STATE_OF_CHARGE_START_VAL);
    App_CanRx_BMS_OkStatuses_BmsOk_Update(CANSIG_BMS_OK_STATUSES_BMS_OK_START_VAL);
    App_CanRx_BMS_OkStatuses_ImdOk_Update(CANSIG_BMS_OK_STATUSES_IMD_OK_START_VAL);
    App_CanRx_BMS_OkStatuses_BspdOk_Update(CANSIG_BMS_OK_STATUSES_BSPD_OK_START_VAL);
    App_CanRx_BMS_OkStatuses_ImdLatchedFaultStatus_Update(CANSIG_BMS_OK_STATUSES_IMD_LATCHED_FAULT_STATUS_START_VAL);
    App_CanRx_BMS_OkStatuses_BspdLatchedFaultStatus_Update(CANSIG_BMS_OK_STATUSES_BSPD_LATCHED_FAULT_STATUS_START_VAL);
    App_CanRx_BMS_Contactors_AirPositive_Update(CANSIG_BMS_CONTACTORS_AIR_POSITIVE_START_VAL);
    App_CanRx_BMS_Contactors_AirNegative_Update(CANSIG_BMS_CONTACTORS_AIR_NEGATIVE_START_VAL);
    App_CanRx_BMS_Contactors_PrechargeRelay_Update(CANSIG_BMS_CONTACTORS_PRECHARGE_RELAY_START_VAL);
    App_CanRx_BMS_CellVoltages_MinCellVoltage_Update(CANSIG_BMS_CELL_VOLTAGES_MIN_CELL_VOLTAGE_START_VAL);
    App_CanRx_BMS_CellVoltages_MaxCellVoltage_Update(CANSIG_BMS_CELL_VOLTAGES_MAX_CELL_VOLTAGE_START_VAL);
    App_CanRx_BMS_TractiveSystem_TsVoltage_Update(CANSIG_BMS_TRACTIVE_SYSTEM_TS_VOLTAGE_START_VAL);
    App_CanRx_BMS_TractiveSystem_TsCurrent_Update(CANSIG_BMS_TRACTIVE_SYSTEM_TS_CURRENT_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_WATCHDOG_TIMEOUT_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_TX_OVERFLOW_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_BMS_Warnings_BMS_WARNING_RX_OVERFLOW_Update(CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_MISSING_HEARTBEAT_Update(CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_STATE_FAULT_Update(CANSIG_BMS_FAULTS_BMS_FAULT_STATE_FAULT_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERVOLTAGE_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERVOLTAGE_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_MODULE_COMM_ERROR_Update(CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMM_ERROR_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERTEMP_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_FAULT_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_FAULT_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_Update(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_HAS_REACHED_MAX_V_Update(CANSIG_BMS_FAULTS_BMS_FAULT_HAS_REACHED_MAX_V_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_TS_OVERCURRENT_Update(CANSIG_BMS_FAULTS_BMS_FAULT_TS_OVERCURRENT_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_PRECHARGE_ERROR_Update(CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_ERROR_START_VAL);
    App_CanRx_BMS_Faults_BMS_FAULT_LATCHED_BMS_FAULT_Update(CANSIG_BMS_FAULTS_BMS_FAULT_LATCHED_BMS_FAULT_START_VAL);
    App_CanRx_DCM_Vitals_Heartbeat_Update(CANSIG_DCM_VITALS_HEARTBEAT_START_VAL);
    App_CanRx_DCM_Vitals_CurrentState_Update(CANSIG_DCM_VITALS_CURRENT_STATE_START_VAL);
    App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_START_VAL);
    App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_START_VAL);
    App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_START_VAL);
    App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_START_VAL);
    App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_START_VAL);
    App_CanRx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Update(CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_DCM_Faults_DCM_FAULT_STATE_FAULT_Update(CANSIG_DCM_FAULTS_DCM_FAULT_STATE_FAULT_START_VAL);
    App_CanRx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Update(CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_WATCHDOG_TIMEOUT_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_TX_OVERFLOW_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_RX_OVERFLOW_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_START_VAL);
    App_CanRx_FSM_Warnings_FSM_WARNING_APPS_HAS_DISAGREEMENT_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_APPS_HAS_DISAGREEMENT_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Update(CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Update(CANSIG_FSM_FAULTS_FSM_FAULT_STATE_FAULT_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_Update(CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_Update(CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Update(CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_Update(CANSIG_FSM_FAULTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_WATCHDOG_TIMEOUT_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_TX_OVERFLOW_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanRx_PDM_Warnings_PDM_WARNING_RX_OVERFLOW_Update(CANSIG_PDM_WARNINGS_PDM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Update(CANSIG_PDM_FAULTS_PDM_FAULT_DUMMY_START_VAL);
    App_CanRx_INVR_Temperatures1_ModuleATemperature_Update(CANSIG_INVR_TEMPERATURES1_MODULE_A_TEMPERATURE_START_VAL);
    App_CanRx_INVR_Temperatures1_ModuleBTemperature_Update(CANSIG_INVR_TEMPERATURES1_MODULE_B_TEMPERATURE_START_VAL);
    App_CanRx_INVR_Temperatures1_ModuleCTemperature_Update(CANSIG_INVR_TEMPERATURES1_MODULE_C_TEMPERATURE_START_VAL);
    App_CanRx_INVR_Temperatures1_GateDriverBoardTemperature_Update(CANSIG_INVR_TEMPERATURES1_GATE_DRIVER_BOARD_TEMPERATURE_START_VAL);
    App_CanRx_INVR_MotorPositionInfo_MotorAngle_Update(CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_START_VAL);
    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_START_VAL);
    App_CanRx_INVR_MotorPositionInfo_ElectricalOutputFrequency_Update(CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_START_VAL);
    App_CanRx_INVR_MotorPositionInfo_DeltaResolverFiltered_Update(CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_START_VAL);
    App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_START_VAL);
    App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_START_VAL);
    App_CanRx_INVL_MotorPositionInfo_ElectricalOutputFrequency_Update(CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_START_VAL);
    App_CanRx_INVL_MotorPositionInfo_DeltaResolverFiltered_Update(CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_START_VAL);
}

void App_CanRx_Debug_CanModes_EnableDebugMode_Update(bool value)
{
    rx_table.Debug_CanModes_signals.EnableDebugMode_value = value;
}

void App_CanRx_BMS_Vitals_Heartbeat_Update(bool value)
{
    rx_table.BMS_Vitals_signals.Heartbeat_value = value;
}

void App_CanRx_BMS_Vitals_CurrentState_Update(BmsState value)
{
    rx_table.BMS_Vitals_signals.CurrentState_value = (value > CANSIG_BMS_VITALS_CURRENT_STATE_MAX) ? CANSIG_BMS_VITALS_CURRENT_STATE_MAX : value;
}

void App_CanRx_BMS_Vitals_StateOfCharge_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_VITALS_STATE_OF_CHARGE_MIN ? CANSIG_BMS_VITALS_STATE_OF_CHARGE_MIN : value;
    rx_table.BMS_Vitals_signals.StateOfCharge_value = tmp > CANSIG_BMS_VITALS_STATE_OF_CHARGE_MAX ? CANSIG_BMS_VITALS_STATE_OF_CHARGE_MAX : tmp;
}

void App_CanRx_BMS_OkStatuses_BmsOk_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BmsOk_value = value;
}

void App_CanRx_BMS_OkStatuses_ImdOk_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.ImdOk_value = value;
}

void App_CanRx_BMS_OkStatuses_BspdOk_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BspdOk_value = value;
}

void App_CanRx_BMS_OkStatuses_ImdLatchedFaultStatus_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.ImdLatchedFaultStatus_value = value;
}

void App_CanRx_BMS_OkStatuses_BspdLatchedFaultStatus_Update(bool value)
{
    rx_table.BMS_OkStatuses_signals.BspdLatchedFaultStatus_value = value;
}

void App_CanRx_BMS_Contactors_AirPositive_Update(ContactorState value)
{
    rx_table.BMS_Contactors_signals.AirPositive_value = (value > CANSIG_BMS_CONTACTORS_AIR_POSITIVE_MAX) ? CANSIG_BMS_CONTACTORS_AIR_POSITIVE_MAX : value;
}

void App_CanRx_BMS_Contactors_AirNegative_Update(ContactorState value)
{
    rx_table.BMS_Contactors_signals.AirNegative_value = (value > CANSIG_BMS_CONTACTORS_AIR_NEGATIVE_MAX) ? CANSIG_BMS_CONTACTORS_AIR_NEGATIVE_MAX : value;
}

void App_CanRx_BMS_Contactors_PrechargeRelay_Update(ContactorState value)
{
    rx_table.BMS_Contactors_signals.PrechargeRelay_value = (value > CANSIG_BMS_CONTACTORS_PRECHARGE_RELAY_MAX) ? CANSIG_BMS_CONTACTORS_PRECHARGE_RELAY_MAX : value;
}

void App_CanRx_BMS_CellVoltages_MinCellVoltage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_CELL_VOLTAGES_MIN_CELL_VOLTAGE_MIN ? CANSIG_BMS_CELL_VOLTAGES_MIN_CELL_VOLTAGE_MIN : value;
    rx_table.BMS_CellVoltages_signals.MinCellVoltage_value = tmp > CANSIG_BMS_CELL_VOLTAGES_MIN_CELL_VOLTAGE_MAX ? CANSIG_BMS_CELL_VOLTAGES_MIN_CELL_VOLTAGE_MAX : tmp;
}

void App_CanRx_BMS_CellVoltages_MaxCellVoltage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_CELL_VOLTAGES_MAX_CELL_VOLTAGE_MIN ? CANSIG_BMS_CELL_VOLTAGES_MAX_CELL_VOLTAGE_MIN : value;
    rx_table.BMS_CellVoltages_signals.MaxCellVoltage_value = tmp > CANSIG_BMS_CELL_VOLTAGES_MAX_CELL_VOLTAGE_MAX ? CANSIG_BMS_CELL_VOLTAGES_MAX_CELL_VOLTAGE_MAX : tmp;
}

void App_CanRx_BMS_TractiveSystem_TsVoltage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_TRACTIVE_SYSTEM_TS_VOLTAGE_MIN ? CANSIG_BMS_TRACTIVE_SYSTEM_TS_VOLTAGE_MIN : value;
    rx_table.BMS_TractiveSystem_signals.TsVoltage_value = tmp > CANSIG_BMS_TRACTIVE_SYSTEM_TS_VOLTAGE_MAX ? CANSIG_BMS_TRACTIVE_SYSTEM_TS_VOLTAGE_MAX : tmp;
}

void App_CanRx_BMS_TractiveSystem_TsCurrent_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_TRACTIVE_SYSTEM_TS_CURRENT_MIN ? CANSIG_BMS_TRACTIVE_SYSTEM_TS_CURRENT_MIN : value;
    rx_table.BMS_TractiveSystem_signals.TsCurrent_value = tmp > CANSIG_BMS_TRACTIVE_SYSTEM_TS_CURRENT_MAX ? CANSIG_BMS_TRACTIVE_SYSTEM_TS_CURRENT_MAX : tmp;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_WATCHDOG_TIMEOUT_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_WATCHDOG_TIMEOUT_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_TX_OVERFLOW_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_TX_OVERFLOW_value = value;
}

void App_CanRx_BMS_Warnings_BMS_WARNING_RX_OVERFLOW_Update(bool value)
{
    rx_table.BMS_Warnings_signals.BMS_WARNING_RX_OVERFLOW_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_MISSING_HEARTBEAT_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_MISSING_HEARTBEAT_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_STATE_FAULT_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_STATE_FAULT_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERVOLTAGE_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CELL_UNDERVOLTAGE_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERVOLTAGE_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CELL_OVERVOLTAGE_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_MODULE_COMM_ERROR_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_MODULE_COMM_ERROR_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERTEMP_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CELL_UNDERTEMP_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CELL_OVERTEMP_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_FAULT_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_FAULT_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_HAS_REACHED_MAX_V_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_HAS_REACHED_MAX_V_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_TS_OVERCURRENT_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_TS_OVERCURRENT_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_PRECHARGE_ERROR_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_PRECHARGE_ERROR_value = value;
}

void App_CanRx_BMS_Faults_BMS_FAULT_LATCHED_BMS_FAULT_Update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_FAULT_LATCHED_BMS_FAULT_value = value;
}

void App_CanRx_DCM_Vitals_Heartbeat_Update(bool value)
{
    rx_table.DCM_Vitals_signals.Heartbeat_value = value;
}

void App_CanRx_DCM_Vitals_CurrentState_Update(DcmState value)
{
    rx_table.DCM_Vitals_signals.CurrentState_value = (value > CANSIG_DCM_VITALS_CURRENT_STATE_MAX) ? CANSIG_DCM_VITALS_CURRENT_STATE_MAX : value;
}

void App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = value;
}

void App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = value;
}

void App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = value;
}

void App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = value;
}

void App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = value;
}

void App_CanRx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_WARNING_WATCHDOG_TIMEOUT_value = value;
}

void App_CanRx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_WARNING_TX_OVERFLOW_value = value;
}

void App_CanRx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Update(bool value)
{
    rx_table.DCM_Warnings_signals.DCM_WARNING_RX_OVERFLOW_value = value;
}

void App_CanRx_DCM_Faults_DCM_FAULT_STATE_FAULT_Update(bool value)
{
    rx_table.DCM_Faults_signals.DCM_FAULT_STATE_FAULT_value = value;
}

void App_CanRx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Update(bool value)
{
    rx_table.DCM_Faults_signals.DCM_FAULT_MISSING_HEARTBEAT_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_WATCHDOG_TIMEOUT_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_WATCHDOG_TIMEOUT_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_TX_OVERFLOW_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_TX_OVERFLOW_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_RX_OVERFLOW_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_RX_OVERFLOW_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_BRAKE_ACC_DISAGREEMENT_value = value;
}

void App_CanRx_FSM_Warnings_FSM_WARNING_APPS_HAS_DISAGREEMENT_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_WARNING_APPS_HAS_DISAGREEMENT_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_MISSING_HEARTBEAT_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_STATE_FAULT_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_WATCHDOG_TIMEOUT_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_WATCHDOG_TIMEOUT_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_TX_OVERFLOW_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_TX_OVERFLOW_value = value;
}

void App_CanRx_PDM_Warnings_PDM_WARNING_RX_OVERFLOW_Update(bool value)
{
    rx_table.PDM_Warnings_signals.PDM_WARNING_RX_OVERFLOW_value = value;
}

void App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Update(bool value)
{
    rx_table.PDM_Faults_signals.PDM_FAULT_DUMMY_value = value;
}

void App_CanRx_INVR_Temperatures1_ModuleATemperature_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_TEMPERATURES1_MODULE_A_TEMPERATURE_MIN ? CANSIG_INVR_TEMPERATURES1_MODULE_A_TEMPERATURE_MIN : value;
    rx_table.INVR_Temperatures1_signals.ModuleATemperature_value = tmp > CANSIG_INVR_TEMPERATURES1_MODULE_A_TEMPERATURE_MAX ? CANSIG_INVR_TEMPERATURES1_MODULE_A_TEMPERATURE_MAX : tmp;
}

void App_CanRx_INVR_Temperatures1_ModuleBTemperature_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_TEMPERATURES1_MODULE_B_TEMPERATURE_MIN ? CANSIG_INVR_TEMPERATURES1_MODULE_B_TEMPERATURE_MIN : value;
    rx_table.INVR_Temperatures1_signals.ModuleBTemperature_value = tmp > CANSIG_INVR_TEMPERATURES1_MODULE_B_TEMPERATURE_MAX ? CANSIG_INVR_TEMPERATURES1_MODULE_B_TEMPERATURE_MAX : tmp;
}

void App_CanRx_INVR_Temperatures1_ModuleCTemperature_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_TEMPERATURES1_MODULE_C_TEMPERATURE_MIN ? CANSIG_INVR_TEMPERATURES1_MODULE_C_TEMPERATURE_MIN : value;
    rx_table.INVR_Temperatures1_signals.ModuleCTemperature_value = tmp > CANSIG_INVR_TEMPERATURES1_MODULE_C_TEMPERATURE_MAX ? CANSIG_INVR_TEMPERATURES1_MODULE_C_TEMPERATURE_MAX : tmp;
}

void App_CanRx_INVR_Temperatures1_GateDriverBoardTemperature_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_TEMPERATURES1_GATE_DRIVER_BOARD_TEMPERATURE_MIN ? CANSIG_INVR_TEMPERATURES1_GATE_DRIVER_BOARD_TEMPERATURE_MIN : value;
    rx_table.INVR_Temperatures1_signals.GateDriverBoardTemperature_value = tmp > CANSIG_INVR_TEMPERATURES1_GATE_DRIVER_BOARD_TEMPERATURE_MAX ? CANSIG_INVR_TEMPERATURES1_GATE_DRIVER_BOARD_TEMPERATURE_MAX : tmp;
}

void App_CanRx_INVR_MotorPositionInfo_MotorAngle_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.MotorAngle_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_MAX : tmp;
}

void App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(int value)
{
    const int tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.MotorSpeed_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_MAX : tmp;
}

void App_CanRx_INVR_MotorPositionInfo_ElectricalOutputFrequency_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.ElectricalOutputFrequency_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MAX : tmp;
}

void App_CanRx_INVR_MotorPositionInfo_DeltaResolverFiltered_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MIN ? CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MIN : value;
    rx_table.INVR_MotorPositionInfo_signals.DeltaResolverFiltered_value = tmp > CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MAX ? CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MAX : tmp;
}

void App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.MotorAngle_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_MAX : tmp;
}

void App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(int value)
{
    const int tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.MotorSpeed_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_MAX : tmp;
}

void App_CanRx_INVL_MotorPositionInfo_ElectricalOutputFrequency_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.ElectricalOutputFrequency_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_MAX : tmp;
}

void App_CanRx_INVL_MotorPositionInfo_DeltaResolverFiltered_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MIN ? CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MIN : value;
    rx_table.INVL_MotorPositionInfo_signals.DeltaResolverFiltered_value = tmp > CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MAX ? CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_MAX : tmp;
}

bool App_CanRx_Debug_CanModes_EnableDebugMode_Get()
{
    return rx_table.Debug_CanModes_signals.EnableDebugMode_value;
}

bool App_CanRx_BMS_Vitals_Heartbeat_Get()
{
    return rx_table.BMS_Vitals_signals.Heartbeat_value;
}

BmsState App_CanRx_BMS_Vitals_CurrentState_Get()
{
    return rx_table.BMS_Vitals_signals.CurrentState_value;
}

float App_CanRx_BMS_Vitals_StateOfCharge_Get()
{
    return rx_table.BMS_Vitals_signals.StateOfCharge_value;
}

bool App_CanRx_BMS_OkStatuses_BmsOk_Get()
{
    return rx_table.BMS_OkStatuses_signals.BmsOk_value;
}

bool App_CanRx_BMS_OkStatuses_ImdOk_Get()
{
    return rx_table.BMS_OkStatuses_signals.ImdOk_value;
}

bool App_CanRx_BMS_OkStatuses_BspdOk_Get()
{
    return rx_table.BMS_OkStatuses_signals.BspdOk_value;
}

bool App_CanRx_BMS_OkStatuses_ImdLatchedFaultStatus_Get()
{
    return rx_table.BMS_OkStatuses_signals.ImdLatchedFaultStatus_value;
}

bool App_CanRx_BMS_OkStatuses_BspdLatchedFaultStatus_Get()
{
    return rx_table.BMS_OkStatuses_signals.BspdLatchedFaultStatus_value;
}

ContactorState App_CanRx_BMS_Contactors_AirPositive_Get()
{
    return rx_table.BMS_Contactors_signals.AirPositive_value;
}

ContactorState App_CanRx_BMS_Contactors_AirNegative_Get()
{
    return rx_table.BMS_Contactors_signals.AirNegative_value;
}

ContactorState App_CanRx_BMS_Contactors_PrechargeRelay_Get()
{
    return rx_table.BMS_Contactors_signals.PrechargeRelay_value;
}

float App_CanRx_BMS_CellVoltages_MinCellVoltage_Get()
{
    return rx_table.BMS_CellVoltages_signals.MinCellVoltage_value;
}

float App_CanRx_BMS_CellVoltages_MaxCellVoltage_Get()
{
    return rx_table.BMS_CellVoltages_signals.MaxCellVoltage_value;
}

float App_CanRx_BMS_TractiveSystem_TsVoltage_Get()
{
    return rx_table.BMS_TractiveSystem_signals.TsVoltage_value;
}

float App_CanRx_BMS_TractiveSystem_TsCurrent_Get()
{
    return rx_table.BMS_TractiveSystem_signals.TsCurrent_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_WATCHDOG_TIMEOUT_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_WATCHDOG_TIMEOUT_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_TX_OVERFLOW_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_TX_OVERFLOW_value;
}

bool App_CanRx_BMS_Warnings_BMS_WARNING_RX_OVERFLOW_Get()
{
    return rx_table.BMS_Warnings_signals.BMS_WARNING_RX_OVERFLOW_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_MISSING_HEARTBEAT_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_MISSING_HEARTBEAT_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_STATE_FAULT_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_STATE_FAULT_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERVOLTAGE_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CELL_UNDERVOLTAGE_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERVOLTAGE_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CELL_OVERVOLTAGE_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_MODULE_COMM_ERROR_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_MODULE_COMM_ERROR_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERTEMP_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CELL_UNDERTEMP_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CELL_OVERTEMP_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_FAULT_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_FAULT_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_HAS_REACHED_MAX_V_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_HAS_REACHED_MAX_V_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_TS_OVERCURRENT_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_TS_OVERCURRENT_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_PRECHARGE_ERROR_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_PRECHARGE_ERROR_value;
}

bool App_CanRx_BMS_Faults_BMS_FAULT_LATCHED_BMS_FAULT_Get()
{
    return rx_table.BMS_Faults_signals.BMS_FAULT_LATCHED_BMS_FAULT_value;
}

bool App_CanRx_DCM_Vitals_Heartbeat_Get()
{
    return rx_table.DCM_Vitals_signals.Heartbeat_value;
}

DcmState App_CanRx_DCM_Vitals_CurrentState_Get()
{
    return rx_table.DCM_Vitals_signals.CurrentState_value;
}

bool App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
}

bool App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
}

bool App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
}

bool App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
}

bool App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
}

bool App_CanRx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_WARNING_WATCHDOG_TIMEOUT_value;
}

bool App_CanRx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_WARNING_TX_OVERFLOW_value;
}

bool App_CanRx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Get()
{
    return rx_table.DCM_Warnings_signals.DCM_WARNING_RX_OVERFLOW_value;
}

bool App_CanRx_DCM_Faults_DCM_FAULT_STATE_FAULT_Get()
{
    return rx_table.DCM_Faults_signals.DCM_FAULT_STATE_FAULT_value;
}

bool App_CanRx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Get()
{
    return rx_table.DCM_Faults_signals.DCM_FAULT_MISSING_HEARTBEAT_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_WATCHDOG_TIMEOUT_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_WATCHDOG_TIMEOUT_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_TX_OVERFLOW_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_TX_OVERFLOW_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_RX_OVERFLOW_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_RX_OVERFLOW_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_BRAKE_ACC_DISAGREEMENT_value;
}

bool App_CanRx_FSM_Warnings_FSM_WARNING_APPS_HAS_DISAGREEMENT_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_WARNING_APPS_HAS_DISAGREEMENT_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_MISSING_HEARTBEAT_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_STATE_FAULT_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_WATCHDOG_TIMEOUT_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_WATCHDOG_TIMEOUT_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_TX_OVERFLOW_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_TX_OVERFLOW_value;
}

bool App_CanRx_PDM_Warnings_PDM_WARNING_RX_OVERFLOW_Get()
{
    return rx_table.PDM_Warnings_signals.PDM_WARNING_RX_OVERFLOW_value;
}

bool App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Get()
{
    return rx_table.PDM_Faults_signals.PDM_FAULT_DUMMY_value;
}

float App_CanRx_INVR_Temperatures1_ModuleATemperature_Get()
{
    return rx_table.INVR_Temperatures1_signals.ModuleATemperature_value;
}

float App_CanRx_INVR_Temperatures1_ModuleBTemperature_Get()
{
    return rx_table.INVR_Temperatures1_signals.ModuleBTemperature_value;
}

float App_CanRx_INVR_Temperatures1_ModuleCTemperature_Get()
{
    return rx_table.INVR_Temperatures1_signals.ModuleCTemperature_value;
}

float App_CanRx_INVR_Temperatures1_GateDriverBoardTemperature_Get()
{
    return rx_table.INVR_Temperatures1_signals.GateDriverBoardTemperature_value;
}

float App_CanRx_INVR_MotorPositionInfo_MotorAngle_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.MotorAngle_value;
}

int App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.MotorSpeed_value;
}

float App_CanRx_INVR_MotorPositionInfo_ElectricalOutputFrequency_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.ElectricalOutputFrequency_value;
}

float App_CanRx_INVR_MotorPositionInfo_DeltaResolverFiltered_Get()
{
    return rx_table.INVR_MotorPositionInfo_signals.DeltaResolverFiltered_value;
}

float App_CanRx_INVL_MotorPositionInfo_MotorAngle_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.MotorAngle_value;
}

int App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.MotorSpeed_value;
}

float App_CanRx_INVL_MotorPositionInfo_ElectricalOutputFrequency_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.ElectricalOutputFrequency_value;
}

float App_CanRx_INVL_MotorPositionInfo_DeltaResolverFiltered_Get()
{
    return rx_table.INVL_MotorPositionInfo_signals.DeltaResolverFiltered_value;
}

