#pragma once
#include <czmq.h>
#include "sil_board.h"

// Resets the manager to a clean state, by freeing up allocated memory, and killing child processes.
// Passing NULL to this function skips killing boards.
// NOTE: boardsToKill must be NULL terminated.
void sil_manager_reset(sil_Board *boardsToKill[]);

// Starts up the Software-In-Loop.
// Calling this function again after the Software-In-Loop has already started will reset it.
// Passing NULL to either argument will not kill/start any boards.
// NOTE: boardsToKill and boardsToStart must be NULL terminated.
void sil_manager_start(sil_Board *boardsToKill[], sil_Board *boardsToStart[]);

// Sets the time for all active boards.
// Blocks while all boards in boardsToVerify catch up.
// NOTE: boardsToVerify must be NULL terminated.
void sil_manager_setTime(uint32_t targetMs, sil_Board *boardsToVerify[]);

// Get the active tx socket of the manager.
zsock_t *sil_manager_getSocketTx();