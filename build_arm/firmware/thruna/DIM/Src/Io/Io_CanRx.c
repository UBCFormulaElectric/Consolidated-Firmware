/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "Io_CanRx.h"
#include "App_CanRx.h"
#include "App_CanUtils.h"

/* ------------------------- Function Definitions ------------------------- */

bool Io_CanRx_FilterMessageId(uint32_t std_id)
{
    bool is_found = false;
    switch (std_id)
    {
        case CAN_MSG_DEBUG_CAN_MODES_ID:
        case CAN_MSG_BMS_VITALS_ID:
        case CAN_MSG_BMS_OK_STATUSES_ID:
        case CAN_MSG_BMS_CONTACTORS_ID:
        case CAN_MSG_BMS_CELL_VOLTAGES_ID:
        case CAN_MSG_BMS_TRACTIVE_SYSTEM_ID:
        case CAN_MSG_BMS_WARNINGS_ID:
        case CAN_MSG_BMS_FAULTS_ID:
        case CAN_MSG_DCM_VITALS_ID:
        case CAN_MSG_DCM_WARNINGS_ID:
        case CAN_MSG_DCM_FAULTS_ID:
        case CAN_MSG_FSM_WARNINGS_ID:
        case CAN_MSG_FSM_FAULTS_ID:
        case CAN_MSG_PDM_WARNINGS_ID:
        case CAN_MSG_PDM_FAULTS_ID:
        case CAN_MSG_INVR_TEMPERATURES1_ID:
        case CAN_MSG_INVR_MOTOR_POSITION_INFO_ID:
        case CAN_MSG_INVL_MOTOR_POSITION_INFO_ID:
        {
            is_found = true;
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
    
    return is_found;
}

void Io_CanRx_UpdateRxTableWithMessage(struct CanMsg* msg)
{
    switch (msg->std_id)
    {
        case CAN_MSG_DEBUG_CAN_MODES_ID:
        {
            Debug_CanModes_Signals out_msg;
            App_CanUtils_Debug_CanModes_Unpack(msg->data, &out_msg);
            
            App_CanRx_Debug_CanModes_EnableDebugMode_Update(out_msg.EnableDebugMode_value);
            break;
        }
        case CAN_MSG_BMS_VITALS_ID:
        {
            BMS_Vitals_Signals out_msg;
            App_CanUtils_BMS_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_Vitals_Heartbeat_Update(out_msg.Heartbeat_value);
            App_CanRx_BMS_Vitals_CurrentState_Update(out_msg.CurrentState_value);
            App_CanRx_BMS_Vitals_StateOfCharge_Update(out_msg.StateOfCharge_value);
            break;
        }
        case CAN_MSG_BMS_OK_STATUSES_ID:
        {
            BMS_OkStatuses_Signals out_msg;
            App_CanUtils_BMS_OkStatuses_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_OkStatuses_BmsOk_Update(out_msg.BmsOk_value);
            App_CanRx_BMS_OkStatuses_ImdOk_Update(out_msg.ImdOk_value);
            App_CanRx_BMS_OkStatuses_BspdOk_Update(out_msg.BspdOk_value);
            App_CanRx_BMS_OkStatuses_ImdLatchedFaultStatus_Update(out_msg.ImdLatchedFaultStatus_value);
            App_CanRx_BMS_OkStatuses_BspdLatchedFaultStatus_Update(out_msg.BspdLatchedFaultStatus_value);
            break;
        }
        case CAN_MSG_BMS_CONTACTORS_ID:
        {
            BMS_Contactors_Signals out_msg;
            App_CanUtils_BMS_Contactors_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_Contactors_AirPositive_Update(out_msg.AirPositive_value);
            App_CanRx_BMS_Contactors_AirNegative_Update(out_msg.AirNegative_value);
            App_CanRx_BMS_Contactors_PrechargeRelay_Update(out_msg.PrechargeRelay_value);
            break;
        }
        case CAN_MSG_BMS_CELL_VOLTAGES_ID:
        {
            BMS_CellVoltages_Signals out_msg;
            App_CanUtils_BMS_CellVoltages_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_CellVoltages_MinCellVoltage_Update(out_msg.MinCellVoltage_value);
            App_CanRx_BMS_CellVoltages_MaxCellVoltage_Update(out_msg.MaxCellVoltage_value);
            break;
        }
        case CAN_MSG_BMS_TRACTIVE_SYSTEM_ID:
        {
            BMS_TractiveSystem_Signals out_msg;
            App_CanUtils_BMS_TractiveSystem_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_TractiveSystem_TsVoltage_Update(out_msg.TsVoltage_value);
            App_CanRx_BMS_TractiveSystem_TsCurrent_Update(out_msg.TsCurrent_value);
            break;
        }
        case CAN_MSG_BMS_WARNINGS_ID:
        {
            BMS_Warnings_Signals out_msg;
            App_CanUtils_BMS_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(out_msg.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value);
            App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(out_msg.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value);
            App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(out_msg.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value);
            App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(out_msg.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value);
            App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(out_msg.BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value);
            App_CanRx_BMS_Warnings_BMS_WARNING_WATCHDOG_TIMEOUT_Update(out_msg.BMS_WARNING_WATCHDOG_TIMEOUT_value);
            App_CanRx_BMS_Warnings_BMS_WARNING_TX_OVERFLOW_Update(out_msg.BMS_WARNING_TX_OVERFLOW_value);
            App_CanRx_BMS_Warnings_BMS_WARNING_RX_OVERFLOW_Update(out_msg.BMS_WARNING_RX_OVERFLOW_value);
            break;
        }
        case CAN_MSG_BMS_FAULTS_ID:
        {
            BMS_Faults_Signals out_msg;
            App_CanUtils_BMS_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_Faults_BMS_FAULT_MISSING_HEARTBEAT_Update(out_msg.BMS_FAULT_MISSING_HEARTBEAT_value);
            App_CanRx_BMS_Faults_BMS_FAULT_STATE_FAULT_Update(out_msg.BMS_FAULT_STATE_FAULT_value);
            App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERVOLTAGE_Update(out_msg.BMS_FAULT_CELL_UNDERVOLTAGE_value);
            App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERVOLTAGE_Update(out_msg.BMS_FAULT_CELL_OVERVOLTAGE_value);
            App_CanRx_BMS_Faults_BMS_FAULT_MODULE_COMM_ERROR_Update(out_msg.BMS_FAULT_MODULE_COMM_ERROR_value);
            App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERTEMP_Update(out_msg.BMS_FAULT_CELL_UNDERTEMP_value);
            App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Update(out_msg.BMS_FAULT_CELL_OVERTEMP_value);
            App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_FAULT_Update(out_msg.BMS_FAULT_CHARGER_FAULT_value);
            App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_Update(out_msg.BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_value);
            App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_Update(out_msg.BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_value);
            App_CanRx_BMS_Faults_BMS_FAULT_HAS_REACHED_MAX_V_Update(out_msg.BMS_FAULT_HAS_REACHED_MAX_V_value);
            App_CanRx_BMS_Faults_BMS_FAULT_TS_OVERCURRENT_Update(out_msg.BMS_FAULT_TS_OVERCURRENT_value);
            App_CanRx_BMS_Faults_BMS_FAULT_PRECHARGE_ERROR_Update(out_msg.BMS_FAULT_PRECHARGE_ERROR_value);
            App_CanRx_BMS_Faults_BMS_FAULT_LATCHED_BMS_FAULT_Update(out_msg.BMS_FAULT_LATCHED_BMS_FAULT_value);
            break;
        }
        case CAN_MSG_DCM_VITALS_ID:
        {
            DCM_Vitals_Signals out_msg;
            App_CanUtils_DCM_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_DCM_Vitals_Heartbeat_Update(out_msg.Heartbeat_value);
            App_CanRx_DCM_Vitals_CurrentState_Update(out_msg.CurrentState_value);
            break;
        }
        case CAN_MSG_DCM_WARNINGS_ID:
        {
            DCM_Warnings_Signals out_msg;
            App_CanUtils_DCM_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(out_msg.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value);
            App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(out_msg.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value);
            App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(out_msg.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value);
            App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(out_msg.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value);
            App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(out_msg.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value);
            App_CanRx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Update(out_msg.DCM_WARNING_WATCHDOG_TIMEOUT_value);
            App_CanRx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Update(out_msg.DCM_WARNING_TX_OVERFLOW_value);
            App_CanRx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Update(out_msg.DCM_WARNING_RX_OVERFLOW_value);
            break;
        }
        case CAN_MSG_DCM_FAULTS_ID:
        {
            DCM_Faults_Signals out_msg;
            App_CanUtils_DCM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_DCM_Faults_DCM_FAULT_STATE_FAULT_Update(out_msg.DCM_FAULT_STATE_FAULT_value);
            App_CanRx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Update(out_msg.DCM_FAULT_MISSING_HEARTBEAT_value);
            break;
        }
        case CAN_MSG_FSM_WARNINGS_ID:
        {
            FSM_Warnings_Signals out_msg;
            App_CanUtils_FSM_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(out_msg.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(out_msg.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(out_msg.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(out_msg.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(out_msg.FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_WATCHDOG_TIMEOUT_Update(out_msg.FSM_WARNING_WATCHDOG_TIMEOUT_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_TX_OVERFLOW_Update(out_msg.FSM_WARNING_TX_OVERFLOW_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_RX_OVERFLOW_Update(out_msg.FSM_WARNING_RX_OVERFLOW_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Update(out_msg.FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Update(out_msg.FSM_WARNING_BRAKE_ACC_DISAGREEMENT_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_APPS_HAS_DISAGREEMENT_Update(out_msg.FSM_WARNING_APPS_HAS_DISAGREEMENT_value);
            break;
        }
        case CAN_MSG_FSM_FAULTS_ID:
        {
            FSM_Faults_Signals out_msg;
            App_CanUtils_FSM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Update(out_msg.FSM_FAULT_MISSING_HEARTBEAT_value);
            App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Update(out_msg.FSM_FAULT_STATE_FAULT_value);
            App_CanRx_FSM_Faults_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_Update(out_msg.FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_value);
            App_CanRx_FSM_Faults_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_Update(out_msg.FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_value);
            App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Update(out_msg.FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_value);
            App_CanRx_FSM_Faults_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_Update(out_msg.FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_value);
            break;
        }
        case CAN_MSG_PDM_WARNINGS_ID:
        {
            PDM_Warnings_Signals out_msg;
            App_CanUtils_PDM_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(out_msg.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value);
            App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(out_msg.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value);
            App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(out_msg.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value);
            App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(out_msg.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value);
            App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(out_msg.PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value);
            App_CanRx_PDM_Warnings_PDM_WARNING_WATCHDOG_TIMEOUT_Update(out_msg.PDM_WARNING_WATCHDOG_TIMEOUT_value);
            App_CanRx_PDM_Warnings_PDM_WARNING_TX_OVERFLOW_Update(out_msg.PDM_WARNING_TX_OVERFLOW_value);
            App_CanRx_PDM_Warnings_PDM_WARNING_RX_OVERFLOW_Update(out_msg.PDM_WARNING_RX_OVERFLOW_value);
            break;
        }
        case CAN_MSG_PDM_FAULTS_ID:
        {
            PDM_Faults_Signals out_msg;
            App_CanUtils_PDM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Update(out_msg.PDM_FAULT_DUMMY_value);
            break;
        }
        case CAN_MSG_INVR_TEMPERATURES1_ID:
        {
            INVR_Temperatures1_Signals out_msg;
            App_CanUtils_INVR_Temperatures1_Unpack(msg->data, &out_msg);
            
            App_CanRx_INVR_Temperatures1_ModuleATemperature_Update(out_msg.ModuleATemperature_value);
            App_CanRx_INVR_Temperatures1_ModuleBTemperature_Update(out_msg.ModuleBTemperature_value);
            App_CanRx_INVR_Temperatures1_ModuleCTemperature_Update(out_msg.ModuleCTemperature_value);
            App_CanRx_INVR_Temperatures1_GateDriverBoardTemperature_Update(out_msg.GateDriverBoardTemperature_value);
            break;
        }
        case CAN_MSG_INVR_MOTOR_POSITION_INFO_ID:
        {
            INVR_MotorPositionInfo_Signals out_msg;
            App_CanUtils_INVR_MotorPositionInfo_Unpack(msg->data, &out_msg);
            
            App_CanRx_INVR_MotorPositionInfo_MotorAngle_Update(out_msg.MotorAngle_value);
            App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(out_msg.MotorSpeed_value);
            App_CanRx_INVR_MotorPositionInfo_ElectricalOutputFrequency_Update(out_msg.ElectricalOutputFrequency_value);
            App_CanRx_INVR_MotorPositionInfo_DeltaResolverFiltered_Update(out_msg.DeltaResolverFiltered_value);
            break;
        }
        case CAN_MSG_INVL_MOTOR_POSITION_INFO_ID:
        {
            INVL_MotorPositionInfo_Signals out_msg;
            App_CanUtils_INVL_MotorPositionInfo_Unpack(msg->data, &out_msg);
            
            App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(out_msg.MotorAngle_value);
            App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(out_msg.MotorSpeed_value);
            App_CanRx_INVL_MotorPositionInfo_ElectricalOutputFrequency_Update(out_msg.ElectricalOutputFrequency_value);
            App_CanRx_INVL_MotorPositionInfo_DeltaResolverFiltered_Update(out_msg.DeltaResolverFiltered_value);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}

