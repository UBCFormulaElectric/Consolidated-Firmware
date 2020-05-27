#include "App_SharedErrorTable.h"
#include "App_CanMsgs.h"
#include "Io_SharedErrorTable.h"

#define SET_ERROR(table, id, is_set) \
    App_SharedErrorTable_SetError(table, id, is_set)

static void Io_ProcessBmsNonCriticalErrorMsg(
    struct ErrorTable *                       error_table,
    struct CanMsgs_bms_non_critical_errors_t *data)
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
    SET_ERROR(
        error_table, BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, data->watchdog_timeout);
}

static void Io_ProcessDcmNonCriticalErrorMsg(
    struct ErrorTable *                       error_table,
    struct CanMsgs_dcm_non_critical_errors_t *data)
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
    SET_ERROR(
        error_table, DCM_NON_CRITICAL_WATCHDOG_TIMEOUT, data->watchdog_timeout);
}

static void Io_ProcessDimNonCriticalErrorMsg(
    struct ErrorTable *                       error_table,
    struct CanMsgs_dim_non_critical_errors_t *data)
{
    SET_ERROR(
        error_table, DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
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
    SET_ERROR(
        error_table, DIM_NON_CRITICAL_WATCHDOG_TIMEOUT, data->watchdog_timeout);
}

static void Io_ProcessFsmNonCriticalErrorMsg(
    struct ErrorTable *                       error_table,
    struct CanMsgs_fsm_non_critical_errors_t *data)
{
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_PAPPS_OUT_OF_RANGE,
        data->papps_out_of_range == 0);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_SAPPS_OUT_OF_RANGE,
        data->sapps_out_of_range == 0);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        data->stack_watermark_above_threshold_task1_hz == 0);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        data->stack_watermark_above_threshold_task1_khz == 0);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        data->stack_watermark_above_threshold_taskcanrx == 0);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        data->stack_watermark_above_threshold_taskcantx == 0);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_WATCHDOG_TIMEOUT,
        data->watchdog_timeout == 0);
    SET_ERROR(error_table, FSM_NON_CRITICAL_BSPD_FAULT, data->bspd_fault == 0);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_LEFT_WHEEL_SPEED_OUT_OF_RANGE,
        data->left_wheel_speed_out_of_range == 0);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_RIGHT_WHEEL_SPEED_OUT_OF_RANGE,
        data->right_wheel_speed_out_of_range == 0);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_PRIMARY_FLOW_RATE_OUT_OF_RANGE,
        data->primary_flow_rate_out_of_range == 0);
    SET_ERROR(
        error_table, FSM_NON_CRITICAL_SECONDARY_FLOW_RATE_OUT_OF_RANGE,
        data->secondary_flow_rate_out_of_range == 0);
}

static void Io_ProcessPdmNonCriticalErrorMsg(
    struct ErrorTable *                       error_table,
    struct CanMsgs_pdm_non_critical_errors_t *data)
{
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_MISSING_HEARTBEAT,
        data->missing_heartbeat == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_BOOST_PGOOD_FAULT,
        data->boost_pgood_fault == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_CELL_BALANCE_OVERVOLTAGE_FAULT,
        data->cell_balance_overvoltage_fault == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_CHARGER_FAULT, data->charger_fault == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_EFUSE_FAULT, data->efuse_fault == 0);
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
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_WATCHDOG_TIMEOUT, data->watchdog_timeout);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_VBAT_VOLTAGE_OUT_OF_RANGE,
        data->vbat_voltage_out_of_range == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL__24V_AUX_VOLTAGE_OUT_OF_RANGE,
        data->_24_v_aux_voltage_out_of_range == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL__24V_ACC_VOLTAGE_OUT_OF_RANGE,
        data->_24_v_acc_voltage_out_of_range == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_AUX1_CURRENT_OUT_OF_RANGE,
        data->aux1_current_out_of_range == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_AUX2_CURRENT_OUT_OF_RANGE,
        data->aux2_current_out_of_range == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_LEFT_INVERTER_CURRENT_OUT_OF_RANGE,
        data->left_inverter_current_out_of_range == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE,
        data->right_inverter_current_out_of_range == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_ENERGY_METER_CURRENT_OUT_OF_RANGE,
        data->energy_meter_current_out_of_range == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_CAN_CURRENT_OUT_OF_RANGE,
        data->can_current_out_of_range == 0);
    SET_ERROR(
        error_table, PDM_NON_CRITICAL_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE,
        data->air_shutdown_current_out_of_range == 0);
}

static void Io_ProcessBmsCriticalErrorMsg(
    struct ErrorTable *                   error_table,
    struct CanMsgs_bms_critical_errors_t *data)
{
    SET_ERROR(
        error_table, BMS_CRITICAL_DUMMY,
        data->charger_disconnected_in_charge_state == 0);
}

static void Io_ProcessDcmCriticalErrorMsg(
    struct ErrorTable *                   error_table,
    struct CanMsgs_dcm_critical_errors_t *data)
{
    SET_ERROR(error_table, DCM_CRITICAL_DUMMY, data->dummy_critical_error == 0);
}

static void Io_ProcessDimCriticalErrorMsg(
    struct ErrorTable *                   error_table,
    struct CanMsgs_dim_critical_errors_t *data)
{
    SET_ERROR(error_table, DIM_CRITICAL_DUMMY, data->dummy_critical_error == 0);
}

static void Io_ProcessFsmCriticalErrorMsg(
    struct ErrorTable *                   error_table,
    struct CanMsgs_fsm_critical_errors_t *data)
{
    SET_ERROR(error_table, FSM_CRITICAL_DUMMY, data->dummy_critical_error == 0);
}

static void Io_ProcessPdmCriticalErrorMsg(
    struct ErrorTable *                   error_table,
    struct CanMsgs_pdm_critical_errors_t *data)
{
    SET_ERROR(error_table, PDM_CRITICAL_DUMMY, data->dummy_critical_error == 0);
}

void Io_SharedErrorTable_SetErrorsFromCanMsg(
    struct ErrorTable *error_table,
    struct CanMsg *    can_msg)
{
    // TODO: Add errors for other boards once I'm sure about other parts of the
    // design
    switch (can_msg->std_id)
    {
        case (CANMSGS_BMS_NON_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_bms_non_critical_errors_t data;
            App_CanMsgs_bms_non_critical_errors_unpack(
                &data, can_msg->data, CANMSGS_BMS_NON_CRITICAL_ERRORS_LENGTH);
            Io_ProcessBmsNonCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DCM_NON_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_dcm_non_critical_errors_t data;
            App_CanMsgs_dcm_non_critical_errors_unpack(
                &data, can_msg->data, CANMSGS_DCM_NON_CRITICAL_ERRORS_LENGTH);
            Io_ProcessDcmNonCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DIM_NON_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_dim_non_critical_errors_t data;
            App_CanMsgs_dim_non_critical_errors_unpack(
                &data, can_msg->data, CANMSGS_DIM_NON_CRITICAL_ERRORS_LENGTH);
            Io_ProcessDimNonCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_FSM_NON_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_fsm_non_critical_errors_t data;
            App_CanMsgs_fsm_non_critical_errors_unpack(
                &data, can_msg->data, CANMSGS_FSM_NON_CRITICAL_ERRORS_LENGTH);
            Io_ProcessFsmNonCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_PDM_NON_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_pdm_non_critical_errors_t data;
            App_CanMsgs_pdm_non_critical_errors_unpack(
                &data, can_msg->data, CANMSGS_PDM_NON_CRITICAL_ERRORS_LENGTH);
            Io_ProcessPdmNonCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_BMS_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_bms_critical_errors_t data;
            App_CanMsgs_bms_critical_errors_unpack(
                &data, can_msg->data, CANMSGS_BMS_CRITICAL_ERRORS_LENGTH);
            Io_ProcessBmsCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DCM_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_dcm_critical_errors_t data;
            App_CanMsgs_dcm_critical_errors_unpack(
                &data, can_msg->data, CANMSGS_DCM_CRITICAL_ERRORS_LENGTH);
            Io_ProcessDcmCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_DIM_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_dim_critical_errors_t data;
            App_CanMsgs_dim_critical_errors_unpack(
                &data, can_msg->data, CANMSGS_DIM_CRITICAL_ERRORS_LENGTH);
            Io_ProcessDimCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_FSM_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_fsm_critical_errors_t data;
            App_CanMsgs_fsm_critical_errors_unpack(
                &data, can_msg->data, CANMSGS_FSM_CRITICAL_ERRORS_LENGTH);
            Io_ProcessFsmCriticalErrorMsg(error_table, &data);
        }
        break;
        case (CANMSGS_PDM_CRITICAL_ERRORS_FRAME_ID):
        {
            struct CanMsgs_pdm_critical_errors_t data;
            App_CanMsgs_pdm_critical_errors_unpack(
                &data, can_msg->data, CANMSGS_PDM_CRITICAL_ERRORS_LENGTH);
            Io_ProcessPdmCriticalErrorMsg(error_table, &data);
        }
        break;
    }
}
