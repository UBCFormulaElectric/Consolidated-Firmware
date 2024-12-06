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

int main()
{
    printf("Starting up SIL Supervisor\n");
    zsock_t *canSocketRx = zsock_new_sub("tcp://localhost:3000", "");

    // Spin-up boards.
    sil_runBoard(FSM_SIL_PATH);

    for (int i = 0; true; i += 1)
    {
        uint64_t data;
        uint32_t id;
        zsock_recv(canSocketRx, "448", &id, NULL, &data);

        printf("ID %d: %016llx\r\n", id, data);
    }

    // Wait for all forks to stop.
    wait(NULL);
}