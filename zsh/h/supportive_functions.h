/* Supportive utilities used by the shell (env, exec, helpers) */
#ifndef MY_ZSH_H_SUPPORTIVE_FUNCTIONS_H_
#define MY_ZSH_H_SUPPORTIVE_FUNCTIONS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <linux/limits.h>

void print_env(char** env);
void error_message(char* message);
void formatting(char* substrings[]);
void deallocate_substring(char* substrings[]);
void clear_buffer(char* buffer, size_t length);
int executor(char** substrings, char** env);
int is_accessible(char* command, char** answer);
char* copy(char* token, char* command);

#endif