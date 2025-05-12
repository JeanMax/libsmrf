#ifndef _SMRF_H
#define _SMRF_H

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <pthread.h>

#include "smrf/d2sdk.h"
#include "smrf/d2structs.h"
#include "smrf/proc.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x)    ((x) > 0   ? (x) : -(x))

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


// TODO: optimize lock use
#define UPDATE_STATUS(game, statux) do {        \
        pthread_mutex_lock(&game->mutex);       \
        strcpy(game->status, statux);           \
        pthread_mutex_unlock(&game->mutex);     \
    } while (0)



typedef  struct PtrList  PtrList;
struct PtrList {
    ptr_t ptr;
    PtrList *pNext;
};


typedef  struct PlayerList  PlayerList;
struct PlayerList {
    Player player;
    ptr_t player_data_addr;
    PlayerList *pNext;
};


typedef  struct PlayerContent  PlayerContent;
struct PlayerContent {
    PlayerData player_data;
    Act act;
    Path path;
    Room1 room1;
    Room2 room2;
};

#define MAX_STATUS_LEN 32

typedef  struct GameState  GameState;
struct GameState {
    char status[MAX_STATUS_LEN];
    Player player;
    Level *level;
    ptr_t _player_addr; //TODO: internal, hide
    pid_t _pid; //TODO: internal, hide
    byte _pad[4];
    pthread_mutex_t mutex;
    Level **all_levels; // Level *all_levels[MAX_AREA]
};


bool       update_game_state(GameState *game);
void       init_game_state(GameState *game);
void       destroy_game_state(GameState *game);


#ifdef __cplusplus
}
#endif

#endif
