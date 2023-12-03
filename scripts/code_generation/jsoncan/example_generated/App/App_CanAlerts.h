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
    FSM_Fault_Fault_Test3 = 2000,
} FSM_FaultCode;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_JCT_Warning_Warning_Test_Set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_JCT_Warning_Warning_Test_Get(void);

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

/**
 * Return whether or not a board has set a warning.
 */
void App_CanAlerts_WarningCode(CanAlertBoard board, uint8_t *alertArray, uint8_t elementNum, uint8_t *p);

/**
 * Return whether or not a board has set a fault.
 */
void App_CanAlerts_FaultCode(CanAlertBoard board, uint8_t *alertArray, uint8_t elementNum, uint8_t *p);

