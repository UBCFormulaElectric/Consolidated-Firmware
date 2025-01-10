#include "app_shdnLoop.h"
#include "app_shdnLoopNode.h"
#include "io_critShdn.h"
#include "app_canTx.h"

static const BoardShdnNode inertia_sen_node   = { &io_critShdn_get_INERTIA_SEN_OK_get,
                                                  &app_canTx_CRIT_InertiaSenOKStatus_set };
static const BoardShdnNode cockpit_estop_node = { &io_critShdn_COCKPIT_ESTOP_OK_get,
                                                  &app_canTx_CRIT_CockpitEStopOKStatus_set };

void app_shdnLoop_broadcast(void) {
    app_shdnLoopNode_broadcast(&inertia_sen_node);
    app_shdnLoopNode_broadcast(&cockpit_estop_node);
}
