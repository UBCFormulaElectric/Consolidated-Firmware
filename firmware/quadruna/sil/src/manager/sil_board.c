#include "sil_board.h"
#include <stdio.h>
#include <czmq.h>
#include "sil_api.h"

void sil_board_run(sil_Board *board, zpoller_t *pollerRx)
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
        void *socket = zpoller_wait(pollerRx, 0);
        if (socket != NULL)
        {
            bool ready = false;
            // Receive the message.
            char   *topic;
            zmsg_t *zmqMsg;

            if (zsock_recv(socket, "sm", &topic, &zmqMsg) == -1)
            {
                perror("Error: Failed to receive on socket");
            }
            else if (strcmp(topic, "ready") == 0)
            {
                // Receive the ready message when it is sent, and check that the board name matches.
                sil_api_Ready *msg = sil_api_ready_rx(zmqMsg);
                assert(msg != NULL && strcmp(msg->boardName, board->name) == 0);
                sil_api_ready_destroy(msg);
                ready = true;
            }

            // Free up zmq-allocated memory.
            free(topic);
            zmsg_destroy(&zmqMsg);

            // If the board is ready, stop blocking.
            if (ready)
                break;
        }
    }

    board->pid = pid;
};

void sil_board_reset(sil_Board *board)
{
    if (board->pid != -1)
    {
        // Send kill signal.
        kill(board->pid, SIGKILL);

        // Wait for the process to die.
        int status;
        waitpid(board->pid, &status, 0);

        // Verify process is dead.
        if (!(WIFEXITED(status) || WIFSIGNALED(status)))
        {
            fprintf(stderr, "Error: Killing board %s resulted in unexpected status.", board->name);
        }
    }

    board->timeMs = 0;
    board->pid    = -1;
}

sil_Board sil_board_new(const char *name, const char *binPath)
{
    sil_Board res = { .name = name, .binPath = binPath, .pid = -1, .timeMs = 0 };

    return res;
};