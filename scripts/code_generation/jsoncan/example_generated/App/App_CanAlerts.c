/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "App_CanRx.h"

/* ------------------------- Function Definitions ------------------------- */

uint8_t *alertarray[62]= {0};
uint8_t pointP = 0;
uint8_t *p = &pointP;

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

void App_CanAlerts_WarningCode(CanAlertBoard board, uint8_t *alertArray, uint8_t elementNum, uint8_t *p)
{
    switch (board)
    {
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_Warning_Warning_Test1_Get())
            {
                alertArray[*p] = FSM_Warning_Warning_Test1Code;
                *p++;
            }
            
            if (App_CanRx_FSM_Warning_Warning_Test2_Get())
            {
                alertArray[*p] = FSM_Warning_Warning_Test2Code;
                *p++;
            }
            
            break;
        }
        case JCT_ALERT_BOARD:
        {
            if (App_CanTx_JCT_Warning_Warning_Test_Get())
            {
                alertArray[*p] = JCT_Warning_Warning_TestCode;
                *p++;
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

void App_CanAlerts_FaultCode(CanAlertBoard board, uint8_t *alertArray, uint8_t elementNum, uint8_t *p)
{
    switch (board)
    {
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_Fault_Fault_Test3_Get())
            {
                alertArray[*p] = FSM_Fault_Fault_Test3Code;
                *p++;
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

