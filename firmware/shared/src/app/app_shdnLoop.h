#pragma once

#include <stdbool.h>

typedef struct
{
    bool (*const get_shutdown_state)(void);
    void (*const shdn_can_ptr)(bool);
} BoardShdnNode;

void app_shdn_loop_init(const BoardShdnNode nodes[], int num_nodes);

// Broadcast shutdown node statuses over CAN.
void app_shdn_loop_broadcast();