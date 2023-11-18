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
    NUM_shared_WARNINGS = 0,
} shared_WarningId;

typedef enum
{
    NUM_shared_FAULTS = 0,
} shared_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

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

