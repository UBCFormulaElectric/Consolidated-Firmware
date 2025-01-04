/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_canRx.h"

/* ------------------------- Function Definitions ------------------------- */


// warning setter functions

void app_canAlerts_BMS_Warning_StackWaterMarkHighTask1Hz_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_StackWaterMarkHighTask1Hz_get())
    {
        app_canTx_BMS_Warning_StackWaterMarkHighTask1HzCount_set(app_canTx_BMS_Warning_StackWaterMarkHighTask1HzCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_StackWaterMarkHighTask1Hz_set(set_alert);
}

void app_canAlerts_BMS_Warning_StackWaterMarkHighTask100Hz_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_StackWaterMarkHighTask100Hz_get())
    {
        app_canTx_BMS_Warning_StackWaterMarkHighTask100HzCount_set(app_canTx_BMS_Warning_StackWaterMarkHighTask100HzCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_StackWaterMarkHighTask100Hz_set(set_alert);
}

void app_canAlerts_BMS_Warning_StackWaterMarkHighTask1kHz_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_StackWaterMarkHighTask1kHz_get())
    {
        app_canTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_set(app_canTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_StackWaterMarkHighTask1kHz_set(set_alert);
}

void app_canAlerts_BMS_Warning_StackWaterMarkHighTaskCanRx_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRx_get())
    {
        app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_set(app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRx_set(set_alert);
}

void app_canAlerts_BMS_Warning_StackWaterMarkHighTaskCanTx_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTx_get())
    {
        app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_set(app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTx_set(set_alert);
}

void app_canAlerts_BMS_Warning_WatchdogTimeout_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_WatchdogTimeout_get())
    {
        app_canTx_BMS_Warning_WatchdogTimeoutCount_set(app_canTx_BMS_Warning_WatchdogTimeoutCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_WatchdogTimeout_set(set_alert);
}

void app_canAlerts_BMS_Warning_TxOverflow_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_TxOverflow_get())
    {
        app_canTx_BMS_Warning_TxOverflowCount_set(app_canTx_BMS_Warning_TxOverflowCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_TxOverflow_set(set_alert);
}

void app_canAlerts_BMS_Warning_RxOverflow_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_RxOverflow_get())
    {
        app_canTx_BMS_Warning_RxOverflowCount_set(app_canTx_BMS_Warning_RxOverflowCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_RxOverflow_set(set_alert);
}

void app_canAlerts_BMS_Warning_OpenWireCheckFault_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_OpenWireCheckFault_get())
    {
        app_canTx_BMS_Warning_OpenWireCheckFaultCount_set(app_canTx_BMS_Warning_OpenWireCheckFaultCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_OpenWireCheckFault_set(set_alert);
}

void app_canAlerts_BMS_Warning_OpenWireCheck_Segment0_GND_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_OpenWireCheck_Segment0_GND_get())
    {
        app_canTx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_set(app_canTx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_OpenWireCheck_Segment0_GND_set(set_alert);
}

void app_canAlerts_BMS_Warning_OpenWireCheck_Segment1_GND_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_OpenWireCheck_Segment1_GND_get())
    {
        app_canTx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_set(app_canTx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_OpenWireCheck_Segment1_GND_set(set_alert);
}

void app_canAlerts_BMS_Warning_OpenWireCheck_Segment2_GND_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_OpenWireCheck_Segment2_GND_get())
    {
        app_canTx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_set(app_canTx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_OpenWireCheck_Segment2_GND_set(set_alert);
}

void app_canAlerts_BMS_Warning_OpenWireCheck_Segment3_GND_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_OpenWireCheck_Segment3_GND_get())
    {
        app_canTx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_set(app_canTx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_OpenWireCheck_Segment3_GND_set(set_alert);
}

void app_canAlerts_BMS_Warning_OpenWireCheck_Segment4_GND_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_OpenWireCheck_Segment4_GND_get())
    {
        app_canTx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_set(app_canTx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_OpenWireCheck_Segment4_GND_set(set_alert);
}

void app_canAlerts_BMS_Warning_MissingVCHeartbeat_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Warning_MissingVCHeartbeat_get())
    {
        app_canTx_BMS_Warning_MissingVCHeartbeatCount_set(app_canTx_BMS_Warning_MissingVCHeartbeatCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Warning_MissingVCHeartbeat_set(set_alert);
}

void app_canAlerts_BMS_Fault_StateMachine_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_StateMachine_get())
    {
        app_canTx_BMS_Fault_StateMachineCount_set(app_canTx_BMS_Fault_StateMachineCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_StateMachine_set(set_alert);
}

void app_canAlerts_BMS_Fault_CellUndervoltage_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_CellUndervoltage_get())
    {
        app_canTx_BMS_Fault_CellUndervoltageCount_set(app_canTx_BMS_Fault_CellUndervoltageCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_CellUndervoltage_set(set_alert);
}

void app_canAlerts_BMS_Fault_CellOvervoltage_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_CellOvervoltage_get())
    {
        app_canTx_BMS_Fault_CellOvervoltageCount_set(app_canTx_BMS_Fault_CellOvervoltageCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_CellOvervoltage_set(set_alert);
}

void app_canAlerts_BMS_Fault_ModuleCommunicationError_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_ModuleCommunicationError_get())
    {
        app_canTx_BMS_Fault_ModuleCommunicationErrorCount_set(app_canTx_BMS_Fault_ModuleCommunicationErrorCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_ModuleCommunicationError_set(set_alert);
}

void app_canAlerts_BMS_Fault_CellUndertemp_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_CellUndertemp_get())
    {
        app_canTx_BMS_Fault_CellUndertempCount_set(app_canTx_BMS_Fault_CellUndertempCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_CellUndertemp_set(set_alert);
}

void app_canAlerts_BMS_Fault_CellOvertemp_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_CellOvertemp_get())
    {
        app_canTx_BMS_Fault_CellOvertempCount_set(app_canTx_BMS_Fault_CellOvertempCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_CellOvertemp_set(set_alert);
}

void app_canAlerts_BMS_Fault_ChargerReportedError_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_ChargerReportedError_get())
    {
        app_canTx_BMS_Fault_ChargerReportedErrorCount_set(app_canTx_BMS_Fault_ChargerReportedErrorCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_ChargerReportedError_set(set_alert);
}

void app_canAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_ChargerDisconnectedDuringCharge_get())
    {
        app_canTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_set(app_canTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_ChargerDisconnectedDuringCharge_set(set_alert);
}

void app_canAlerts_BMS_Fault_ChargerShutdownLoopOpen_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_ChargerShutdownLoopOpen_get())
    {
        app_canTx_BMS_Fault_ChargerShutdownLoopOpenCount_set(app_canTx_BMS_Fault_ChargerShutdownLoopOpenCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_ChargerShutdownLoopOpen_set(set_alert);
}

void app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_TractiveSystemOvercurrent_get())
    {
        app_canTx_BMS_Fault_TractiveSystemOvercurrentCount_set(app_canTx_BMS_Fault_TractiveSystemOvercurrentCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_TractiveSystemOvercurrent_set(set_alert);
}

void app_canAlerts_BMS_Fault_PrechargeFailure_set(bool set_alert)
{
    // Increment alert counter.
    if (set_alert && !app_canTx_BMS_Fault_PrechargeFailure_get())
    {
        app_canTx_BMS_Fault_PrechargeFailureCount_set(app_canTx_BMS_Fault_PrechargeFailureCount_get() + 1);
    }
    
    // Set alert.
    app_canTx_BMS_Fault_PrechargeFailure_set(set_alert);
}



// warning getter functions

bool app_canAlerts_BMS_Warning_StackWaterMarkHighTask1Hz_get()
{
    return app_canTx_BMS_Warning_StackWaterMarkHighTask1Hz_get();
}

bool app_canAlerts_BMS_Warning_StackWaterMarkHighTask100Hz_get()
{
    return app_canTx_BMS_Warning_StackWaterMarkHighTask100Hz_get();
}

bool app_canAlerts_BMS_Warning_StackWaterMarkHighTask1kHz_get()
{
    return app_canTx_BMS_Warning_StackWaterMarkHighTask1kHz_get();
}

bool app_canAlerts_BMS_Warning_StackWaterMarkHighTaskCanRx_get()
{
    return app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRx_get();
}

bool app_canAlerts_BMS_Warning_StackWaterMarkHighTaskCanTx_get()
{
    return app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTx_get();
}

bool app_canAlerts_BMS_Warning_WatchdogTimeout_get()
{
    return app_canTx_BMS_Warning_WatchdogTimeout_get();
}

bool app_canAlerts_BMS_Warning_TxOverflow_get()
{
    return app_canTx_BMS_Warning_TxOverflow_get();
}

bool app_canAlerts_BMS_Warning_RxOverflow_get()
{
    return app_canTx_BMS_Warning_RxOverflow_get();
}

bool app_canAlerts_BMS_Warning_OpenWireCheckFault_get()
{
    return app_canTx_BMS_Warning_OpenWireCheckFault_get();
}

bool app_canAlerts_BMS_Warning_OpenWireCheck_Segment0_GND_get()
{
    return app_canTx_BMS_Warning_OpenWireCheck_Segment0_GND_get();
}

bool app_canAlerts_BMS_Warning_OpenWireCheck_Segment1_GND_get()
{
    return app_canTx_BMS_Warning_OpenWireCheck_Segment1_GND_get();
}

bool app_canAlerts_BMS_Warning_OpenWireCheck_Segment2_GND_get()
{
    return app_canTx_BMS_Warning_OpenWireCheck_Segment2_GND_get();
}

bool app_canAlerts_BMS_Warning_OpenWireCheck_Segment3_GND_get()
{
    return app_canTx_BMS_Warning_OpenWireCheck_Segment3_GND_get();
}

bool app_canAlerts_BMS_Warning_OpenWireCheck_Segment4_GND_get()
{
    return app_canTx_BMS_Warning_OpenWireCheck_Segment4_GND_get();
}

bool app_canAlerts_BMS_Warning_MissingVCHeartbeat_get()
{
    return app_canTx_BMS_Warning_MissingVCHeartbeat_get();
}

bool app_canAlerts_BMS_Fault_StateMachine_get()
{
    return app_canTx_BMS_Fault_StateMachine_get();
}

bool app_canAlerts_BMS_Fault_CellUndervoltage_get()
{
    return app_canTx_BMS_Fault_CellUndervoltage_get();
}

bool app_canAlerts_BMS_Fault_CellOvervoltage_get()
{
    return app_canTx_BMS_Fault_CellOvervoltage_get();
}

bool app_canAlerts_BMS_Fault_ModuleCommunicationError_get()
{
    return app_canTx_BMS_Fault_ModuleCommunicationError_get();
}

bool app_canAlerts_BMS_Fault_CellUndertemp_get()
{
    return app_canTx_BMS_Fault_CellUndertemp_get();
}

bool app_canAlerts_BMS_Fault_CellOvertemp_get()
{
    return app_canTx_BMS_Fault_CellOvertemp_get();
}

bool app_canAlerts_BMS_Fault_ChargerReportedError_get()
{
    return app_canTx_BMS_Fault_ChargerReportedError_get();
}

bool app_canAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_get()
{
    return app_canTx_BMS_Fault_ChargerDisconnectedDuringCharge_get();
}

bool app_canAlerts_BMS_Fault_ChargerShutdownLoopOpen_get()
{
    return app_canTx_BMS_Fault_ChargerShutdownLoopOpen_get();
}

bool app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get()
{
    return app_canTx_BMS_Fault_TractiveSystemOvercurrent_get();
}

bool app_canAlerts_BMS_Fault_PrechargeFailure_get()
{
    return app_canTx_BMS_Fault_PrechargeFailure_get();
}



bool app_canAlerts_BoardHasWarning(CanAlertBoard board)
{
    switch (board)
    {
        
        case BMS_ALERT_BOARD :
        {
            if (app_canTx_BMS_Warning_StackWaterMarkHighTask1Hz_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_StackWaterMarkHighTask100Hz_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_StackWaterMarkHighTask1kHz_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRx_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTx_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_WatchdogTimeout_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_TxOverflow_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_RxOverflow_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_OpenWireCheckFault_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_OpenWireCheck_Segment0_GND_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_OpenWireCheck_Segment1_GND_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_OpenWireCheck_Segment2_GND_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_OpenWireCheck_Segment3_GND_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_OpenWireCheck_Segment4_GND_get()) 
            {
                return true;
            }if (app_canTx_BMS_Warning_MissingVCHeartbeat_get()) 
            {
                return true;
            }
            break;
        }
        
        case VC_ALERT_BOARD :
        {
            if (app_canRx_VC_Warning_StackWaterMarkHighTask1Hz_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_StackWaterMarkHighTask100Hz_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_StackWaterMarkHighTask1kHz_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_StackWaterMarkHighTaskCanRx_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_StackWaterMarkHighTaskCanTx_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_WatchdogTimeout_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_TxOverflow_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_RxOverflow_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_RegenNotAvailable_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_ImuInitFailed_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_FlowRateInputOutOfRange_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_BatteryRailOvercurrentFault_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_AccumulatorRailOvercurrentFault_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_LvChargeFault_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_BoostControllerFault_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_HighNumberOfCanDataLogs_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_CanLoggingSdCardNotPresent_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_CanLoggingErrored_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_BrakeAppsDisagreement_get()) 
            {
                return true;
            }if (app_canRx_VC_Warning_SbgInitFailed_get()) 
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
        case BMS_ALERT_BOARD :
        {
            if (app_canTx_BMS_Fault_StateMachine_get()) 
            {
                return true;
            }if (app_canTx_BMS_Fault_CellUndervoltage_get()) 
            {
                return true;
            }if (app_canTx_BMS_Fault_CellOvervoltage_get()) 
            {
                return true;
            }if (app_canTx_BMS_Fault_ModuleCommunicationError_get()) 
            {
                return true;
            }if (app_canTx_BMS_Fault_CellUndertemp_get()) 
            {
                return true;
            }if (app_canTx_BMS_Fault_CellOvertemp_get()) 
            {
                return true;
            }if (app_canTx_BMS_Fault_ChargerReportedError_get()) 
            {
                return true;
            }if (app_canTx_BMS_Fault_ChargerDisconnectedDuringCharge_get()) 
            {
                return true;
            }if (app_canTx_BMS_Fault_ChargerShutdownLoopOpen_get()) 
            {
                return true;
            }if (app_canTx_BMS_Fault_TractiveSystemOvercurrent_get()) 
            {
                return true;
            }if (app_canTx_BMS_Fault_PrechargeFailure_get()) 
            {
                return true;
            }
            break;
        }
        
        case VC_ALERT_BOARD :
        {
            if (app_canRx_VC_Fault_MissingBMSHeartbeat_get()) 
            {
                return true;
            }if (app_canRx_VC_Fault_MissingFSMHeartbeat_get()) 
            {
                return true;
            }if (app_canRx_VC_Fault_MissingRSMHeartbeat_get()) 
            {
                return true;
            }if (app_canRx_VC_Fault_MissingCRITHeartbeat_get()) 
            {
                return true;
            }if (app_canRx_VC_Fault_LeftInverterFault_get()) 
            {
                return true;
            }if (app_canRx_VC_Fault_RightInverterFault_get()) 
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
    if (app_canAlerts_BoardHasWarning(BMS_ALERT_BOARD))
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
    
    
    return false;
}

uint8_t app_canAlerts_WarningInfo(Fault_Warning_Info *alert_array)
{
    uint8_t element_num = 0;
    // tx faults
    
    
    if (app_canTx_BMS_Warning_StackWaterMarkHighTask1Hz_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_StackWaterMarkHighTask1Hz";
        alert_array[element_num].description = "1Hz task stack watermark exceeded.";
        alert_array[element_num].id = 101;
        element_num++;
    }
    if (app_canTx_BMS_Warning_StackWaterMarkHighTask100Hz_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_StackWaterMarkHighTask100Hz";
        alert_array[element_num].description = "100Hz task stack watermark exceeded.";
        alert_array[element_num].id = 102;
        element_num++;
    }
    if (app_canTx_BMS_Warning_StackWaterMarkHighTask1kHz_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_StackWaterMarkHighTask1kHz";
        alert_array[element_num].description = "1kHz task stack watermark exceeded.";
        alert_array[element_num].id = 103;
        element_num++;
    }
    if (app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRx_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_StackWaterMarkHighTaskCanRx";
        alert_array[element_num].description = "CAN RX task stack watermark exceeded.";
        alert_array[element_num].id = 104;
        element_num++;
    }
    if (app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTx_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_StackWaterMarkHighTaskCanTx";
        alert_array[element_num].description = "CAN TX task stack watermark exceeded.";
        alert_array[element_num].id = 105;
        element_num++;
    }
    if (app_canTx_BMS_Warning_WatchdogTimeout_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_WatchdogTimeout";
        alert_array[element_num].description = "Watchdog timeout detected.";
        alert_array[element_num].id = 106;
        element_num++;
    }
    if (app_canTx_BMS_Warning_TxOverflow_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_TxOverflow";
        alert_array[element_num].description = "CAN TX queue has overflowed.";
        alert_array[element_num].id = 107;
        element_num++;
    }
    if (app_canTx_BMS_Warning_RxOverflow_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_RxOverflow";
        alert_array[element_num].description = "CAN RX queue has overflowed.";
        alert_array[element_num].id = 108;
        element_num++;
    }
    if (app_canTx_BMS_Warning_OpenWireCheckFault_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_OpenWireCheckFault";
        alert_array[element_num].description = "Open wire cell detected (cell voltage tap is open circuit).";
        alert_array[element_num].id = 109;
        element_num++;
    }
    if (app_canTx_BMS_Warning_OpenWireCheck_Segment0_GND_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_OpenWireCheck_Segment0_GND";
        alert_array[element_num].description = "Open wire cell detected in segment 0 (cell voltage tap is open circuit).";
        alert_array[element_num].id = 110;
        element_num++;
    }
    if (app_canTx_BMS_Warning_OpenWireCheck_Segment1_GND_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_OpenWireCheck_Segment1_GND";
        alert_array[element_num].description = "Open wire cell detected in segment 1 (cell voltage tap is open circuit).";
        alert_array[element_num].id = 111;
        element_num++;
    }
    if (app_canTx_BMS_Warning_OpenWireCheck_Segment2_GND_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_OpenWireCheck_Segment2_GND";
        alert_array[element_num].description = "Open wire cell detected in segment 2 (cell voltage tap is open circuit).";
        alert_array[element_num].id = 112;
        element_num++;
    }
    if (app_canTx_BMS_Warning_OpenWireCheck_Segment3_GND_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_OpenWireCheck_Segment3_GND";
        alert_array[element_num].description = "Open wire cell detected in segment 3 (cell voltage tap is open circuit).";
        alert_array[element_num].id = 113;
        element_num++;
    }
    if (app_canTx_BMS_Warning_OpenWireCheck_Segment4_GND_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_OpenWireCheck_Segment4_GND";
        alert_array[element_num].description = "Open wire cell detected in segment 4 (cell voltage tap is open circuit).";
        alert_array[element_num].id = 114;
        element_num++;
    }
    if (app_canTx_BMS_Warning_MissingVCHeartbeat_get()) // tx BMS faults
    {
        alert_array[element_num].name = "BMS_Warning_MissingVCHeartbeat";
        alert_array[element_num].description = "Missing heartbeat from the VC.";
        alert_array[element_num].id = 115;
        element_num++;
    }
    if (app_canRx_VC_Warning_StackWaterMarkHighTask1Hz_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_StackWaterMarkHighTask1Hz";
        alert_array[element_num].description = "1Hz task stack watermark exceeded.";
        alert_array[element_num].id = 501;
        element_num++;
    }if (app_canRx_VC_Warning_StackWaterMarkHighTask100Hz_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_StackWaterMarkHighTask100Hz";
        alert_array[element_num].description = "100Hz task stack watermark exceeded.";
        alert_array[element_num].id = 502;
        element_num++;
    }if (app_canRx_VC_Warning_StackWaterMarkHighTask1kHz_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_StackWaterMarkHighTask1kHz";
        alert_array[element_num].description = "1kHz task stack watermark exceeded.";
        alert_array[element_num].id = 503;
        element_num++;
    }if (app_canRx_VC_Warning_StackWaterMarkHighTaskCanRx_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_StackWaterMarkHighTaskCanRx";
        alert_array[element_num].description = "CAN RX task stack watermark exceeded.";
        alert_array[element_num].id = 504;
        element_num++;
    }if (app_canRx_VC_Warning_StackWaterMarkHighTaskCanTx_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_StackWaterMarkHighTaskCanTx";
        alert_array[element_num].description = "CAN TX task stack watermark exceeded.";
        alert_array[element_num].id = 505;
        element_num++;
    }if (app_canRx_VC_Warning_WatchdogTimeout_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_WatchdogTimeout";
        alert_array[element_num].description = "Watchdog timeout detected.";
        alert_array[element_num].id = 506;
        element_num++;
    }if (app_canRx_VC_Warning_TxOverflow_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_TxOverflow";
        alert_array[element_num].description = "CAN TX queue has overflowed.";
        alert_array[element_num].id = 507;
        element_num++;
    }if (app_canRx_VC_Warning_RxOverflow_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_RxOverflow";
        alert_array[element_num].description = "CAN RX queue has overflowed.";
        alert_array[element_num].id = 508;
        element_num++;
    }if (app_canRx_VC_Warning_RegenNotAvailable_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_RegenNotAvailable";
        alert_array[element_num].description = "Regen is not currently available.";
        alert_array[element_num].id = 509;
        element_num++;
    }if (app_canRx_VC_Warning_ImuInitFailed_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_ImuInitFailed";
        alert_array[element_num].description = "Initializing the embedded IMU failed.";
        alert_array[element_num].id = 516;
        element_num++;
    }if (app_canRx_VC_Warning_FlowRateInputOutOfRange_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_FlowRateInputOutOfRange";
        alert_array[element_num].description = "Input to app_pumpControl_setFlowRate out of range";
        alert_array[element_num].id = 517;
        element_num++;
    }if (app_canRx_VC_Warning_BatteryRailOvercurrentFault_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_BatteryRailOvercurrentFault";
        alert_array[element_num].description = "LV rail from the 18650 batteries was overcurrent.";
        alert_array[element_num].id = 557;
        element_num++;
    }if (app_canRx_VC_Warning_AccumulatorRailOvercurrentFault_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_AccumulatorRailOvercurrentFault";
        alert_array[element_num].description = "LV rail from the HV DCDC was overcurrent.";
        alert_array[element_num].id = 558;
        element_num++;
    }if (app_canRx_VC_Warning_LvChargeFault_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_LvChargeFault";
        alert_array[element_num].description = "LV battery charger has faulted.";
        alert_array[element_num].id = 559;
        element_num++;
    }if (app_canRx_VC_Warning_BoostControllerFault_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_BoostControllerFault";
        alert_array[element_num].description = "Boost controller has faulted.";
        alert_array[element_num].id = 560;
        element_num++;
    }if (app_canRx_VC_Warning_HighNumberOfCanDataLogs_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_HighNumberOfCanDataLogs";
        alert_array[element_num].description = "High number of CAN data logs (>200) on the SD card, which slows down mounting the filesystem.";
        alert_array[element_num].id = 561;
        element_num++;
    }if (app_canRx_VC_Warning_CanLoggingSdCardNotPresent_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_CanLoggingSdCardNotPresent";
        alert_array[element_num].description = "CAN logging SD card is not detected.";
        alert_array[element_num].id = 562;
        element_num++;
    }if (app_canRx_VC_Warning_CanLoggingErrored_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_CanLoggingErrored";
        alert_array[element_num].description = "CAN logging received multiple errors and has been disabled.";
        alert_array[element_num].id = 563;
        element_num++;
    }if (app_canRx_VC_Warning_BrakeAppsDisagreement_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_BrakeAppsDisagreement";
        alert_array[element_num].description = "Brakes and apps are active at same time.";
        alert_array[element_num].id = 564;
        element_num++;
    }if (app_canRx_VC_Warning_SbgInitFailed_get()) // rx VC faults
    {
        alert_array[element_num].name = "VC_Warning_SbgInitFailed";
        alert_array[element_num].description = "Initializing the SBG Ellipse GPS failed";
        alert_array[element_num].id = 565;
        element_num++;
    }return element_num;
    
}

uint8_t app_canAlerts_FaultInfo(Fault_Warning_Info *alert_array)
{
    uint8_t element_num = 0;
    // tx faults
    
    
    return element_num;
    
}
