#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include "config.h"

#define FDEBUG(file, ...) { \
	fprintf(file, __FILE__ ":%s:%d: ", __func__, __LINE__); \
	fprintf(file, __VA_ARGS__); \
	fputc('\n', file); \
}

#ifndef DISABLE_DEBUG
#  define DEBUG(...) FDEBUG(stdout, __VA_ARGS__)
#  define E_DEBUG(...) FDEBUG(stderr, __VA_ARGS__)
#else
#  define DEBUG(...)
#  define E_DEBUG(...)
#endif

#endif /* DEBUG_H */
