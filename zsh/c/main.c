#include "../h/main.h"

#define MAX_BUFFER_SIZE 256
#define MAX_SUBSTRINGS 100

/* Stores the previous working directory for `cd -` support */
static char* prev_dir = NULL;

/* Print text or expand and print an environment variable */
void my_echo(char** substrings, char** env)
{
    if (substrings[1][0] == '$')
    {
        char* tmp;
        if (!(tmp = getenv(&(substrings[1][1]))))
        {
            printf("echo: variable not found!\n");
            return;
        };
        printf("%s\n", tmp);
    }
    else
    {
        executor(substrings, env);
    }
}

/* Save the current working directory in `prev_dir` */
void save_previous_path()
{
    char* buffer;
    buffer = calloc(sizeof(char), MAX_BUFFER_SIZE);
    getcwd(buffer, MAX_BUFFER_SIZE);
    if (prev_dir != NULL)
    {
        free(prev_dir);
    }
    prev_dir = strdup(buffer);
    free(buffer);
}

/* Change the current working directory; handle '-' (previous dir) */
void my_cd(char** substrings)
{
    char* buff;
    if (!substrings[1])
    {
        error_message("No such file or directory\n");
    }
    else if (strcmp(substrings[1], "-") == 0)
    {
        if (prev_dir == NULL)
        {
            save_previous_path();
            return;
        }
        buff = calloc(sizeof(char), MAX_BUFFER_SIZE);
        getcwd(buff, MAX_BUFFER_SIZE);
        chdir(prev_dir);
        free(prev_dir);
        prev_dir = strdup(buff);
        free(buff);
        return;
    }

    save_previous_path();

    if (chdir(substrings[1]) != 0)
    {
        perror("chdir");
        return;
    }
}

/* Dispatch built-in commands (cd, echo, env, setenv, unsetenv)
    or forward to external executor */
void redirect(char* substrings[], char** env)
{
    if ((strcmp(substrings[0], "quit") == 0) ||
        (strcmp(substrings[0], "exit") == 0))
    {
        deallocate_substring(substrings);
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(substrings[0], "env") == 0)
    {
        print_env(env);
    }
    else if (strcmp(substrings[0], "setenv") == 0)
    {
        if (strchr(substrings[1], '=') != NULL)
        {
            int i = 1;
            char* token = strtok(substrings[1], "=");
            while (token != NULL)
            {
                substrings[i++] = strdup(token);
                token = strtok(NULL, "=");
            }
        }
        else if (strchr(substrings[2], '=') != NULL) {
            if (substrings[3] != NULL) {
                free(substrings[2]);
                substrings[2] = strdup(substrings[3]);
            }
        }
        if (setenv(substrings[1], substrings[2], 0) == -1)
        {
            error_message("setenv: failure!\n");
        }
    }
    else if (strcmp(substrings[0], "unsetenv") == 0)
    {
        if (unsetenv(substrings[1]) == -1)
        {
            error_message("unsetenv:  failure!\n");
        }
    }
    else if (strcmp(substrings[0], "echo") == 0)
    {
        my_echo(substrings, env);
    }
    else if (strcmp(substrings[0], "cd") == 0)
    {
        my_cd(substrings);
    }
    else
    {
        executor(substrings, env);
    }
}

/* Read-eval loop: show prompt, read a line, split and execute */
void promp(char* buffer, size_t length, char* substrings[], char** env)
{
    while (1)
    {
        clear_buffer(buffer, length);
        getcwd(buffer, length);
        printf("[%s]>", buffer);
        clear_buffer(buffer, length);
        if (getline(&buffer, &length, stdin) == -1)
        {
            if (prev_dir != NULL)
            {
                free(prev_dir);
            }
            break;
        }
        my_split(buffer, " ", substrings);
        if (!substrings[0])
        {
            continue;
        }
        redirect(substrings, env);
        deallocate_substring(substrings);
    }
    free(buffer);
}

/* Program entry: allocate buffers and start the prompt loop */
int main(int argc, char** argv, char** env)
{
    if (argc > 2 || !argv)
    {
        return 0;
    }
    char* buffer = (char*)malloc(MAX_BUFFER_SIZE);
    size_t length = MAX_BUFFER_SIZE;
    char* substrings[MAX_SUBSTRINGS];
    promp(buffer, length, substrings, env);
    return 0;
}