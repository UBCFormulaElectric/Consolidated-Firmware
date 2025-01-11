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
    VC_ALERT_BOARD = 2,
    
} CanAlertBoard;

typedef struct
{
    char* description;
    char* name;
    uint16_t id;

} Fault_Warning_Info;

/* ------------------------- Function Prototypes -------------------------- */


/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_StackWaterMarkHighTask1Hz_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_StackWaterMarkHighTask100Hz_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_StackWaterMarkHighTask1kHz_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_StackWaterMarkHighTaskCanRx_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_StackWaterMarkHighTaskCanTx_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_WatchdogTimeout_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_TxOverflow_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_RxOverflow_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_RegenNotAvailable_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_ImuInitFailed_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_FlowRateInputOutOfRange_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_BatteryRailOvercurrentFault_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_AccumulatorRailOvercurrentFault_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_LvChargeFault_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_BoostControllerFault_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_HighNumberOfCanDataLogs_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_CanLoggingSdCardNotPresent_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_CanLoggingErrored_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_BrakeAppsDisagreement_set(bool set_alert);

/**
 * Set or clear an warning for VC board.
 */
void app_canAlerts_VC_Warning_SbgInitFailed_set(bool set_alert);




/**
 * Set or clear an fault for VC board.
 */
void app_canAlerts_VC_Fault_MissingBMSHeartbeat_set(bool set_alert);

/**
 * Set or clear an fault for VC board.
 */
void app_canAlerts_VC_Fault_MissingFSMHeartbeat_set(bool set_alert);

/**
 * Set or clear an fault for VC board.
 */
void app_canAlerts_VC_Fault_MissingRSMHeartbeat_set(bool set_alert);

/**
 * Set or clear an fault for VC board.
 */
void app_canAlerts_VC_Fault_MissingCRITHeartbeat_set(bool set_alert);

/**
 * Set or clear an fault for VC board.
 */
void app_canAlerts_VC_Fault_LeftInverterFault_set(bool set_alert);

/**
 * Set or clear an fault for VC board.
 */
void app_canAlerts_VC_Fault_RightInverterFault_set(bool set_alert);




/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_StackWaterMarkHighTask1Hz_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_StackWaterMarkHighTask100Hz_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_StackWaterMarkHighTask1kHz_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_StackWaterMarkHighTaskCanRx_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_StackWaterMarkHighTaskCanTx_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_WatchdogTimeout_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_TxOverflow_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_RxOverflow_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_RegenNotAvailable_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_ImuInitFailed_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_FlowRateInputOutOfRange_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_BatteryRailOvercurrentFault_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_AccumulatorRailOvercurrentFault_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_LvChargeFault_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_BoostControllerFault_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_HighNumberOfCanDataLogs_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_CanLoggingSdCardNotPresent_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_CanLoggingErrored_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_BrakeAppsDisagreement_get(void);

/**
 * Return whether or not a specific warning transmitted by VC is set.
 */
bool app_canAlerts_VC_Warning_SbgInitFailed_get(void);



/**
 * Return whether or not a specific fault transmitted by VC is set.
 */
bool app_canAlerts_VC_Fault_MissingBMSHeartbeat_get(void);

/**
 * Return whether or not a specific fault transmitted by VC is set.
 */
bool app_canAlerts_VC_Fault_MissingFSMHeartbeat_get(void);

/**
 * Return whether or not a specific fault transmitted by VC is set.
 */
bool app_canAlerts_VC_Fault_MissingRSMHeartbeat_get(void);

/**
 * Return whether or not a specific fault transmitted by VC is set.
 */
bool app_canAlerts_VC_Fault_MissingCRITHeartbeat_get(void);

/**
 * Return whether or not a specific fault transmitted by VC is set.
 */
bool app_canAlerts_VC_Fault_LeftInverterFault_get(void);

/**
 * Return whether or not a specific fault transmitted by VC is set.
 */
bool app_canAlerts_VC_Fault_RightInverterFault_get(void);


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
