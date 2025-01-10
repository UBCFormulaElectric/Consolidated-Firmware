/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "app_canRx.h"

/* ------------------------------- Structs -------------------------------- */

/**
 * Struct for holding all messages received by  (i.e. the RX table).
 */
typedef struct
{
    VC_Vitals_Signals VC_Vitals_signals;
} BMS_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static BMS_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canRx_init()
{
    memset(&rx_table, 0, sizeof(BMS_RxMsgs));
    // Initialize all signals to Start-up value
    app_canRx_VC_Heartbeat_update(CANSIG_VC_HEARTBEAT_START_VAL);
    app_canRx_VC_State_update(CANSIG_VC_STATE_START_VAL);
    app_canRx_VC_BuzzerOn_update(CANSIG_VC_BUZZER_ON_START_VAL);
    app_canRx_VC_NumberOfCanDataLogs_update(CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_START_VAL);
    app_canRx_VC_CanLoggingRemainingErrors_update(CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_START_VAL);
    
}

void app_canRx_BRUSA_IsConnected_update(bool value)
{
    rx_table.BRUSA_Vitals_signals.BRUSA_IsConnected_value = value;
}

void app_canRx_VC_Heartbeat_update(bool value)
{
    tx_table.VC_Vitals_signals.VC_Heartbeat_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_HEARTBEAT_MAX_VAL)
    {
        value = CANSIG_VC_HEARTBEAT_MAX_VAL;
    }
    if (value < CANSIG_VC_HEARTBEAT_MIN_VAL)
    {
        value = CANSIG_VC_HEARTBEAT_MIN_VAL;
    }
}
void app_canRx_VC_State_update(VcState value)
{
    tx_table.VC_Vitals_signals.VC_State_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_STATE_MAX_VAL)
    {
        value = CANSIG_VC_STATE_MAX_VAL;
    }
    if (value < CANSIG_VC_STATE_MIN_VAL)
    {
        value = CANSIG_VC_STATE_MIN_VAL;
    }
}
void app_canRx_VC_BuzzerOn_update(bool value)
{
    tx_table.VC_Vitals_signals.VC_BuzzerOn_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_BUZZER_ON_MAX_VAL)
    {
        value = CANSIG_VC_BUZZER_ON_MAX_VAL;
    }
    if (value < CANSIG_VC_BUZZER_ON_MIN_VAL)
    {
        value = CANSIG_VC_BUZZER_ON_MIN_VAL;
    }
}
void app_canRx_VC_NumberOfCanDataLogs_update(uint32_t value)
{
    tx_table.VC_Vitals_signals.VC_NumberOfCanDataLogs_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_MAX_VAL)
    {
        value = CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_MAX_VAL;
    }
    if (value < CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_MIN_VAL)
    {
        value = CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_MIN_VAL;
    }
}
void app_canRx_VC_CanLoggingRemainingErrors_update(uint32_t value)
{
    tx_table.VC_Vitals_signals.VC_CanLoggingRemainingErrors_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_MAX_VAL)
    {
        value = CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_MAX_VAL;
    }
    if (value < CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_MIN_VAL)
    {
        value = CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_MIN_VAL;
    }
}




bool app_canRx_VC_Heartbeat_get()
{
    return rx_table.VC_Vitals_signals.VC_Heartbeat_value;
}
VcState app_canRx_VC_State_get()
{
    return rx_table.VC_Vitals_signals.VC_State_value;
}
bool app_canRx_VC_BuzzerOn_get()
{
    return rx_table.VC_Vitals_signals.VC_BuzzerOn_value;
}
uint32_t app_canRx_VC_NumberOfCanDataLogs_get()
{
    return rx_table.VC_Vitals_signals.VC_NumberOfCanDataLogs_value;
}
uint32_t app_canRx_VC_CanLoggingRemainingErrors_get()
{
    return rx_table.VC_Vitals_signals.VC_CanLoggingRemainingErrors_value;
}



