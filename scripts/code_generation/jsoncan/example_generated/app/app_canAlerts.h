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

typedef struct
{
    uint8_t id;
    char* description;
    char* name;
} FSM_Warning_Warning_Test1_WarningCode;

typedef struct
{
    uint8_t id;
    char* description;
    char* name;
} FSM_Warning_Warning_Test2_WarningCode;

typedef struct
{
    uint8_t id;
    char* description;
    char* name;
} JCT_Warning_Warning_Test_WarningCode;

typedef struct
{
    uint8_t id;
    char* description;
    char* name;
} FSM_Fault_Fault_Test3_FaultCode;


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
uint8_t App_CanAlerts_WarningCode(CanAlertBoard board, uint8_t *alertArray);

/**
 * Return whether or not a board has set a fault.
 */
uint8_t App_CanAlerts_FaultCode(CanAlertBoard board, uint8_t *alertArray);

