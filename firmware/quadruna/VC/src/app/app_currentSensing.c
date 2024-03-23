#include "app_currentSensing.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_currentSensing.h"

void app_currentSensing_broadcast(void)
{
    app_canAlerts_VC_Fault_AccumulatorFault_set(io_currentSensing_hasAccumulatorFault());
    app_canAlerts_VC_Fault_BatteryFault_set(io_currentSensing_hasBatteryFault());
    app_canTx_VC_AccCurrent_set(io_currentSensing_getAccumulatorCurrent());
    app_canTx_VC_BatCurrent_set(io_currentSensing_getBatteryCurrent());
}