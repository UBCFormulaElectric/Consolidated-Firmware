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

uint8_t App_CanAlerts_WarningCode(Fault_Warning_Info *alert_array)
{
    uint8_t element_num = 0;
    
    if (App_CanRx_FSM_Warning_Warning_Test1_Get())
    {
        alert_array[element_num].name = "FSM_Warning_Warning_Test1";
        alert_array[element_num].id = 2059;
        alert_array[element_num].description = "Example";
        element_num++;
    }
    
    if (App_CanRx_FSM_Warning_Warning_Test2_Get())
    {
        alert_array[element_num].name = "FSM_Warning_Warning_Test2";
        alert_array[element_num].id = 2069;
        alert_array[element_num].description = "Example";
        element_num++;
    }
    
    if (App_CanTx_JCT_Warning_Warning_Test_Get())
    {
        alert_array[element_num].name = "JCT_Warning_Warning_Test";
        alert_array[element_num].id = 2059;
        alert_array[element_num].description = "Example";
        element_num++;
    }
    
    return element_num;
    
}

uint8_t App_CanAlerts_FaultCode(Fault_Warning_Info *alert_array)
{
    uint8_t element_num = 0;
    
    if (App_CanRx_FSM_Fault_Fault_Test3_Get())
    {
        alert_array[element_num].name = "FSM_Fault_Fault_Test3";
        alert_array[element_num].id = 3000;
        alert_array[element_num].description = "Example";
        element_num++;
    }
    
    return element_num;
    
}

