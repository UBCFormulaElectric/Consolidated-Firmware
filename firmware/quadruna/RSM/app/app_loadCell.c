#include "io_loadCell.h"
#include <stdlib.h>
#include <assert.h>
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "app_loadCell.h"

void app_loadcell_broadcast()
{
    app_canTx_RSM_LoadCell3_set();
    app_canTx_RSM_LoadCell4_set();

    const bool loadcell3_OCSC = io_loadCell3_OCSC();
    app_canAlerts_RSM_Warning_LoadCell3OCSC_set(loadcell3_OCSC);

    const bool loadcell4_OCSC = io_loadCell4_OCSC();
    app_canAlerts_RSM_Warning_LoadCell3OCSC_set(loadcell4_OCSC);
}
