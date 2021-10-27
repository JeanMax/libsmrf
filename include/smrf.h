#ifndef _SMRF_H
#define _SMRF_H

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <pthread.h>

#include "smrf/d2structs.h"
#include "smrf/proc.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define FREE(p) do {                              \
         if ((p)) {                               \
             free((p));                           \
             (p) = NULL;                          \
         }                                        \
     } while (0)

#define MALLOC(dst, size) do {                                      \
        if (!((dst) = malloc((size)))) {                            \
            LOG_ERROR("malloc(%lu) failed, that sucks", (size));    \
            exit(EXIT_FAILURE);                                     \
        }                                                           \
    } while (0)

#define DUPE(dst, src, size) do {               \
        MALLOC((dst), (size));                  \
        memcpy((dst), (src), (size));           \
    } while (0)


#define MAX_AREA 137

typedef  struct PlayerContent  PlayerContent;
struct PlayerContent {
    PlayerData player_data;
    Act act;
    Path path;
    Room1 room1;
    Room2 room2;
};

typedef  struct GameState  GameState;
struct GameState {
    Player player;
    Level *level;
    PTR _player_addr; //TODO: internal, hide
    pid_t _pid; //TODO: internal, hide
    BYTE _pad[4];
    pthread_mutex_t mutex;
    Level **all_levels; // Level *all_levels[MAX_AREA]
};

GameState *refresh(void);


#ifdef __cplusplus
}
#endif

#endif
