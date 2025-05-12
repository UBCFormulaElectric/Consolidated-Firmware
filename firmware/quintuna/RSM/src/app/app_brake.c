#include "io_brake.h"
#include "app_brake.h"
#include "app_canTx.h"

void app_brake_broadcast(void)
{
    float pressure = io_brake_getPressure();
    app_canTx_RSM_BrakePressure_set(pressure);
}
