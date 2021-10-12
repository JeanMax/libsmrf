/*
D2R.exe + 0x2055E40 == pPlayerUnit
                       pPlayerUnit + 0x20 == pAct
                                             pAct + 0x14 == map seed int32_t.
*/
#ifndef _VMREAD_H
#define _VMREAD_H

#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/ptrace.h>
#include <sys/wait.h>


#include <stdint.h>
typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#include <byteswap.h>
/* #define SWAP16(x) ((x >> 8) | (x << 8)) */

/* #define SWAP32(x) (((x >> 24) & 0x000000ff)     \ */
/*                  | ((x << 8)  & 0x00ff0000)     \ */
/*                  | ((x >> 8)  & 0x0000ff00)     \ */
/*                  | ((x << 24) & 0xff000000)) */



typedef  struct UnitAny  UnitAny;

typedef  struct CollMap  CollMap;
struct CollMap {
    DWORD dwPosGameX;  // 0x00
    DWORD dwPosGameY;  // 0x04
    DWORD dwSizeGameX; // 0x08
    DWORD dwSizeGameY; // 0x0C
    DWORD dwPosRoomX;  // 0x10
    DWORD dwPosRoomY;  // 0x14
    DWORD dwSizeRoomX; // 0x18
    DWORD dwSizeRoomY; // 0x1C
    WORD* pMapStart;   // 0x20
    WORD* pMapEnd;     // 0x22
};

typedef  struct PresetUnit  PresetUnit;
struct PresetUnit {
    DWORD _1;                // 0x00
    DWORD dwTxtFileNo;       // 0x04
    DWORD dwPosX;            // 0x08
    PresetUnit* pPresetNext; // 0x0C
    DWORD _3;                // 0x10
    DWORD dwType;            // 0x14
    DWORD dwPosY;            // 0x18
};

typedef  struct Room2  Room2;
typedef  struct ActMisc  ActMisc;
typedef  struct Level  Level;
struct Level {
    DWORD _1[4];        // 0x00
    Room2* pRoom2First; // 0x10
    DWORD _2[2];        // 0x14
    DWORD dwPosX;       // 0x1C
    DWORD dwPosY;       // 0x20
    DWORD dwSizeX;      // 0x24
    DWORD dwSizeY;      // 0x28
    DWORD _3[96];       // 0x2C
    Level* pNextLevel;  // 0x1AC
    DWORD _4;           // 0x1B0
    ActMisc* pMisc;     // 0x1B4
    DWORD _5[6];        // 0x1BC
    DWORD dwLevelNo;    // 0x1D0
    DWORD _6[3];        // 0x1D4
    union {
        DWORD RoomCenterX[9];
        DWORD WarpX[9];
    }; // 0x1E0
    union {
        DWORD RoomCenterY[9];
        DWORD WarpY[9];
    };                   // 0x204
    DWORD dwRoomEntries; // 0x228
};

typedef  struct RoomTile  RoomTile;
struct RoomTile {
    Room2* pRoom2;   // 0x00
    RoomTile* pNext; // 0x04
    DWORD _2[2];     // 0x08
    DWORD* nNum;     // 0x10
};

typedef  struct Room1  Room1;
struct Room1 {
    Room1** pRoomsNear;  // 0x00
    DWORD _1[3];         // 0x04
    Room2* pRoom2;       // 0x10
    DWORD _2[3];         // 0x14
    CollMap* Coll;       // 0x20
    DWORD dwRoomsNear;   // 0x24
    DWORD _3[9];         // 0x28
    DWORD dwXStart;      // 0x4C
    DWORD dwYStart;      // 0x50
    DWORD dwXSize;       // 0x54
    DWORD dwYSize;       // 0x58
    DWORD _4[6];         // 0x5C
    UnitAny* pUnitFirst; // 0x74
    DWORD _5;            // 0x78
    Room1* pRoomNext;    // 0x7C
};

struct Room2 {
    DWORD _1[2];        // 0x00
    Room2** pRoom2Near; // 0x08
    DWORD _2[5];        // 0x0C
    struct {
        DWORD dwRoomNumber;  // 0x00
        DWORD _1;            // 0x04
        DWORD* pdwSubNumber; // 0x08
    } * pType2Info;          // 0x20
    Room2* pRoom2Next;       // 0x24
    DWORD dwRoomFlags;       // 0x28
    DWORD dwRoomsNear;       // 0x2C
    Room1* pRoom1;           // 0x30
    DWORD dwPosX;            // 0x34
    DWORD dwPosY;            // 0x38
    DWORD dwSizeX;           // 0x3C
    DWORD dwSizeY;           // 0x40
    DWORD _3;                // 0x44
    DWORD dwPresetType;      // 0x48
    RoomTile* pRoomTiles;    // 0x4C
    DWORD _4[2];             // 0x50
    Level* pLevel;           // 0x58
    PresetUnit* pPreset;     // 0x5C
};

typedef  struct Path  Path;
struct Path {
    WORD xOffset;         // 0x00
    WORD xPos;            // 0x02
    WORD yOffset;         // 0x04
    WORD yPos;            // 0x06
    DWORD _1[2];          // 0x08
    WORD xTarget;         // 0x10
    WORD yTarget;         // 0x12
    DWORD _2[2];          // 0x14
    Room1* pRoom1;        // 0x1C
    Room1* pRoomUnk;      // 0x20
    DWORD _3[3];          // 0x24
    UnitAny* pUnit;       // 0x30
    DWORD dwFlags;        // 0x34
    DWORD _4;             // 0x38
    DWORD dwPathType;     // 0x3C
    DWORD dwPrevPathType; // 0x40
    DWORD dwUnitSize;     // 0x44
    DWORD _5[4];          // 0x48
    UnitAny* pTargetUnit; // 0x58
    DWORD dwTargetType;   // 0x5C
    DWORD dwTargetId;     // 0x60
    BYTE bDirection;      // 0x64
};


typedef  struct Act  Act;
struct Act {
    DWORD _1[3];     // 0x00
    DWORD dwMapSeed; // 0x0C
    Room1* pRoom1;   // 0x10
    DWORD dwAct;     // 0x14
    DWORD _2[12];    // 0x18
    ActMisc* pMisc;  // 0x48
};

struct ActMisc {
    DWORD _1[37];           // 0x00
    DWORD dwStaffTombLevel; // 0x94
    DWORD _2[245];          // 0x98
    Act* pAct;              // 0x46C
    DWORD _3[3];            // 0x470
    Level* pLevelFirst;     // 0x47C
};


typedef  struct QuestInfo  QuestInfo;
struct QuestInfo {
    void* pBuffer; // 0x00
    DWORD _1;      // 0x04
};

typedef  struct Waypoint  Waypoint;
struct Waypoint {
    BYTE flags; // 0x00
};

typedef  struct PlayerData  PlayerData;
struct PlayerData {
    char szName[0x10];            // 0x00
    QuestInfo* pNormalQuest;      // 0x10
    QuestInfo* pNightmareQuest;   // 0x14
    QuestInfo* pHellQuest;        // 0x18
    Waypoint* pNormalWaypoint;    // 0x1c
    Waypoint* pNightmareWaypoint; // 0x20
    Waypoint* pHellWaypoint;      // 0x24
};


typedef  struct Player  Player;
struct Player {
    DWORD dwType;
    DWORD dwTxtFileNo;
    DWORD dwUnitId;
    DWORD dwMode;
    PlayerData* pPlayerData;
    DWORD _1;
    DWORD dwAct;
    Act* pAct;
    DWORD dwSeed[2];
    DWORD _2;
    Path* pPath;
};

#endif // _VMREAD_H
