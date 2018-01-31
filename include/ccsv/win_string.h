#include <stdio.h>
#include <stdlib.h>
#include "ccsv_config.h"

ssize_t getline(char **lineptr, size_t *n, FILE *stream);
char *strsep(char** stringp, const char* delim);
