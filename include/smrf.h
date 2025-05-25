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
#include "smrf/util/hash.h"

#include "smrf/d2sdk.h"  // for convenience when using the lib

extern Htable *g_unit_table;  //TODO: berk

#define UPDATE_STATUS(game, statux) do {        \
        strcpy(game->status, statux);           \
    } while (0)


typedef  struct PlayerList  PlayerList;
struct PlayerList {
    Player player;
    ptr_t player_addr;
    long  idx;                   /* DEBUG */
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


#define WINDOW_TITLE_MAX 256
#define STATUS_MAX 32

typedef  struct GameState  GameState;
struct GameState {
    char window_title_setting[WINDOW_TITLE_MAX];  // need to be 1st
    char status[STATUS_MAX];
    Level *level;
    Player player;
    ptr_t _player_addr; //TODO: internal, hide
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
