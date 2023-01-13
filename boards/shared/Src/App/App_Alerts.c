#include "App_Alerts.h"
#include "App_SharedMacros.h"

static uint32_t alerts_data;

void App_Alerts_Init() 
{
    alerts_data = 0U;
}

void App_Alerts_Set(Board_Alert id, bool set)
{   
    if(id > NUM_BOARD_ALERTS)
    {
        return;
    }

    const bool prev_set = IS_BIT_SET(alerts_data, id);
    const bool needs_update = prev_set ^ set;

    if(needs_update)
    {
        if (set) {
            // Set alert bit
            alerts_data = SET_BIT_HIGH(alerts_data, id);
            App_AlertsConfig_SendAlertsSet(id);
        }
        else {
            // Clear alert bit
            alerts_data = SET_BIT_LOW(alerts_data, id);
            App_AlertsConfig_SendAlertsCleared(id);
        }
    }
}

void App_Alerts_Push(Board_Alert id)
{
    if(id > NUM_BOARD_ALERTS)
    {
        return;
    }

    if(!IS_BIT_SET(alerts_data, id))
    {
        App_Alerts_Set(id, true);
        App_Alerts_Set(id, false);
    }
}
