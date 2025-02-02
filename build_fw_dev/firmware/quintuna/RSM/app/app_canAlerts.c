/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_canRx.h"

/* ------------------------- Function Definitions ------------------------- */


void app_canAlerts_RSM_Fault_DummyFault_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_RSM_Fault_DummyFault_get())
    {
        app_canTx_RSM_Fault_DummyFaultCount_set(app_canTx_RSM_Fault_DummyFaultCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_RSM_Fault_DummyFault_set(set_alert);
}

bool app_canAlerts_RSM_Fault_DummyFault_get()
{
    return app_canTx_RSM_Fault_DummyFault_get();
}

bool app_canAlerts_BoardHasWarning(CanAlertBoard board)
{
    switch (board)
    {
        case CRIT_ALERT_BOARD:
        {
            if (app_canRx_CRIT_Warning_StackWaterMarkHighTask1Hz_get())
            {
                return true;
            }
            
            if (app_canRx_CRIT_Warning_StackWaterMarkHighTask100Hz_get())
            {
                return true;
            }
            
            if (app_canRx_CRIT_Warning_StackWaterMarkHighTask1kHz_get())
            {
                return true;
            }
            
            if (app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanRx_get())
            {
                return true;
            }
            
            if (app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanTx_get())
            {
                return true;
            }
            
            if (app_canRx_CRIT_Warning_WatchdogTimeout_get())
            {
                return true;
            }
            
            if (app_canRx_CRIT_Warning_TxOverflow_get())
            {
                return true;
            }
            
            if (app_canRx_CRIT_Warning_RxOverflow_get())
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

bool app_canAlerts_BoardHasFault(CanAlertBoard board)
{
    switch (board)
    {
        case BMS_ALERT_BOARD:
        {
            if (app_canRx_BMS_Fault_DummyFault_get())
            {
                return true;
            }
            
            break;
        }
        case FSM_ALERT_BOARD:
        {
            if (app_canRx_FSM_Fault_DummyFault_get())
            {
                return true;
            }
            
            break;
        }
        case CRIT_ALERT_BOARD:
        {
            if (app_canRx_CRIT_Fault_MissingBMSHeartbeat_get())
            {
                return true;
            }
            
            if (app_canRx_CRIT_Fault_MissingFSMHeartbeat_get())
            {
                return true;
            }
            
            if (app_canRx_CRIT_Fault_MissingVCHeartbeat_get())
            {
                return true;
            }
            
            if (app_canRx_CRIT_Fault_MissingRSMHeartbeat_get())
            {
                return true;
            }
            
            break;
        }
        case VC_ALERT_BOARD:
        {
            if (app_canRx_VC_Fault_DummyFault_get())
            {
                return true;
            }
            
            break;
        }
        case RSM_ALERT_BOARD:
        {
            if (app_canTx_RSM_Fault_DummyFault_get())
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

bool app_canAlerts_AnyBoardHasWarning()
{
    if (app_canAlerts_BoardHasWarning(CRIT_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

bool app_canAlerts_AnyBoardHasFault()
{
    if (app_canAlerts_BoardHasFault(BMS_ALERT_BOARD))
    {
        return true;
    }
    
    if (app_canAlerts_BoardHasFault(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    if (app_canAlerts_BoardHasFault(CRIT_ALERT_BOARD))
    {
        return true;
    }
    
    if (app_canAlerts_BoardHasFault(VC_ALERT_BOARD))
    {
        return true;
    }
    
    if (app_canAlerts_BoardHasFault(RSM_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

uint8_t app_canAlerts_WarningInfo(Fault_Warning_Info *alert_array)
{
    uint8_t element_num = 0;
    
    if (app_canRx_CRIT_Warning_StackWaterMarkHighTask1Hz_get())
    {
        alert_array[element_num].name = "CRIT_Warning_StackWaterMarkHighTask1Hz";
        alert_array[element_num].description = "1Hz task stack watermark exceeded.";
        alert_array[element_num].id = 801;
        element_num++;
    }
    
    if (app_canRx_CRIT_Warning_StackWaterMarkHighTask100Hz_get())
    {
        alert_array[element_num].name = "CRIT_Warning_StackWaterMarkHighTask100Hz";
        alert_array[element_num].description = "100Hz task stack watermark exceeded.";
        alert_array[element_num].id = 802;
        element_num++;
    }
    
    if (app_canRx_CRIT_Warning_StackWaterMarkHighTask1kHz_get())
    {
        alert_array[element_num].name = "CRIT_Warning_StackWaterMarkHighTask1kHz";
        alert_array[element_num].description = "1kHz task stack watermark exceeded.";
        alert_array[element_num].id = 803;
        element_num++;
    }
    
    if (app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanRx_get())
    {
        alert_array[element_num].name = "CRIT_Warning_StackWaterMarkHighTaskCanRx";
        alert_array[element_num].description = "CAN RX task stack watermark exceeded.";
        alert_array[element_num].id = 804;
        element_num++;
    }
    
    if (app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanTx_get())
    {
        alert_array[element_num].name = "CRIT_Warning_StackWaterMarkHighTaskCanTx";
        alert_array[element_num].description = "CAN TX task stack watermark exceeded.";
        alert_array[element_num].id = 805;
        element_num++;
    }
    
    if (app_canRx_CRIT_Warning_WatchdogTimeout_get())
    {
        alert_array[element_num].name = "CRIT_Warning_WatchdogTimeout";
        alert_array[element_num].description = "Watchdog timeout detected.";
        alert_array[element_num].id = 806;
        element_num++;
    }
    
    if (app_canRx_CRIT_Warning_TxOverflow_get())
    {
        alert_array[element_num].name = "CRIT_Warning_TxOverflow";
        alert_array[element_num].description = "CAN TX queue has overflowed.";
        alert_array[element_num].id = 807;
        element_num++;
    }
    
    if (app_canRx_CRIT_Warning_RxOverflow_get())
    {
        alert_array[element_num].name = "CRIT_Warning_RxOverflow";
        alert_array[element_num].description = "CAN RX queue has overflowed.";
        alert_array[element_num].id = 808;
        element_num++;
    }
    
    return element_num;
    
}

uint8_t app_canAlerts_FaultInfo(Fault_Warning_Info *alert_array)
{
    uint8_t element_num = 0;
    
    if (app_canRx_BMS_Fault_DummyFault_get())
    {
        alert_array[element_num].name = "BMS_Fault_DummyFault";
        alert_array[element_num].description = "Dummy Fault";
        alert_array[element_num].id = 450;
        element_num++;
    }
    
    if (app_canRx_FSM_Fault_DummyFault_get())
    {
        alert_array[element_num].name = "FSM_Fault_DummyFault";
        alert_array[element_num].description = "Dummy Fault";
        alert_array[element_num].id = 500;
        element_num++;
    }
    
    if (app_canRx_CRIT_Fault_MissingBMSHeartbeat_get())
    {
        alert_array[element_num].name = "CRIT_Fault_MissingBMSHeartbeat";
        alert_array[element_num].description = "Missing heartbeat signal from the BMS.";
        alert_array[element_num].id = 850;
        element_num++;
    }
    
    if (app_canRx_CRIT_Fault_MissingFSMHeartbeat_get())
    {
        alert_array[element_num].name = "CRIT_Fault_MissingFSMHeartbeat";
        alert_array[element_num].description = "Missing heartbeat signal from the FSM.";
        alert_array[element_num].id = 851;
        element_num++;
    }
    
    if (app_canRx_CRIT_Fault_MissingVCHeartbeat_get())
    {
        alert_array[element_num].name = "CRIT_Fault_MissingVCHeartbeat";
        alert_array[element_num].description = "Missing heartbeat signal from the VC.";
        alert_array[element_num].id = 853;
        element_num++;
    }
    
    if (app_canRx_CRIT_Fault_MissingRSMHeartbeat_get())
    {
        alert_array[element_num].name = "CRIT_Fault_MissingRSMHeartbeat";
        alert_array[element_num].description = "Missing heartbeat signal from the RSM.";
        alert_array[element_num].id = 852;
        element_num++;
    }
    
    if (app_canRx_VC_Fault_DummyFault_get())
    {
        alert_array[element_num].name = "VC_Fault_DummyFault";
        alert_array[element_num].description = "Dummy Fault";
        alert_array[element_num].id = 600;
        element_num++;
    }
    
    if (app_canTx_RSM_Fault_DummyFault_get())
    {
        alert_array[element_num].name = "RSM_Fault_DummyFault";
        alert_array[element_num].description = "Dummy Fault";
        alert_array[element_num].id = 700;
        element_num++;
    }
    
    return element_num;
    
}

