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
    DIM_ALERT_BOARD = 0,
    DCM_ALERT_BOARD = 1,
    BMS_ALERT_BOARD = 2,
    PDM_ALERT_BOARD = 3,
    FSM_ALERT_BOARD = 4,
} CanAlertBoard;
typedef enum
{
    BMS_StackWaterMarkHighTask1HzWarning = 0,
    BMS_StackWaterMarkHighTask100HzWarning = 1,
    BMS_StackWaterMarkHighTask1kHzWarning = 2,
    BMS_StackWaterMarkHighTaskCanRxWarning = 3,
    BMS_StackWaterMarkHighTaskCanTxWarning = 4,
    BMS_WatchdogTimeoutWarning = 5,
    BMS_TxOverflowWarning = 6,
    BMS_RxOverflowWarning = 7,
    NUM_BMS_WARNINGS = 8,
} BMS_WarningId;

typedef enum
{
    BMS_MissingHeartbeatFault = 0,
    BMS_StateMachineFault = 1,
    BMS_CellUndervoltageFault = 2,
    BMS_CellOvervoltageFault = 3,
    BMS_ModuleCommunicationFault = 4,
    BMS_CellUndertempFault = 5,
    BMS_CellOvertempFault = 6,
    BMS_ChargerFault = 7,
    BMS_ChargerDisconnectedDuringChargeFault = 8,
    BMS_ChargerExternalShutdownFault = 9,
    BMS_TractiveSystemOvercurrentFault = 10,
    BMS_PrechargeFailureFault = 11,
    NUM_BMS_FAULTS = 12,
} BMS_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_StackWaterMarkHighTask1HzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_StackWaterMarkHighTask100HzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_StackWaterMarkHighTask1kHzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_StackWaterMarkHighTaskCanRxWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_StackWaterMarkHighTaskCanTxWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_WatchdogTimeoutWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_TxOverflowWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_RxOverflowWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_MissingHeartbeatFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_StateMachineFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_CellUndervoltageFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_CellOvervoltageFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_ModuleCommunicationFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_CellUndertempFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_CellOvertempFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_ChargerFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_ChargerDisconnectedDuringChargeFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_ChargerExternalShutdownFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_TractiveSystemOvercurrentFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_BMS_PrechargeFailureFault_Set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_WatchdogTimeoutWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_TxOverflowWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_RxOverflowWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_MissingHeartbeatFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_StateMachineFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_CellUndervoltageFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_CellOvervoltageFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_ModuleCommunicationFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_CellUndertempFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_CellOvertempFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_ChargerFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_ChargerDisconnectedDuringChargeFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_ChargerExternalShutdownFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_TractiveSystemOvercurrentFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_BMS_PrechargeFailureFault_Get(void);

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

