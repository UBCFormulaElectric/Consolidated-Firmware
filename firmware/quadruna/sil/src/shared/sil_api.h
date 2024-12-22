#pragma once
#include <stdlib.h>
#include <czmq.h>

// This header contains methods to handle communications with the SIL api.
// Every method is named sil_api_<tx/rx>_<topicInCamelCase>, ie. sil_api_tx_can.
// tx methods return -1 if they fail, and 0 if they succeed.

// can topic.
// Sent from boards, each message denotes a JsonCanMsg.

// Send a can message.
int sil_api_tx_can(zsock_t *socket, uint32_t stdId, uint32_t dlc, const uint8_t data[8]);

// ready topic.
// Sent from boards on startup, notifies the SIL manager that the board is ready to go.

// Send a ready signal.
int sil_api_tx_ready(zsock_t *socket, const char boardName[]);

// time_resp topic.
// Sent from boards, lets the SIL manager know how much time has passed for an individual board process.

// Send a notification of the given board's current time.
int sil_api_tx_timeResp(zsock_t *socket, const char boardName[], uint32_t timeMs);

// time_req topic.
// Sent from the supervisor, tells all the boards to target a given time.

// Make a request for all boards to advance to a given time.
int sil_api_tx_timeReq(zsock_t *socket, uint32_t timeMs);