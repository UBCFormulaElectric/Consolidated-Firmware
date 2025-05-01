#include "app_allStates.h"
#include "app_sbgEllipse.h"
#include "io_sbgEllipse.h"

void app_allStates_runOnTick100Hz(void)
{
    io_sbgEllipse_handleLogs();
    app_sbgEllipse_broadcast();
}

void app_allStates_runOnTick1Hz(void) {}
