#ifndef _MALLOC_H
#define _MALLOC_H

#include "log.h"

#include <stdlib.h>  // malloc / free
#include <string.h>  // memcpy

#define FREE(p) do {                              \
        if ((p)) {                                \
            free((p));                            \
            (p) = NULL;                           \
        }                                         \
    } while (0)

#define MALLOC(dst, size) do {                                          \
        if (!((dst) = malloc((size)))) {                                \
            LOG_ERROR(AT ": malloc(%zu) failed, that sucks", (size));   \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
    } while (0)

#define ZALLOC(dst, size) do {                                          \
        MALLOC((dst), (size));                                          \
        memset((dst), 0, size);                                         \
     } while (0)

#define DUPE(dst, src, size) do {               \
        MALLOC((dst), (size));                  \
        memcpy((dst), (src), (size));           \
    } while (0)

#endif
