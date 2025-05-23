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
    PresetUnit* pNext; // 0x0C
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
    dword _2bis;        // 0x14
    dword dwPosX;       // 0x1C
    dword dwPosY;       // 0x20
    dword dwSizeX;      // 0x24
    dword dwSizeY;      // 0x28
    dword _3[16];       // 0x2C
    dword _zero[80];       // 0x2C
    Level* pNext;  // 0x1AC
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
    Room2* pNext;    //72
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
    Room1* pNext; //176
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
    Room1* pRoom1_bis;        // 0x14
    /* dword _2[2];          // 0x14 */
    /* dword _pad1; */
    Room1* pRoom1;        // 0x1C
    Room1* pRoomUnk;      // 0x20
    dword _3[4];          // 0x24
    UnitAny* pUnit;       // 0x30
    dword dwFlags;        // 0x34
    dword _4;             // 0x38
    dword dwPathType;     // 0x3C
    dword dwPrevPathType; // 0x40
    dword dwUnitSize;     // 0x44
    dword _5[5];          // 0x48
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
    /* dword _debug[32]; */
};

struct ActMisc {
    dword _1[37];
    dword dwStaffTombLevel;
    dword _2[245];
    dword _pad;
    Act* pAct;
    dword _3[3];
    Level* pLevelFirst;
    /* dword _debug[256]; */
};


typedef  struct QuestInfo  QuestInfo;
/* struct QuestInfo { */
/*     void* pBuffer; // 0x00 */
/*     dword _1;      // 0x04 */
/*     dword _pad;            */
/* }; */

typedef  struct Waypoint  Waypoint;
/* struct Waypoint { */
/*     byte flags; // 0x00 */
/* }; */


#define PLAYER_DATA_NAME_MAX 0x40
typedef  struct PlayerData  PlayerData;
struct PlayerData {
    char szName[PLAYER_DATA_NAME_MAX];            // 0x00
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
    dword dwUnitId;          // 0x08
    dword dwMode;    // 0x0C
    union {
        PlayerData* pPlayerData;
        void* pItemData;
        void* pMonsterData;
        void* pObjectData;
        // TileData *pTileData doesn't appear to exist anymore
    };               // 0x10
    dword dwAct;     // 0x18
    dword _pad1;     // 0x1C
    Act* pAct;       // 0x20
    dword _dunno1[2];// 0x30
    dword dwSeed[2]; // 0x28
    union {
        Path* pPath;
        void* pItemPath;
        void* pObjectPath;
    };                        // 0x38
    dword _dunno2[18];        // 0x40
    void* pStats;             // 0x88
    void* pInventory;         // 0x90
    dword _dunno3[11];        // 0x98
    word xPos;                // 0xc4
    word yPos;                // 0xc6
    dword _dunno4[14];        // 0xc8
    void* pSkills;            // 0x100
    dword _dunno5[20];        // 0x108
    UnitAny* pNext;           // 0x150
    void*    pRoomNext;       // 0x158
    /* dword _dunno6[5];         // 0x160 */
    /* dword dwPlayerClass;      // 0x174 */
    /* dword _dunno7[11];        // 0x178 */
    /* word _pad2;               // 0x1a4 */
    /* word wIsCorpse;           // 0x1a6 */
};


void log_Level(Level *ptr);
void log_Room2(Room2 *ptr);
void log_Room1(Room1 *ptr);
void log_CollMap(CollMap *ptr);
void log_PresetUnit(PresetUnit *ptr);
void log_Path(Path *ptr);
void log_Act(Act *ptr);
void log_ActMisc(ActMisc *ptr);
void log_PlayerData(PlayerData *ptr);
void log_Player(Player *ptr);
void log_UnitAny(UnitAny *ptr);

bool is_valid_Level(Level *ptr);
bool is_valid_Room2(Room2 *ptr);
bool is_valid_Room1(Room1 *ptr);
bool is_valid_CollMap(CollMap *ptr);
bool is_valid_PresetUnit(PresetUnit *ptr);
bool is_valid_Path(Path *ptr);
bool is_valid_Act(Act *ptr);
bool is_valid_ActMisc(ActMisc *ptr);
bool is_valid_PlayerData(PlayerData *ptr);
bool is_valid_Player(Player *ptr);
bool is_valid_UnitAny(UnitAny *ptr);

bool is_valid_player_name_str(const char *b, size_t len);

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
DEF_STRUCT_CPY_CALLBACK(ActMisc)
DEF_STRUCT_CPY_CALLBACK(PlayerData)
DEF_STRUCT_CPY_CALLBACK(Player)
DEF_STRUCT_CPY_CALLBACK(UnitAny)

#endif
