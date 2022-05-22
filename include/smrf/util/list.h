#ifndef _LIST_H
#define _LIST_H

#include <string.h>
#include "types.h"
#include "log.h"

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


typedef  struct list_t  list_t;
struct list_t {
    list_t *next;
    size_t p_size;
    void *p;
};

static inline list_t *list_dupe(const void *src, size_t p_size)
{
    void *relou;
    list_t *newby;
    MALLOC(relou, sizeof(list_t) - sizeof(byte *) + p_size);
    newby = (list_t *)relou;
    newby->next = NULL;
    newby->p_size = p_size;
    if (src) {
        memcpy(newby->p, src, p_size);
    } else {
        newby->p = NULL;
    }
    return newby;
}

static inline void list_free(list_t *list)
{
    list_t *next;
    while (list) {
        next = list->next;
        free(list);
        list = next;
    }
}


#endif
