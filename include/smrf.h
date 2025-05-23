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

#include "smrf/d2sdk.h"  // for convenience when using the lib


// TODO: optimize lock use
#define UPDATE_STATUS(game, statux) do {        \
        pthread_mutex_lock(&game->mutex);       \
        strcpy(game->status, statux);           \
        pthread_mutex_unlock(&game->mutex);     \
    } while (0)



typedef  struct PlayerList  PlayerList;
struct PlayerList {
    Player player;
    ptr_t player_addr;
    int  idx;                   /* DEBUG */
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

#define STATUS_LEN_MAX 32

typedef  struct GameState  GameState;
struct GameState {
    char player_name_setting[PLAYER_DATA_NAME_MAX];
    char status[STATUS_LEN_MAX];
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
