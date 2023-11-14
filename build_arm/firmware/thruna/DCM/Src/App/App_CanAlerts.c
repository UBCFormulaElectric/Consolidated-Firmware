/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "App_CanRx.h"

/* ------------------------- Function Definitions ------------------------- */

void App_CanAlerts_DCM_StackWaterMarkHighTask1HzWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_StackWaterMarkHighTask1HzWarning_Get())
    {
        App_CanTx_DCM_StackWaterMarkHighTask1HzWarningCount_Set(App_CanTx_DCM_StackWaterMarkHighTask1HzWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_StackWaterMarkHighTask1HzWarning_Set(set_alert);
}

void App_CanAlerts_DCM_StackWaterMarkHighTask100HzWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_StackWaterMarkHighTask100HzWarning_Get())
    {
        App_CanTx_DCM_StackWaterMarkHighTask100HzWarningCount_Set(App_CanTx_DCM_StackWaterMarkHighTask100HzWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_StackWaterMarkHighTask100HzWarning_Set(set_alert);
}

void App_CanAlerts_DCM_StackWaterMarkHighTask1kHzWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_StackWaterMarkHighTask1kHzWarning_Get())
    {
        App_CanTx_DCM_StackWaterMarkHighTask1kHzWarningCount_Set(App_CanTx_DCM_StackWaterMarkHighTask1kHzWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_StackWaterMarkHighTask1kHzWarning_Set(set_alert);
}

void App_CanAlerts_DCM_StackWaterMarkHighTaskCanRxWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_StackWaterMarkHighTaskCanRxWarning_Get())
    {
        App_CanTx_DCM_StackWaterMarkHighTaskCanRxWarningCount_Set(App_CanTx_DCM_StackWaterMarkHighTaskCanRxWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_StackWaterMarkHighTaskCanRxWarning_Set(set_alert);
}

void App_CanAlerts_DCM_StackWaterMarkHighTaskCanTxWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_StackWaterMarkHighTaskCanTxWarning_Get())
    {
        App_CanTx_DCM_StackWaterMarkHighTaskCanTxWarningCount_Set(App_CanTx_DCM_StackWaterMarkHighTaskCanTxWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_StackWaterMarkHighTaskCanTxWarning_Set(set_alert);
}

void App_CanAlerts_DCM_WatchdogTimeoutWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_WatchdogTimeoutWarning_Get())
    {
        App_CanTx_DCM_WatchdogTimeoutWarningCount_Set(App_CanTx_DCM_WatchdogTimeoutWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_WatchdogTimeoutWarning_Set(set_alert);
}

void App_CanAlerts_DCM_TxOverflowWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_TxOverflowWarning_Get())
    {
        App_CanTx_DCM_TxOverflowWarningCount_Set(App_CanTx_DCM_TxOverflowWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_TxOverflowWarning_Set(set_alert);
}

void App_CanAlerts_DCM_RxOverflowWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_RxOverflowWarning_Get())
    {
        App_CanTx_DCM_RxOverflowWarningCount_Set(App_CanTx_DCM_RxOverflowWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_RxOverflowWarning_Set(set_alert);
}

void App_CanAlerts_DCM_MissingHeartbeatFault_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_MissingHeartbeatFault_Get())
    {
        App_CanTx_DCM_MissingHeartbeatFaultCount_Set(App_CanTx_DCM_MissingHeartbeatFaultCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_MissingHeartbeatFault_Set(set_alert);
}

void App_CanAlerts_DCM_LeftInverterFault_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_LeftInverterFault_Get())
    {
        App_CanTx_DCM_LeftInverterFaultCount_Set(App_CanTx_DCM_LeftInverterFaultCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_LeftInverterFault_Set(set_alert);
}

void App_CanAlerts_DCM_RightInverterFault_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_DCM_RightInverterFault_Get())
    {
        App_CanTx_DCM_RightInverterFaultCount_Set(App_CanTx_DCM_RightInverterFaultCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_DCM_RightInverterFault_Set(set_alert);
}

bool App_CanAlerts_DCM_StackWaterMarkHighTask1HzWarning_Get()
{
    return App_CanTx_DCM_StackWaterMarkHighTask1HzWarning_Get();
}

bool App_CanAlerts_DCM_StackWaterMarkHighTask100HzWarning_Get()
{
    return App_CanTx_DCM_StackWaterMarkHighTask100HzWarning_Get();
}

bool App_CanAlerts_DCM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return App_CanTx_DCM_StackWaterMarkHighTask1kHzWarning_Get();
}

bool App_CanAlerts_DCM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return App_CanTx_DCM_StackWaterMarkHighTaskCanRxWarning_Get();
}

bool App_CanAlerts_DCM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return App_CanTx_DCM_StackWaterMarkHighTaskCanTxWarning_Get();
}

bool App_CanAlerts_DCM_WatchdogTimeoutWarning_Get()
{
    return App_CanTx_DCM_WatchdogTimeoutWarning_Get();
}

bool App_CanAlerts_DCM_TxOverflowWarning_Get()
{
    return App_CanTx_DCM_TxOverflowWarning_Get();
}

bool App_CanAlerts_DCM_RxOverflowWarning_Get()
{
    return App_CanTx_DCM_RxOverflowWarning_Get();
}

bool App_CanAlerts_DCM_MissingHeartbeatFault_Get()
{
    return App_CanTx_DCM_MissingHeartbeatFault_Get();
}

bool App_CanAlerts_DCM_LeftInverterFault_Get()
{
    return App_CanTx_DCM_LeftInverterFault_Get();
}

bool App_CanAlerts_DCM_RightInverterFault_Get()
{
    return App_CanTx_DCM_RightInverterFault_Get();
}

bool App_CanAlerts_BoardHasWarning(CanAlertBoard board)
{
    switch (board)
    {
        case DIM_ALERT_BOARD:
        {
            if (App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_WatchdogTimeoutWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_TxOverflowWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DIM_RxOverflowWarning_Get())
            {
                return true;
            }
            
            break;
        }
        case DCM_ALERT_BOARD:
        {
            if (App_CanTx_DCM_StackWaterMarkHighTask1HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_StackWaterMarkHighTask100HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_StackWaterMarkHighTask1kHzWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_StackWaterMarkHighTaskCanRxWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_StackWaterMarkHighTaskCanTxWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_WatchdogTimeoutWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_TxOverflowWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_RxOverflowWarning_Get())
            {
                return true;
            }
            
            break;
        }
        case BMS_ALERT_BOARD:
        {
            if (App_CanRx_BMS_StackWaterMarkHighTask1HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_StackWaterMarkHighTask100HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_StackWaterMarkHighTask1kHzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_StackWaterMarkHighTaskCanRxWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_StackWaterMarkHighTaskCanTxWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_WatchdogTimeoutWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_TxOverflowWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_RxOverflowWarning_Get())
            {
                return true;
            }
            
            break;
        }
        case PDM_ALERT_BOARD:
        {
            if (App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_WatchdogTimeoutWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_TxOverflowWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_PDM_RxOverflowWarning_Get())
            {
                return true;
            }
            
            break;
        }
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_StackWaterMarkHighTask1HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_StackWaterMarkHighTask100HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_StackWaterMarkHighTask1kHzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_StackWaterMarkHighTaskCanRxWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_StackWaterMarkHighTaskCanTxWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_WatchdogTimeoutWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_TxOverflowWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_RxOverflowWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_LeftWheelSpeedOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_RightWheelSpeedOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_SteeringAngleOCSCWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_SteeringAngleOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_FrontBrakePressureOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_RearBrakePressureOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_FlowRateOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_BrakeAppsDisagreementWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_AppsDisagreementWarning_Get())
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
        case DIM_ALERT_BOARD:
        {
            if (App_CanRx_DIM_MissingHeartbeatFault_Get())
            {
                return true;
            }
            
            break;
        }
        case DCM_ALERT_BOARD:
        {
            if (App_CanTx_DCM_MissingHeartbeatFault_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_LeftInverterFault_Get())
            {
                return true;
            }
            
            if (App_CanTx_DCM_RightInverterFault_Get())
            {
                return true;
            }
            
            break;
        }
        case BMS_ALERT_BOARD:
        {
            if (App_CanRx_BMS_MissingHeartbeatFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_StateMachineFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_CellUndervoltageFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_CellOvervoltageFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_ModuleCommunicationFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_CellUndertempFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_CellOvertempFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_ChargerFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_ChargerDisconnectedDuringChargeFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_ChargerExternalShutdownFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_TractiveSystemOvercurrentFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_BMS_PrechargeFailureFault_Get())
            {
                return true;
            }
            
            break;
        }
        case PDM_ALERT_BOARD:
        {
            if (App_CanRx_PDM_DummyFault_Get())
            {
                return true;
            }
            
            break;
        }
        case FSM_ALERT_BOARD:
        {
            if (App_CanRx_FSM_MissingHeartbeatFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_StateMachineFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_PappsOCSCFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_SappsOCSCFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_FlowMeterUnderflowFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_FSM_TorquePlausabilityFailedFault_Get())
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
    if (App_CanAlerts_BoardHasWarning(DIM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(DCM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(BMS_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(PDM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

bool App_CanAlerts_AnyBoardHasFault()
{
    if (App_CanAlerts_BoardHasFault(DIM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(DCM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(BMS_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(PDM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

