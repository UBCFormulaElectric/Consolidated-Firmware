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
    DCM_Warning_StackWaterMarkHighTask1Hz = 0,
    DCM_Warning_StackWaterMarkHighTask100Hz = 1,
    DCM_Warning_StackWaterMarkHighTask1kHz = 2,
    DCM_Warning_StackWaterMarkHighTaskCanRx = 3,
    DCM_Warning_StackWaterMarkHighTaskCanTx = 4,
    DCM_Warning_WatchdogTimeout = 5,
    DCM_Warning_TxOverflow = 6,
    DCM_Warning_RxOverflow = 7,
    NUM_DCM_WARNINGS = 8,
} DCM_WarningId;

typedef enum
{
    DCM_Fault_MissingHeartbeat = 0,
    DCM_Fault_LeftInverterFault = 1,
    DCM_Fault_RightInverterFault = 2,
    NUM_DCM_FAULTS = 3,
} DCM_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Warning_StackWaterMarkHighTask1Hz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Warning_StackWaterMarkHighTask100Hz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Warning_StackWaterMarkHighTask1kHz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Warning_StackWaterMarkHighTaskCanRx_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Warning_StackWaterMarkHighTaskCanTx_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Warning_WatchdogTimeout_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Warning_TxOverflow_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Warning_RxOverflow_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Fault_MissingHeartbeat_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Fault_LeftInverterFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_Fault_RightInverterFault_Set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Warning_WatchdogTimeout_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Warning_TxOverflow_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Warning_RxOverflow_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Fault_MissingHeartbeat_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Fault_LeftInverterFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_Fault_RightInverterFault_Get(void);

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

