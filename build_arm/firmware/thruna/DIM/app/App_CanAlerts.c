/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "App_CanRx.h"

/* ------------------------- Function Definitions ------------------------- */

void App_CanAlerts_DIM_Warning_StackWaterMarkHighTask1Hz_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DIM_Warning_StackWaterMarkHighTask1Hz_Get())
    {
        App_CanTx_DIM_Warning_StackWaterMarkHighTask1HzCount_Set(App_CanTx_DIM_Warning_StackWaterMarkHighTask1HzCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DIM_Warning_StackWaterMarkHighTask1Hz_Set(set_alert);
}

void App_CanAlerts_DIM_Warning_StackWaterMarkHighTask100Hz_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DIM_Warning_StackWaterMarkHighTask100Hz_Get())
    {
        App_CanTx_DIM_Warning_StackWaterMarkHighTask100HzCount_Set(App_CanTx_DIM_Warning_StackWaterMarkHighTask100HzCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DIM_Warning_StackWaterMarkHighTask100Hz_Set(set_alert);
}

void App_CanAlerts_DIM_Warning_StackWaterMarkHighTask1kHz_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHz_Get())
    {
        App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHzCount_Set(App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHzCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHz_Set(set_alert);
}

void App_CanAlerts_DIM_Warning_StackWaterMarkHighTaskCanRx_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRx_Get())
    {
        App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRxCount_Set(App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRxCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRx_Set(set_alert);
}

void App_CanAlerts_DIM_Warning_StackWaterMarkHighTaskCanTx_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTx_Get())
    {
        App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTxCount_Set(App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTxCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTx_Set(set_alert);
}

void App_CanAlerts_DIM_Warning_WatchdogTimeout_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DIM_Warning_WatchdogTimeout_Get())
    {
        App_CanTx_DIM_Warning_WatchdogTimeoutCount_Set(App_CanTx_DIM_Warning_WatchdogTimeoutCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DIM_Warning_WatchdogTimeout_Set(set_alert);
}

void App_CanAlerts_DIM_Warning_TxOverflow_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DIM_Warning_TxOverflow_Get())
    {
        App_CanTx_DIM_Warning_TxOverflowCount_Set(App_CanTx_DIM_Warning_TxOverflowCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DIM_Warning_TxOverflow_Set(set_alert);
}

void App_CanAlerts_DIM_Warning_RxOverflow_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DIM_Warning_RxOverflow_Get())
    {
        App_CanTx_DIM_Warning_RxOverflowCount_Set(App_CanTx_DIM_Warning_RxOverflowCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DIM_Warning_RxOverflow_Set(set_alert);
}

void App_CanAlerts_DIM_Fault_MissingHeartbeat_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DIM_Fault_MissingHeartbeat_Get())
    {
        App_CanTx_DIM_Fault_MissingHeartbeatCount_Set(App_CanTx_DIM_Fault_MissingHeartbeatCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DIM_Fault_MissingHeartbeat_Set(set_alert);
}

bool App_CanAlerts_DIM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return App_CanTx_DIM_Warning_StackWaterMarkHighTask1Hz_Get();
}

bool App_CanAlerts_DIM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return App_CanTx_DIM_Warning_StackWaterMarkHighTask100Hz_Get();
}

bool App_CanAlerts_DIM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHz_Get();
}

bool App_CanAlerts_DIM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRx_Get();
}

bool App_CanAlerts_DIM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTx_Get();
}

bool App_CanAlerts_DIM_Warning_WatchdogTimeout_Get()
{
    return App_CanTx_DIM_Warning_WatchdogTimeout_Get();
}

bool App_CanAlerts_DIM_Warning_TxOverflow_Get()
{
    return App_CanTx_DIM_Warning_TxOverflow_Get();
}

bool App_CanAlerts_DIM_Warning_RxOverflow_Get()
{
    return App_CanTx_DIM_Warning_RxOverflow_Get();
}

bool App_CanAlerts_DIM_Fault_MissingHeartbeat_Get()
{
    return App_CanTx_DIM_Fault_MissingHeartbeat_Get();
}

bool App_CanAlerts_BoardHasWarning(CanAlertBoard board)
{
    switch (board)
    {
        case DCM_ALERT_BOARD:
        {
            if (App_CanRx_DCM_Warning_StackWaterMarkHighTask1Hz_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_Warning_StackWaterMarkHighTask100Hz_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_Warning_StackWaterMarkHighTask1kHz_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanRx_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanTx_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_Warning_WatchdogTimeout_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_Warning_TxOverflow_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_Warning_RxOverflow_Get())
            {
                return true;
            }
            
            break;
        }
        case DIM_ALERT_BOARD:
        {
            if (App_CanTx_DIM_Warning_StackWaterMarkHighTask1Hz_Get())
            {
                return true;
            }
            
            if (App_CanTx_DIM_Warning_StackWaterMarkHighTask100Hz_Get())
            {
                return true;
            }
            
            if (App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHz_Get())
            {
                return true;
            }
            
            if (App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRx_Get())
            {
                return true;
            }
            
            if (App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTx_Get())
            {
                return true;
            }
            
            if (App_CanTx_DIM_Warning_WatchdogTimeout_Get())
            {
                return true;
            }
            
            if (App_CanTx_DIM_Warning_TxOverflow_Get())
            {
                return true;
            }
            
            if (App_CanTx_DIM_Warning_RxOverflow_Get())
            {
                return true;
            }
            
            break;
        }
        case BMS_ALERT_BOARD:
        {
            if (App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warning_WatchdogTimeout_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warning_TxOverflow_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Warning_RxOverflow_Get())
            {
                return true;
            }
            
            break;
        }
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_Warning_StackWaterMarkHighTask1Hz_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_StackWaterMarkHighTask100Hz_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_StackWaterMarkHighTask1kHz_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanRx_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanTx_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_WatchdogTimeout_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_TxOverflow_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_RxOverflow_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_LeftWheelSpeedOutOfRange_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_RightWheelSpeedOutOfRange_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_SteeringAngleOCSC_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_SteeringAngleOutOfRange_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_FrontBrakePressureOutOfRange_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_FlowRateOutOfRange_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_BrakeAppsDisagreement_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Warning_AppsDisagreement_Get())
            {
                return true;
            }
            
            break;
        }
        case PDM_ALERT_BOARD:
        {
            if (App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warning_WatchdogTimeout_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warning_TxOverflow_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_Warning_RxOverflow_Get())
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
        case DCM_ALERT_BOARD:
        {
            if (App_CanRx_DCM_Fault_MissingHeartbeat_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_Fault_LeftInverterFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_Fault_RightInverterFault_Get())
            {
                return true;
            }
            
            break;
        }
        case DIM_ALERT_BOARD:
        {
            if (App_CanTx_DIM_Fault_MissingHeartbeat_Get())
            {
                return true;
            }
            
            break;
        }
        case BMS_ALERT_BOARD:
        {
            if (App_CanRx_BMS_Fault_MissingHeartbeat_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_StateMachine_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_CellUndervoltage_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_CellOvervoltage_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_ModuleCommunicationError_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_CellUndertemp_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_CellOvertemp_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_Charger_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_ChargerExternalShutdown_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_Fault_PrechargeFailure_Get())
            {
                return true;
            }
            
            break;
        }
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_Fault_MissingHeartbeat_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Fault_StateMachine_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Fault_PappsOCSC_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Fault_SappsOCSCFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Fault_FlowMeterUnderflow_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_Fault_TorquePlausabilityFailed_Get())
            {
                return true;
            }
            
            break;
        }
        case PDM_ALERT_BOARD:
        {
            if (App_CanRx_PDM_Fault_DummyFault_Get())
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
    if (App_CanAlerts_BoardHasWarning(DCM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(DIM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(BMS_ALERT_BOARD))
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
    if (App_CanAlerts_BoardHasFault(DCM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(DIM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(BMS_ALERT_BOARD))
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

