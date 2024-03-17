#include "io_lowVoltageBattery.h"
#include "app_canRx.h"
#include "app_canTx.h"

void app_lowVoltageBattery_broadcast(void) {
    app_canTx_VC_BatVoltage_set(io_lowVoltageBattery_getBatVoltage());
    app_canTx_VC_AccVoltage_set(io_lowVoltageBattery_getAccVoltage());
    app_canTx_VC_BoostVoltage_set(io_lowVoltageBattery_getBoostVoltage());
}