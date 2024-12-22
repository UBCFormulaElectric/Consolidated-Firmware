#include "sil_api.h"
#include <czmq.h>
#include "sil_atoi.h"

// can topic.

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
    // Extract stdId.
    char    *stdIdStr = zmsg_popstr(zmqMsg);
    uint32_t stdId    = 0;
    if (stdIdStr != NULL)
    {
        stdId = sil_atoi_uint32_t(stdIdStr);
        free(stdIdStr);
    }
    else
    {
        return NULL;
    }

    // Extract dlc.
    char    *dlcStr = zmsg_popstr(zmqMsg);
    uint32_t dlc    = 0;
    if (dlcStr != NULL)
    {
        dlc = sil_atoi_uint32_t(dlcStr);
        free(dlcStr);
    }
    else
    {
        return NULL;
    }

    // Extract data.
    char    *dataStr    = zmsg_popstr(zmqMsg);
    uint64_t dataUInt64 = 0;
    if (dataStr != NULL)
    {
        dataUInt64 = sil_atoi_uint64_t(dataStr);
        free(dataStr);
    }
    else
    {
        return NULL;
    }

    // Allocate and return the result.
    sil_api_Can *res = malloc(sizeof(sil_api_Can));
    res->stdId       = stdId;
    res->dlc         = dlc;
    memcpy(res->data, &dataUInt64, sizeof(uint64_t));
    return res;
}

void sil_api_can_destroy(sil_api_Can *msg)
{
    free(msg);
}

// ready topic.

sil_api_Ready *sil_api_ready_new(char *boardName)
{
    sil_api_Ready *res           = malloc(sizeof(sil_api_Ready));
    size_t         boardNameSize = sizeof(char) * (strlen(boardName) + 1);
    res->boardName               = malloc(boardNameSize);
    memcpy(res->boardName, boardName, boardNameSize);

    return res;
}

int sil_api_ready_tx(sil_api_Ready *msg, zsock_t *socket)
{
    return zsock_send(socket, "ss", "ready", msg->boardName);
}

sil_api_Ready *sil_api_ready_rx(zmsg_t *zmqMsg)
{
    // Extract board name.
    char *boardName = zmsg_popstr(zmqMsg);
    if (boardName == NULL)
        return NULL;

    // Allocate and return result.
    sil_api_Ready *res = malloc(sizeof(sil_api_Ready));
    res->boardName     = boardName;
    return res;
}

void sil_api_ready_destroy(sil_api_Ready *msg)
{
    free(msg->boardName);
    free(msg);
}

// time_resp topic.

sil_api_TimeResp sil_api_timeResp_new(const char *boardName, uint32_t timeMs)
{
    sil_api_TimeResp res = { .boardName = boardName, .timeMs = timeMs };
    return res;
}

int sil_api_timeResp_tx(sil_api_TimeResp msg, zsock_t *socket)
{
    return zsock_send(socket, "ss4", "time_resp", msg.boardName, msg.timeMs);
}

// time_req topic.

sil_api_TimeReq sil_api_timeReq_new(uint32_t timeMs)
{
    sil_api_TimeReq res = { .timeMs = timeMs };
    return res;
}

int sil_api_timeReq_tx(sil_api_TimeReq msg, zsock_t *socket)
{
    return zsock_send(socket, "s4", "time_req", msg.timeMs);
}