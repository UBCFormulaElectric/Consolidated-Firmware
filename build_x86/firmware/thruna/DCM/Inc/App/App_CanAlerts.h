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
    DCM_StackWaterMarkHighTask1HzWarning = 0,
    DCM_StackWaterMarkHighTask100HzWarning = 1,
    DCM_StackWaterMarkHighTask1kHzWarning = 2,
    DCM_StackWaterMarkHighTaskCanRxWarning = 3,
    DCM_StackWaterMarkHighTaskCanTxWarning = 4,
    DCM_WatchdogTimeoutWarning = 5,
    DCM_TxOverflowWarning = 6,
    DCM_RxOverflowWarning = 7,
    NUM_DCM_WARNINGS = 8,
} DCM_WarningId;

typedef enum
{
    DCM_MissingHeartbeatFault = 0,
    DCM_LeftInverterFault = 1,
    DCM_RightInverterFault = 2,
    NUM_DCM_FAULTS = 3,
} DCM_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_StackWaterMarkHighTask1HzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_StackWaterMarkHighTask100HzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_StackWaterMarkHighTask1kHzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_StackWaterMarkHighTaskCanRxWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_StackWaterMarkHighTaskCanTxWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_WatchdogTimeoutWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_TxOverflowWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_RxOverflowWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_MissingHeartbeatFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_LeftInverterFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_DCM_RightInverterFault_Set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_WatchdogTimeoutWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_TxOverflowWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_RxOverflowWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_MissingHeartbeatFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_LeftInverterFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_DCM_RightInverterFault_Get(void);

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

