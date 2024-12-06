#include <stdio.h>
#include <stdlib.h>
#include <czmq.h>

#define FSM_SIL_PATH "./build_fw_sil/firmware/quadruna/FSM/quadruna_FSM_sil"

// Returns the PID of the subprocess in which the requested binary runs.
pid_t sil_runBoard(const char *bin_path)
{
    printf("Forking process for %s\n", bin_path);

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
        // Run binary on child process.
        char const *argv[2] = { bin_path, NULL };
        if (execv(FSM_SIL_PATH, (char *const *)argv) == -1)
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

static zsock_t *canProxySocketTx;
static zsock_t *canProxySocketRx;

// Graciously exit process by freeing memory allocated by czmq.
void exitHandler()
{
    zsock_destroy(&canProxySocketTx);
    zsock_destroy(&canProxySocketRx);
}

int main()
{
    printf("Starting up SIL Supervisor\n");

    // Normal pub/sub connections are one-to-many, or many-to-one.
    // To allow for free comms between boards, this process acts as a proxy.
    // Eg. FSM --> canProxySocketRx --> canProxySocketTx --> VC.
    canProxySocketTx = zsock_new_pub("tcp://localhost:3000");
    if (canProxySocketTx == NULL)
    {
        perror("Error opening can tx proxy socket");
        exit(1);
    }

    // By default (ie. with zsock_new_sub()/zsock_new_pub()),
    // there can only be one pub, and many subs, since subs connect, and pubs bind.
    // In order to invert the order (many pubs, and one sub), we must build the socket manually.
    canProxySocketRx = zsock_new(ZMQ_SUB);
    if (canProxySocketRx == NULL)
    {
        perror("Error opening can rx proxy socket");
        exit(1);
    }
    if (zsock_bind(canProxySocketRx, "tcp://localhost:3001") == -1)
    {
        perror("Error binding can rx proxy socket");
        exit(1);
    }
    zsock_set_subscribe(canProxySocketRx, "");

    atexit(exitHandler);

    // Spin-up boards.
    sil_runBoard(FSM_SIL_PATH);

    // Forward messages from rx to tx.
    for (int i = 0; true; i += 1)
    {
        zmsg_t *msg = zmsg_recv(canProxySocketRx);
        if (msg == NULL)
            continue;
        zmsg_send(&msg, canProxySocketTx);
    }

    // Wait for all forks to stop.
    wait(NULL);
}