#include "io_fsmShdn.hpp"
extern "C"
{
#include "app_shdnLoopNode.h"
#include "app_canTx.h"
}

namespace app::shdnLoop{
    static const BoardShdnNode bots_node    = { io::fsmShdn::BOTS_SHDN_OK_get, app_canTx_FSM_BOTSOKStatus_set };
    static const BoardShdnNode cockpit_node = { io::fsmShdn::COCKPIT_SHDN_OK_get, app_canTx_FSM_COCKPITOKStatus_set };
    static const BoardShdnNode fl_node      = { io::fsmShdn::FL_SHDN_OK_get, app_canTx_FSM_FrontLeftILCKInertiaOKStatus_set };
    static const BoardShdnNode fr_node      = { io::fsmShdn::FR_SHDN_OK_get, app_canTx_FSM_FrontRightILCKOKStatus_set };

    void broadcast(void)
    {
        app_shdnLoopNode_broadcast(&bots_node);
        app_shdnLoopNode_broadcast(&cockpit_node);
        app_shdnLoopNode_broadcast(&fl_node);
        app_shdnLoopNode_broadcast(&fr_node);
    }
}
