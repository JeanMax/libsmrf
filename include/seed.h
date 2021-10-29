#ifndef _SEED_H
#define _SEED_H

#define _GNU_SOURCE

#include <unistd.h> // sleep

#include "d2structs.h"
#include "proc.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


typedef  struct GameState  GameState;
struct GameState {
    Player player;
    PlayerData player_data;
    Path path;
    Act act;
    Room1 room1;
    Room2 room2;
    Level level;
    ptr_t player_addr;
    pid_t pid;
    byte _pad[4];
};

#endif
