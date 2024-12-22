#include "sil_api.h"
#include <czmq.h>

int sil_api_tx_can(zsock_t *socket, uint32_t stdId, uint32_t dlc, const uint8_t data[8])
{
    uint64_t uint64Data;
    memcpy(&uint64Data, data, sizeof(uint64Data));
    return zsock_send(socket, "s448", "can", stdId, dlc, uint64Data);
}

int sil_api_tx_ready(zsock_t *socket, const char boardName[])
{
    return zsock_send(socket, "ss", "ready", boardName);
}

int sil_api_tx_timeResp(zsock_t *socket, const char boardName[], uint32_t timeMs)
{
    return zsock_send(socket, "ss4", "time_resp", boardName, timeMs);
}

int sil_api_tx_timeReq(zsock_t *socket, uint32_t timeMs)
{
    return zsock_send(socket, "s4", "time_req", timeMs);
}