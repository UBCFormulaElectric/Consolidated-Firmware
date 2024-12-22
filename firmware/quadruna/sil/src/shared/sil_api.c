#include "sil_api.h"
#include <czmq.h>
#include "sil_atoi.h"

sil_api_Can *sil_api_can_new(uint32_t stdId, uint32_t dlc, const uint8_t data[8])
{
    sil_api_Can *res = malloc(sizeof(sil_api_Can));
    res->stdId       = stdId;
    res->dlc         = dlc;
    memcpy(res->data, data, sizeof(uint8_t) * 8);

    return res;
}

int sil_api_can_tx(sil_api_Can *msg, zsock_t *socket)
{
    uint64_t uint64Data;
    memcpy(&uint64Data, msg->data, sizeof(uint64Data));
    return zsock_send(socket, "s448", "can", msg->stdId, msg->dlc, uint64Data);
}

sil_api_Can *sil_api_can_rx(zmsg_t *zmqMsg)
{
    sil_api_Can *res = malloc(sizeof(sil_api_Can));

    // Extract stdId.
    char *stdIdStr = zmsg_popstr(zmqMsg);
    if (stdIdStr != NULL)
    {
        res->stdId = sil_atoi_uint32_t(stdIdStr);
        free(stdIdStr);
    }

    // Extract dlc.
    char *dlcStr = zmsg_popstr(zmqMsg);
    if (dlcStr != NULL)
    {
        res->dlc = sil_atoi_uint32_t(dlcStr);
        free(dlcStr);
    }

    // Extract data.
    char *dataStr = zmsg_popstr(zmqMsg);
    if (dataStr != NULL)
    {
        uint64_t dataUInt64 = sil_atoi_uint64_t(dataStr);
        memcpy(res->data, &dataUInt64, 8);
        free(dataStr);
    }

    return res;
}

void sil_api_can_destroy(sil_api_Can *msg)
{
    free(msg);
}

sil_api_Ready sil_api_ready_new(const char *boardName)
{
    sil_api_Ready res = { .boardName = boardName };
    return res;
}

int sil_api_ready_tx(sil_api_Ready msg, zsock_t *socket)
{
    return zsock_send(socket, "ss", "ready", msg.boardName);
}

sil_api_TimeResp sil_api_timeResp_new(const char *boardName, uint32_t timeMs)
{
    sil_api_TimeResp res = { .boardName = boardName, .timeMs = timeMs };
    return res;
}

int sil_api_timeResp_tx(sil_api_TimeResp msg, zsock_t *socket)
{
    return zsock_send(socket, "ss4", "time_resp", msg.boardName, msg.timeMs);
}

sil_api_TimeReq sil_api_timeReq_new(uint32_t timeMs)
{
    sil_api_TimeReq res = { .timeMs = timeMs };
    return res;
}

int sil_api_timeReq_tx(sil_api_TimeReq msg, zsock_t *socket)
{
    return zsock_send(socket, "s4", "time_req", msg.timeMs);
}