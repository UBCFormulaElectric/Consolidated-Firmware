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
    DIM_ALERT_BOARD = 2,
    FSM_ALERT_BOARD = 3,
    PDM_ALERT_BOARD = 4,
} CanAlertBoard;
typedef enum
{
    DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ = 0,
    DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ = 1,
    DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ = 2,
    DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX = 3,
    DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX = 4,
    DCM_WARNING_WATCHDOG_TIMEOUT = 5,
    DCM_WARNING_TX_OVERFLOW = 6,
    DCM_WARNING_RX_OVERFLOW = 7,
    NUM_DCM_WARNINGS = 8,
} DCM_WarningId;

typedef enum
{
    DCM_FAULT_STATE_FAULT = 0,
    DCM_FAULT_MISSING_HEARTBEAT = 1,
    NUM_DCM_FAULTS = 2,
} DCM_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear a warning.
 */
void App_CanAlerts_SetWarning(DCM_WarningId alert_id, bool set_alert);

/**
 * Set or clear a fault.
 */
void App_CanAlerts_SetFault(DCM_FaultId alert_id, bool set_alert);

/**
 * Return whether or not a specific warning transmitted by this board is set.
 */
bool App_CanAlerts_GetWarning(DCM_WarningId alert_id);

/**
 * Return whether or not a specific fault transmitted by this board is set.
 */
bool App_CanAlerts_GetFault(DCM_FaultId alert_id);

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

