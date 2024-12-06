#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// CZMQ for sockets.
#include <czmq.h>

// Fake io Layer.
#include "fake_io_apps.hpp"
#include "fake_io_brake.hpp"
#include "fake_io_steering.hpp"
#include "fake_io_suspension.hpp"
#include "fake_io_wheels.hpp"
#include "fake_io_loadCell.hpp"

extern "C"
{
#include "io_canRx.h"
#include "io_canTx.h"
}

// App layer.
extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "app_mainState.h"
#include "app_apps.h"
#include "app_brake.h"
#include "app_steering.h"
#include "app_suspension.h"
#include "app_wheels.h"
}

static zsock_t   *canSocketTx;
static zsock_t   *canSocketRx;
static zpoller_t *canPollerRx;

// Graciously exit process by freeing memory allocated by czmq.
void exitHandler()
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
// Socket is a void pointer to be consistent with czmq api.
int io_fakeCan_recvJsonCanMsg(void *socket, JsonCanMsg *msg)
{
    return zsock_recv(canSocketRx, "448", &(msg->std_id), &(msg->dlc), msg->data);
}

// Wrapper around zsock_send for sending a JsonCanMsg.
// Gets a pointer to the message to send.
// Socket is a void pointer to be consistent with czmq api.
int io_fakeCan_sendJsonCanMsg(void *socket, const JsonCanMsg *msg)
{
    uint64_t uint64Data;
    memcpy(&uint64Data, msg->data, sizeof(uint64_t));
    return zsock_send(canSocketTx, "448", msg->std_id, msg->dlc, uint64Data);
}

// Interface between sil and canbus.
// Hook for can to transmit a message via fakeCan.
void io_fakeCan_txCallback(const JsonCanMsg *msg)
{
    if (io_fakeCan_sendJsonCanMsg(canSocketTx, msg) == -1)
        perror("Error sending jsoncan tx message");
}

// Insert a JsonCanMsg into the board's internal can system.
void io_fakeCan_rx(JsonCanMsg *msg)
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

int main()
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

    // Setup can with fake callback for transmission.
    io_canTx_init(io_fakeCan_txCallback);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    app_canTx_init();
    app_canRx_init();

    // Setup apps.
    app_apps_init();

    // Start up state machine and begin ticking.
    app_stateMachine_init(app_mainState_get());

    // Register exit handler after creation of sockets, but before main loop,
    // to avoid czmq's own exit handler warnings.
    atexit(exitHandler);
    for (int time_ms = 0; true; time_ms += 1)
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
                if (io_fakeCan_recvJsonCanMsg(socket, &msg) == -1)
                {
                    perror("Error: Invalid can message received");
                }
                else
                {
                    // Update the internal can table.
                    io_fakeCan_rx(&msg);
                }
            }
            else
                break;
        }

        // 1 kHz task.
        if (time_ms % 1 == 0)
        {
            io_canTx_enqueueOtherPeriodicMsgs(time_ms);
        }

        // 100 Hz task.
        if (time_ms % 10 == 0)
        {
            app_stateMachine_tick100Hz();
            io_canTx_enqueue100HzMsgs();
        }

        // 1 Hz task.
        if (time_ms % 1000 == 0)
        {
            app_stateMachine_tick1Hz();
            io_canTx_enqueue1HzMsgs();
        }
    }
}