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
    DIM_StackWaterMarkHighTask1HzWarning = 0,
    DIM_StackWaterMarkHighTask100HzWarning = 1,
    DIM_StackWaterMarkHighTask1kHzWarning = 2,
    DIM_StackWaterMarkHighTaskCanRxWarning = 3,
    DIM_StackWaterMarkHighTaskCanTxWarning = 4,
    DIM_WatchdogTimeoutWarning = 5,
    DIM_TxOverflowWarning = 6,
    DIM_RxOverflowWarning = 7,
    NUM_DIM_WARNINGS = 8,
} DIM_WarningId;

typedef enum
{
    DIM_MissingHeartbeatFault = 0,
    NUM_DIM_FAULTS = 1,
} DIM_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DIM_StackWaterMarkHighTask1HzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DIM_StackWaterMarkHighTask100HzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DIM_StackWaterMarkHighTask1kHzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DIM_StackWaterMarkHighTaskCanRxWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DIM_StackWaterMarkHighTaskCanTxWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DIM_WatchdogTimeoutWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DIM_TxOverflowWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DIM_RxOverflowWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DIM_MissingHeartbeatFault_Set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DIM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DIM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DIM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DIM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DIM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DIM_WatchdogTimeoutWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DIM_TxOverflowWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DIM_RxOverflowWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DIM_MissingHeartbeatFault_Get(void);

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

