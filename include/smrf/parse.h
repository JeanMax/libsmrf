#ifndef _PARSE_H
#define _PARSE_H

#include "d2structs.h"
#include "d2sdk.h"


#define FREE(p) do {                              \
         if ((p)) {                               \
             free((p));                           \
             (p) = NULL;                          \
         }                                        \
     } while (0)

#define MALLOC(dst, size) do {                                          \
         if (!((dst) = malloc((size)))) {                               \
             LOG_ERROR(AT ": malloc(%zu) failed, that sucks", (size));  \
             exit(EXIT_FAILURE);                                        \
         }                                                              \
     } while (0)

#define DUPE(dst, src, size) do {               \
        MALLOC((dst), (size));                  \
        memcpy((dst), (src), (size));           \
    } while (0)


extern Level      *g_levels[MAX_AREA];

void free_all_levels(void);
Level *parse_level_list(pid_t pid, ptr_t level_addr);


#endif
