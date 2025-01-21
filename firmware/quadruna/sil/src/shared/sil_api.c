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
    return zsock_send(socket, "s448", SIL_API_CAN_TOPIC, msg->stdId, msg->dlc, uint64Data);
}

sil_api_Can *sil_api_can_rx(zmsg_t *zmqMsg)
{
    // Extract std id.
    char *stdIdStr = zmsg_popstr(zmqMsg);
    if (stdIdStr == NULL)
        return NULL;
    uint32_t stdId = sil_atoi_uint32_t(stdIdStr);
    free(stdIdStr);

    // Extract dlc.
    char *dlcStr = zmsg_popstr(zmqMsg);
    if (dlcStr == NULL)
        return NULL;
    uint32_t dlc = sil_atoi_uint32_t(dlcStr);
    free(dlcStr);

    // Extract data.
    char *dataStr = zmsg_popstr(zmqMsg);
    if (dataStr == NULL)
        return NULL;
    uint32_t dataUInt64 = sil_atoi_uint32_t(dataStr);
    free(dataStr);

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
    return zsock_send(socket, "ss", SIL_API_READY_TOPIC, msg->boardName);
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
sil_api_TimeResp *sil_api_timeResp_new(char *boardName, uint32_t timeMs)
{
    sil_api_TimeResp *res           = malloc(sizeof(sil_api_TimeResp));
    size_t            boardNameSize = sizeof(char) * (strlen(boardName) + 1);
    res->boardName                  = malloc(boardNameSize);
    memcpy(res->boardName, boardName, boardNameSize);

    res->timeMs = timeMs;
    return res;
}

int sil_api_timeResp_tx(sil_api_TimeResp *msg, zsock_t *socket)
{
    return zsock_send(socket, "ss4", SIL_API_TIME_RESP_TOPIC, msg->boardName, msg->timeMs);
}

sil_api_TimeResp *sil_api_timeResp_rx(zmsg_t *zmqMsg)
{
    // Extract board name.
    char *boardName = zmsg_popstr(zmqMsg);
    if (boardName == NULL)
        return NULL;

    // Extract time.
    char    *timeMsStr = zmsg_popstr(zmqMsg);
    uint32_t timeMs    = 0;
    if (timeMsStr == NULL)
    {
        free(boardName);
        return NULL;
    }
    timeMs = sil_atoi_uint32_t(timeMsStr);
    free(timeMsStr);

    // Allocate and return result.
    sil_api_TimeResp *res = malloc(sizeof(sil_api_TimeResp));
    res->boardName        = boardName;
    res->timeMs           = timeMs;
    return res;
}

void sil_api_timeResp_destroy(sil_api_TimeResp *msg)
{
    free(msg->boardName);
    free(msg);
}

// time_req topic.

sil_api_TimeReq *sil_api_timeReq_new(uint32_t timeMs)
{
    sil_api_TimeReq *res = malloc(sizeof(sil_api_TimeReq));
    res->timeMs          = timeMs;
    return res;
}

int sil_api_timeReq_tx(sil_api_TimeReq *msg, zsock_t *socket)
{
    return zsock_send(socket, "s4", SIL_API_TIME_REQ_TOPIC, msg->timeMs);
}

sil_api_TimeReq *sil_api_timeReq_rx(zmsg_t *zmqMsg)
{
    // Extract time.
    char *timeMsStr = zmsg_popstr(zmqMsg);
    if (timeMsStr == NULL)
        return NULL;
    uint32_t timeMs = sil_atoi_uint32_t(timeMsStr);

    // Allocate and return result.
    sil_api_TimeReq *res = malloc(sizeof(sil_api_TimeReq));
    res->timeMs          = timeMs;
    return res;
}

void sil_api_timeReq_destroy(sil_api_TimeReq *msg)
{
    free(msg);
}

sil_api_Procedure *sil_api_procedure_new(void (*board_procedure)(void))
{
    sil_api_Procedure *res = malloc(sizeof(sil_api_Procedure));
    res->board_procedure   = board_procedure;
    return res;
}

int sil_api_procedure_tx(sil_api_Procedure *msg, zsock_t *socket)
{
    return zsock_send(socket, "sss", SIL_API_PROCEDURE_TOPIC, msg->board_procedure);
}

sil_api_Procedure *sil_api_procedure_rx(zmsg_t *zmqMsg)
{
    // extract topic, board name and procedure
    char *topic_id   = zmsg_popstr(zmqMsg);
    char *board_name = zmsg_popstr(zmqMsg);
    char *procedure  = zmsg_popstr(zmqMsg);
    if (procedure == NULL)
        return NULL;
    sil_api_Procedure *res = malloc(sizeof(sil_api_Procedure));
    res->board_procedure   = procedure;
    return res;
}

void sil_api_procedure_destroy(sil_api_Procedure *msg)
{
    free(msg);
}