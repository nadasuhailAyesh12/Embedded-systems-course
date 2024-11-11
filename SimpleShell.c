#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LINE 80 // Maximum length of a command

int main(void) {
    char *args[MAX_LINE ]; // Command line arguments
    char input[MAX_LINE]; // User input
    int should_run = 1; // Flag to determine when to exit the shell

    while (should_run) {
        printf("enter shell command > ");
        fflush(stdout);

        // Read the user input
        if (fgets(input, MAX_LINE, stdin) == NULL) {
            perror("fgets failed");
            exit(1);
        }

        // Remove the newline character from the input
        input[strcspn(input, "\n")] = '\0';

        // Exit the shell if the user types 'exit'
        if (strcmp(input, "exit") == 0) {
            should_run = 0;
            continue;
        }

        // Parse the input into arguments
        int i = 0;
        char *token = strtok(input, " \n");
        while (token != NULL) {
           args[i++] = token;
            token = strtok(NULL, " \n");
        }
        args[i] = NULL;
         // Null-terminate the arguments array


         if (strcmp(args[0], "cd") == 0) {
            if (i < 2) {
                printf("Usage: cd <directory>\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("chdir failed");
                }
            }
            continue;
        }

        // Create a child process
        pid_t pid = fork();
        if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process
            if (execvp(args[0], args) < 0) {
                perror("Exec failed");
            }
            exit(1);
        } else {
            // Parent process
            wait(NULL); // Wait for the child process to finish
        }
    }

    return 0;
}