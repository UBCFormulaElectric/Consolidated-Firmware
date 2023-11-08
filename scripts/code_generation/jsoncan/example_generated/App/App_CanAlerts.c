/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "App_CanRx.h"

/* ------------------------- Function Definitions ------------------------- */

void App_CanAlerts_JCT_Warning_Warning_Test_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_JCT_Warning_Warning_Test_Get())
    {
        App_CanTx_JCT_Warning_Warning_TestCount_Set(App_CanTx_JCT_Warning_Warning_TestCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_JCT_Warning_Warning_Test_Set(set_alert);
}

bool App_CanAlerts_JCT_Warning_Warning_Test_Get()
{
    return App_CanTx_JCT_Warning_Warning_Test_Get();
}

bool App_CanAlerts_BoardHasWarning(CanAlertBoard board)
{
    switch (board)
    {
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_Warning_Warning_Test1_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_Warning_Test2_Get())
            {
                return true;
            }
            
            break;
        }
        case JCT_ALERT_BOARD:
        {
            if (App_CanTx_JCT_Warning_Warning_Test_Get())
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
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_Fault_Fault_Test3_Get())
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
    if (App_CanAlerts_BoardHasWarning(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(JCT_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

bool App_CanAlerts_AnyBoardHasFault()
{
    if (App_CanAlerts_BoardHasFault(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

