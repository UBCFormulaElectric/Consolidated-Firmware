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
    FSM_Warning_Warning_Test1 = 2059,
    FSM_Warning_Warning_Test2 = 2060,
} FSM_WarningCode;

typedef enum
{
    JCT_Warning_Warning_Test = 20000,
} JCT_WarningCode;

typedef enum
{
    FSM_Fault_Fault_Test3 = 2000,
} FSM_FaultCode;


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

/**
 * Return whether or not a board has set a warning.
 */
void App_CanAlerts_WarningCode(CanAlertBoard board, uint8_t *alertArray, uint8_t *p);

/**
 * Return whether or not a board has set a fault.
 */
void App_CanAlerts_FaultCode(CanAlertBoard board, uint8_t *alertArray, uint8_t *p);

