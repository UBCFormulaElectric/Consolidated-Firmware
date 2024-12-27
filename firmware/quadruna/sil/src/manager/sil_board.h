#pragma once
#include <stdlib.h>
#include <czmq.h>

// Wrapper around a board to put in the loop.
typedef struct sil_Board
{
    const char *name;
    const char *binPath;
    uint32_t    timeMs;
    pid_t       pid;
} sil_Board;

// Runs the binary of a given board in a parallel process.
// Gets a czmq poller to verify the board started up, verified over the ready topic.
void sil_board_run(sil_Board *board, zpoller_t *pollerRx);

// Reset the relevant fields in the board, and kill the process.
void sil_board_reset(sil_Board *board);

// Create a new sil_Board.
sil_Board sil_board_new(const char *name, const char *binPath);