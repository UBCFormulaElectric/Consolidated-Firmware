#include "app_shdnLoop.h"

static const BoardShdnNode *board_nodes;
static int                  length;

void app_shdn_loop_init(const BoardShdnNode *nodes, int num_nodes)
{
    length      = num_nodes;
    board_nodes = nodes;
}

void app_shdn_loop_broadcast(void)
{
    for (int i = 0; i < length; i++)
    {
        const bool state = board_nodes[i].get_shutdown_state();
        board_nodes[i].shdn_can_ptr(state);
    }
}
