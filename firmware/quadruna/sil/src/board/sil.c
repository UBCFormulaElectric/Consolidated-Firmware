#include "sil.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// CZMQ for sockets.
#include <czmq.h>

#include "io_canRx.h"
#include "io_canTx.h"

static zsock_t   *canSocketTx;
static zsock_t   *canSocketRx;
static zpoller_t *canPollerRx;

// Graciously exit process by freeing memory allocated by czmq.
void sil_exitHandler()
{
    zpoller_destroy(&canPollerRx);
    zsock_destroy(&canSocketTx);
    zsock_destroy(&canSocketRx);
}
// Can messages are transmitted in std_id, dlc, data order.
// We cast the 8 byte data array to a uint64_t for easier transmit.
// Hence, the czmq image of the message is "448" (uint32_t, uint32_t, uint64_t).

// Wrapper around zsock_recv for receiving a JsonCanMsg.
// Gets a pointer to the destination message.
// Socket is a void pointer to be consistent with czmq api, can be socket or actor.
int sil_recvJsonCanMsg(void *socket, JsonCanMsg *msg)
{
    return zsock_recv(canSocketRx, "448", &(msg->std_id), &(msg->dlc), msg->data);
}

// Wrapper around zsock_send for sending a JsonCanMsg.
// Gets a pointer to the message to send.
// Socket is a void pointer to be consistent with czmq api, can be socket or actor.
int sil_sendJsonCanMsg(void *socket, const JsonCanMsg *msg)
{
    uint64_t uint64Data;
    memcpy(&uint64Data, msg->data, sizeof(uint64_t));
    return zsock_send(canSocketTx, "448", msg->std_id, msg->dlc, uint64Data);
}

// Interface between sil and canbus.
// Hook for can to transmit a message via fakeCan.
void sil_txCallback(const JsonCanMsg *msg)
{
    if (sil_sendJsonCanMsg(canSocketTx, msg) == -1)
        perror("Error sending jsoncan tx message");
}

// Insert a JsonCanMsg into the board's internal can system.
void sil_rx(JsonCanMsg *msg)
{
    uint64_t uint64Data;
    memcpy(&uint64Data, msg->data, sizeof(uint64_t));
    if (uint64Data != 0)
        printf(
            "ID: %d, DATA: %016llx, RECEIVED?: %d\n", msg->std_id, uint64Data, io_canRx_filterMessageId(msg->std_id));

    if (io_canRx_filterMessageId(msg->std_id))
    {
        io_canRx_updateRxTableWithMessage(msg);
    };
}

// Main SIL loop logic.
void sil_main(
    void tasks_init(),
    void tasks_1Hz(uint32_t time_ms),
    void tasks_100Hz(uint32_t time_ms),
    void tasks_1kHz(uint32_t time_ms))
{
    printf("Starting up SIL FSM\n");

    // Prefixing the endpoint with ">" connects to the endpoint,
    // rather than the default bind behavior.
    canSocketTx = zsock_new_pub(">tcp://localhost:3001");
    if (canSocketTx == NULL)
    {
        perror("Error opening can tx socket");
        exit(1);
    }

    // Subscribing to an empty string subscribes to all messages.
    // Topics in czmq are just prefix strings.
    canSocketRx = zsock_new_sub("tcp://localhost:3000", "");
    if (canSocketRx == NULL)
    {
        perror("Error opening can rx socket");
        exit(1);
    }

    // Poll the rx can socket.
    canPollerRx = zpoller_new(canSocketRx, NULL);
    if (canPollerRx == NULL)
    {
        perror("Error opening can rx poller");
        exit(1);
    }

    tasks_init();

    // Register exit handler after creation of sockets, but before main loop,
    // to avoid czmq's own exit handler warnings.
    atexit(sil_exitHandler);
    for (uint32_t time_ms = 0; true; time_ms += 1)
    {
        // Parent process id becomes 1 when parent dies.
        // Every tick we poll to make sure we exit this child process.
        if (getppid() == 1)
            exit(0);

        // FakeCan sockets capture loop.
        for (;;)
        {
            // zpoller_wait returns reference to the socket that is ready to recieve, or NULL.
            // there is only one such socket attachted to canPollerRx, which is canSocketRx.
            // Socket must be void pointer, uncast, since it may return NULL.
            void *socket = zpoller_wait(canPollerRx, 1);
            if (socket != NULL)
            {
                // Receive the message.
                JsonCanMsg msg = {};
                if (sil_recvJsonCanMsg(socket, &msg) == -1)
                {
                    perror("Error: Invalid can message received");
                }
                else
                {
                    // Update the internal can table.
                    sil_rx(&msg);
                }
            }
            else
                break;
        }

        // 1 kHz task.
        if (time_ms % 1 == 0)
        {
            tasks_1kHz(time_ms);
        }

        // 100 Hz task.
        if (time_ms % 10 == 0)
        {
            tasks_100Hz(time_ms);
        }

        // 1 Hz task.
        if (time_ms % 1000 == 0)
        {
            tasks_1Hz(time_ms);
        }
    }
}