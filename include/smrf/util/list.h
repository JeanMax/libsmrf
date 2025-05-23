#ifndef _LIST_H
#define _LIST_H


#define ADD_LINK(first, last, newby) do {       \
        if (!(first)) {                         \
            (first) = (newby);                  \
        } else {                                \
            (last)->pNext = (newby);            \
        }                                       \
        (last) = (newby);                       \
    } while (0)

#define PUSH_LINK(head, newby) do {             \
        (newby)->pNext = (head);                \
        (head) = (newby);                       \
    } while (0)

#define LAST_LINK(first, last) do {             \
        if ((first)) {                          \
            (last) = (first);                   \
            while ((last)->pNext) {             \
                (last) = (last)->pNext;         \
            }                                   \
        }                                       \
    } while (0)

#endif
