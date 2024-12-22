#include "sil_api.h"
#include <czmq.h>

sil_api_Can sil_api_can_new(uint32_t stdId, uint32_t dlc, uint8_t data[8])
{
    sil_api_Can res = { .stdId = stdId, .dlc = dlc, .data = data };
    return res;
}

int sil_api_can_tx(sil_api_Can *msg, zsock_t *socket)
{
    uint64_t uint64Data;
    memcpy(&uint64Data, msg->data, sizeof(uint64Data));
    return zsock_send(socket, "s448", "can", msg->stdId, msg->dlc, uint64Data);
}

sil_api_Ready sil_api_ready_new(const char *boardName)
{
    sil_api_Ready res = { .boardName = boardName };
    return res;
}

int sil_api_ready_tx(sil_api_Ready *msg, zsock_t *socket)
{
    return zsock_send(socket, "ss", "ready", msg->boardName);
}

sil_api_TimeResp sil_api_timeResp_new(const char *boardName, uint32_t timeMs)
{
    sil_api_TimeResp res = { .boardName = boardName, .timeMs = timeMs };
    return res;
}

int sil_api_timeResp_tx(sil_api_TimeResp *msg, zsock_t *socket)
{
    return zsock_send(socket, "ss4", "time_resp", msg->boardName, msg->timeMs);
}

sil_api_TimeReq sil_api_timeReq_new(uint32_t timeMs)
{
    sil_api_TimeReq res = { .timeMs = timeMs };
    return res;
}

int sil_api_timeReq_tx(sil_api_TimeReq *msg, zsock_t *socket)
{
    return zsock_send(socket, "s4", "time_req", msg->timeMs);
}