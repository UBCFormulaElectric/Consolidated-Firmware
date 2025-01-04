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
    CRIT_ALERT_BOARD = 1,
    FSM_ALERT_BOARD = 2,
    RSM_ALERT_BOARD = 3,
    VC_ALERT_BOARD = 4,
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
void app_canAlerts_BMS_Warning_StackWaterMarkHighTask1Hz_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_StackWaterMarkHighTask100Hz_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_StackWaterMarkHighTask1kHz_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_StackWaterMarkHighTaskCanRx_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_StackWaterMarkHighTaskCanTx_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_WatchdogTimeout_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_TxOverflow_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_RxOverflow_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_OpenWireCheckFault_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_OpenWireCheck_Segment0_GND_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_OpenWireCheck_Segment1_GND_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_OpenWireCheck_Segment2_GND_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_OpenWireCheck_Segment3_GND_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_OpenWireCheck_Segment4_GND_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Warning_MissingVCHeartbeat_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_StateMachine_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_CellUndervoltage_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_CellOvervoltage_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_ModuleCommunicationError_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_CellUndertemp_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_CellOvertemp_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_ChargerReportedError_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_ChargerShutdownLoopOpen_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_set(bool set_alert);

/**
 * Set or clear an alert for this board.
 */
void app_canAlerts_BMS_Fault_PrechargeFailure_set(bool set_alert);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_StackWaterMarkHighTask1Hz_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_StackWaterMarkHighTask100Hz_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_StackWaterMarkHighTask1kHz_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_StackWaterMarkHighTaskCanRx_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_StackWaterMarkHighTaskCanTx_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_WatchdogTimeout_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_TxOverflow_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_RxOverflow_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_OpenWireCheckFault_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_OpenWireCheck_Segment0_GND_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_OpenWireCheck_Segment1_GND_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_OpenWireCheck_Segment2_GND_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_OpenWireCheck_Segment3_GND_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_OpenWireCheck_Segment4_GND_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Warning_MissingVCHeartbeat_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_StateMachine_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_CellUndervoltage_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_CellOvervoltage_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_ModuleCommunicationError_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_CellUndertemp_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_CellOvertemp_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_ChargerReportedError_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_ChargerShutdownLoopOpen_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_TractiveSystemOvercurrent_get(void);

/**
 * Return whether or not a specific alert transmitted by this board is set.
 */
bool app_canAlerts_BMS_Fault_PrechargeFailure_get(void);

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
