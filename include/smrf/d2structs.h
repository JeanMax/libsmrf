#ifndef _D2STRUCTS_H
#define _D2STRUCTS_H

#include "d2sdk.h"
#include "util/log.h"
#include "util/types.h"

// these Room1 Room2 naming are so confusing ):
typedef  struct Room1  RoomSmall;
typedef  struct Room2  RoomBig;

typedef  struct UnitAny  UnitAny;
typedef  struct Room1  Room1;
typedef  struct Room2  Room2;
typedef  struct ActMisc  ActMisc;


enum GameInfoFlag {
     GAME_FLAG_UNSET = 0,
     GAME_FLAG_OFF,
     GAME_FLAG_ON
};
typedef enum GameInfoFlag  GameInfoFlag;  //TODO: move



#define GAME_INFO_STR_MAX 0x40
typedef  struct GameInfo  GameInfo;
struct GameInfo {
    char gameName[GAME_INFO_STR_MAX];
    qword inGame;
    void *_dunno1;
    qword _dunno2;
    byte _dunno3a;
    byte _zero1[6];
    byte _dunno3b;

    char gamePass[GAME_INFO_STR_MAX];
    qword inGameBis;
    void *_dunno1Bis;
    qword _dunno2Bis;
    byte _dunno3aBis;
    byte _zero2[6];
    byte _dunno3bBis;

    qword _dunno1Ter;
    qword _dunno2Ter;  // NULL
    qword _dunno3Ter;  // NULL
    qword inGameTer;

    dword something1;  // enum GameInfoFlag  for the remaining dwords
    dword expansionInfo;  // 1=classic 2=xpac
    dword hardcoreInfo; // 1=soft 2=hard
    dword ladderInfo;  // 1=noladd 2=ladder
    dword difficultyId;  // enum DifficultyId
    dword something2;
    qword _dunnoLast;  // NULL
};

typedef  struct Inventory  Inventory;
struct Inventory {
    dword dwSignature;
    dword _pad1;
    UnitAny* pOwner;
    UnitAny* pFirstItem;
    UnitAny* pLastItem;
    void* pInventoryGrid;
    byte _1[8];
    word wIsMainClassic;
    byte _2[2];
    dword dwWeaponId;
    void *pIsMain;
    UnitAny* pCursorItem;
    dword dwOwnerId;
    dword dwFilledSockets;
    byte _4[32];
    word wIsMainXpac;
    byte _pad2[6];
};

typedef  struct CollMap  CollMap;
struct CollMap {
    dword dwPosGameX;
    dword dwPosGameY;
    dword dwSizeGameX;
    dword dwSizeGameY;
    dword dwPosRoomX;
    dword dwPosRoomY;
    dword dwSizeRoomX;
    dword dwSizeRoomY;
    word* pMapStart;
    word* coll;
    /* word coll[dwSizeGameY][dwSizeGameX]; //0x22 */
};

typedef  struct PresetUnit  PresetUnit;
struct PresetUnit {
    dword _1;
    dword dwTxtFileNo;
    dword dwPosX;
    dword _2;
    PresetUnit* pNext;
    dword _3;
    dword _4;
    dword dwType;
    dword dwPosY;
};

typedef  struct Level  Level;
struct Level {
    Level *pLevelBis;
    // somewhere close
    // ( 1216 bytes further == 2 * sizeof(Level) )
    // OR: 0000000[123] 00000010
    dword _1[2];
    Room2* pRoom2First;
    void* pDunno;
    dword _2;
    dword _2bis;
    dword dwPosX;
    dword dwPosY;
    dword dwSizeX;
    dword dwSizeY;
    dword _3[16];
    dword _zero[80];
    Level* pNext;
    dword _4[4];
    ActMisc* pMisc;
    dword _5[8];
    dword  dwLevelNo;
    /* byte  unk[6]; */
    dword _6[3];
    union {
        dword RoomCenterX[9];
        dword WarpX[9];
    }; // 0x1E0
    union {
        dword RoomCenterY[9];
        dword WarpY[9];
    };
    dword dwRoomEntries;
    byte _pad1[12];
};

typedef  struct RoomTile  RoomTile;
/* struct RoomTile { */
/*     Room2* pRoom2;   // 0x00 */
/*     RoomTile* pNext; // 0x04 */
/*     dword _2[2];     // 0x08 */
/*     dword* nNum;     // 0x10 */
/* }; */

struct Room2 { //big
    dword _1[2];
    Room2** pRoom2Near;
    dword _2[6];
    void* pDunno1;
    dword dwRoomFlags;
    dword dwRoomsNear;
    void* pDunno2;
    RoomTile* pRoomTiles;
    Room2* pNext;
    void* pDunno3;
    Room1* pRoom1;
    dword dwPosX;
    dword dwPosY;
    dword dwSizeX;
    dword dwSizeY;
    dword _3;
    dword dwPresetType;
    dword _4[6];
    Level *pLevel;
    PresetUnit* pPreset;
};

struct Room1 { //small
    Room1** pRoomsNear;
    void *pDunno1;
    dword _1[2];
    Room2* pRoom2;
    dword _2[2];
    void *pDunno2;
    dword dwRoomsNear;
    dword _2bis;
    CollMap* Coll;
    dword *pDunno2bis;
    void *pDunno3;
    dword _3[12];
    dword dwPosXBig;  // in big coord
    dword dwPosYBig;
    dword dwSizeXBig;
    dword dwSizeYBig;
    dword dwPosX;  // in small coord
    dword dwPosY;
    dword dwSizeX;
    dword dwSizeY;
    dword _4[2];
    UnitAny* pUnitFirst;
    Room1* pNext;
    dword _5[2];
    dword _6[2];
};

typedef  struct Path  Path;
struct Path {
    word xOffset;
    word xPos;
    word yOffset;
    word yPos;
    dword _1[2];
    word xTarget;
    word yTarget;
    dword _pad;
    Room1* pRoom1_bis;
    /* dword _2[2]; */
    /* dword _pad1; */
    Room1* pRoom1;
    Room1* pRoomUnk;
    dword _3[4];
    UnitAny* pUnit;
    dword dwFlags;
    dword _4;
    dword dwPathType;
    dword dwPrevPathType;
    dword dwUnitSize;
    dword _5[5];
    UnitAny* pTargetUnit;
    dword dwTargetType;
    dword dwTargetId;
    byte bDirection;
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
    dword _pad2;
    Level* pLevelFirst;
    /* dword _debug[256]; */
};


typedef  struct QuestInfo  QuestInfo;
/* struct QuestInfo { */
/*     void* pBuffer;  */
/*     dword _1;       */
/*     dword _pad;     */
/* }; */

typedef  struct Waypoint  Waypoint;
/* struct Waypoint { */
/*     byte flags;  */
/* }; */

#define UNIT_HASH_TABLE_LEN 128
typedef struct UnitHashTable  UnitHashTable;
struct UnitHashTable {
    UnitAny *table[UNIT_HASH_TABLE_LEN * MAX_UNIT];
};

typedef  struct MissileData  MissileData;
struct MissileData {
    void *_0;
    byte _1[2];
    byte something1;
    byte _2;
    byte skillId;
    byte _3;
    byte something3;
    byte _4;
    byte something4;
    byte _5[3];
    byte _5bis[2];
    byte something4bis;  // frame animation index ?
    byte something4ter;  // animation state
    dword _7; // ff ff ff 7f
    byte _8[4];
    byte something5;
    byte _9[3];
    dword ownerId;  //owner uid / ff ff ff ff?
};

#define PLAYER_DATA_NAME_MAX 0x40
typedef  struct PlayerData  PlayerData;
struct PlayerData {
    char szName[PLAYER_DATA_NAME_MAX];
    QuestInfo* pNormalQuest;
    QuestInfo* pNightmareQuest;
    QuestInfo* pHellQuest;
    Waypoint* pNormalWaypoint;
    Waypoint* pNightmareWaypoint;
    Waypoint* pHellWaypoint;
};


typedef  struct MonsterData  MonsterData;
struct MonsterData {
    void *pDunno;  // same ptr accross different monsters
    byte _1[4];
    dword dwOwnerId;
    byte _2[8];
    word wIsUnique;
    byte fType;
    byte _pad[5];
};

typedef  struct UnitAny  Player;
typedef  struct UnitAny  Missile;
typedef  struct UnitAny  Monster;
struct UnitAny {
    dword dwType;      // enum UnitType
    dword dwTxtFileNo;
    dword dwUnitId;
    dword dwMode;
    union {
        PlayerData* pPlayerData;
        MonsterData *pMonsterData;
        MissileData *pMissileData;
        void* pItemData;
        void* pObjectData;
        // TileData *pTileData doesn't appear to exist anymore
    };
    dword dwAct;
    dword _pad1;
    Act* pAct;
    dword _dunno1[2];
    dword dwSeed[2];
    union {
        Path* pPath;  // for monster / player / npc
        void* pItemPath;
        void* pObjectPath;
    };
    dword _dunno2[18];
    void* pStats;
    Inventory* pInventory;
    dword _dunno3[11];
    word xPos;  // not updated with coords (seen non-0 in classic)
    word yPos;  // not updated with coords (always 0?)
    dword _dunno4[14];
    void* pSkills;
    dword _dunno5[20];
    UnitAny* pNext;
    void*    pRoomNext;
    dword _dunno6[5];
    dword dwPlayerClass;
    dword _dunno7[11];
    word _pad2;
    byte wIsCorpse;
    byte _pad3;
};


void log_GameInfo(GameInfo *ptr);
void log_Inventory(Inventory *ptr);
void log_Level(Level *ptr);
void log_Room2(Room2 *ptr);
void log_Room1(Room1 *ptr);
void log_CollMap(CollMap *ptr);
void log_PresetUnit(PresetUnit *ptr);
void log_Path(Path *ptr);
void log_Act(Act *ptr);
void log_ActMisc(ActMisc *ptr);
void log_MissileData(MissileData *ptr);
void log_MonsterData(MonsterData *ptr);
void log_PlayerData(PlayerData *ptr);
void log_Player(Player *ptr);
void log_UnitAny(UnitAny *ptr);
void log_UnitHashTable(UnitHashTable *ptr);

bool is_valid_GameInfo(GameInfo *ptr);
bool is_valid_Inventory(Inventory *ptr);
bool is_valid_Level(Level *ptr);
bool is_valid_Room2(Room2 *ptr);
bool is_valid_Room1(Room1 *ptr);
bool is_valid_CollMap(CollMap *ptr);
bool is_valid_PresetUnit(PresetUnit *ptr);
bool is_valid_Path(Path *ptr);
bool is_valid_Act(Act *ptr);
bool is_valid_ActMisc(ActMisc *ptr);
bool is_valid_MissileData(MissileData *ptr);
bool is_valid_MonsterData(MonsterData *ptr);
bool is_valid_PlayerData(PlayerData *ptr);
bool is_valid_Monster(Monster *ptr);
bool is_valid_Missile(Missile *ptr);
bool is_valid_Player(Player *ptr);
bool is_valid_UnitAny(UnitAny *ptr);
bool is_valid_UnitHashTable(UnitHashTable *ptr);

#define DEF_STRUCT_CPY_CALLBACK(STRUCT) \
    inline static void *find_##STRUCT##_callback(byte *buf, size_t buf_len, ptr_t address, void *data) \
    {                                                                       \
        (void)buf_len, (void)address;                                       \
        if (!is_valid_##STRUCT((STRUCT *)buf)) {                            \
            /* LOG_WARNING("Invalid "#STRUCT" %16jx", address); */ \
            /* log_##STRUCT((STRUCT *)buf); */                          \
            return NULL;                                                    \
        }                                                                   \
        memcpy(data, buf, sizeof(STRUCT));                                  \
        return data;                                                        \
    }

DEF_STRUCT_CPY_CALLBACK(GameInfo)
DEF_STRUCT_CPY_CALLBACK(Inventory)
DEF_STRUCT_CPY_CALLBACK(Level)
DEF_STRUCT_CPY_CALLBACK(Room2)
DEF_STRUCT_CPY_CALLBACK(Room1)
DEF_STRUCT_CPY_CALLBACK(CollMap)
DEF_STRUCT_CPY_CALLBACK(PresetUnit)
DEF_STRUCT_CPY_CALLBACK(Path)
DEF_STRUCT_CPY_CALLBACK(Act)
DEF_STRUCT_CPY_CALLBACK(ActMisc)
DEF_STRUCT_CPY_CALLBACK(PlayerData)
DEF_STRUCT_CPY_CALLBACK(MonsterData)
DEF_STRUCT_CPY_CALLBACK(MissileData)
DEF_STRUCT_CPY_CALLBACK(Player)
DEF_STRUCT_CPY_CALLBACK(UnitAny)
DEF_STRUCT_CPY_CALLBACK(UnitHashTable)


#define DEF_TYPE_CPY_CALLBACK(TYPE) \
    inline static void *find_##TYPE##_callback(byte *buf, size_t buf_len, ptr_t address, void *data) \
    {                                                                   \
        (void)buf_len, (void)address;                                   \
        memcpy(data, buf, sizeof(TYPE));                                \
        return data;                                                    \
    }

DEF_TYPE_CPY_CALLBACK(byte)
DEF_TYPE_CPY_CALLBACK(word)
DEF_TYPE_CPY_CALLBACK(dword)
DEF_TYPE_CPY_CALLBACK(qword)
DEF_TYPE_CPY_CALLBACK(ptr_t)


inline static void *memcpy_callback(byte *buf, size_t buf_len, ptr_t address, void *data)
{
    (void)address;
    memcpy(data, buf, buf_len);
    return data;
}


#endif
