#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>
#include "configuration.h"

int readArgs(int argc, char *argv[], FILE **fileclass1, FILE **fileclass2);

int compare(const void *a, const void *b);

void createFilename(char *filename, char *str1, char *str2);

#endif //UTIL_H