#include "sil.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <czmq.h>

#include "sil_atoi.h"
#include "sil_api.h"

#include "io_canRx.h"
#include "io_canTx.h"

// Store socket and poll pointers for graceful exit.
static zsock_t   *socketTx;
static zsock_t   *socketRx;
static zpoller_t *pollerRx;

// Note: only valid since the program exits when main exits.
static char *boardName;

// Graciously exit process by freeing memory allocated by czmq.
void sil_exitHandler()
{
    zpoller_destroy(&pollerRx);
    zsock_destroy(&socketTx);
    zsock_destroy(&socketRx);
}

// Utility function for debugging, prints out a JsonCanMsg.
void sil_printCanMsg(bool isTx, JsonCanMsg *msg)
{
    uint64_t uint64Data;
    memcpy(&uint64Data, msg->data, sizeof(uint64_t));

    printf(
        "%s | ACTIVE BOARD: %4s, ID: %4d, DATA: %016llx, DLC: %d, PASSED FILTER?: %d\n", isTx ? "TX" : "RX", boardName,
        msg->std_id, uint64Data, msg->dlc, io_canRx_filterMessageId(msg->std_id));
}

// Interface between sil and canbus.
// Hook for can to transmit a message via fakeCan.
void sil_txCallback(const JsonCanMsg *jsonCanMsg)
{
    sil_api_Can *msg = sil_api_can_new(jsonCanMsg->std_id, jsonCanMsg->dlc, jsonCanMsg->data);
    if (sil_api_can_tx(msg, socketTx) == -1)
        perror("Error sending jsoncan tx message");
    else
        sil_printCanMsg(true, (JsonCanMsg *)jsonCanMsg);
    sil_api_can_destroy(msg);
}

// Insert a JsonCanMsg into the board's internal can system.
void sil_canRx(JsonCanMsg *msg)
{
    if (io_canRx_filterMessageId(msg->std_id))
    {
        sil_printCanMsg(false, msg);
        io_canRx_updateRxTableWithMessage(msg);
    };
}

// Main SIL loop logic.
void sil_main(
    int   argc,
    char *argv[],
    void (*tasks_init)(),
    void (*tasks_1Hz)(uint32_t timeMs),
    void (*tasks_100Hz)(uint32_t timeMs),
    void (*tasks_1kHz)(uint32_t timeMs))
{
    // Validate arguments.
    if (argc != 2)
    {
        printf("Please invoke this board binary with the following arguments\n");
        printf("%s <name of board>\n", argv[0]);
        exit(1);
    }

    boardName = argv[1];
    printf("Starting up SIL %s\n", boardName);

    // Prefixing the endpoint with ">" connects to the endpoint,
    // rather than the default bind behavior.
    socketTx = zsock_new_pub(">tcp://localhost:3001");
    if (socketTx == NULL)
    {
        perror("Error opening tx socket");
        exit(1);
    }

    // Topics in CZMQ are just prefix strings in sent messages.
    socketRx = zsock_new_sub("tcp://localhost:3000", NULL);
    if (socketRx == NULL)
    {
        perror("Error opening rx socket");
        exit(1);
    }
    zsock_set_subscribe(socketRx, "time_req");
    zsock_set_subscribe(socketRx, "can");

    // Poll the rx socket.
    pollerRx = zpoller_new(socketRx, NULL);
    if (pollerRx == NULL)
    {
        perror("Error opening rx poller");
        exit(1);
    }

    // Register exit handler after creation of sockets, but before main loop,
    // to avoid CZMQ's own exit handler warnings.
    atexit(sil_exitHandler);

    // Tell the parent process we are ready.
    // Give the manager a 50ms grace period so that it can catch the ready signal.
    zclock_sleep(50);
    sil_api_Ready *msg = sil_api_ready_new(boardName);
    if (msg == NULL || sil_api_ready_tx(msg, socketTx) == -1)
        perror("Error transmitting ready message");
    sil_api_ready_destroy(msg);

    // Init task.
    tasks_init();

    // Main loop.
    uint32_t timeMs       = 0;
    uint32_t targetTimeMs = 0;
    for (;;)
    {
        // Parent process id becomes 1 when parent dies.
        // Every tick we poll to make sure we exit this child process.
        if (getppid() == 1)
            exit(0);

        // zpoller_wait returns reference to the socket that is ready to recieve, or NULL.
        // there is only one such socket attachted to pollerRx, which is socketRx.
        // Since zpoller_wait may technically also return a zactor,
        // CZMQ common practice is to make this a void pointer.
        void *socket = zpoller_wait(pollerRx, 0);
        if (socket != NULL)
        {
            // Receive the message.
            char   *topic;
            zmsg_t *zmqMsg;

            if (zsock_recv(socket, "sm", &topic, &zmqMsg) == -1)
            {
                perror("Error: Failed to receive on socket");
            }
            else if (strcmp(topic, "can") == 0)
            {
                // Can topic case.
                sil_api_Can *msg    = sil_api_can_rx(zmqMsg);
                JsonCanMsg   canMsg = {
                      .std_id = msg->stdId,
                      .dlc    = msg->dlc,
                };
                memcpy(canMsg.data, msg->data, 8 * sizeof(uint8_t));
                sil_api_can_destroy(msg);
                sil_canRx(&canMsg);
            }
            else if (strcmp(topic, "time_req") == 0)
            {
                char *newTargetimeMsStr = zmsg_popstr(zmqMsg);
                if (newTargetimeMsStr != NULL)
                {
                    uint32_t newTargetimeMs = sil_atoi_uint32_t(newTargetimeMsStr);
                    targetTimeMs            = newTargetimeMs;
                    free(newTargetimeMsStr);
                }
            }

            // Free up zmq-allocated memory.
            free(topic);
            zmsg_destroy(&zmqMsg);
        }
        else if (timeMs < targetTimeMs)
        {
            timeMs += 1;

            // 1 kHz task.
            if (timeMs % 1 == 0)
                tasks_1kHz(timeMs);

            // 100 Hz task.
            if (timeMs % 10 == 0)
                tasks_100Hz(timeMs);

            // 1 Hz task.
            if (timeMs % 1000 == 0)
                tasks_1Hz(timeMs);

            // Tell the supervisor what the current time for this board is.
            sil_api_TimeResp msg = sil_api_timeResp_new(boardName, timeMs);
            sil_api_timeResp_tx(msg, socketTx);
        }
    }
}