#include "app_shdnLoop.h"
#include "app_shdnLoopNode.h"
#include "io_bmsShdn.h"
#include "app_canTx.h"

static const BoardShdnNode ts_ilck_ok_node = { &io_bmsShdn_TS_ILCK_OK_get, &app_canTx_BMS_TSIlckOKStatus_set };
static const BoardShdnNode hvd_ok_node     = { &io_bmsShdn_HVD_OK_get, &app_canTx_BMS_HVDShdnOKStatus_set };

void app_shdnLoop_broadcast(void) {
    app_shdnLoopNode_broadcast(&ts_ilck_ok_node);
    app_shdnLoopNode_broadcast(&hvd_ok_node);
}
