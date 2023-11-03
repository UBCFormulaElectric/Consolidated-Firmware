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
    DCM_ALERT_BOARD = 0,
    DIM_ALERT_BOARD = 1,
    BMS_ALERT_BOARD = 2,
    FSM_ALERT_BOARD = 3,
    PDM_ALERT_BOARD = 4,
} CanAlertBoard;
typedef enum
{
    FSM_Warning_StackWaterMarkHighTask1Hz = 0,
    FSM_Warning_StackWaterMarkHighTask100Hz = 1,
    FSM_Warning_StackWaterMarkHighTask1kHz = 2,
    FSM_Warning_StackWaterMarkHighTaskCanRx = 3,
    FSM_Warning_StackWaterMarkHighTaskCanTx = 4,
    FSM_Warning_WatchdogTimeout = 5,
    FSM_Warning_TxOverflow = 6,
    FSM_Warning_RxOverflow = 7,
    FSM_Warning_LeftWheelSpeedOutOfRange = 8,
    FSM_Warning_RightWheelSpeedOutOfRange = 9,
    FSM_Warning_SteeringAngleOCSC = 10,
    FSM_Warning_SteeringAngleOutOfRange = 11,
    FSM_Warning_FrontBrakePressureOutOfRange = 12,
    FSM_Warning_RearBrakePressureOutOfRangeWarning = 13,
    FSM_Warning_FlowRateOutOfRange = 14,
    FSM_Warning_BrakeAppsDisagreement = 15,
    FSM_Warning_AppsDisagreement = 16,
    NUM_FSM_WARNINGS = 17,
} FSM_WarningId;

typedef enum
{
    FSM_Fault_MissingHeartbeat = 0,
    FSM_Fault_StateMachine = 1,
    FSM_Fault_PappsOCSC = 2,
    FSM_Fault_SappsOCSCFault = 3,
    FSM_Fault_FlowMeterUnderflow = 4,
    FSM_Fault_TorquePlausabilityFailed = 5,
    NUM_FSM_FAULTS = 6,
} FSM_FaultId;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_StackWaterMarkHighTask1Hz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_StackWaterMarkHighTask100Hz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_StackWaterMarkHighTask1kHz_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_StackWaterMarkHighTaskCanRx_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_StackWaterMarkHighTaskCanTx_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_WatchdogTimeout_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_TxOverflow_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_RxOverflow_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_LeftWheelSpeedOutOfRange_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_RightWheelSpeedOutOfRange_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_SteeringAngleOCSC_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_SteeringAngleOutOfRange_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_FrontBrakePressureOutOfRange_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_RearBrakePressureOutOfRangeWarning_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_FlowRateOutOfRange_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_BrakeAppsDisagreement_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Warning_AppsDisagreement_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Fault_MissingHeartbeat_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Fault_StateMachine_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Fault_PappsOCSC_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Fault_SappsOCSCFault_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Fault_FlowMeterUnderflow_Set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void App_CanAlerts_FSM_Fault_TorquePlausabilityFailed_Set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_WatchdogTimeout_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_TxOverflow_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_RxOverflow_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_LeftWheelSpeedOutOfRange_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_RightWheelSpeedOutOfRange_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_SteeringAngleOCSC_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_SteeringAngleOutOfRange_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_FrontBrakePressureOutOfRange_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_RearBrakePressureOutOfRangeWarning_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_FlowRateOutOfRange_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_BrakeAppsDisagreement_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Warning_AppsDisagreement_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Fault_MissingHeartbeat_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Fault_StateMachine_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Fault_PappsOCSC_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Fault_SappsOCSCFault_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Fault_FlowMeterUnderflow_Get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool App_CanAlerts_FSM_Fault_TorquePlausabilityFailed_Get(void);

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

