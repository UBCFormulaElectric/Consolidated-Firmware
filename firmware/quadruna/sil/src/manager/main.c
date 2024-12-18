#include <stdio.h>
#include <stdlib.h>
#include <czmq.h>
#include <stdatomic.h>

typedef struct
{
    const char *name;
    const char *binPath;
    uint32_t    timeMs;
} Board;

Board boards[] = {
    { .name = "FSM", .binPath = "./build_fw_sil/firmware/quadruna/FSM/quadruna_FSM_sil", .timeMs = 0 },
    { .name = "RSM", .binPath = "./build_fw_sil/firmware/quadruna/RSM/quadruna_RSM_sil", .timeMs = 0 },
    { .name = "BMS", .binPath = "./build_fw_sil/firmware/quadruna/BMS/quadruna_BMS_sil", .timeMs = 0 },
    { .name = "CRIT", .binPath = "./build_fw_sil/firmware/quadruna/CRIT/quadruna_CRIT_sil", .timeMs = 0 },
    { .name = "VC", .binPath = "./build_fw_sil/firmware/quadruna/VC/quadruna_VC_sil", .timeMs = 0 },
};
size_t boardCount = sizeof(boards) / sizeof(boards[0]);

// Store socket poll, and proxy pointers for graceful exit.
static zactor_t  *proxy;
static zsock_t   *socketTx;
static zsock_t   *socketRx;
static zpoller_t *pollerRx;

// Store global reference for time.
uint32_t timeMs = 0;

// Utility function to convert char * to uint32_t.
// Returns 0 if invalid input.
uint32_t sil_atoiUint32(char *in)
{
    uint32_t res = 0;
    int      len = strlen(in);
    for (int i = 0; i < len; i += 1)
    {
        if ('0' > in[i] || in[i] > '9')
            return 0;

        res += (in[i] - '0') * pow(10, (len - i - 1));
    }

    return res;
}

// Returns the PID of the subprocess in which the requested binary runs.
// Blocks until a ready signal is received from the child board.
pid_t sil_board_run(Board *board)
{
    printf("Forking process for %s: %s\n", board->name, board->binPath);

    // Fork returns:
    //  -1 on error,
    //  0 if the current process is the child,
    //  the pid of the child process if the current process is the parent.
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Error forking process");
        exit(1);
    }

    if (pid == 0)
    {
        // Run binary on child process, argv must be NULL terminated.
        char const *argv[] = { board->binPath, board->name, NULL };
        if (execv(board->binPath, (char *const *)argv) == -1)
        {
            perror("Error running board binary");
            exit(1);
        }

        // Make sure to exit child process, to make sure parent process code is unreachable.
        exit(0);
    }

    // Unreachable from child process, return pid if parent process running.
    printf("Process forked on pid %ld\n", (long)pid);

    // Block until the board reports ready.
    // Control the main loop of all the boards.
    for (;;)
    {
        // zpoller_wait returns reference to the socket that is ready to recieve, or NULL.
        // there is only one such socket attachted to pollerRx, which is socketRx.
        // Since zpoller_wait may technically also return a zactor,
        // CZMQ common practice is to make this a void pointer.
        void *socket = zpoller_wait(pollerRx, 1);
        if (socket != NULL)
        {
            // Receive the message.
            char   *topic;
            zmsg_t *zmqMsg;

            if (zsock_recv(socket, "sm", &topic, &zmqMsg) == -1)
            {
                perror("Error: Failed to receive on socket");
            }
            else if (strcmp(topic, "ready") == 0)
            {
                // Receive ready topic.
                // Extract name of reporting board.
                char *readiedBoardName = zmsg_popstr(zmqMsg);
                if (readiedBoardName != NULL)
                {
                    // Check if returned boardName does not match, and if so, exit and error.
                    if (strcmp(readiedBoardName, board->name) != 0)
                    {
                        fprintf(
                            stderr, "Expected to receive ready signal from boardName: %s, instead received from %s",
                            board->name, readiedBoardName);
                        exit(1);
                    }

                    printf("Received ready signal from %s\n", readiedBoardName);

                    // Free up zmq-allocated memory.
                    free(topic);
                    free(readiedBoardName);
                    zmsg_destroy(&zmqMsg);

                    // Stop blocking.
                    break;
                }
            }

            // Free up zmq-allocated memory.
            free(topic);
            zmsg_destroy(&zmqMsg);
        }
    }

    return pid;
}

// Graciously exit process by freeing memory allocated by CZMQ.
void exitHandler()
{
    zpoller_destroy(&pollerRx);
    zactor_destroy(&proxy);
    zsock_destroy(&socketTx);
    zsock_destroy(&socketRx);
}

// Set the time to a given target.
// Will block until the target is reached.
// If a target lower than the current time is provided, will fail on assertion.
void sil_setTime(uint32_t targetMs)
{
    assert(targetMs >= timeMs);
    timeMs = targetMs;

    // Make a request to all the boards to set the time to targetMs.
    zsock_send(socketTx, "s4", "time_req", targetMs);

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
        void *socket = zpoller_wait(pollerRx, 1);
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
                        uint32_t receivedTimeMs = sil_atoiUint32(receivedTimeMsStr);

                        // Update record.
                        for (size_t boardIndex = 0; boardIndex < boardCount; boardIndex += 1)
                            if (strcmp(receivedBoardName, boards[boardIndex].name) == 0)
                                boards[boardIndex].timeMs = receivedTimeMs;

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
            for (size_t boardIndex = 0; boardIndex < boardCount; boardIndex += 1)
                boardsHitTargetTime &= boards[boardIndex].timeMs >= targetMs;

            // If they have, break out.
            if (boardsHitTargetTime)
                break;
        }
    }
}

int main()
{
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

    atexit(exitHandler);

    // Spin-up boards.
    for (size_t boardIndex = 0; boardIndex < boardCount; boardIndex += 1)
    {
        sil_board_run(&boards[boardIndex]);
    }

    int64_t start = zclock_mono();
    sil_setTime(1000);
    printf("\nReal Time / Simulated Time: %lld\n", zclock_mono() - start);
    printf("\n--- Sleeping for 5 s ---\n");
    zclock_sleep(5000);
    sil_setTime(2000);
}