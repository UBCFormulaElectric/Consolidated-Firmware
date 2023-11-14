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
    DIM_ALERT_BOARD = 0,
    DCM_ALERT_BOARD = 1,
    BMS_ALERT_BOARD = 2,
    PDM_ALERT_BOARD = 3,
    FSM_ALERT_BOARD = 4,
} CanAlertBoard;
typedef enum
{
    FSM_StackWaterMarkHighTask1HzWarning = 0,
    FSM_StackWaterMarkHighTask100HzWarning = 1,
    FSM_StackWaterMarkHighTask1kHzWarning = 2,
    FSM_StackWaterMarkHighTaskCanRxWarning = 3,
    FSM_StackWaterMarkHighTaskCanTxWarning = 4,
    FSM_WatchdogTimeoutWarning = 5,
    FSM_TxOverflowWarning = 6,
    FSM_RxOverflowWarning = 7,
    FSM_LeftWheelSpeedOutOfRangeWarning = 8,
    FSM_RightWheelSpeedOutOfRangeWarning = 9,
    FSM_SteeringAngleOCSCWarning = 10,
    FSM_SteeringAngleOutOfRangeWarning = 11,
    FSM_FrontBrakePressureOutOfRangeWarning = 12,
    FSM_RearBrakePressureOutOfRangeWarning = 13,
    FSM_FlowRateOutOfRangeWarning = 14,
    FSM_BrakeAppsDisagreementWarning = 15,
    FSM_AppsDisagreementWarning = 16,
    NUM_FSM_WARNINGS = 17,
} FSM_WarningId;

typedef enum
{
    FSM_MissingHeartbeatFault = 0,
    FSM_StateMachineFault = 1,
    FSM_PappsOCSCFault = 2,
    FSM_SappsOCSCFault = 3,
    FSM_FlowMeterUnderflowFault = 4,
    FSM_TorquePlausabilityFailedFault = 5,
    NUM_FSM_FAULTS = 6,
} FSM_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_StackWaterMarkHighTask1HzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_StackWaterMarkHighTask100HzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_StackWaterMarkHighTask1kHzWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_StackWaterMarkHighTaskCanRxWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_StackWaterMarkHighTaskCanTxWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_WatchdogTimeoutWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_TxOverflowWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_RxOverflowWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_LeftWheelSpeedOutOfRangeWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_RightWheelSpeedOutOfRangeWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_SteeringAngleOCSCWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_SteeringAngleOutOfRangeWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_FrontBrakePressureOutOfRangeWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_RearBrakePressureOutOfRangeWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_FlowRateOutOfRangeWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_BrakeAppsDisagreementWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_AppsDisagreementWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_MissingHeartbeatFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_StateMachineFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_PappsOCSCFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_SappsOCSCFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_FlowMeterUnderflowFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_TorquePlausabilityFailedFault_Set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_WatchdogTimeoutWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_TxOverflowWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_RxOverflowWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_LeftWheelSpeedOutOfRangeWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_RightWheelSpeedOutOfRangeWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_SteeringAngleOCSCWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_SteeringAngleOutOfRangeWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_FrontBrakePressureOutOfRangeWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_RearBrakePressureOutOfRangeWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_FlowRateOutOfRangeWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_BrakeAppsDisagreementWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_AppsDisagreementWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_MissingHeartbeatFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_StateMachineFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_PappsOCSCFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_SappsOCSCFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_FlowMeterUnderflowFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_TorquePlausabilityFailedFault_Get(void);

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

