/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include <stdbool.h>
#include "app_canUtils.h"

/* -------------------------------- Enums --------------------------------- */

typedef enum
{
    FSM_ALERT_BOARD = 0,
    JCT_ALERT_BOARD = 1,
} CanAlertBoard;
typedef enum
{
    JCT_Warning_Warning_Test = 0,
    NUM_JCT_WARNINGS = 1,
} JCT_WarningId;

typedef enum
{
    NUM_JCT_FAULTS = 0,
} JCT_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_JCT_Warning_Warning_Test_set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_JCT_Warning_Warning_Test_get(void);

/**
 * Return whether or not a board has set a warning.
 */
bool app_canAlerts_BoardHasWarning(CanAlertBoard board);

/**
 * Return whether or not a board has set a fault.
 */
bool app_canAlerts_BoardHasFault(CanAlertBoard board);

/**
 * Return whether or not any board has set a warning.
 */
bool app_canAlerts_AnyBoardHasWarning(void);

/**
 * Return whether or not any board has set a fault.
 */
bool app_canAlerts_AnyBoardHasFault(void);

