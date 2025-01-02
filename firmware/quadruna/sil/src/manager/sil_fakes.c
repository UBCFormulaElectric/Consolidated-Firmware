#include "sil_fakes.h"
#include <czmq.h>

int _sil_fakes_set_tx(zsock_t *socket, char *boardName, char *fnName, void *retPointer, size_t retSize)
{
    byte retData[retSize];
    memcpy(retData, retPointer, retSize);
    return zsock_send(socket, "sssb", SIL_FAKES_SET_TOPIC, boardName, fnName, retData, retSize);
}