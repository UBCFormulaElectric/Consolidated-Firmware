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
    FSM_ALERT_BOARD = 0,
    JCT_ALERT_BOARD = 1,
} CanAlertBoard;
typedef enum
{
    JCT_TEST = 0,
    NUM_JCT_WARNINGS = 1,
} JCT_WarningId;

typedef enum
{
    NUM_JCT_FAULTS = 0,
} JCT_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear a warning.
 */
void App_CanAlerts_SetWarning(JCT_WarningId alert_id, bool set_alert);

/**
 * Set or clear a fault.
 */
void App_CanAlerts_SetFault(JCT_FaultId alert_id, bool set_alert);

/**
 * Return whether or not a specific warning transmitted by this board is set.
 */
bool App_CanAlerts_GetWarning(JCT_WarningId alert_id);

/**
 * Return whether or not a specific fault transmitted by this board is set.
 */
bool App_CanAlerts_GetFault(JCT_FaultId alert_id);

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

