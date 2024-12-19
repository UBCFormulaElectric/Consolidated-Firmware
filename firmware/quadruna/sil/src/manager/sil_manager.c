#include "sil_board.h"
#include <czmq.h>
#include <stdio.h>
#include "sil_atoi.h"

// Store socket, poll, and proxy pointers for graceful exit.
static zactor_t  *proxy    = NULL;
static zsock_t   *socketTx = NULL;
static zsock_t   *socketRx = NULL;
static zpoller_t *pollerRx = NULL;

// Track manager time.
static uint32_t timeMs = 0;

// Free any heap-allocated CZMQ memory, any un-allocated memory or board will not be freed.
// ie. this function is safe to call even if the manager has not been started before.
// NOTE: THIS METHOD SHOULD BE INVOKED WITH AN atexit HANDLER.
void sil_manager_destroy()
{
    // Free up CZMQ memory.
    if (proxy != NULL)
        zactor_destroy(&proxy);
    proxy = NULL;

    if (pollerRx != NULL)
        zpoller_destroy(&pollerRx);
    pollerRx = NULL;

    if (socketTx != NULL)
        zsock_destroy(&socketTx);
    socketTx = NULL;

    if (socketRx != NULL)
        zsock_destroy(&socketRx);
    socketRx = NULL;
}

void sil_manager_reset(sil_Board *boardsToKill[])
{
    sil_manager_destroy();
    timeMs = 0;

    // Kill boards.
    if (boardsToKill != NULL)
    {
        for (size_t boardIndex = 0; boardsToKill[boardIndex] != NULL; boardIndex += 1)
        {
            sil_board_reset(boardsToKill[boardIndex]);
        }
    }
}

void sil_manager_start(sil_Board *boardsToKill[], sil_Board *boardsToStart[])
{
    sil_manager_reset(boardsToKill);
    printf("Starting up SIL Manager\n");

    // See https://zguide.zeromq.org/docs/chapter2/#The-Dynamic-Discovery-Problem - Figure 13.
    // Normal PUB/SUB zmq architectures support one-to-many and many-to-one.
    // For fake comms (ie. can), we need many-to-many. To do this, we create a proxy between an XSUB and XPUB port.
    // Individual boards transmit to the XSUB endpoint, which gets forwarded to the XPUB endpoint.
    // We call the receiver end of the proxy the frontend, and the sender end the badkend.
    proxy = zactor_new(zproxy, NULL);
    if (proxy == NULL)
        perror("Error creating proxy");

    // Setup the xsub frontend.
    if (zstr_sendx(proxy, "FRONTEND", "XSUB", "tcp://localhost:3001", NULL) == -1)
        perror("Error setting up xsub frontend on proxy");
    if (zsock_wait(proxy) == -1)
        perror("Error waiting for frontend setup on proxy");

    // Setup the xpub backend.
    if (zstr_sendx(proxy, "BACKEND", "XPUB", "tcp://localhost:3000", NULL) == -1)
        perror("Error setting up xsub backend on proxy");
    if (zsock_wait(proxy) == -1)
        perror("Error waiting for backend setup on proxy");

    // Open up rx/tx sockets and rx poller, to allow the manager to tap into the pub/sub network.

    // Setup a tx socket into the pub/sub network, to allow the manager to inject messages.
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
    zsock_set_subscribe(socketRx, "ready");
    zsock_set_subscribe(socketRx, "time_resp");

    // Poll the rx socket.
    pollerRx = zpoller_new(socketRx, NULL);
    if (pollerRx == NULL)
    {
        perror("Error opening rx poller");
        exit(1);
    }

    // Add an exit hook to destroy these CZMQ-malloced objects if they are still around, in case of early exits.
    // Must be invoked after the CZMQ objects are created, in order to run before they are cleared.
    atexit(sil_manager_destroy);

    // Spin up boards.
    if (boardsToStart != NULL)
    {
        for (size_t boardIndex = 0; boardsToStart[boardIndex] != NULL; boardIndex += 1)
            sil_board_run(boardsToStart[boardIndex], pollerRx);
    }
}

void sil_manager_setTime(uint32_t targetMs, sil_Board *boardsToVerify[])
{
    assert(targetMs >= timeMs);
    timeMs = targetMs;

    // Make a request to match the time for all the other boards.
    zsock_send(socketTx, "s4", "time_req", timeMs);

    // Block until the the request is satisfied, reading all incomming messages in the process.
    // Every loop either:
    //  - Captures a message to update records.
    //  - Checks the records for times matching the target.
    for (;;)
    {
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
            else if (strcmp(topic, "time_resp") == 0)
            {
                // time_resp topic.
                // Message are built in the following order:
                //  1) Name of board.
                //  2) Time in ms.

                // Extract board name.
                char *receivedBoardName = zmsg_popstr(zmqMsg);
                if (receivedBoardName != NULL)
                {
                    // If successful, extract time in ms.
                    char *receivedTimeMsStr = zmsg_popstr(zmqMsg);
                    if (receivedTimeMsStr != NULL)
                    {
                        // If successful, convert time to uint32_t.
                        uint32_t receivedTimeMs = sil_atoi_uint32_t(receivedTimeMsStr);

                        // Update record.
                        if (boardsToVerify != NULL)
                        {
                            for (size_t boardIndex = 0; boardsToVerify[boardIndex] != NULL; boardIndex += 1)
                            {
                                if (strcmp(receivedBoardName, boardsToVerify[boardIndex]->name) == 0)
                                    boardsToVerify[boardIndex]->timeMs = receivedTimeMs;
                            }
                        }

                        free(receivedTimeMsStr);
                    }

                    free(receivedBoardName);
                }
            }

            // Free up zmq-allocated memory.
            free(topic);
            zmsg_destroy(&zmqMsg);
        }
        else
        {
            // If we are not receiving messages, check if all the boards have passed the target time.
            bool boardsHitTargetTime = true;
            if (boardsToVerify != NULL)
            {
                for (size_t boardIndex = 0; boardsToVerify[boardIndex] != NULL; boardIndex += 1)
                    boardsHitTargetTime &= boardsToVerify[boardIndex]->timeMs >= timeMs;
            }

            // If they have, break out.
            if (boardsHitTargetTime)
                break;
        }
    }
}