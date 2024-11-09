#include "io_loadCell.h"
#include <stdlib.h>
#include <assert.h>

extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "app_loadCell.h
}

namespace app::loadCell
{
void broadcast()
{
    app_canTx_RSM_LoadCell3_set(io::loadCell::getMechanicalLoad3());
    app_canTx_RSM_LoadCell4_set(io::loadCell::getMechanicalLoad4());
    app_canAlerts_RSM_Warning_LoadCell3OCSC_set(io::loadCell::sensor3OCSC());
    app_canAlerts_RSM_Warning_LoadCell4OCSC_set(io::loadCell::sensor4OCSC());
}
} // namespace app::loadCell
