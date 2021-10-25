#ifndef _SMRF_H
#define _SMRF_H

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <unistd.h> // sleep

#include "smrf/d2structs.h"
#include "smrf/proc.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

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


typedef  struct GameState  GameState;
struct GameState {
    Player player;
    PlayerData player_data;
    Path path;
    Act act; //TODO: remove?
    Room1 room1; //TODO: remove?
    Room2 room2; //TODO: remove?
    Level *level;
    PTR player_addr;
    pid_t pid;
    BYTE _pad[4];
};

GameState *refresh(void);


#ifdef __cplusplus
}
#endif

#endif