#include "app_lowVoltageBattery.h"
#include "io_lowVoltageBattery.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_canTx.h"

void app_lowVoltageBattery_broadcast(void)
{
    app_canTx_VC_BatVoltage_set(io_lowVoltageBattery_getBatVoltage());
    app_canTx_VC_AccVoltage_set(io_lowVoltageBattery_getAccVoltage());
    app_canTx_VC_BoostVoltage_set(io_lowVoltageBattery_getBoostVoltage());

    app_canAlerts_VC_Fault_LvChargeFault_set(io_lowVoltageBattery_hasChargeFault());
    app_canAlerts_VC_Fault_BoostControllerFault_set(io_lowVoltageBattery_hasBoostControllerFault());
}