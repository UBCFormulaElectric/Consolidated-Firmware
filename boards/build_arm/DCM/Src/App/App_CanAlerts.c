/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "App_CanRx.h"

/* ------------------------- Function Definitions ------------------------- */

void App_CanAlerts_SetWarning(DCM_WarningId alert_id, bool set_alert)
{
    switch (alert_id)
    {
        case DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ:
        {
            App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Set(set_alert);
            break;
        }
        case DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ:
        {
            App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Set(set_alert);
            break;
        }
        case DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ:
        {
            App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Set(set_alert);
            break;
        }
        case DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX:
        {
            App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Set(set_alert);
            break;
        }
        case DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX:
        {
            App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Set(set_alert);
            break;
        }
        case DCM_WARNING_WATCHDOG_TIMEOUT:
        {
            App_CanTx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Set(set_alert);
            break;
        }
        case DCM_WARNING_TX_OVERFLOW:
        {
            App_CanTx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Set(set_alert);
            break;
        }
        case DCM_WARNING_RX_OVERFLOW:
        {
            App_CanTx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Set(set_alert);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}

void App_CanAlerts_SetFault(DCM_FaultId alert_id, bool set_alert)
{
    switch (alert_id)
    {
        case DCM_FAULT_STATE_FAULT:
        {
            App_CanTx_DCM_Faults_DCM_FAULT_STATE_FAULT_Set(set_alert);
            break;
        }
        case DCM_FAULT_MISSING_HEARTBEAT:
        {
            App_CanTx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Set(set_alert);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}

bool App_CanAlerts_GetWarning(DCM_WarningId alert_id)
{
    switch (alert_id)
    {
        case DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ:
        {
            return App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get();
            break;
        }
        case DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ:
        {
            return App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get();
            break;
        }
        case DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ:
        {
            return App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get();
            break;
        }
        case DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX:
        {
            return App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get();
            break;
        }
        case DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX:
        {
            return App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get();
            break;
        }
        case DCM_WARNING_WATCHDOG_TIMEOUT:
        {
            return App_CanTx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Get();
            break;
        }
        case DCM_WARNING_TX_OVERFLOW:
        {
            return App_CanTx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Get();
            break;
        }
        case DCM_WARNING_RX_OVERFLOW:
        {
            return App_CanTx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Get();
            break;
        }
        default:
        {
            return false;
            break;
        }
    }
}

bool App_CanAlerts_GetFault(DCM_FaultId alert_id)
{
    switch (alert_id)
    {
        case DCM_FAULT_STATE_FAULT:
        {
            return App_CanTx_DCM_Faults_DCM_FAULT_STATE_FAULT_Get();
            break;
        }
        case DCM_FAULT_MISSING_HEARTBEAT:
        {
            return App_CanTx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Get();
            break;
        }
        default:
        {
            return false;
            break;
        }
    }
}

bool App_CanAlerts_BoardHasWarning(CanAlertBoard board)
{
    switch (board)
    {
        case BMS_ALERT_BOARD:
        {
            if (App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warnings_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warnings_BMS_WARNING_WATCHDOG_TIMEOUT_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warnings_BMS_WARNING_TX_OVERFLOW_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warnings_BMS_WARNING_RX_OVERFLOW_Get())
            {
                return true;
            }
            
            break;
        }
        case DCM_ALERT_BOARD:
        {
            if (App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Get())
            {
                return true;
            }
            
            break;
        }
        case DIM_ALERT_BOARD:
        {
            if (App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Get())
            {
                return true;
            }
            
            break;
        }
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_WATCHDOG_TIMEOUT_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_TX_OVERFLOW_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_RX_OVERFLOW_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warnings_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_Get())
            {
                return true;
            }
            
            break;
        }
        case PDM_ALERT_BOARD:
        {
            if (App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warnings_PDM_WARNING_WATCHDOG_TIMEOUT_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warnings_PDM_WARNING_TX_OVERFLOW_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warnings_PDM_WARNING_RX_OVERFLOW_Get())
            {
                return true;
            }
            
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
    
    return false;
}

bool App_CanAlerts_BoardHasFault(CanAlertBoard board)
{
    switch (board)
    {
        case BMS_ALERT_BOARD:
        {
            if (App_CanRx_BMS_Faults_BMS_FAULT_MISSING_HEARTBEAT_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_STATE_FAULT_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERVOLTAGE_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERVOLTAGE_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_MODULE_COMM_ERROR_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_CELL_UNDERTEMP_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_FAULT_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_HAS_REACHED_MAX_V_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_TS_OVERCURRENT_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Faults_BMS_FAULT_PRECHARGE_ERROR_Get())
            {
                return true;
            }
            
            break;
        }
        case DCM_ALERT_BOARD:
        {
            if (App_CanTx_DCM_Faults_DCM_FAULT_STATE_FAULT_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Get())
            {
                return true;
            }
            
            break;
        }
        case DIM_ALERT_BOARD:
        {
            if (App_CanRx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Get())
            {
                return true;
            }
            
            break;
        }
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Faults_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Faults_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Faults_FSM_FAULT_APPS_HAS_DISAGREEMENT_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Faults_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_Get())
            {
                return true;
            }
            
            break;
        }
        case PDM_ALERT_BOARD:
        {
            if (App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Get())
            {
                return true;
            }
            
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
    
    return false;
}

bool App_CanAlerts_AnyBoardHasWarning()
{
    if (App_CanAlerts_BoardHasWarning(BMS_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(DCM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(DIM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(PDM_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

bool App_CanAlerts_AnyBoardHasFault()
{
    if (App_CanAlerts_BoardHasFault(BMS_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(DCM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(DIM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(PDM_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

