#ifndef _D2STRUCTS_H
#define _D2STRUCTS_H

#include <ctype.h>

#include "proc.h"  // is_valid_ptr

// these Room1 Room2 naming are so confusing ):
typedef  struct Room1  RoomSmall;
typedef  struct Room2  RoomBig;

typedef  struct UnitAny  UnitAny;
typedef  struct Room1  Room1;
typedef  struct Room2  Room2;
typedef  struct ActMisc  ActMisc;


typedef  struct CollMap  CollMap;
struct CollMap {
    dword dwPosGameX;  // 0x00
    dword dwPosGameY;  // 0x04
    dword dwSizeGameX; // 0x08
    dword dwSizeGameY; // 0x0C
    dword dwPosRoomX;  // 0x10
    dword dwPosRoomY;  // 0x14
    dword dwSizeRoomX; // 0x18
    dword dwSizeRoomY; // 0x1C
    word* pMapStart;   // 0x20
    word* coll;     // 0x22
    /* word coll[dwSizeGameY][dwSizeGameX]; //0x22 */
};

typedef  struct PresetUnit  PresetUnit;
struct PresetUnit {
    dword _1;                // 0x00
    dword dwTxtFileNo;       // 0x04
    dword dwPosX;            // 0x08
    dword _2;
    PresetUnit* pPresetNext; // 0x0C
    dword _3;                // 0x10
    dword _4;
    dword dwType;            // 0x14
    dword dwPosY;            // 0x18
};

typedef  struct Level  Level;
struct Level {
    dword _1[4];        // 0x00
    Room2* pRoom2First; // 0x10
    void* pDunno;
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
/* struct RoomTile { */
/*     Room2* pRoom2;   // 0x00 */
/*     RoomTile* pNext; // 0x04 */
/*     dword _2[2];     // 0x08 */
/*     dword* nNum;     // 0x10 */
/* }; */

struct Room2 { //big
    dword _1[2];        //0
    Room2** pRoom2Near; //8
    dword _2[6];        //16
    void* pDunno1;       //40
    dword dwRoomFlags;  //48
    dword dwRoomsNear;  //52
    void* pDunno2;       //56
    RoomTile* pRoomTiles; //64
    Room2* pRoom2Next;    //72
    void* pDunno3;         //80
    Room1* pRoom1;        //88
    dword dwPosX;         //96
    dword dwPosY;         //100
    dword dwSizeX;        //104
    dword dwSizeY;        //108
    dword _3;             //112
    dword dwPresetType;   //116
    dword _4[6];          //120
    Level *pLevel;        //144
    PresetUnit* pPreset; //152
};

struct Room1 { //small
    Room1** pRoomsNear;  //0
    void *pDunno1; //8
    dword _1[2];    //16
    Room2* pRoom2;      //24
    dword _2[2];         //32
    void *pDunno2;       //40
    dword dwRoomsNear;   //48
    dword _2bis; //52
    CollMap* Coll; //56
    dword *pDunno2bis;         //64
    void *pDunno3; //72
    dword _3[12];         //80
    dword dwPosXBig; //128  in big coord
    dword dwPosYBig; //132
    dword dwSizeXBig; //136
    dword dwSizeYBig; //140
    dword dwPosX; //128  in small coord
    dword dwPosY; //132
    dword dwSizeX; //136
    dword dwSizeY; //140
    dword _4[2];
    UnitAny* pUnitFirst; //168
    Room1* pRoomNext; //176
    dword _5[2]; //184
    dword _6[2]; //192
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


void log_Level(Level *ptr);
void log_Room2(Room2 *ptr);
void log_Room1(Room1 *ptr);
void log_CollMap(CollMap *ptr);
void log_PresetUnit(PresetUnit *ptr);
void log_Path(Path *ptr);
void log_Act(Act *ptr);
void log_PlayerData(PlayerData *ptr);
void log_Player(Player *ptr);

bool is_valid_Level(Level *ptr);
bool is_valid_Room2(Room2 *ptr);
bool is_valid_Room1(Room1 *ptr);
bool is_valid_CollMap(CollMap *ptr);
bool is_valid_PresetUnit(PresetUnit *ptr);
bool is_valid_Path(Path *ptr);
bool is_valid_Act(Act *ptr);
bool is_valid_PlayerData(PlayerData *ptr);
bool is_valid_Player(Player *ptr);

#define DEF_STRUCT_CPY_CALLBACK(STRUCT) \
    inline static bool find_##STRUCT##_callback(byte *buf, size_t buf_len, ptr_t address, void *data) \
    {                                                                       \
        (void)buf_len, (void)address;                                       \
        if (!is_valid_##STRUCT((STRUCT *)buf)) {                            \
            LOG_WARNING("Invalid "#STRUCT" %16jx", address);                \
            log_##STRUCT((STRUCT *)buf);                                    \
            return TRUE;                                                    \
        }                                                                   \
        memcpy(data, buf, sizeof(STRUCT));                                  \
        return FALSE;                                                       \
    }

DEF_STRUCT_CPY_CALLBACK(Level)
DEF_STRUCT_CPY_CALLBACK(Room2)
DEF_STRUCT_CPY_CALLBACK(Room1)
DEF_STRUCT_CPY_CALLBACK(CollMap)
DEF_STRUCT_CPY_CALLBACK(PresetUnit)
DEF_STRUCT_CPY_CALLBACK(Path)
DEF_STRUCT_CPY_CALLBACK(Act)
DEF_STRUCT_CPY_CALLBACK(PlayerData)
DEF_STRUCT_CPY_CALLBACK(Player)

#endif
