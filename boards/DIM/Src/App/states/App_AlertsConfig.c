#include "App_AlertsConfig.h"
#include "App_CanTx.h"
#include "Io_CanTx.h"

void App_AlertsConfig_SendAlertsSet(uint32_t data)
{
    App_CanTx_DIM_AlertSet_DIM_AlertSet_Set(data);
    App_CanTx_DIM_AlertSet_SendAperiodic();
}

void App_AlertsConfig_SendAlertsCleared(uint32_t data)
{
    App_CanTx_DIM_AlertCleared_DIM_AlertCleared_Set(data);
    App_CanTx_DIM_AlertCleared_SendAperiodic();
}