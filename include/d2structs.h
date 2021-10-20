#ifndef _D2STRUCTS_H
#define _D2STRUCTS_H

#include <ctype.h>

#include "proc.h"  // is_valid_ptr
#include "util/types.h"
#include "util/log.h"

typedef  struct UnitAny  UnitAny;

typedef  struct CollMap  CollMap;
/* struct CollMap { */
/*     DWORD dwPosGameX;  // 0x00 */
/*     DWORD dwPosGameY;  // 0x04 */
/*     DWORD dwSizeGameX; // 0x08 */
/*     DWORD dwSizeGameY; // 0x0C */
/*     DWORD dwPosRoomX;  // 0x10 */
/*     DWORD dwPosRoomY;  // 0x14 */
/*     DWORD dwSizeRoomX; // 0x18 */
/*     DWORD dwSizeRoomY; // 0x1C */
/*     WORD* pMapStart;   // 0x20 */
/*     WORD* pMapEnd;     // 0x22 */
/* }; */

typedef  struct PresetUnit  PresetUnit;
/* struct PresetUnit { */
/*     DWORD _1;                // 0x00 */
/*     DWORD dwTxtFileNo;       // 0x04 */
/*     DWORD dwPosX;            // 0x08 */
/*     DWORD _pad; */
/*     PresetUnit* pPresetNext; // 0x0C */
/*     DWORD _3;                // 0x10 */
/*     DWORD dwType;            // 0x14 */
/*     DWORD dwPosY;            // 0x18 */
/* }; */

typedef  struct Room2  Room2;
typedef  struct ActMisc  ActMisc;
typedef  struct Level  Level;
struct Level {
    DWORD _1[4];        // 0x00
    Room2* pRoom2First; // 0x10
    void* pDunno;
    DWORD _2;        // 0x14
    DWORD dwPosX;       // 0x1C
    DWORD dwPosY;       // 0x20
    DWORD dwSizeX;      // 0x24
    DWORD dwSizeY;      // 0x28
    DWORD _3[96];       // 0x2C
    DWORD _pad;
    Level* pNextLevel;  // 0x1AC
    DWORD _4[4];           // 0x1B0
    ActMisc* pMisc;     // 0x1B4
    DWORD _5[8];        // 0x1BC
    DWORD  dwLevelNo;    // 0x1D0
    /* BYTE  unk[6]; */
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
    BYTE _pad1[4];
};

typedef  struct RoomTile  RoomTile;
/* struct RoomTile { */
/*     Room2* pRoom2;   // 0x00 */
/*     RoomTile* pNext; // 0x04 */
/*     DWORD _2[2];     // 0x08 */
/*     DWORD* nNum;     // 0x10 */
/* }; */

typedef  struct Room1  Room1;
struct Room1 {
    Room1** pRoomsNear;  //0
    void *pDunno1; //8
    DWORD _1[2];    //16
    Room2* pRoom2;      //24
    DWORD _2[2];         //32
    CollMap* Coll;       //40 nop?
    DWORD dwRoomsNear;   //48
    DWORD _2bis; //52
    void *pDunno2; //56
    DWORD *pDunno2bis;         //64
    void *pDunno3; //72  next?
    DWORD _3[12];         //80
    DWORD dwXStart; //128
    DWORD dwYStart; //132
    DWORD dwXSize; //136
    DWORD dwYSize; //140
    DWORD _4[6]; //144
    UnitAny* pUnitFirst; //168
    Room1* pRoomNext; //176
    DWORD _5[2]; //184
    DWORD _6[2]; //192
    DWORD _debug[8]; //200
};

struct Room2 {
    DWORD _1[2];        //0
    Room2** pRoom2Near; //8
    DWORD _2[6];        //16
    void* pDunno1;       //40
    DWORD dwRoomFlags;  //48
    DWORD dwRoomsNear;  //52
    void* pDunno2;       //56
    RoomTile* pRoomTiles; //64
    Room2* pRoom2Next;    //72
    void* pDunno3;         //80
    Room1* pRoom1;        //88
    DWORD dwPosX;         //96
    DWORD dwPosY;         //100
    DWORD dwSizeX;        //104
    DWORD dwSizeY;        //108
    DWORD _3;             //112
    DWORD dwPresetType;   //116
    DWORD _4[2];          //120
    PresetUnit* pPreset;  //128
    DWORD _5;             //136
    BYTE _pad1[4];        //140
    Level *pLevel;        //144
    DWORD _debug[10]; //152
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
    DWORD _pad1;
    Room1* pRoom1;        // 0x1C
    Room1* pRoomUnk;      // 0x20
    DWORD _3[3];          // 0x24
    DWORD _pad2;
    UnitAny* pUnit;       // 0x30
    DWORD dwFlags;        // 0x34
    DWORD _4;             // 0x38
    DWORD dwPathType;     // 0x3C
    DWORD dwPrevPathType; // 0x40
    DWORD dwUnitSize;     // 0x44
    DWORD _5[4];          // 0x48
    DWORD _pad;
    UnitAny* pTargetUnit; // 0x58
    DWORD dwTargetType;   // 0x5C
    DWORD dwTargetId;     // 0x60
    BYTE bDirection;      // 0x64
    BYTE _pad3[7];
};


typedef  struct Act  Act;
struct Act {
    DWORD _1[2];
    void  *pDunno;
    DWORD _3;
    DWORD dwMapSeed;
    Room1* pRoom1;
    DWORD dwAct;
    DWORD _2[9];
    ActMisc* pMisc;
};

/* struct ActMisc { */
/*     DWORD _1[37];           // 0x00 */
/*     DWORD dwStaffTombLevel; // 0x94 */
/*     DWORD _2[245];          // 0x98 */
/*     DWORD _pad;          // 0x98 */
/*     Act* pAct;              // 0x46C */
/*     DWORD _3[3];            // 0x470 */
/*     Level* pLevelFirst;     // 0x47C */
/* }; */


typedef  struct QuestInfo  QuestInfo;
/* struct QuestInfo { */
/*     void* pBuffer; // 0x00 */
/*     DWORD _1;      // 0x04 */
/*     DWORD _pad;      // 0x04 */
/* }; */

typedef  struct Waypoint  Waypoint;
/* struct Waypoint { */
/*     BYTE flags; // 0x00 */
/* }; */

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


typedef  struct UnitAny  Player;
struct UnitAny {
    DWORD dwType;      // 0x00
    DWORD dwTxtFileNo; // 0x04
    DWORD _1;          // 0x08
    DWORD dwUnitId;    // 0x0C
    DWORD dwMode;      // 0x10
    DWORD _pad1;
    union {
        PlayerData* pPlayerData;
        void* pItemData;
        void* pMonsterData;
        void* pObjectData;
        // TileData *pTileData doesn't appear to exist anymore
    };               // 0x14
    DWORD dwAct;     // 0x18
    DWORD _pad2;
    Act* pAct;       // 0x1C
    DWORD dwSeed[2]; // 0x20
    DWORD _2;        // 0x28
    DWORD _pad3;
    union {
        Path* pPath;
        void* pItemPath;
        void* pObjectPath;
    };                        // 0x2C
    /* DWORD _3[5];              // 0x30 */
    /* DWORD dwGfxFrame;         // 0x44 */
    /* DWORD dwFrameRemain;      // 0x48 */
    /* WORD wFrameRate;          // 0x4C */
    /* WORD _4;                  // 0x4E */
    /* BYTE* pGfxUnk;            // 0x50 */
    /* DWORD* pGfxInfo;          // 0x54 */
    /* DWORD _5;                 // 0x58 */
    /* void* pStats;             // 0x5C */
    /* void* pInventory;         // 0x60 */
    /* void* ptLight;            // 0x64 */
    /* DWORD dwStartLightRadius; // 0x68 */
    /* WORD nPl2ShiftIdx;        // 0x6C */
    /* WORD nUpdateType;         // 0x6E */
    /* UnitAny* pUpdateUnit;     // 0x70 - Used when updating unit. */
    /* DWORD* pQuestRecord;      // 0x74 */
    /* DWORD bSparklyChest;      // 0x78 bool */
    /* DWORD* pTimerArgs;        // 0x7C */
    /* DWORD dwSoundSync;        // 0x80 */
    /* DWORD _6[2];              // 0x84 */
    /* WORD wX;                  // 0x8C */
    /* WORD wY;                  // 0x8E */
    /* DWORD _7;                 // 0x90 */
    /* DWORD dwOwnerType;        // 0x94 */
    /* DWORD dwOwnerId;          // 0x98 */
    /* DWORD _8[2];              // 0x9C */
    /* void* pOMsg;              // 0xA4 */
    /* void* pInfo;              // 0xA8 */
    /* DWORD _9[6];              // 0xAC */
    /* DWORD dwFlags;            // 0xC4 */
    /* DWORD dwFlags2;           // 0xC8 */
    /* DWORD _10[5];             // 0xCC */
    /* UnitAny* pChangedNext;    // 0xE0 */
    /* UnitAny* pListNext;       // 0xE4 -> 0xD8 */
    /* UnitAny* pRoomNext;       // 0xE8 */
};


void hex_dump(void *ptr, size_t len);

void log_Level(Level *ptr);
void log_Room1(Room1 *ptr);
void log_Room2(Room2 *ptr);
void log_Path(Path *ptr);
void log_Act(Act *ptr);
void log_PlayerData(PlayerData *ptr);
void log_Player(Player *ptr);

BOOL is_valid_Level(Level *ptr);
BOOL is_valid_Room1(Room1 *ptr);
BOOL is_valid_Room2(Room2 *ptr);
BOOL is_valid_Path(Path *ptr);
BOOL is_valid_Act(Act *ptr);
BOOL is_valid_PlayerData(PlayerData *ptr);
BOOL is_valid_Player(Player *ptr);


#endif
