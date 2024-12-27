#include "sil_fakes.h"
#include <czmq.h>

void _sil_fakes_set_tx(zsock_t *socket, char *boardName, char *fnName, void *retPointer, size_t retSize)
{
    byte retBytes[retSize];
    memcpy(retBytes, retPointer, retSize);
    zsock_send("sssb", SIL_FAKES_SET_TOPIC, boardName, fnName, retBytes, retSize);
}