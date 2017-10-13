#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>

int readArgs(int argc, char *argv[], FILE **fileclass1, FILE **fileclass2);

int compare(const void *a, const void *b);

#endif //UTIL_H