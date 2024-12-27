#pragma once
#include <czmq.h>

// Topic to set return value for io level fakes.
#define SIL_FAKES_SET_TOPIC ("fakes_set")

// Set the return value of an io-level fake on a given board.
void _sil_fakes_set_tx(zsock_t *socket, char *boardName, char *fnName, void *retPointer, size_t retSize);

// Set the return value of an io-level fake on a given board.
// Macro to automatically find the size of retPointer, assumes that retPointer is deref-able.
#define sil_fakes_set_tx(socket, boardName, fnName, retPointer) \
    _sil_fakes_set_tx(socket, boardName, fnName, retPointer, sizeof(*retPointer))