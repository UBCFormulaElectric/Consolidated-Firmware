#include "app_shdnLoop.h"
#include "app_canTx.h"

static BoardShdnNode *board_nodes; // length of this?
static int            length;

void app_shdn_loop_init(BoardShdnNode *nodes, int num_nodes)
{
    for (int i = 0; i < length; i++)
    {
        board_nodes[i] = nodes[i];
    }
}

void app_shdn_loop_broadcast()
{
    for (int i = 0; i < length; i++)
    {
        const bool state = board_nodes[i].get_shutdown_state();
        board_nodes[i].shdn_can_ptr(state);
    }
}