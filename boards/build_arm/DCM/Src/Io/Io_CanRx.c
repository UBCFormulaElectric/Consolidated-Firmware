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
        case CAN_MSG_BMS_VITALS_ID:
        case CAN_MSG_BMS_AVAILABLE_POWER_ID:
        case CAN_MSG_BMS_WARNINGS_ID:
        case CAN_MSG_BMS_FAULTS_ID:
        case CAN_MSG_DEBUG_CAN_MODES_ID:
        case CAN_MSG_DIM_VITALS_ID:
        case CAN_MSG_DIM_SWITCHES_ID:
        case CAN_MSG_DIM_WARNINGS_ID:
        case CAN_MSG_DIM_FAULTS_ID:
        case CAN_MSG_FSM_VITALS_ID:
        case CAN_MSG_FSM_BRAKE_ID:
        case CAN_MSG_FSM_WARNINGS_ID:
        case CAN_MSG_FSM_FAULTS_ID:
        case CAN_MSG_INVL_MOTOR_POSITION_INFO_ID:
        case CAN_MSG_INVL_INTERNAL_STATES_ID:
        case CAN_MSG_INVR_MOTOR_POSITION_INFO_ID:
        case CAN_MSG_INVR_INTERNAL_STATES_ID:
        case CAN_MSG_PDM_WARNINGS_ID:
        case CAN_MSG_PDM_FAULTS_ID:
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
        case CAN_MSG_BMS_VITALS_ID:
        {
            BMS_Vitals_Signals out_msg;
            App_CanUtils_BMS_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_Vitals_Heartbeat_Update(out_msg.Heartbeat_value);
            App_CanRx_BMS_Vitals_CurrentState_Update(out_msg.CurrentState_value);
            App_CanRx_BMS_Vitals_StateOfCharge_Update(out_msg.StateOfCharge_value);
            break;
        }
        case CAN_MSG_BMS_AVAILABLE_POWER_ID:
        {
            BMS_AvailablePower_Signals out_msg;
            App_CanUtils_BMS_AvailablePower_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_AvailablePower_AvailablePower_Update(out_msg.AvailablePower_value);
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
            break;
        }
        case CAN_MSG_DEBUG_CAN_MODES_ID:
        {
            Debug_CanModes_Signals out_msg;
            App_CanUtils_Debug_CanModes_Unpack(msg->data, &out_msg);
            
            App_CanRx_Debug_CanModes_EnableDebugMode_Update(out_msg.EnableDebugMode_value);
            break;
        }
        case CAN_MSG_DIM_VITALS_ID:
        {
            DIM_Vitals_Signals out_msg;
            App_CanUtils_DIM_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_DIM_Vitals_Heartbeat_Update(out_msg.Heartbeat_value);
            App_CanRx_DIM_Vitals_State_Update(out_msg.State_value);
            break;
        }
        case CAN_MSG_DIM_SWITCHES_ID:
        {
            DIM_Switches_Signals out_msg;
            App_CanUtils_DIM_Switches_Unpack(msg->data, &out_msg);
            
            App_CanRx_DIM_Switches_StartSwitch_Update(out_msg.StartSwitch_value);
            App_CanRx_DIM_Switches_AuxSwitch_Update(out_msg.AuxSwitch_value);
            break;
        }
        case CAN_MSG_DIM_WARNINGS_ID:
        {
            DIM_Warnings_Signals out_msg;
            App_CanUtils_DIM_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(out_msg.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value);
            App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Update(out_msg.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value);
            App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Update(out_msg.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value);
            App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Update(out_msg.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value);
            App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Update(out_msg.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value);
            App_CanRx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Update(out_msg.DIM_WARNING_WATCHDOG_TIMEOUT_value);
            App_CanRx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Update(out_msg.DIM_WARNING_TX_OVERFLOW_value);
            App_CanRx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Update(out_msg.DIM_WARNING_RX_OVERFLOW_value);
            break;
        }
        case CAN_MSG_DIM_FAULTS_ID:
        {
            DIM_Faults_Signals out_msg;
            App_CanUtils_DIM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Update(out_msg.DIM_FAULT_MISSING_HEARTBEAT_value);
            break;
        }
        case CAN_MSG_FSM_VITALS_ID:
        {
            FSM_Vitals_Signals out_msg;
            App_CanUtils_FSM_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_Vitals_Heartbeat_Update(out_msg.Heartbeat_value);
            App_CanRx_FSM_Vitals_State_Update(out_msg.State_value);
            break;
        }
        case CAN_MSG_FSM_BRAKE_ID:
        {
            FSM_Brake_Signals out_msg;
            App_CanUtils_FSM_Brake_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_Brake_FrontBrakePressure_Update(out_msg.FrontBrakePressure_value);
            App_CanRx_FSM_Brake_RearBrakePressure_Update(out_msg.RearBrakePressure_value);
            App_CanRx_FSM_Brake_BrakePedalPercentage_Update(out_msg.BrakePedalPercentage_value);
            App_CanRx_FSM_Brake_IsActuated_Update(out_msg.IsActuated_value);
            App_CanRx_FSM_Brake_PressureSensorOpenShortCircuit_Update(out_msg.PressureSensorOpenShortCircuit_value);
            App_CanRx_FSM_Brake_PedalOpenShortCircuit_Update(out_msg.PedalOpenShortCircuit_value);
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
            App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Update(out_msg.FSM_WARNING_BRAKE_ACC_DISAGREEMENT_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Update(out_msg.FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_value);
            App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Update(out_msg.FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_value);
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
            App_CanRx_FSM_Faults_FSM_FAULT_APPS_HAS_DISAGREEMENT_Update(out_msg.FSM_FAULT_APPS_HAS_DISAGREEMENT_value);
            App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Update(out_msg.FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_value);
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
        case CAN_MSG_INVL_INTERNAL_STATES_ID:
        {
            INVL_InternalStates_Signals out_msg;
            App_CanUtils_INVL_InternalStates_Unpack(msg->data, &out_msg);
            
            App_CanRx_INVL_InternalStates_VsmState_Update(out_msg.VsmState_value);
            App_CanRx_INVL_InternalStates_PwmFrequency_Update(out_msg.PwmFrequency_value);
            App_CanRx_INVL_InternalStates_InverterState_Update(out_msg.InverterState_value);
            App_CanRx_INVL_InternalStates_Relay1State_Update(out_msg.Relay1State_value);
            App_CanRx_INVL_InternalStates_Relay2State_Update(out_msg.Relay2State_value);
            App_CanRx_INVL_InternalStates_Relay3State_Update(out_msg.Relay3State_value);
            App_CanRx_INVL_InternalStates_Relay4State_Update(out_msg.Relay4State_value);
            App_CanRx_INVL_InternalStates_Relay5State_Update(out_msg.Relay5State_value);
            App_CanRx_INVL_InternalStates_Relay6State_Update(out_msg.Relay6State_value);
            App_CanRx_INVL_InternalStates_InverterRunMode_Update(out_msg.InverterRunMode_value);
            App_CanRx_INVL_InternalStates_InverterActiveDischargeState_Update(out_msg.InverterActiveDischargeState_value);
            App_CanRx_INVL_InternalStates_InverterCommandMode_Update(out_msg.InverterCommandMode_value);
            App_CanRx_INVL_InternalStates_RollingCounter_Update(out_msg.RollingCounter_value);
            App_CanRx_INVL_InternalStates_InverterEnableState_Update(out_msg.InverterEnableState_value);
            App_CanRx_INVL_InternalStates_StartModeActive_Update(out_msg.StartModeActive_value);
            App_CanRx_INVL_InternalStates_InverterLockoutEnabled_Update(out_msg.InverterLockoutEnabled_value);
            App_CanRx_INVL_InternalStates_DirectionCommand_Update(out_msg.DirectionCommand_value);
            App_CanRx_INVL_InternalStates_BmsActive_Update(out_msg.BmsActive_value);
            App_CanRx_INVL_InternalStates_BmsTorqueLimiting_Update(out_msg.BmsTorqueLimiting_value);
            App_CanRx_INVL_InternalStates_MaxSpeedLimiting_Update(out_msg.MaxSpeedLimiting_value);
            App_CanRx_INVL_InternalStates_HotSpotLimiting_Update(out_msg.HotSpotLimiting_value);
            App_CanRx_INVL_InternalStates_LowSpeedLimiting_Update(out_msg.LowSpeedLimiting_value);
            App_CanRx_INVL_InternalStates_CoolantTemperatureLimiting_Update(out_msg.CoolantTemperatureLimiting_value);
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
        case CAN_MSG_INVR_INTERNAL_STATES_ID:
        {
            INVR_InternalStates_Signals out_msg;
            App_CanUtils_INVR_InternalStates_Unpack(msg->data, &out_msg);
            
            App_CanRx_INVR_InternalStates_VsmState_Update(out_msg.VsmState_value);
            App_CanRx_INVR_InternalStates_PwmFrequency_Update(out_msg.PwmFrequency_value);
            App_CanRx_INVR_InternalStates_InverterState_Update(out_msg.InverterState_value);
            App_CanRx_INVR_InternalStates_Relay1State_Update(out_msg.Relay1State_value);
            App_CanRx_INVR_InternalStates_Relay2State_Update(out_msg.Relay2State_value);
            App_CanRx_INVR_InternalStates_Relay3State_Update(out_msg.Relay3State_value);
            App_CanRx_INVR_InternalStates_Relay4State_Update(out_msg.Relay4State_value);
            App_CanRx_INVR_InternalStates_Relay5State_Update(out_msg.Relay5State_value);
            App_CanRx_INVR_InternalStates_Relay6State_Update(out_msg.Relay6State_value);
            App_CanRx_INVR_InternalStates_InverterRunMode_Update(out_msg.InverterRunMode_value);
            App_CanRx_INVR_InternalStates_InverterActiveDischargeState_Update(out_msg.InverterActiveDischargeState_value);
            App_CanRx_INVR_InternalStates_InverterCommandMode_Update(out_msg.InverterCommandMode_value);
            App_CanRx_INVR_InternalStates_RollingCounter_Update(out_msg.RollingCounter_value);
            App_CanRx_INVR_InternalStates_InverterEnableState_Update(out_msg.InverterEnableState_value);
            App_CanRx_INVR_InternalStates_StartModeActive_Update(out_msg.StartModeActive_value);
            App_CanRx_INVR_InternalStates_InverterLockoutEnabled_Update(out_msg.InverterLockoutEnabled_value);
            App_CanRx_INVR_InternalStates_DirectionCommand_Update(out_msg.DirectionCommand_value);
            App_CanRx_INVR_InternalStates_BmsActive_Update(out_msg.BmsActive_value);
            App_CanRx_INVR_InternalStates_BmsTorqueLimiting_Update(out_msg.BmsTorqueLimiting_value);
            App_CanRx_INVR_InternalStates_MaxSpeedLimiting_Update(out_msg.MaxSpeedLimiting_value);
            App_CanRx_INVR_InternalStates_HotSpotLimiting_Update(out_msg.HotSpotLimiting_value);
            App_CanRx_INVR_InternalStates_LowSpeedLimiting_Update(out_msg.LowSpeedLimiting_value);
            App_CanRx_INVR_InternalStates_CoolantTemperatureLimiting_Update(out_msg.CoolantTemperatureLimiting_value);
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
        default:
        {
            // Do nothing
            break;
        }
    }
}

