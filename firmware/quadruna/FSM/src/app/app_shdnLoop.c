#include "app_shdnLoop.h"
#include "app_shdnLoopNode.h"
#include "io_fsmShdn.h"
#include "app_canTx.h"

static const BoardShdnNode bots_node = { &io_fsmShdn_FSM_SHDN_OK_get, &app_canTx_FSM_BOTSOKStatus_set };

void app_shdnLoop_broadcast(void) {
    app_shdnLoopNode_broadcast(&bots_node);
}
