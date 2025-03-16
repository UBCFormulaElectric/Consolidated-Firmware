#pragma once

#include <stdbool.h>

typedef struct
{
    bool (*const get_shutdown_state)(void);
    void (*const shdn_can_ptr)(bool);
} BoardShdnNode;

// Broadcast shutdown node statuses over CAN.
void app_shdnLoopNode_broadcast(const BoardShdnNode *node);