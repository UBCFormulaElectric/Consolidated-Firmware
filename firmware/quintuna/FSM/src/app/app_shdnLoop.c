#include "app_shdnLoopNode.h"
#include "io_fsmShdn.h"
#include "app_canTx.h"

static const BoardShdnNode bots_node    = { &io_fsmShdn_BOTS_SHDN_OK_get,
                                            &app_canTx_FSM_Fault_DummyFault_set }; // TODO: fix app_canTX <- this one
static const BoardShdnNode cockpit_node = { &io_fsmShdn_COCKPIT_SHDN_OK_get,
                                            &app_canTx_FSM_Fault_DummyFault_set }; // TODO: fix app_canTX <- this one
static const BoardShdnNode fl_node      = { &io_fsmShdn_FL_SHDN_OK_get,
                                            &app_canTx_FSM_Fault_DummyFault_set }; // TODO: fix app_canTX <- this one
static const BoardShdnNode fr_node      = { &io_fsmShdn_FR_SHDN_OK_get,
                                            &app_canTx_FSM_Fault_DummyFault_set }; // TODO: fix app_canTX <- this one

void app_shdnLoop_broadcast(void)
{
    app_shdnLoopNode_broadcast(&bots_node);
    app_shdnLoopNode_broadcast(&cockpit_node);
    app_shdnLoopNode_broadcast(&fl_node);
    app_shdnLoopNode_broadcast(&fr_node);
}
