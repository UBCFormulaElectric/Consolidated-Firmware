#include "app_shdnLoopNode.h"

#include "io_fsmShdn.h"
#include "app_canTx.h"

static const BoardShdnNode bots_node    = { io_fsmShdn_BOTS_SHDN_OK_get, app_canTx_FSM_BOTSOKStatus_set };
static const BoardShdnNode cockpit_node = { io_fsmShdn_COCKPIT_SHDN_OK_get, app_canTx_FSM_COCKPITOKStatus_set };
static const BoardShdnNode fl_node      = { io_fsmShdn_FL_SHDN_OK_get, app_canTx_FSM_FrontLeftILCKInertiaOKStatus_set };
static const BoardShdnNode fr_node      = { io_fsmShdn_FR_SHDN_OK_get, app_canTx_FSM_FrontRightILCKOKStatus_set };

void app_shdnLoop_broadcast(void)
{
    app_shdnLoopNode_broadcast(&bots_node);
    app_shdnLoopNode_broadcast(&cockpit_node);
    app_shdnLoopNode_broadcast(&fl_node);
    app_shdnLoopNode_broadcast(&fr_node);
}
