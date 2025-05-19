#include "app_allStates.h"
#include "app_pumpControl.h"
#include "app_powerManager.h"

void app_allStates_runOnTick100Hz(void)
{
    app_powerManager_EfuseProtocolTick_100Hz();
    app_pumpControl_MonitorPumps();
}

void app_allStates_runOnTick1Hz(void) {}
