#ifndef _PLAYER_H
#define _PLAYER_H

enum PlayerId  {
    PLAYER_AMA = 0,
    PLAYER_SOR,
    PLAYER_NEC,
    PLAYER_PAL,
    PLAYER_BAR,
    PLAYER_DRU,
    PLAYER_ASS,
    MAX_PLAYER,
};
typedef enum PlayerId  PlayerId;


#define MAX_PLAYER_NAME 16
#define MAX_PLAYER_CLASS 8

struct PlayerInfo {
    PlayerId id;
    char name[MAX_PLAYER_NAME];
    char classId[MAX_PLAYER_CLASS];
};
typedef struct PlayerInfo  PlayerInfo;

extern const PlayerInfo PLAYER_INFO[MAX_PLAYER];


#endif
