#include "sil.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// CZMQ for sockets.
#include <czmq.h>

#include "io_canRx.h"
#include "io_canTx.h"

static zsock_t   *socketTx;
static zsock_t   *socketRx;
static zpoller_t *pollerRx;

// Note: only valid since the program exits when main exits.
static const char *boardName;

// Utility function to convert char * to uint64_t.
// Returns 0 if invalid input.
uint64_t sil_atoiUint64(char *in)
{
    uint64_t res = 0;
    int      len = strlen(in);
    for (int i = 0; i < len; i += 1)
    {
        if ('0' > in[i] || in[i] > '9')
            return 0;

        res += (in[i] - '0') * pow(10, (len - i - 1));
    }

    return res;
}

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

// Graciously exit process by freeing memory allocated by czmq.
void sil_exitHandler()
{
    zpoller_destroy(&pollerRx);
    zsock_destroy(&socketTx);
    zsock_destroy(&socketRx);
}

// Utility function for debugging, prints out a JsonCanMsg.
void sil_printCanMsg(JsonCanMsg *msg)
{
    uint64_t uint64Data;
    memcpy(&uint64Data, msg->data, sizeof(uint64_t));

    printf(
        "ACTIVE BOARD: %4s, ID: %4d, DATA: %016llx, DLC: %d, PASSED FILTER?: %d\n", boardName, msg->std_id, uint64Data,
        msg->dlc, io_canRx_filterMessageId(msg->std_id));
}

// Can messages are transmitted in std_id, dlc, data order.
// We cast the 8 byte data array to a uint64_t for easier transmit.
// CZMQ topics are sent as a string prefix.
// Hence, the CZMQ image of the message is "s448" (topic, uint32_t, uint32_t, uint64_t).
// CZMQ images are like printf format strings, they define the format of a message in an easier foramt.

// Wrapper around zsock_send for sending a JsonCanMsg.
// Gets a pointer to the message to send.
// Socket is a void pointer to be consistent with CZMQ api, can be socket or actor.
int sil_sendJsonCanMsg(void *socket, JsonCanMsg *msg)
{
    uint64_t uint64Data;
    memcpy(&uint64Data, msg->data, sizeof(uint64_t));
    return zsock_send(socketTx, "s448", "can", msg->std_id, msg->dlc, uint64Data);
}

// Parse a JsonCanMsg from a zmsg_t.
// Gets zmqMsg, a pointer to the zmsg_t, and dumps the parsed data into the provided canMsg.
void sil_parseJsonCanMsg(zmsg_t *zmqMsg, JsonCanMsg *canMsg)
{
    // NOTE: zmsg_popstr allocates memory on the heap, which must be freed.
    // Extract std_id.
    char *stdIdStr = zmsg_popstr(zmqMsg);
    if (stdIdStr != NULL)
    {
        canMsg->std_id = sil_atoiUint32(stdIdStr);
        free(stdIdStr);
    }

    // Extract dlc.
    char *dlcStr = zmsg_popstr(zmqMsg);
    if (dlcStr != NULL)
    {
        canMsg->dlc = sil_atoiUint32(dlcStr);
        free(dlcStr);
    }

    // Extract data.
    char *dataStr = zmsg_popstr(zmqMsg);
    if (dataStr != NULL)
    {
        uint64_t uint64data = atoi(dataStr);
        memcpy(canMsg->data, &uint64data, 8);
        free(dataStr);
    }
}

// Task messages are received in taskTitle, timeMs order.
// Run a task message.
void sil_runTaskMsg(
    zmsg_t *zmqMsg,
    void    tasks_init(),
    void    tasks_1Hz(uint32_t timeMs),
    void    tasks_100Hz(uint32_t timeMs),
    void    tasks_1kHz(uint32_t timeMs))
{
    // NOTE: zmsg_popstr allocates memory on the heap, which must be freed.
    // Select task function.
    char *taskName         = zmsg_popstr(zmqMsg);
    void (*task)(uint32_t) = NULL;
    if (taskName != NULL)
    {
        if (strcmp(taskName, "1Hz") == 0)
            task = tasks_1Hz;
        else if (strcmp(taskName, "100Hz") == 0)
            task = tasks_100Hz;
        else if (strcmp(taskName, "1kHz") == 0)
            task = tasks_1kHz;
        else if (strcmp(taskName, "init") == 0)
        {
            // If given an init task, just invoke it.
            tasks_init();
        }

        free(taskName);
    }

    // Invoke the task.
    if (task != NULL)
    {
        // Parse time.
        char *timeMsStr = zmsg_popstr(zmqMsg);
        if (timeMsStr != NULL)
        {
            uint32_t timeMs = sil_atoiUint32(timeMsStr);

            // Invoke task with parsed time.
            task(timeMs);
            free(timeMsStr);
        }
    }
}

// Interface between sil and canbus.
// Hook for can to transmit a message via fakeCan.
void sil_txCallback(const JsonCanMsg *msg)
{
    if (sil_sendJsonCanMsg(socketTx, (JsonCanMsg *)msg) == -1)
        perror("Error sending jsoncan tx message");
    else
    {
        printf("TX | ");
        sil_printCanMsg((JsonCanMsg *)msg);
    }
}

// Insert a JsonCanMsg into the board's internal can system.
void sil_canRx(JsonCanMsg *msg)
{
    if (io_canRx_filterMessageId(msg->std_id))
    {
        printf("RX | ");
        sil_printCanMsg(msg);
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
    zsock_set_subscribe(socketRx, "task");
    zsock_set_subscribe(socketRx, "can");

    // Poll the rx socket.
    pollerRx = zpoller_new(socketRx, NULL);
    if (pollerRx == NULL)
    {
        perror("Error opening rx poller");
        exit(1);
    }

    tasks_init();

    // Register exit handler after creation of sockets, but before main loop,
    // to avoid CZMQ's own exit handler warnings.
    atexit(sil_exitHandler);
    for (;;)
    {
        // Parent process id becomes 1 when parent dies.
        // Every tick we poll to make sure we exit this child process.
        if (getppid() == 1)
            exit(0);

        // Sockets capture loop.
        for (;;)
        {
            // zpoller_wait returns reference to the socket that is ready to recieve, or NULL.
            // there is only one such socket attachted to pollerRx, which is socketRx.
            // Socket must be void pointer, uncast, since it may return NULL.
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
                else if (strcmp(topic, "can") == 0)
                {
                    // Can topic case.
                    JsonCanMsg canMsg;
                    sil_parseJsonCanMsg(zmqMsg, &canMsg);
                    sil_canRx(&canMsg);
                }
                else if (strcmp(topic, "task") == 0)
                {
                    // Task topic case.
                    sil_runTaskMsg(zmqMsg, tasks_init, tasks_1Hz, tasks_100Hz, tasks_1kHz);
                }
                else
                    fprintf(stderr, "Error: Unsupported topic %s", topic);

                // Free up zmq-allocated memory.
                free(topic);
                zmsg_destroy(&zmqMsg);
            }
            else
                break;
        }
    }
}