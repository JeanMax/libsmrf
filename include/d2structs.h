#ifndef _D2STRUCTS_H
#define _D2STRUCTS_H

#include <ctype.h>

#include "proc.h"  // is_valid_ptr

typedef  struct UnitAny  UnitAny;

typedef  struct BnetData  BnetData;
struct BnetData {
    dword dwId;                 // 0x00
    dword dwId2;                // 0x04
    byte _1[0x10];              // 0x08
    dword dwId3;                // 0x18
    word Unk3;                  // 0x1C
    byte _2;                    // 0x1E
    char szGameName[0x16];      // 0x1F
    byte _pad1[7];
    word _3;                    // 0x35
    char szGameIP[0x10];        // 0x37
    byte _5[0x42];              // 0x47
    dword dwId4;                // 0x89
    char szAccountName[0x30];   // 0x8D
    char szPlayerName[0x18];    // 0xBD
    char szRealmName[0x08];     // 0xD5
    byte _8[0x111];             // 0xDD
    byte nCharClass;            // 0x1EE
    byte nCharFlags;            // 0x1EF
    byte nMaxDiff;              // 0x1F0
    byte _9[0x1F];              // 0x1F1
    byte CreatedGameDifficulty; // 0x210
    void* _10;                  // 0x211
    byte _11[0x15];             // 0x215
    byte _pad2[1];
    word _12;                   // 0x22A
    byte _13;                   // 0x22C
    char szRealmName2[0x18];    // 0x22D
    char szGamePass[0x18];      // 0x245
    char szGameDesc[0x104];     // 0x25D
    char channelname[0x20];     //+0x35f
    byte _14[0x40];             //+0x37f
    byte charlevel;             //+0x3bf
    byte ladderflag;            //+0x3c0
    byte _pad3[1];
    dword passhash;             //+0x3c1
    byte passlength;            //+0x3c5
    byte _pad4[3];
};

typedef  struct CollMap  CollMap;
/* struct CollMap { */
/*     dword dwPosGameX;  // 0x00 */
/*     dword dwPosGameY;  // 0x04 */
/*     dword dwSizeGameX; // 0x08 */
/*     dword dwSizeGameY; // 0x0C */
/*     dword dwPosRoomX;  // 0x10 */
/*     dword dwPosRoomY;  // 0x14 */
/*     dword dwSizeRoomX; // 0x18 */
/*     dword dwSizeRoomY; // 0x1C */
/*     word* pMapStart;   // 0x20 */
/*     word* pMapEnd;     // 0x22 */
/* }; */

typedef  struct PresetUnit  PresetUnit;
/* struct PresetUnit { */
/*     dword _1;                // 0x00 */
/*     dword dwTxtFileNo;       // 0x04 */
/*     dword dwPosX;            // 0x08 */
/*     dword _pad; */
/*     PresetUnit* pPresetNext; // 0x0C */
/*     dword _3;                // 0x10 */
/*     dword dwType;            // 0x14 */
/*     dword dwPosY;            // 0x18 */
/* }; */

typedef  struct Room2  Room2;
typedef  struct ActMisc  ActMisc;
typedef  struct Level  Level;
struct Level {
    dword _1[4];        // 0x00
    Room2* pRoom2First; // 0x10
    void* dunno;
    dword _2;        // 0x14
    dword dwPosX;       // 0x1C
    dword dwPosY;       // 0x20
    dword dwSizeX;      // 0x24
    dword dwSizeY;      // 0x28
    dword _3[96];       // 0x2C
    dword _pad;
    Level* pNextLevel;  // 0x1AC
    dword _4[4];           // 0x1B0
    ActMisc* pMisc;     // 0x1B4
    dword _5[8];        // 0x1BC
    dword  dwLevelNo;    // 0x1D0
    /* byte  unk[6]; */
    dword _6[3];        // 0x1D4
    union {
        dword RoomCenterX[9];
        dword WarpX[9];
    }; // 0x1E0
    union {
        dword RoomCenterY[9];
        dword WarpY[9];
    };                   // 0x204
    dword dwRoomEntries; // 0x228
    byte _pad1[4];
};

typedef  struct RoomTile  RoomTile;
struct RoomTile {
    Room2* pRoom2;   // 0x00
    RoomTile* pNext; // 0x04
    dword _2[2];     // 0x08
    dword* nNum;     // 0x10
};

typedef  struct Room1  Room1;
struct Room1 {
    Room1** pRoomsNear;  // 0x00
    void *dunno;
    dword _1[2];         // 0x04
    Room2* pRoom2;       // 0x10
    dword _2[2];         // 0x14
    CollMap* Coll;       // 0x20
    dword dwRoomsNear;   // 0x24
    dword _3[9];         // 0x28
    dword dwXStart;      // 0x4C
    dword dwYStart;      // 0x50
    dword dwXSize;       // 0x54
    dword dwYSize;       // 0x58
    dword _4[6];         // 0x5C
    UnitAny* pUnitFirst; // 0x74
    dword _5;            // 0x78
    dword _6;            // 0x78
    Room1* pRoomNext;    // 0x7C
};

struct Room2 {
    dword _1[2];        // 0x00
    Room2** pRoom2Near; // 0x08
    dword _2[6];        // 0x0C
    Room2* pRoom2Next;       // 0x24
    dword dwRoomFlags;       // 0x28
    dword dwRoomsNear;       // 0x2C
    Room1* pRoom1;           // 0x30
    RoomTile* pRoomTiles;    // 0x4C
    void* pNotLevel;           // 0x58
    void* _2bis;
    void* _2ter;
    dword dwPosX;            // 0x34
    dword dwPosY;            // 0x38
    dword dwSizeX;           // 0x3C
    dword dwSizeY;           // 0x40
    dword _3;                // 0x44
    dword dwPresetType;      // 0x48
    dword _4[2];             // 0x50
    PresetUnit* pPreset;     // 0x5C
    dword _5;
    byte _pad1[4];
    Level *pLevel;
};

typedef  struct Path  Path;
struct Path {
    word xOffset;         // 0x00
    word xPos;            // 0x02
    word yOffset;         // 0x04
    word yPos;            // 0x06
    dword _1[2];          // 0x08
    word xTarget;         // 0x10
    word yTarget;         // 0x12
    dword _2[2];          // 0x14
    dword _pad1;
    Room1* pRoom1;        // 0x1C
    Room1* pRoomUnk;      // 0x20
    dword _3[3];          // 0x24
    dword _pad2;
    UnitAny* pUnit;       // 0x30
    dword dwFlags;        // 0x34
    dword _4;             // 0x38
    dword dwPathType;     // 0x3C
    dword dwPrevPathType; // 0x40
    dword dwUnitSize;     // 0x44
    dword _5[4];          // 0x48
    dword _pad;
    UnitAny* pTargetUnit; // 0x58
    dword dwTargetType;   // 0x5C
    dword dwTargetId;     // 0x60
    byte bDirection;      // 0x64
    byte _pad3[7];
};


typedef  struct Act  Act;
struct Act {
    dword _1[2];
    void  *pDunno;
    dword _3;
    dword dwMapSeed;
    Room1* pRoom1;
    dword dwAct;
    dword _2[9];
    ActMisc* pMisc;
};

/* struct ActMisc { */
/*     dword _1[37];           // 0x00 */
/*     dword dwStaffTombLevel; // 0x94 */
/*     dword _2[245];          // 0x98 */
/*     dword _pad;          // 0x98 */
/*     Act* pAct;              // 0x46C */
/*     dword _3[3];            // 0x470 */
/*     Level* pLevelFirst;     // 0x47C */
/* }; */


typedef  struct QuestInfo  QuestInfo;
/* struct QuestInfo { */
/*     void* pBuffer; // 0x00 */
/*     dword _1;      // 0x04 */
/*     dword _pad;      // 0x04 */
/* }; */

typedef  struct Waypoint  Waypoint;
/* struct Waypoint { */
/*     byte flags; // 0x00 */
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
    dword dwType;      // 0x00
    dword dwTxtFileNo; // 0x04
    dword _1;          // 0x08
    dword dwUnitId;    // 0x0C
    dword dwMode;      // 0x10
    dword _pad1;
    union {
        PlayerData* pPlayerData;
        void* pItemData;
        void* pMonsterData;
        void* pObjectData;
        // TileData *pTileData doesn't appear to exist anymore
    };               // 0x14
    dword dwAct;     // 0x18
    dword _pad2;
    Act* pAct;       // 0x1C
    dword dwSeed[2]; // 0x20
    dword _2;        // 0x28
    dword _pad3;
    union {
        Path* pPath;
        void* pItemPath;
        void* pObjectPath;
    };                        // 0x2C
    /* dword _3[5];              // 0x30 */
    /* dword dwGfxFrame;         // 0x44 */
    /* dword dwFrameRemain;      // 0x48 */
    /* word wFrameRate;          // 0x4C */
    /* word _4;                  // 0x4E */
    /* byte* pGfxUnk;            // 0x50 */
    /* dword* pGfxInfo;          // 0x54 */
    /* dword _5;                 // 0x58 */
    /* void* pStats;             // 0x5C */
    /* void* pInventory;         // 0x60 */
    /* void* ptLight;            // 0x64 */
    /* dword dwStartLightRadius; // 0x68 */
    /* word nPl2ShiftIdx;        // 0x6C */
    /* word nUpdateType;         // 0x6E */
    /* UnitAny* pUpdateUnit;     // 0x70 - Used when updating unit. */
    /* dword* pQuestRecord;      // 0x74 */
    /* dword bSparklyChest;      // 0x78 bool */
    /* dword* pTimerArgs;        // 0x7C */
    /* dword dwSoundSync;        // 0x80 */
    /* dword _6[2];              // 0x84 */
    /* word wX;                  // 0x8C */
    /* word wY;                  // 0x8E */
    /* dword _7;                 // 0x90 */
    /* dword dwOwnerType;        // 0x94 */
    /* dword dwOwnerId;          // 0x98 */
    /* dword _8[2];              // 0x9C */
    /* void* pOMsg;              // 0xA4 */
    /* void* pInfo;              // 0xA8 */
    /* dword _9[6];              // 0xAC */
    /* dword dwFlags;            // 0xC4 */
    /* dword dwFlags2;           // 0xC8 */
    /* dword _10[5];             // 0xCC */
    /* UnitAny* pChangedNext;    // 0xE0 */
    /* UnitAny* pListNext;       // 0xE4 -> 0xD8 */
    /* UnitAny* pRoomNext;       // 0xE8 */
};


void hex_dump(void *ptr, size_t len);

void log_BnetData(BnetData *ptr);
void log_Level(Level *ptr);
void log_Room1(Room1 *ptr);
void log_Room2(Room2 *ptr);
void log_Path(Path *ptr);
void log_Act(Act *ptr);
void log_PlayerData(PlayerData *ptr);
void log_Player(Player *ptr);

bool is_valid_BnetData(BnetData *ptr);
bool is_valid_Level(Level *ptr);
bool is_valid_Room1(Room1 *ptr);
bool is_valid_Room2(Room2 *ptr);
bool is_valid_Path(Path *ptr);
bool is_valid_Act(Act *ptr);
bool is_valid_PlayerData(PlayerData *ptr);
bool is_valid_Player(Player *ptr);


#endif
