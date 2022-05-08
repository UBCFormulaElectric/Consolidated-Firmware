#include "App_SharedErrorTable.h"
#include "App_CanMsgs.h"
#include "Io_SharedErrorTable.h"

#define SET_ERROR(table, id, is_set) App_SharedErrorTable_SetError(table, id, is_set)

static void
    Io_ProcessBmsNonCriticalErrorMsg(struct ErrorTable *error_table, struct CanMsgs_bms_non_critical_errors_t *data)
{
    SET_ERROR(
        error_table, BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        data->stack_watermark_above_threshold_task1_khz);
    SET_ERROR(
        error_table, BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        data->stack_watermark_above_threshold_taskcanrx);
    SET_ERROR(
        error_table, BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        data->stack_watermark_above_threshold_taskcantx);
    SET_ERROR(error_table, BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, data->watchdog_timeout);
}

static void
    Io_ProcessDcmNonCriticalErrorMsg(struct ErrorTable *error_table, struct CanMsgs_dcm_non_critical_errors_t *data)
{
    SET_ERROR(
        error_table, DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        data->stack_watermark_above_threshold_task1_khz);
    SET_ERROR(
        error_table, DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        data->stack_watermark_above_threshold_taskcanrx);
    SET_ERROR(
        error_table, DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        data->stack_watermark_above_threshold_taskcantx);
    SET_ERROR(error_table, DCM_NON_CRITICAL_WATCHDOG_TIMEOUT, data->watchdog_timeout);
    SET_ERROR(error_table, DCM_NON_CRITICAL_ACCELERATION_X_OUT_OF_RANGE, data->acceleration_x_out_of_range);
    SET_ERROR(error_table, DCM_NON_CRITICAL_ACCELERATION_Y_OUT_OF_RANGE, data->acceleration_y_out_of_range);
    SET_ERROR(error_table, DCM_NON_CRITICAL_ACCELERATION_Z_OUT_OF_RANGE, data->acceleration_z_out_of_range);
}

static void
    Io_ProcessDimNonCriticalErrorMsg(struct ErrorTable *error_table, struct CanMsgs_dim_non_critical_errors_t *data)
{
    SET_ERROR(
        error_table, DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK100HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        data->stack_watermark_above_threshold_task1_khz);
    SET_ERROR(
        error_table, DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        data->stack_watermark_above_threshold_taskcanrx);
    SET_ERROR(
        error_table, DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        data->stack_watermark_above_threshold_taskcantx);
    SET_ERROR(error_table, DIM_NON_CRITICAL_WATCHDOG_TIMEOUT, data->watchdog_timeout);
}

static void
    Io_ProcessFsmNonCriticalErrorMsg(struct ErrorTable *error_table, struct CanMsgs_fsm_non_critical_errors_t *data)
{
    SET_ERROR(error_table, FSM_NON_CRITICAL_PAPPS_OUT_OF_RANGE, data->papps_out_of_range);
    SET_ERROR(error_table, FSM_NON_CRITICAL_SAPPS_OUT_OF_RANGE, data->sapps_out_of_range);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        data->stack_watermark_above_threshold_task1_khz);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        data->stack_watermark_above_threshold_taskcanrx);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        data->stack_watermark_above_threshold_taskcantx);
    SET_ERROR(error_table, FSM_NON_CRITICAL_WATCHDOG_TIMEOUT, data->watchdog_timeout);
    SET_ERROR(error_table, FSM_NON_CRITICAL_BSPD_FAULT, data->bspd_fault);
    SET_ERROR(error_table, FSM_NON_CRITICAL_LEFT_WHEEL_SPEED_OUT_OF_RANGE, data->left_wheel_speed_out_of_range);
    SET_ERROR(error_table, FSM_NON_CRITICAL_RIGHT_WHEEL_SPEED_OUT_OF_RANGE, data->right_wheel_speed_out_of_range);
    SET_ERROR(error_table, FSM_NON_CRITICAL_PRIMARY_FLOW_RATE_OUT_OF_RANGE, data->primary_flow_rate_out_of_range);
    SET_ERROR(error_table, FSM_NON_CRITICAL_SECONDARY_FLOW_RATE_OUT_OF_RANGE, data->secondary_flow_rate_out_of_range);
    SET_ERROR(error_table, FSM_NON_CRITICAL_STEERING_ANGLE_OUT_OF_RANGE, data->steering_angle_out_of_range);
    SET_ERROR(error_table, FSM_NON_CRITICAL_BRAKE_PRESSURE_OUT_OF_RANGE, data->secondary_flow_rate_out_of_range);
}

static void
    Io_ProcessPdmNonCriticalErrorMsg(struct ErrorTable *error_table, struct CanMsgs_pdm_non_critical_errors_t *data)
{
    SET_ERROR(error_table, PDM_NON_CRITICAL_MISSING_HEARTBEAT, data->missing_heartbeat);
    SET_ERROR(error_table, PDM_NON_CRITICAL_BOOST_PGOOD_FAULT, data->boost_pgood_fault);
    SET_ERROR(error_table, PDM_NON_CRITICAL_CHARGER_FAULT, data->charger_fault);
    SET_ERROR(error_table, PDM_NON_CRITICAL_EFUSE_FAULT, data->efuse_fault);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        data->stack_watermark_above_threshold_task1_khz);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        data->stack_watermark_above_threshold_taskcanrx);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        data->stack_watermark_above_threshold_taskcantx);
    SET_ERROR(error_table, PDM_NON_CRITICAL_WATCHDOG_TIMEOUT, data->watchdog_timeout);
    SET_ERROR(error_table, PDM_NON_CRITICAL_VBAT_VOLTAGE_OUT_OF_RANGE, data->vbat_voltage_out_of_range);
    SET_ERROR(error_table, PDM_NON_CRITICAL__24V_AUX_VOLTAGE_OUT_OF_RANGE, data->_24_v_aux_voltage_out_of_range);
    SET_ERROR(error_table, PDM_NON_CRITICAL__24V_ACC_VOLTAGE_OUT_OF_RANGE, data->_24_v_acc_voltage_out_of_range);
    SET_ERROR(error_table, PDM_NON_CRITICAL_AUX1_CURRENT_OUT_OF_RANGE, data->aux1_current_out_of_range);
    SET_ERROR(error_table, PDM_NON_CRITICAL_AUX2_CURRENT_OUT_OF_RANGE, data->aux2_current_out_of_range);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_LEFT_INVERTER_CURRENT_OUT_OF_RANGE, data->left_inverter_current_out_of_range);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE, data->right_inverter_current_out_of_range);
    SET_ERROR(error_table, PDM_NON_CRITICAL_ENERGY_METER_CURRENT_OUT_OF_RANGE, data->energy_meter_current_out_of_range);
    SET_ERROR(error_table, PDM_NON_CRITICAL_CAN_CURRENT_OUT_OF_RANGE, data->can_current_out_of_range);
    SET_ERROR(error_table, PDM_NON_CRITICAL_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE, data->air_shutdown_current_out_of_range);
}

static void
    Io_ProcessBmsAirShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_bms_air_shutdown_errors_t *data)
{
    SET_ERROR(
        error_table, BMS_AIR_SHUTDOWN_CHARGER_DISCONNECTED_IN_CHARGE_STATE, data->charger_disconnected_in_charge_state);
    SET_ERROR(error_table, BMS_AIR_SHUTDOWN_MIN_CELL_VOLTAGE_OUT_OF_RANGE, data->min_cell_voltage_out_of_range);
    SET_ERROR(error_table, BMS_AIR_SHUTDOWN_MAX_CELL_VOLTAGE_OUT_OF_RANGE, data->max_cell_voltage_out_of_range);
    SET_ERROR(error_table, BMS_AIR_SHUTDOWN_CHARGER_FAULT_DETECTED, data->charger_fault_detected);
    SET_ERROR(error_table, BMS_AIR_SHUTDOWN_HAS_PEC_ERROR, data->has_pec_error);
}

static void
    Io_ProcessDcmAirShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_dcm_air_shutdown_errors_t *data)
{
    SET_ERROR(error_table, DCM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, data->dummy_air_shutdown);
}

static void
    Io_ProcessDimAirShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_dim_air_shutdown_errors_t *data)
{
    SET_ERROR(error_table, DIM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, data->dummy_air_shutdown);
}

static void
    Io_ProcessFsmAirShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_fsm_air_shutdown_errors_t *data)
{
    SET_ERROR(error_table, FSM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, data->dummy_air_shutdown);
}

static void
    Io_ProcessPdmAirShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_pdm_air_shutdown_errors_t *data)
{
    SET_ERROR(error_table, PDM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, data->dummy_air_shutdown);
}

static void
    Io_ProcessBmsMotorShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_bms_motor_shutdown_errors_t *data)
{
    SET_ERROR(error_table, BMS_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, data->dummy_motor_shutdown);
}

static void
    Io_ProcessDcmMotorShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_dcm_motor_shutdown_errors_t *data)
{
    SET_ERROR(error_table, DCM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, data->dummy_motor_shutdown);
}

static void
    Io_ProcessDimMotorShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_dim_motor_shutdown_errors_t *data)
{
    SET_ERROR(error_table, DIM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, data->dummy_motor_shutdown);
}

static void
    Io_ProcessFsmMotorShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_fsm_motor_shutdown_errors_t *data)
{
    SET_ERROR(error_table, FSM_MOTOR_SHUTDOWN_APPS_HAS_DISAGREEMENT, data->apps_has_disagreement);
    SET_ERROR(error_table, FSM_MOTOR_SHUTDOWN_PAPPS_ALARM_IS_ACTIVE, data->papps_alarm_is_active);
    SET_ERROR(error_table, FSM_MOTOR_SHUTDOWN_SAPPS_ALARM_IS_ACTIVE, data->sapps_alarm_is_active);
    SET_ERROR(error_table, FSM_MOTOR_SHUTDOWN_PLAUSIBILITY_CHECK_HAS_FAILED, data->plausibility_check_has_failed);
    SET_ERROR(error_table, FSM_MOTOR_SHUTDOWN_PRIMARY_FLOW_RATE_HAS_UNDERFLOW, data->primary_flow_rate_has_underflow);
    SET_ERROR(
        error_table, FSM_MOTOR_SHUTDOWN_SECONDARY_FLOW_RATE_HAS_UNDERFLOW, data->secondary_flow_rate_has_underflow);
}

static void
    Io_ProcessPdmMotorShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_pdm_motor_shutdown_errors_t *data)
{
    SET_ERROR(error_table, PDM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, data->dummy_motor_shutdown);
}

void Io_SharedErrorTable_SetErrorsFromCanMsg(struct ErrorTable *error_table, struct CanMsg *can_msg)
{
    // TODO: Add errors for other boards once I'm sure about other parts of the
    // design
    switch (can_msg->std_id)
    {
        case (CANMSGS_BMS_NON_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_bms_non_critical_errors_t data;
            App_CanMsgs_bms_non_critical_errors_unpack(&data, can_msg->data, CANMSGS_BMS_NON_CRITICAL_ERRORS_LENGTH);
            Io_ProcessBmsNonCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DCM_NON_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_dcm_non_critical_errors_t data;
            App_CanMsgs_dcm_non_critical_errors_unpack(&data, can_msg->data, CANMSGS_DCM_NON_CRITICAL_ERRORS_LENGTH);
            Io_ProcessDcmNonCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DIM_NON_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_dim_non_critical_errors_t data;
            App_CanMsgs_dim_non_critical_errors_unpack(&data, can_msg->data, CANMSGS_DIM_NON_CRITICAL_ERRORS_LENGTH);
            Io_ProcessDimNonCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_FSM_NON_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_fsm_non_critical_errors_t data;
            App_CanMsgs_fsm_non_critical_errors_unpack(&data, can_msg->data, CANMSGS_FSM_NON_CRITICAL_ERRORS_LENGTH);
            Io_ProcessFsmNonCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_PDM_NON_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_pdm_non_critical_errors_t data;
            App_CanMsgs_pdm_non_critical_errors_unpack(&data, can_msg->data, CANMSGS_PDM_NON_CRITICAL_ERRORS_LENGTH);
            Io_ProcessPdmNonCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_FRAME_ID):
        {
            struct CanMsgs_bms_air_shutdown_errors_t data;
            App_CanMsgs_bms_air_shutdown_errors_unpack(&data, can_msg->data, CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_LENGTH);
            Io_ProcessBmsAirShutdownErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DCM_AIR_SHUTDOWN_ERRORS_FRAME_ID):
        {
            struct CanMsgs_dcm_air_shutdown_errors_t data;
            App_CanMsgs_dcm_air_shutdown_errors_unpack(&data, can_msg->data, CANMSGS_DCM_AIR_SHUTDOWN_ERRORS_LENGTH);
            Io_ProcessDcmAirShutdownErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DIM_AIR_SHUTDOWN_ERRORS_FRAME_ID):
        {
            struct CanMsgs_dim_air_shutdown_errors_t data;
            App_CanMsgs_dim_air_shutdown_errors_unpack(&data, can_msg->data, CANMSGS_DIM_AIR_SHUTDOWN_ERRORS_LENGTH);
            Io_ProcessDimAirShutdownErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_FSM_AIR_SHUTDOWN_ERRORS_FRAME_ID):
        {
            struct CanMsgs_fsm_air_shutdown_errors_t data;
            App_CanMsgs_fsm_air_shutdown_errors_unpack(&data, can_msg->data, CANMSGS_FSM_AIR_SHUTDOWN_ERRORS_LENGTH);
            Io_ProcessFsmAirShutdownErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_PDM_AIR_SHUTDOWN_ERRORS_FRAME_ID):
        {
            struct CanMsgs_pdm_air_shutdown_errors_t data;
            App_CanMsgs_pdm_air_shutdown_errors_unpack(&data, can_msg->data, CANMSGS_PDM_AIR_SHUTDOWN_ERRORS_LENGTH);
            Io_ProcessPdmAirShutdownErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_BMS_MOTOR_SHUTDOWN_ERRORS_FRAME_ID):
        {
            struct CanMsgs_bms_motor_shutdown_errors_t data;
            App_CanMsgs_bms_motor_shutdown_errors_unpack(
                &data, can_msg->data, CANMSGS_BMS_MOTOR_SHUTDOWN_ERRORS_LENGTH);
            Io_ProcessBmsMotorShutdownErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DCM_MOTOR_SHUTDOWN_ERRORS_FRAME_ID):
        {
            struct CanMsgs_dcm_motor_shutdown_errors_t data;
            App_CanMsgs_dcm_motor_shutdown_errors_unpack(
                &data, can_msg->data, CANMSGS_DCM_MOTOR_SHUTDOWN_ERRORS_LENGTH);
            Io_ProcessDcmMotorShutdownErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DIM_MOTOR_SHUTDOWN_ERRORS_FRAME_ID):
        {
            struct CanMsgs_dim_motor_shutdown_errors_t data;
            App_CanMsgs_dim_motor_shutdown_errors_unpack(
                &data, can_msg->data, CANMSGS_DIM_MOTOR_SHUTDOWN_ERRORS_LENGTH);
            Io_ProcessDimMotorShutdownErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FRAME_ID):
        {
            struct CanMsgs_fsm_motor_shutdown_errors_t data;
            App_CanMsgs_fsm_motor_shutdown_errors_unpack(
                &data, can_msg->data, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_LENGTH);
            Io_ProcessFsmMotorShutdownErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_PDM_MOTOR_SHUTDOWN_ERRORS_FRAME_ID):
        {
            struct CanMsgs_pdm_motor_shutdown_errors_t data;
            App_CanMsgs_pdm_motor_shutdown_errors_unpack(
                &data, can_msg->data, CANMSGS_PDM_MOTOR_SHUTDOWN_ERRORS_LENGTH);
            Io_ProcessPdmMotorShutdownErrorMsg(error_table, &data);
        }
        break;
    }
}
