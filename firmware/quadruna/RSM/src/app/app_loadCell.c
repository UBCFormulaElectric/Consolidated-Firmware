#include "io_loadCell.h"
#include <stdlib.h>
#include <assert.h>
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "app_loadCell.h"

void app_loadcell_broadcast()
{
    app_canTx_RSM_LoadCell3_set(io_loadCell_getMechanicalLoad3());
    app_canTx_RSM_LoadCell4_set(io_loadCell_getMechanicalLoad4());
    app_canAlerts_RSM_Warning_LoadCell3OCSC_set(io_loadCell_sensor3OCSC());
    app_canAlerts_RSM_Warning_LoadCell3OCSC_set(io_loadCell_sensor4OCSC());
}
