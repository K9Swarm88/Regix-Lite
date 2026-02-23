#ifndef MATCH_H
#define MATCH_H
#include <stdbool.h>

// return true if pattern match
bool match(const char *pattern, const char *text);

// recursive matcher
bool match_here(const char *pattern, const char *text);

#endif