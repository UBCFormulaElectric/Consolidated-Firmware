#include "app_loadCell.h"
#include "io_loadCell.h"
#include "app_canTx.h"
#include "app_canAlerts.h"

void app_loadCell_broadcast()
{
    app_canTx_FSM_LoadCell1_set(io_loadCell_getMechanicalLoad1());
    app_canTx_FSM_LoadCell2_set(io_loadCell_getMechanicalLoad2());

    app_canAlerts_FSM_Warning_LoadCell1OCSC_set(io_loadCell_sensor1OCSC());
    app_canAlerts_FSM_Warning_LoadCell2OCSC_set(io_loadCell_sensor2OCSC());
}