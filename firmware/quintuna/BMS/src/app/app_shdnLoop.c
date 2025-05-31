#include "app_shdnLoop.h"
#include "app_shdnLoopNode.h"
#include "io_bmsShdn.h"
#include "app_canTx.h"

static const BoardShdnNode hv_p_ok_node = { &io_bmsShdn_hv_p_intlck_sns_pin_get, &app_canTx_BMS_HVPShdnOKStatus_set };
static const BoardShdnNode hv_n_ok_node = { &io_bmsShdn_hv_n_intlck_sns_pin_get, &app_canTx_BMS_HVNShdnOKStatus_set };

void app_shdnLoop_broadcast(void)
{
    app_shdnLoopNode_broadcast(&hv_p_ok_node);
    app_shdnLoopNode_broadcast(&hv_n_ok_node);
}
