#pragma once
#include <stdlib.h>
#include <string.h>
#include <czmq.h>
#include <stdint.h>

// can topic.
// Sent from boards, each message denotes a JsonCanMsg.
#define SIL_API_CAN_TOPIC ("can")

// Wrapper around a single SIL can message.
typedef struct sil_api_Can
{
    uint32_t stdId;
    uint32_t dlc;
    uint8_t  data[8];
} sil_api_Can;

// Create a new SIL can message.
// Always make sure to destroy the message afterwards.
sil_api_Can *sil_api_can_new(uint32_t stdId, uint32_t dlc, const uint8_t data[8]);

// Send a SIL can message.
int sil_api_can_tx(sil_api_Can *msg, zsock_t *socket);

// Receive a SIL can message.
// Allocates result to the heap.
// Expects a message without the topic string included.
// ie.
//  ```
//  zmsg_t *zmqMsg;
//  zsock_recv(socket, "sm", ..., zmqMsg);
//  sil_api_Can *msg = sil_api_can_rx(zmqMsg);
//  sil_api_can_destroy(msg);
//  ```
sil_api_Can *sil_api_can_rx(zmsg_t *zmqMsg);

// Destroy a SIL can message.
void sil_api_can_destroy(sil_api_Can *msg);

// ready topic.
// Sent from boards on startup, notifies the SIL manager that the board is ready to go.
#define SIL_API_READY_TOPIC ("ready")

// Wrapper for a ready signal.
typedef struct sil_api_Ready
{
    char *boardName;
} sil_api_Ready;

// Create a new ready signal.
// Always make sure to destroy the message afterwards.
sil_api_Ready *sil_api_ready_new(char *boardName);

// Send a ready signal.
int sil_api_ready_tx(sil_api_Ready *msg, zsock_t *socket);

// Receive a ready signal.
// Allocates result to the heap.
// Expects a message without the topic string included.
// ie.
//  ```
//  zmsg_t *zmqMsg;
//  zsock_recv(socket, "sm", ..., zmqMsg);
//  sil_api_Ready *msg = sil_api_ready_rx(zmqMsg);
//  sil_api_ready_destroy(msg);
//  ```
sil_api_Ready *sil_api_ready_rx(zmsg_t *zmqMsg);

// Destroy a ready signal.
void sil_api_ready_destroy(sil_api_Ready *msg);

// time_resp topic.
// Sent from boards, lets the SIL manager know how much time has passed for an individual board process.
#define SIL_API_TIME_RESP_TOPIC ("time_resp")

// Wrapper around a single time response.
typedef struct sil_api_TimeResp
{
    char    *boardName;
    uint32_t timeMs;
} sil_api_TimeResp;

// Create a new time response message.
// Always make sure to destroy the message afterwards.
sil_api_TimeResp *sil_api_timeResp_new(char *boardName, uint32_t timeMs);

// Send a notification of the given board's current time.
int sil_api_timeResp_tx(sil_api_TimeResp *msg, zsock_t *socket);

// Receive a time response message.
// Allocates result to the heap.
// Expects a message without the topic string included.
// ie.
//  ```
//  zmsg_t *zmqMsg;
//  zsock_recv(socket, "sm", ..., zmqMsg);
//  sil_api_TimeResp *msg = sil_api_timeResp_rx(zmqMsg);
//  sil_api_timeResp_destroy(msg);
//  ```
sil_api_TimeResp *sil_api_timeResp_rx(zmsg_t *zmqMsg);

// Destroy a time response.
void sil_api_timeResp_destroy(sil_api_TimeResp *msg);

// time_req topic.
// Sent from the supervisor, tells all the boards to target a given time.
#define SIL_API_TIME_REQ_TOPIC ("time_req")

// Wrapper around a single type request.
typedef struct sil_api_TimeReq
{
    uint32_t timeMs;
} sil_api_TimeReq;

// Create a new time request message.
sil_api_TimeReq *sil_api_timeReq_new(uint32_t timeMs);

// Make a request for all boards to advance to a given time.
int sil_api_timeReq_tx(sil_api_TimeReq *msg, zsock_t *socket);

// Receive a time request message.
// Allocates result to the heap.
// Expects a message without the topic string included.
// ie.
//  ```
//  zmsg_t *zmqMsg;
//  zsock_recv(socket, "sm", ..., zmqMsg);
//  sil_api_TimeReq *msg = sil_api_timeReq_rx(zmqMsg);
//  sil_api_timeReq_destroy(msg);
//  ```
sil_api_TimeReq *sil_api_timeReq_rx(zmsg_t *zmqMsg);

// Destroy a time request.
void sil_api_timeReq_destroy(sil_api_TimeReq *msg);

// Sent from supervisor indicating what procedure to invoke for what board
#define SIL_API_PROCEDURE_TOPIC ("board_procedure")

// struct pointing to the appropriate board prcedure
typedef struct sil_api_Procedure
{
    const char *board_procedure;
    void (*board_procedure_ptr)(void);
} sil_api_Procedure;

typedef struct BoardFunctionMapping
{
    const char        *board_name;
    sil_api_Procedure *table;
} BoardFunctionMapping;

// creating a new message rx/tx and destroy
// sil_api_Procedure *sil_api_procedure_new(char board_procedure);
sil_api_Procedure *sil_api_procedure_new(void (*board_procedure)(void));

int                sil_api_procedure_tx(sil_api_Procedure *msg, zsock_t *socket);
sil_api_Procedure *sil_api_procedure_rx(zmsg_t *zmqMsg);
void               sil_api_procedure_destroy(sil_api_Procedure *msg);