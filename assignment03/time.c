#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage - %s COMMAND [ARGS...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    pid_t pid;
    struct timeval start, end;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (gettimeofday(&start, NULL) == -1) {
        perror("gettimeofday");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // child
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        execvp(argv[1], &argv[1]);

        fprintf(stderr, "Failed to execute '%s': %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    } else {
        // parents
        close(pipefd[1]); 

        char buffer[BUFSIZ];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, 1, bytesRead, stdout);
        }
        if (bytesRead == -1) {
            perror("read");
        }
        close(pipefd[0]);

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        if (gettimeofday(&end, NULL) == -1) {
            perror("gettimeofday");
            exit(EXIT_FAILURE);
        }

        double elapsed = (end.tv_sec - start.tv_sec) +
                         (end.tv_usec - start.tv_usec) / 1000000.0;

        printf("\nElapsed time %.5f seconds\n", elapsed);

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return EXIT_FAILURE;
        }
    }
}