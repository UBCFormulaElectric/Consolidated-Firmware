#pragma once
#include <stdlib.h>
#include <czmq.h>

// Wrapper around a board to put in the loop.
typedef struct sil_Board
{
    const char *name;
    const char *binPath;
    uint32_t    timeMs;
} sil_Board;

// Runs the binary of a given board in a parallel process.
// Gets a czmq poller to verify the board started up, verified over the "ready" topic.
// Returns the pid of the forked process.
pid_t sil_board_run(sil_Board *board, zpoller_t *pollerRx);
