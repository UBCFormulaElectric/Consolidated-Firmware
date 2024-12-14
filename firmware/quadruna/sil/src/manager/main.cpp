#include <stdio.h>
#include <stdlib.h>
#include <czmq.h>

static zactor_t *proxy;
static zsock_t  *socketTx;

// Returns the PID of the subprocess in which the requested binary runs.
pid_t sil_runBoard(const char *binPath, const char *boardName)
{
    printf("Forking process for %s: %s\n", boardName, binPath);

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
        char const *argv[] = { binPath, boardName, NULL };
        if (execv(binPath, (char *const *)argv) == -1)
        {
            perror("Error forking process");
            exit(1);
        }

        // Make sure to exit child process, to make sure parent process code is unreachable.
        exit(0);
    }

    // Unreachable from child process, return pid if parent process running.
    printf("  Process forked on pid %ld\n", (long)pid);
    return pid;
}

// Send a task message.
// Tasks are transmited in taskName, timeMs order.
// Hence the CZMQ image of this message is "ss4" (topic, char *, uint32_t).
int sil_sendTaskMsg(const char *taskName, uint32_t timeMs)
{
    return zsock_send(socketTx, "ss4", "task", taskName, timeMs);
}

// Graciously exit process by freeing memory allocated by CZMQ.
void exitHandler()
{
    zactor_destroy(&proxy);
    zsock_destroy(&socketTx);
}

int main()
{
    printf("Starting up SIL Supervisor\n");

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

    // Setup a tx socket into the pub/sub network, to allow the manager to inject messages.
    // Prefixing the endpoint with ">" connects to the endpoint,
    // rather than the default bind behavior.
    socketTx = zsock_new_pub(">tcp://localhost:3001");
    if (socketTx == NULL)
    {
        perror("Error opening tx socket");
        exit(1);
    }

    atexit(exitHandler);

    // Spin-up boards.
    sil_runBoard("./build_fw_sil/firmware/quadruna/RSM/quadruna_RSM_sil", "RSM");
    sil_runBoard("./build_fw_sil/firmware/quadruna/FSM/quadruna_FSM_sil", "FSM");
    sil_runBoard("./build_fw_sil/firmware/quadruna/VC/quadruna_VC_sil", "VC");
    sil_runBoard("./build_fw_sil/firmware/quadruna/CRIT/quadruna_CRIT_sil", "CRIT");
    sil_runBoard("./build_fw_sil/firmware/quadruna/BMS/quadruna_BMS_sil", "BMS");

    // Control the main loop of all the boards.
    sil_sendTaskMsg("init", 0);
    for (int timeMs = 0; true; timeMs += 1)
    {
        zclock_sleep(1);

        // 1 kHz task.
        if (timeMs % 1 == 0)
            sil_sendTaskMsg("1kHz", timeMs);

        // 100 Hz task.
        if (timeMs % 10 == 0)
            sil_sendTaskMsg("100Hz", timeMs);

        // 1 Hz task.
        if (timeMs % 1000 == 0)
            sil_sendTaskMsg("1Hz", timeMs);
    }

    // Wait for all forks to stop.
    wait(NULL);
}