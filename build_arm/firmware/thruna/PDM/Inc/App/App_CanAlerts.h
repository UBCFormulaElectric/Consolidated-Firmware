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
    BMS_ALERT_BOARD = 0,
    DCM_ALERT_BOARD = 1,
    FSM_ALERT_BOARD = 2,
    PDM_ALERT_BOARD = 3,
    DIM_ALERT_BOARD = 4,
} CanAlertBoard;
typedef enum
{
    PDM_StackWaterMarkHighTask1HzWarning = 0,
    PDM_StackWaterMarkHighTask100HzWarning = 1,
    PDM_StackWaterMarkHighTask1kHzWarning = 2,
    PDM_StackWaterMarkHighTaskCanRxWarning = 3,
    PDM_StackWaterMarkHighTaskCanTxWarning = 4,
    PDM_WatchdogTimeoutWarning = 5,
    PDM_TxOverflowWarning = 6,
    PDM_RxOverflowWarning = 7,
    NUM_PDM_WARNINGS = 8,
} PDM_WarningId;

typedef enum
{
    PDM_DummyFault = 0,
    NUM_PDM_FAULTS = 1,
} PDM_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_StackWaterMarkHighTask1HzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_StackWaterMarkHighTask100HzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_StackWaterMarkHighTask1kHzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_StackWaterMarkHighTaskCanRxWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_StackWaterMarkHighTaskCanTxWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_WatchdogTimeoutWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_TxOverflowWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_RxOverflowWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_PDM_DummyFault_Set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_WatchdogTimeoutWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_TxOverflowWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_RxOverflowWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_PDM_DummyFault_Get(void);

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

