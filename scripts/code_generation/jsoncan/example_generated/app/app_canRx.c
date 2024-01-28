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
 * Struct for holding all messages received by JCT (i.e. the RX table).
 */
typedef struct
{
    FSM_Apps_Signals FSM_Apps_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
} JCT_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static JCT_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canRx_init()
{
    memset(&rx_table, 0, sizeof(JCT_RxMsgs));
    app_canRx_FSM_PappsMappedPedalPercentage_update(CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    app_canRx_FSM_SappsMappedPedalPercentage_update(CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    app_canRx_FSM_Warning_Warning_Test1_update(CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST1_START_VAL);
    app_canRx_FSM_Warning_Warning_Test2_update(CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST2_START_VAL);
    app_canRx_FSM_Fault_Fault_Test3_update(CANSIG_FSM_FAULTS_FSM_FAULT_FAULT_TEST3_START_VAL);
}

void app_canRx_FSM_PappsMappedPedalPercentage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FSM_Apps_signals.FSM_PappsMappedPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX : tmp;
}

void app_canRx_FSM_SappsMappedPedalPercentage_update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FSM_Apps_signals.FSM_SappsMappedPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX : tmp;
}

void app_canRx_FSM_Warning_Warning_Test1_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_Warning_Test1_value = value;
}

void app_canRx_FSM_Warning_Warning_Test2_update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_Warning_Test2_value = value;
}

void app_canRx_FSM_Fault_Fault_Test3_update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_Fault_Test3_value = value;
}

float app_canRx_FSM_PappsMappedPedalPercentage_get()
{
    return rx_table.FSM_Apps_signals.FSM_PappsMappedPedalPercentage_value;
}

float app_canRx_FSM_SappsMappedPedalPercentage_get()
{
    return rx_table.FSM_Apps_signals.FSM_SappsMappedPedalPercentage_value;
}

bool app_canRx_FSM_Warning_Warning_Test1_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_Warning_Test1_value;
}

bool app_canRx_FSM_Warning_Warning_Test2_get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_Warning_Test2_value;
}

bool app_canRx_FSM_Fault_Fault_Test3_get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_Fault_Test3_value;
}

