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
    PDM_Warning_StackWaterMarkHighTask1Hz = 0,
    PDM_Warning_StackWaterMarkHighTask100Hz = 1,
    PDM_Warning_StackWaterMarkHighTask1kHz = 2,
    PDM_Warning_StackWaterMarkHighTaskCanRx = 3,
    PDM_Warning_StackWaterMarkHighTaskCanTx = 4,
    PDM_Warning_WatchdogTimeout = 5,
    PDM_Warning_TxOverflow = 6,
    PDM_Warning_RxOverflow = 7,
    NUM_PDM_WARNINGS = 8,
} PDM_WarningId;

typedef enum
{
    PDM_Fault_DummyFault = 0,
    NUM_PDM_FAULTS = 1,
} PDM_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_Warning_StackWaterMarkHighTask1Hz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_Warning_StackWaterMarkHighTask100Hz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_Warning_StackWaterMarkHighTask1kHz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_Warning_StackWaterMarkHighTaskCanRx_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_Warning_StackWaterMarkHighTaskCanTx_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_Warning_WatchdogTimeout_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_Warning_TxOverflow_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_Warning_RxOverflow_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_Fault_DummyFault_Set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_Warning_WatchdogTimeout_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_Warning_TxOverflow_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_Warning_RxOverflow_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_Fault_DummyFault_Get(void);

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

