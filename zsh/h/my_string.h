/* String helper: tokenization for shell input */
#ifndef MY_ZSH_H_MY_STRING_H_
#define MY_ZSH_H_MY_STRING_H_

#include <stdlib.h>
#include <string.h>

int my_split(char *str, const char *delimeter, char **array);

#endif