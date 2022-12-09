#include "App_SharedErrorTable.h"
#include "App_CanMsgs.h"
#include "Io_SharedErrorTable.h"

#define SET_ERROR(table, id, is_set) App_SharedErrorTable_SetError(table, id, is_set)

static void Io_ProcessDcmWarningMsg(struct ErrorTable *error_table, struct CanMsgs_dcm_warnings_t *data)
{
    SET_ERROR(
        error_table, DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        data->stack_watermark_above_threshold_task1_khz);
    SET_ERROR(
        error_table, DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        data->stack_watermark_above_threshold_taskcanrx);
    SET_ERROR(
        error_table, DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        data->stack_watermark_above_threshold_taskcantx);
    SET_ERROR(error_table, DCM_WARNING_WATCHDOG_TIMEOUT, data->watchdog_timeout);
    SET_ERROR(error_table, DCM_WARNING_ACCELERATION_X_OUT_OF_RANGE, data->acceleration_x_out_of_range);
    SET_ERROR(error_table, DCM_WARNING_ACCELERATION_Y_OUT_OF_RANGE, data->acceleration_y_out_of_range);
    SET_ERROR(error_table, DCM_WARNING_ACCELERATION_Z_OUT_OF_RANGE, data->acceleration_z_out_of_range);
}

static void Io_ProcessDimWarningMsg(struct ErrorTable *error_table, struct CanMsgs_dim_warnings_t *data)
{
    SET_ERROR(
        error_table, DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK100HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        data->stack_watermark_above_threshold_task1_khz);
    SET_ERROR(
        error_table, DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        data->stack_watermark_above_threshold_taskcanrx);
    SET_ERROR(
        error_table, DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        data->stack_watermark_above_threshold_taskcantx);
    SET_ERROR(error_table, DIM_WARNING_WATCHDOG_TIMEOUT, data->watchdog_timeout);
}

static void Io_ProcessFsmWarningMsg(struct ErrorTable *error_table, struct CanMsgs_fsm_warnings_t *data)
{
    SET_ERROR(error_table, FSM_WARNING_PAPPS_OUT_OF_RANGE, data->papps_out_of_range);
    SET_ERROR(error_table, FSM_WARNING_SAPPS_OUT_OF_RANGE, data->sapps_out_of_range);
    SET_ERROR(
        error_table, FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        data->stack_watermark_above_threshold_task1_khz);
    SET_ERROR(
        error_table, FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        data->stack_watermark_above_threshold_taskcanrx);
    SET_ERROR(
        error_table, FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        data->stack_watermark_above_threshold_taskcantx);
    SET_ERROR(error_table, FSM_WARNING_WATCHDOG_TIMEOUT, data->watchdog_timeout);
    SET_ERROR(error_table, FSM_WARNING_BSPD_FAULT, data->bspd_fault);
    SET_ERROR(error_table, FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE, data->left_wheel_speed_out_of_range);
    SET_ERROR(error_table, FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE, data->right_wheel_speed_out_of_range);
    SET_ERROR(error_table, FSM_WARNING_FLOW_RATE_OUT_OF_RANGE, data->flow_rate_out_of_range);
    SET_ERROR(error_table, FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE, data->steering_angle_out_of_range);
    SET_ERROR(error_table, FSM_WARNING_BRAKE_PRESSURE_OUT_OF_RANGE, data->brake_pressure_out_of_range);
    SET_ERROR(error_table, FSM_WARNING_BRAKE_PRESSURE_OPEN_OC, data->brake_pressure_open_oc);
    SET_ERROR(error_table, FSM_WARNING_BRAKE_PRESSURE_OPEN_SC, data->brake_pressure_open_sc);
}

static void Io_ProcessPdmWarningMsg(struct ErrorTable *error_table, struct CanMsgs_pdm_warnings_t *data)
{
    SET_ERROR(
        error_table, PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        data->stack_watermark_above_threshold_task1_hz);
    SET_ERROR(
        error_table, PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        data->stack_watermark_above_threshold_task1_khz);
    SET_ERROR(
        error_table, PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        data->stack_watermark_above_threshold_taskcanrx);
    SET_ERROR(
        error_table, PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        data->stack_watermark_above_threshold_taskcantx);
    SET_ERROR(error_table, PDM_WARNING_WATCHDOG_TIMEOUT, data->watchdog_timeout);
}

static void
    Io_ProcessDcmAirShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_dcm_air_shutdown_errors_t *data)
{
    SET_ERROR(error_table, DCM_AIR_SHUTDOWN_MISSING_HEARTBEAT, data->missing_heartbeat);
}

static void
    Io_ProcessDimAirShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_dim_air_shutdown_errors_t *data)
{
    SET_ERROR(error_table, DIM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, data->dummy_air_shutdown);
}

static void
    Io_ProcessFsmAirShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_fsm_air_shutdown_errors_t *data)
{
    SET_ERROR(error_table, FSM_AIR_SHUTDOWN_MISSING_HEARTBEAT, data->missing_heartbeat);
}

static void
    Io_ProcessPdmAirShutdownErrorMsg(struct ErrorTable *error_table, struct CanMsgs_pdm_air_shutdown_errors_t *data)
{
    SET_ERROR(error_table, PDM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, data->dummy_air_shutdown);
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
    SET_ERROR(error_table, FSM_MOTOR_SHUTDOWN_FLOW_METER_HAS_UNDERFLOW, data->flow_meter_has_underflow);
    SET_ERROR(error_table, FSM_MOTOR_SHUTDOWN_TORQUE_PLAUSIBILITY_CHECK_FAILED, data->torque_plausibility_check_failed);
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
        case (CANMSGS_DCM_WARNINGS_FRAME_ID):
        {
            struct CanMsgs_dcm_warnings_t data;
            App_CanMsgs_dcm_warnings_unpack(&data, can_msg->data, CANMSGS_DCM_WARNINGS_LENGTH);
            Io_ProcessDcmWarningMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DIM_WARNINGS_FRAME_ID):
        {
            struct CanMsgs_dim_warnings_t data;
            App_CanMsgs_dim_warnings_unpack(&data, can_msg->data, CANMSGS_DIM_WARNINGS_LENGTH);
            Io_ProcessDimWarningMsg(error_table, &data);
        }
        break;
        case (CANMSGS_FSM_WARNINGS_FRAME_ID):
        {
            struct CanMsgs_fsm_warnings_t data;
            App_CanMsgs_fsm_warnings_unpack(&data, can_msg->data, CANMSGS_FSM_WARNINGS_LENGTH);
            Io_ProcessFsmWarningMsg(error_table, &data);
        }
        break;
        case (CANMSGS_PDM_WARNINGS_FRAME_ID):
        {
            struct CanMsgs_pdm_warnings_t data;
            App_CanMsgs_pdm_warnings_unpack(&data, can_msg->data, CANMSGS_PDM_WARNINGS_LENGTH);
            Io_ProcessPdmWarningMsg(error_table, &data);
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
