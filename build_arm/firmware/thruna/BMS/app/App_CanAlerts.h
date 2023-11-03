/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include <stdbool.h>
#include "App_CanUtils.h"

/* -------------------------------- Enums --------------------------------- */

typedef enum
{
    DCM_ALERT_BOARD = 0,
    DIM_ALERT_BOARD = 1,
    BMS_ALERT_BOARD = 2,
    FSM_ALERT_BOARD = 3,
    PDM_ALERT_BOARD = 4,
} CanAlertBoard;
typedef enum
{
    BMS_Warning_StackWaterMarkHighTask1Hz = 0,
    BMS_Warning_StackWaterMarkHighTask100Hz = 1,
    BMS_Warning_StackWaterMarkHighTask1kHz = 2,
    BMS_Warning_StackWaterMarkHighTaskCanRx = 3,
    BMS_Warning_StackWaterMarkHighTaskCanTx = 4,
    BMS_Warning_WatchdogTimeout = 5,
    BMS_Warning_TxOverflow = 6,
    BMS_Warning_RxOverflow = 7,
    NUM_BMS_WARNINGS = 8,
} BMS_WarningId;

typedef enum
{
    BMS_Fault_MissingHeartbeat = 0,
    BMS_Fault_StateMachine = 1,
    BMS_Fault_CellUndervoltage = 2,
    BMS_Fault_CellOvervoltage = 3,
    BMS_Fault_ModuleCommunicationError = 4,
    BMS_Fault_CellUndertemp = 5,
    BMS_Fault_CellOvertemp = 6,
    BMS_Fault_Charger = 7,
    BMS_Fault_ChargerDisconnectedDuringCharge = 8,
    BMS_Fault_ChargerExternalShutdown = 9,
    BMS_Fault_TractiveSystemOvercurrent = 10,
    BMS_Fault_PrechargeFailure = 11,
    NUM_BMS_FAULTS = 12,
} BMS_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Warning_StackWaterMarkHighTask1Hz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Warning_StackWaterMarkHighTask100Hz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Warning_StackWaterMarkHighTask1kHz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Warning_StackWaterMarkHighTaskCanRx_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Warning_StackWaterMarkHighTaskCanTx_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Warning_WatchdogTimeout_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Warning_TxOverflow_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Warning_RxOverflow_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_MissingHeartbeat_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_StateMachine_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_CellUndervoltage_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_CellOvervoltage_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_ModuleCommunicationError_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_CellUndertemp_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_CellOvertemp_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_Charger_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_ChargerExternalShutdown_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_Fault_PrechargeFailure_Set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Warning_WatchdogTimeout_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Warning_TxOverflow_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Warning_RxOverflow_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_MissingHeartbeat_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_StateMachine_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_CellUndervoltage_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_CellOvervoltage_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_ModuleCommunicationError_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_CellUndertemp_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_CellOvertemp_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_Charger_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_ChargerExternalShutdown_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_TractiveSystemOvercurrent_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_Fault_PrechargeFailure_Get(void);

/**
 * Return whether or not a board has set a warning.
 */
bool App_CanAlerts_BoardHasWarning(CanAlertBoard board);

/**
 * Return whether or not a board has set a fault.
 */
bool App_CanAlerts_BoardHasFault(CanAlertBoard board);

/**
 * Return whether or not any board has set a warning.
 */
bool App_CanAlerts_AnyBoardHasWarning(void);

/**
 * Return whether or not any board has set a fault.
 */
bool App_CanAlerts_AnyBoardHasFault(void);

