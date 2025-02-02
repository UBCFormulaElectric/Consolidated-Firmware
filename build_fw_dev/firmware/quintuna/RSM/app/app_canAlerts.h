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
    BMS_ALERT_BOARD = 0,
    FSM_ALERT_BOARD = 1,
    CRIT_ALERT_BOARD = 2,
    VC_ALERT_BOARD = 3,
    RSM_ALERT_BOARD = 4,
} CanAlertBoard;

typedef struct
{
    char* description;
    char* name;
    uint16_t id;
} Fault_Warning_Info;

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_RSM_Fault_DummyFault_set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_RSM_Fault_DummyFault_get(void);

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
uint8_t app_canAlerts_WarningInfo(Fault_Warning_Info *alert_array);

/**
 * Return whether or not a board has set a fault.
 */
uint8_t app_canAlerts_FaultInfo(Fault_Warning_Info *alert_array);

