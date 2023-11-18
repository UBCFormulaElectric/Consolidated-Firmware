/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "App_CanRx.h"

/* ------------------------- Function Definitions ------------------------- */

void App_CanAlerts_FSM_StackWaterMarkHighTask1HzWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_StackWaterMarkHighTask1HzWarning_Get())
    {
        App_CanTx_FSM_StackWaterMarkHighTask1HzWarningCount_Set(App_CanTx_FSM_StackWaterMarkHighTask1HzWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_StackWaterMarkHighTask1HzWarning_Set(set_alert);
}

void App_CanAlerts_FSM_StackWaterMarkHighTask100HzWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_StackWaterMarkHighTask100HzWarning_Get())
    {
        App_CanTx_FSM_StackWaterMarkHighTask100HzWarningCount_Set(App_CanTx_FSM_StackWaterMarkHighTask100HzWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_StackWaterMarkHighTask100HzWarning_Set(set_alert);
}

void App_CanAlerts_FSM_StackWaterMarkHighTask1kHzWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_StackWaterMarkHighTask1kHzWarning_Get())
    {
        App_CanTx_FSM_StackWaterMarkHighTask1kHzWarningCount_Set(App_CanTx_FSM_StackWaterMarkHighTask1kHzWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_StackWaterMarkHighTask1kHzWarning_Set(set_alert);
}

void App_CanAlerts_FSM_StackWaterMarkHighTaskCanRxWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarning_Get())
    {
        App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarningCount_Set(App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarning_Set(set_alert);
}

void App_CanAlerts_FSM_StackWaterMarkHighTaskCanTxWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarning_Get())
    {
        App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarningCount_Set(App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarning_Set(set_alert);
}

void App_CanAlerts_FSM_WatchdogTimeoutWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_WatchdogTimeoutWarning_Get())
    {
        App_CanTx_FSM_WatchdogTimeoutWarningCount_Set(App_CanTx_FSM_WatchdogTimeoutWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_WatchdogTimeoutWarning_Set(set_alert);
}

void App_CanAlerts_FSM_TxOverflowWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_TxOverflowWarning_Get())
    {
        App_CanTx_FSM_TxOverflowWarningCount_Set(App_CanTx_FSM_TxOverflowWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_TxOverflowWarning_Set(set_alert);
}

void App_CanAlerts_FSM_RxOverflowWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_RxOverflowWarning_Get())
    {
        App_CanTx_FSM_RxOverflowWarningCount_Set(App_CanTx_FSM_RxOverflowWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_RxOverflowWarning_Set(set_alert);
}

void App_CanAlerts_FSM_LeftWheelSpeedOutOfRangeWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarning_Get())
    {
        App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarningCount_Set(App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarning_Set(set_alert);
}

void App_CanAlerts_FSM_RightWheelSpeedOutOfRangeWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_RightWheelSpeedOutOfRangeWarning_Get())
    {
        App_CanTx_FSM_RightWheelSpeedOutOfRangeWarningCount_Set(App_CanTx_FSM_RightWheelSpeedOutOfRangeWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_RightWheelSpeedOutOfRangeWarning_Set(set_alert);
}

void App_CanAlerts_FSM_SteeringAngleOCSCWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_SteeringAngleOCSCWarning_Get())
    {
        App_CanTx_FSM_SteeringAngleOCSCWarningCount_Set(App_CanTx_FSM_SteeringAngleOCSCWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_SteeringAngleOCSCWarning_Set(set_alert);
}

void App_CanAlerts_FSM_SteeringAngleOutOfRangeWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_SteeringAngleOutOfRangeWarning_Get())
    {
        App_CanTx_FSM_SteeringAngleOutOfRangeWarningCount_Set(App_CanTx_FSM_SteeringAngleOutOfRangeWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_SteeringAngleOutOfRangeWarning_Set(set_alert);
}

void App_CanAlerts_FSM_FrontBrakePressureOutOfRangeWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_FrontBrakePressureOutOfRangeWarning_Get())
    {
        App_CanTx_FSM_FrontBrakePressureOutOfRangeWarningCount_Set(App_CanTx_FSM_FrontBrakePressureOutOfRangeWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_FrontBrakePressureOutOfRangeWarning_Set(set_alert);
}

void App_CanAlerts_FSM_RearBrakePressureOutOfRangeWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_RearBrakePressureOutOfRangeWarning_Get())
    {
        App_CanTx_FSM_RearBrakePressureOutOfRangeWarningCount_Set(App_CanTx_FSM_RearBrakePressureOutOfRangeWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_RearBrakePressureOutOfRangeWarning_Set(set_alert);
}

void App_CanAlerts_FSM_FlowRateOutOfRangeWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_FlowRateOutOfRangeWarning_Get())
    {
        App_CanTx_FSM_FlowRateOutOfRangeWarningCount_Set(App_CanTx_FSM_FlowRateOutOfRangeWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_FlowRateOutOfRangeWarning_Set(set_alert);
}

void App_CanAlerts_FSM_BrakeAppsDisagreementWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_BrakeAppsDisagreementWarning_Get())
    {
        App_CanTx_FSM_BrakeAppsDisagreementWarningCount_Set(App_CanTx_FSM_BrakeAppsDisagreementWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_BrakeAppsDisagreementWarning_Set(set_alert);
}

void App_CanAlerts_FSM_AppsDisagreementWarning_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_AppsDisagreementWarning_Get())
    {
        App_CanTx_FSM_AppsDisagreementWarningCount_Set(App_CanTx_FSM_AppsDisagreementWarningCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_AppsDisagreementWarning_Set(set_alert);
}

void App_CanAlerts_FSM_MissingHeartbeatFault_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_MissingHeartbeatFault_Get())
    {
        App_CanTx_FSM_MissingHeartbeatFaultCount_Set(App_CanTx_FSM_MissingHeartbeatFaultCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_MissingHeartbeatFault_Set(set_alert);
}

void App_CanAlerts_FSM_StateMachineFault_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_StateMachineFault_Get())
    {
        App_CanTx_FSM_StateMachineFaultCount_Set(App_CanTx_FSM_StateMachineFaultCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_StateMachineFault_Set(set_alert);
}

void App_CanAlerts_FSM_PappsOCSCFault_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_PappsOCSCFault_Get())
    {
        App_CanTx_FSM_PappsOCSCFaultCount_Set(App_CanTx_FSM_PappsOCSCFaultCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_PappsOCSCFault_Set(set_alert);
}

void App_CanAlerts_FSM_SappsOCSCFault_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_SappsOCSCFault_Get())
    {
        App_CanTx_FSM_SappsOCSCFaultCount_Set(App_CanTx_FSM_SappsOCSCFaultCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_SappsOCSCFault_Set(set_alert);
}

void App_CanAlerts_FSM_FlowMeterUnderflowFault_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_FlowMeterUnderflowFault_Get())
    {
        App_CanTx_FSM_FlowMeterUnderflowFaultCount_Set(App_CanTx_FSM_FlowMeterUnderflowFaultCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_FlowMeterUnderflowFault_Set(set_alert);
}

void App_CanAlerts_FSM_TorquePlausabilityFailedFault_Set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !App_CanTx_FSM_TorquePlausabilityFailedFault_Get())
    {
        App_CanTx_FSM_TorquePlausabilityFailedFaultCount_Set(App_CanTx_FSM_TorquePlausabilityFailedFaultCount_Get() + 1);
    }
    
    // Set alert.
    App_CanTx_FSM_TorquePlausabilityFailedFault_Set(set_alert);
}

bool App_CanAlerts_FSM_StackWaterMarkHighTask1HzWarning_Get()
{
    return App_CanTx_FSM_StackWaterMarkHighTask1HzWarning_Get();
}

bool App_CanAlerts_FSM_StackWaterMarkHighTask100HzWarning_Get()
{
    return App_CanTx_FSM_StackWaterMarkHighTask100HzWarning_Get();
}

bool App_CanAlerts_FSM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return App_CanTx_FSM_StackWaterMarkHighTask1kHzWarning_Get();
}

bool App_CanAlerts_FSM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarning_Get();
}

bool App_CanAlerts_FSM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarning_Get();
}

bool App_CanAlerts_FSM_WatchdogTimeoutWarning_Get()
{
    return App_CanTx_FSM_WatchdogTimeoutWarning_Get();
}

bool App_CanAlerts_FSM_TxOverflowWarning_Get()
{
    return App_CanTx_FSM_TxOverflowWarning_Get();
}

bool App_CanAlerts_FSM_RxOverflowWarning_Get()
{
    return App_CanTx_FSM_RxOverflowWarning_Get();
}

bool App_CanAlerts_FSM_LeftWheelSpeedOutOfRangeWarning_Get()
{
    return App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarning_Get();
}

bool App_CanAlerts_FSM_RightWheelSpeedOutOfRangeWarning_Get()
{
    return App_CanTx_FSM_RightWheelSpeedOutOfRangeWarning_Get();
}

bool App_CanAlerts_FSM_SteeringAngleOCSCWarning_Get()
{
    return App_CanTx_FSM_SteeringAngleOCSCWarning_Get();
}

bool App_CanAlerts_FSM_SteeringAngleOutOfRangeWarning_Get()
{
    return App_CanTx_FSM_SteeringAngleOutOfRangeWarning_Get();
}

bool App_CanAlerts_FSM_FrontBrakePressureOutOfRangeWarning_Get()
{
    return App_CanTx_FSM_FrontBrakePressureOutOfRangeWarning_Get();
}

bool App_CanAlerts_FSM_RearBrakePressureOutOfRangeWarning_Get()
{
    return App_CanTx_FSM_RearBrakePressureOutOfRangeWarning_Get();
}

bool App_CanAlerts_FSM_FlowRateOutOfRangeWarning_Get()
{
    return App_CanTx_FSM_FlowRateOutOfRangeWarning_Get();
}

bool App_CanAlerts_FSM_BrakeAppsDisagreementWarning_Get()
{
    return App_CanTx_FSM_BrakeAppsDisagreementWarning_Get();
}

bool App_CanAlerts_FSM_AppsDisagreementWarning_Get()
{
    return App_CanTx_FSM_AppsDisagreementWarning_Get();
}

bool App_CanAlerts_FSM_MissingHeartbeatFault_Get()
{
    return App_CanTx_FSM_MissingHeartbeatFault_Get();
}

bool App_CanAlerts_FSM_StateMachineFault_Get()
{
    return App_CanTx_FSM_StateMachineFault_Get();
}

bool App_CanAlerts_FSM_PappsOCSCFault_Get()
{
    return App_CanTx_FSM_PappsOCSCFault_Get();
}

bool App_CanAlerts_FSM_SappsOCSCFault_Get()
{
    return App_CanTx_FSM_SappsOCSCFault_Get();
}

bool App_CanAlerts_FSM_FlowMeterUnderflowFault_Get()
{
    return App_CanTx_FSM_FlowMeterUnderflowFault_Get();
}

bool App_CanAlerts_FSM_TorquePlausabilityFailedFault_Get()
{
    return App_CanTx_FSM_TorquePlausabilityFailedFault_Get();
}

bool App_CanAlerts_BoardHasWarning(CanAlertBoard board)
{
    switch (board)
    {
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
        case DCM_ALERT_BOARD:
        {
            if (App_CanRx_DCM_StackWaterMarkHighTask1HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_StackWaterMarkHighTask100HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_StackWaterMarkHighTask1kHzWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_StackWaterMarkHighTaskCanRxWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_StackWaterMarkHighTaskCanTxWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_WatchdogTimeoutWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_TxOverflowWarning_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_RxOverflowWarning_Get())
            {
                return true;
            }
            
            break;
        }
        case FSM_ALERT_BOARD:
        {
            if (App_CanTx_FSM_StackWaterMarkHighTask1HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_StackWaterMarkHighTask100HzWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_StackWaterMarkHighTask1kHzWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_WatchdogTimeoutWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_TxOverflowWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_RxOverflowWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_RightWheelSpeedOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_SteeringAngleOCSCWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_SteeringAngleOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_FrontBrakePressureOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_RearBrakePressureOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_FlowRateOutOfRangeWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_BrakeAppsDisagreementWarning_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_AppsDisagreementWarning_Get())
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
        case DCM_ALERT_BOARD:
        {
            if (App_CanRx_DCM_MissingHeartbeatFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_LeftInverterFault_Get())
            {
                return true;
            }
            
            if (App_CanRx_DCM_RightInverterFault_Get())
            {
                return true;
            }
            
            break;
        }
        case FSM_ALERT_BOARD:
        {
            if (App_CanTx_FSM_MissingHeartbeatFault_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_StateMachineFault_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_PappsOCSCFault_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_SappsOCSCFault_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_FlowMeterUnderflowFault_Get())
            {
                return true;
            }
            
            if (App_CanTx_FSM_TorquePlausabilityFailedFault_Get())
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
        case DIM_ALERT_BOARD:
        {
            if (App_CanRx_DIM_MissingHeartbeatFault_Get())
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
    
    if (App_CanAlerts_BoardHasWarning(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(PDM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasWarning(DIM_ALERT_BOARD))
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
    
    if (App_CanAlerts_BoardHasFault(FSM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(PDM_ALERT_BOARD))
    {
        return true;
    }
    
    if (App_CanAlerts_BoardHasFault(DIM_ALERT_BOARD))
    {
        return true;
    }
    
    return false;
}

