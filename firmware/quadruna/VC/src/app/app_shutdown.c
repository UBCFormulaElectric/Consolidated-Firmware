#include "app_shutdown.h"
#include <stdbool.h>
#include "io_shutdown.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"

void app_shutdown_broadcast(void)
{
    app_canAlerts_VC_Warning_LEStopFault_set(io_shutdown_hasLEStopFault());
    app_canAlerts_VC_Warning_REStopFault_set(io_shutdown_hasREStopFault());
    app_canAlerts_VC_Warning_PCMInterlockFault_set(io_shutdown_hasPcmFault());
    app_canAlerts_VC_Warning_TSMSFault_set(io_shutdown_hasTsmsFault());
}