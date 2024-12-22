#pragma once
#include <stdlib.h>
#include <string.h>
#include <czmq.h>

// can topic.
// Sent from boards, each message denotes a JsonCanMsg.
typedef struct sil_api_Can
{
    uint32_t stdId;
    uint32_t dlc;
    uint8_t  data[8];
} sil_api_Can;

// Create a new SIL can message.
sil_api_Can sil_api_can_new(uint32_t stdId, uint32_t dlc, const uint8_t data[8]);

// Send a SIL can message.
int sil_api_can_tx(sil_api_Can msg, zsock_t *socket);

// Receive a SIL can message.
// Expects a message without the topic string included.
// ie.
//  ```
//  zmsg_t zmqMsg;
//  zsock_recv(socket, "sm", ..., &zmqMsg);
//  sil_api_Can msg = sil_api_can_rx(&zmqMsg);
//  ```
sil_api_Can sil_api_can_rx(zmsg_t *zmqMsg);

// ready topic.
// Sent from boards on startup, notifies the SIL manager that the board is ready to go.
typedef struct sil_api_Ready
{
    const char *boardName;
} sil_api_Ready;

// Create a new ready signal.
sil_api_Ready sil_api_ready_new(const char *boardName);

// Send a ready signal.
int sil_api_ready_tx(sil_api_Ready msg, zsock_t *socket);

// time_resp topic.
// Sent from boards, lets the SIL manager know how much time has passed for an individual board process.
typedef struct sil_api_TimeResp
{
    const char *boardName;
    uint32_t    timeMs;
} sil_api_TimeResp;

// Create a new time response message.
sil_api_TimeResp sil_api_timeResp_new(const char *boardName, uint32_t timeMs);

// Send a notification of the given board's current time.
int sil_api_timeResp_tx(sil_api_TimeResp msg, zsock_t *socket);

// time_req topic.
// Sent from the supervisor, tells all the boards to target a given time.
typedef struct sil_api_TimeReq
{
    uint32_t timeMs;
} sil_api_TimeReq;

// Create a new time request message.
sil_api_TimeReq sil_api_timeReq_new(uint32_t timeMs);

// Make a request for all boards to advance to a given time.
int sil_api_timeReq_tx(sil_api_TimeReq msg, zsock_t *socket);