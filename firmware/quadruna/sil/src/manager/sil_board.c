#include "sil_board.h"
#include <stdio.h>
#include <czmq.h>

pid_t sil_board_run(sil_Board *board, zpoller_t *pollerRx)
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
};