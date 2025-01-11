#include "app_shdnLoopNode.h"
#include "app_shdnLoop.h"
#include "io_vcShdn.h"
#include "app_canTx.h"

static const BoardShdnNode tsms_node    = { io_vcShdn_TsmsFault_get, &app_canTx_VC_TSMSOKStatus_set };
static const BoardShdnNode le_stop_node = { io_vcShdn_LEStopFault_get, &app_canTx_VC_LEStopOKStatus_set };
static const BoardShdnNode re_stop_node = { io_vcShdn_REStopFault_get, &app_canTx_VC_REStopOKStatus_set };
static const BoardShdnNode splitter_box_interlock_node = { io_vcShdn_SplitterBoxInterlockFault_get,
                                                           &app_canTx_VC_SplitterBoxInterlockOKStatus_set };

void app_shdnLoop_broadcast(void)
{
    app_shdnLoopNode_broadcast(&tsms_node);
    app_shdnLoopNode_broadcast(&le_stop_node);
    app_shdnLoopNode_broadcast(&re_stop_node);
    app_shdnLoopNode_broadcast(&splitter_box_interlock_node);
}
