/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_canRx.h"

/* ------------------------- Function Definitions ------------------------- */


void app_canAlerts_JCT_Warning_Warning_Test_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_JCT_Warning_Warning_Test_get())
    {
        app_canTx_JCT_Warning_Warning_TestCount_set(app_canTx_JCT_Warning_Warning_TestCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_JCT_Warning_Warning_Test_set(set_alert);
}

bool app_canAlerts_JCT_Warning_Warning_Test_get()
{
    return app_canTx_JCT_Warning_Warning_Test_get();
}

bool app_canAlerts_BoardHasWarning(CanAlertBoard board)
{
    switch (board)
    {
        case FSM_ALERT_BOARD:
        {
            if (app_canRx_FSM_Warning_Warning_Test1_get())
            {
                return true;
            }
            
            if (app_canRx_FSM_Warning_Warning_Test2_get())
            {
                return true;
            }
            
            break;
        }
        case JCT_ALERT_BOARD:
        {
            if (app_canTx_JCT_Warning_Warning_Test_get())
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
        case FSM_ALERT_BOARD:
        {
            if (app_canRx_FSM_Fault_Fault_Test3_get())
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
    if (app_canAlerts_BoardHasWarning(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    if (app_canAlerts_BoardHasWarning(JCT_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

bool app_canAlerts_AnyBoardHasFault()
{
    if (app_canAlerts_BoardHasFault(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

uint8_t app_canAlerts_WarningInfo(Fault_Warning_Info *alert_array)
{
    uint8_t element_num = 0;
    
    if (app_canRx_FSM_Warning_Warning_Test1_get())
    {
        alert_array[element_num].name = "FSM_Warning_Warning_Test1";
        alert_array[element_num].description = "Example";
        alert_array[element_num].id = "1000";
        element_num++;
    }
    
    if (app_canRx_FSM_Warning_Warning_Test2_get())
    {
        alert_array[element_num].name = "FSM_Warning_Warning_Test2";
        alert_array[element_num].description = "Example";
        alert_array[element_num].id = "21000";
        element_num++;
    }
    
    if (app_canTx_JCT_Warning_Warning_Test_get())
    {
        alert_array[element_num].name = "JCT_Warning_Warning_Test";
        alert_array[element_num].description = "Example";
        alert_array[element_num].id = "2000";
        element_num++;
    }
    
    return element_num;
    
}

uint8_t app_canAlerts_FaultInfo(Fault_Warning_Info *alert_array)
{
    uint8_t element_num = 0;
    
    if (app_canRx_FSM_Fault_Fault_Test3_get())
    {
        alert_array[element_num].name = "FSM_Fault_Fault_Test3";
        alert_array[element_num].description = "";
        alert_array[element_num].id = "0";
        element_num++;
    }
    
    return element_num;
    
}

