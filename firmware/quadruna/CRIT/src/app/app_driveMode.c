#include "app_driveMode.h"
#include "io_driveMode.h"
#include "app_canTx.h"

void app_driveMode_broadcast(void)
{
    uint16_t driveMode = io_driveMode_readPins();

    app_canTx_CRIT_DriveMode_set(driveMode);
}
