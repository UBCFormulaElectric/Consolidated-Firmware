#include "app_driveMode.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_driveMode.h"

void app_driveMode_broadcast(void)
{
    uint16_t driveMode = io_driveMode_readPins();

    app_canTx_CRIT_DriveMode_set(driveMode);
}
