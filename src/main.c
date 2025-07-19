#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CMD 1000000
#define MAX_ARGS 64

void handle_sigint(int sig) {
    printf("\n");
    printf("ezshell> ");
    fflush(stdout);
}

int main(void) {
    char buffer[MAX_CMD];
    char *args[MAX_ARGS];
    pid_t pid;
    int status;

    signal(SIGINT, handle_sigint);

	printf("exit with ctrl+d\n");
	
    while (1) {
        printf("ezshell> ");
        fflush(stdout);

        if (fgets(buffer, MAX_CMD, stdin) == NULL) {
            printf("\n");
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;

        char *token;
        int i = 0;
        token = strtok(buffer, " ");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            signal(SIGINT, SIG_DFL);
            if (execvp(args[0], args) == -1) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        } else {
            waitpid(pid, &status, 0);
        }
    }

    printf("exiting ezshell\n");
    return 0;
}
