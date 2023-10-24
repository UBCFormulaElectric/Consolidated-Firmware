/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "App_CanRx.h"

/* ------------------------- Function Definitions ------------------------- */

void App_CanAlerts_SetWarning(JCT_WarningId alert_id, bool set_alert)
{
    switch (alert_id)
    {
        case JCT_TEST:
        {
            App_CanTx_JCT_Warnings_JCT_TEST_Set(set_alert);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}

void App_CanAlerts_SetFault(JCT_FaultId alert_id, bool set_alert)
{
    switch (alert_id)
    {
        default:
        {
            // Do nothing
            break;
        }
    }
}

bool App_CanAlerts_GetWarning(JCT_WarningId alert_id)
{
    switch (alert_id)
    {
        case JCT_TEST:
        {
            return App_CanTx_JCT_Warnings_JCT_TEST_Get();
            break;
        }
        default:
        {
            return false;
            break;
        }
    }
}

bool App_CanAlerts_GetFault(JCT_FaultId alert_id)
{
    switch (alert_id)
    {
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
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_TEST1_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_TEST2_Get())
            {
                return true;
            }
            
            break;
        }
        case JCT_ALERT_BOARD:
        {
            if (App_CanTx_JCT_Warnings_JCT_TEST_Get())
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
            if (App_CanRx_FSM_FAULT_TEST3_Get())
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

