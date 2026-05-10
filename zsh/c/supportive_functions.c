#include "../h/supportive_functions.h"

/* Zero out `buffer` of given `length` */
void clear_buffer(char* buffer, size_t length)
{
    memset(buffer, 0, length);
}

/* Free memory allocated for an array of strings */
void deallocate_substring(char* substrings[])
{
    for (int i = 0; substrings[i] != NULL; i++)
    {
        free(substrings[i]);
    }
}

/* Write an error message to stderr (fd 2) */
void error_message(char* message)
{
    write(2, message, strlen(message));
}

/* Print each environment variable on a new line */
void print_env(char** env)
{
    for (int i = 0; env[i] != NULL; i++)
    {
        printf("%s\n", env[i]);
    }
}

/* Split environment assignment like KEY=VALUE into tokens */
void formatting(char* substrings[])
{
    int i = 1;
    char* token = strtok(substrings[1], "=");
    while (token != NULL)
    {
        substrings[i++] = strdup(token);
        token = strtok(substrings[1], "=");
    }
}

/* Build a path by joining `token` (path) and `command` with '/' */
char* copy(char* token, char* command)
{
    int length = (strlen(token) + strlen(command) + 2);
    char* tmp = calloc(length, sizeof(char));
    strcat(tmp, token);
    strcat(tmp, "/");
    strcat(tmp, command);
    return tmp;
}

/* Search PATH for executable `command`; set `*answer` to full path */
int is_accessible(char* command, char** answer)
{
    char* path = malloc(sizeof(char) * PATH_MAX);
    strcpy(path, getenv("PATH"));
    char* token = strtok(path, ":");
    char* tmp;
    while (token != NULL)
    {
        tmp = copy(token, command);
        if (access(tmp, X_OK) == 0)
        {
            *answer = strdup(tmp);
            free(path);
            free(tmp);
            return 1;
        }
        token = strtok(NULL, ":");
        free(tmp);
    }
    free(path);
    return 0;
}

/* Fork and exec the command in `substrings` using `env` */
int executor(char** substrings, char** env)
{
    pid_t pid;
    int status;
    char* path = NULL;
    if (strstr(substrings[0], "./") != NULL)
    {
        path = realpath(substrings[0], path);
        if (access(path, X_OK) != 0)
        {
            error_message("command not found\n");
            return 0;
        }
        if (path == NULL)
        {
            error_message("No such file or directory\n");
            return 0;
        }
    }
    else if (is_accessible(substrings[0], &path) == 0)
    {
        free(path);
        error_message("command not found\n");
        return 0;
    }

    pid = fork();

    if (pid == 0)
    {
        if (execve(path, substrings, env) == -1)
        {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        int signal = WTERMSIG(status);
        if (signal == SIGSEGV)
        {
            error_message("segmentation fault\n");
        }
    }
    free(path);
    return 1;
}