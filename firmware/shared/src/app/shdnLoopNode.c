#include "app/shdnLoopNode.h"

void app_shdnLoopNode_broadcast(const BoardShdnNode *node)
{
    node->shdn_can_ptr(node->get_shutdown_state());
}
