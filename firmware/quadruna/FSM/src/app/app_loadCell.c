#include "app_loadCell.h"
#include "io_loadCell.h"
#include "app_canTx.h"
#include "app_canAlerts.h"

void app_loadCell_broadcast()
{
    app_canTx_FSM_LoadCell1_set(io_loadCell_getMechanicalLoad1());
    app_canTx_FSM_LoadCell2_set(io_loadCell_getMechanicalLoad2());

    bool load_cell_1_ocsc = io_loadCell_sensor1OCSC();
    bool load_cell_2_ocsc = io_loadCell_sensor2OCSC();

    if (load_cell_1_ocsc)
    {
        app_canTx_FSM_LoadCell1_set(0);
    }

    if (load_cell_2_ocsc)
    {
        app_canTx_FSM_LoadCell2_set(0);
    }

    app_canAlerts_FSM_Warning_LoadCell1OCSC_set(load_cell_1_ocsc);
    app_canAlerts_FSM_Warning_LoadCell2OCSC_set(load_cell_2_ocsc);
}