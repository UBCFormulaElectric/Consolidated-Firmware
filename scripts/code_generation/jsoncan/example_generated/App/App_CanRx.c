/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "App_CanRx.h"

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

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(JCT_RxMsgs));
    App_CanRx_FSM_PappsMappedPedalPercentage_Update(CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    App_CanRx_FSM_SappsMappedPedalPercentage_Update(CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    App_CanRx_FSM_Warning_Warning_Test1_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST1_START_VAL);
    App_CanRx_FSM_Warning_Warning_Test2_Update(CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST2_START_VAL);
    App_CanRx_FSM_Fault_Fault_Test3_Update(CANSIG_FSM_FAULTS_FSM_FAULT_FAULT_TEST3_START_VAL);
}

void App_CanRx_FSM_PappsMappedPedalPercentage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FSM_Apps_signals.FSM_PappsMappedPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanRx_FSM_SappsMappedPedalPercentage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FSM_Apps_signals.FSM_SappsMappedPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanRx_FSM_Warning_Warning_Test1_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_Warning_Test1_value = value;
}

void App_CanRx_FSM_Warning_Warning_Test2_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_Warning_Warning_Test2_value = value;
}

void App_CanRx_FSM_Fault_Fault_Test3_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_Fault_Test3_value = value;
}

float App_CanRx_FSM_PappsMappedPedalPercentage_Get()
{
    return rx_table.FSM_Apps_signals.FSM_PappsMappedPedalPercentage_value;
}

float App_CanRx_FSM_SappsMappedPedalPercentage_Get()
{
    return rx_table.FSM_Apps_signals.FSM_SappsMappedPedalPercentage_value;
}

bool App_CanRx_FSM_Warning_Warning_Test1_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_Warning_Test1_value;
}

bool App_CanRx_FSM_Warning_Warning_Test2_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_Warning_Warning_Test2_value;
}

bool App_CanRx_FSM_Fault_Fault_Test3_Get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_Fault_Test3_value;
}

