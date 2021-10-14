#ifndef _D2STRUCTS_H
#define _D2STRUCTS_H

#include <stdint.h>
typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
typedef uint64_t PTR;

typedef int      BOOL;
#ifndef TRUE
# define TRUE  1
# define FALSE 0
#endif


typedef  struct UnitAny  UnitAny;

typedef  struct BnetData  BnetData;
struct BnetData {
    DWORD dwId;                 // 0x00
    DWORD dwId2;                // 0x04
    BYTE _1[0x10];              // 0x08
    DWORD dwId3;                // 0x18
    WORD Unk3;                  // 0x1C
    BYTE _2;                    // 0x1E
    char szGameName[0x16];      // 0x1F
    WORD _3;                    // 0x35
    char szGameIP[0x10];        // 0x37
    BYTE _5[0x42];              // 0x47
    DWORD dwId4;                // 0x89
    char szAccountName[0x30];   // 0x8D
    char szPlayerName[0x18];    // 0xBD
    char szRealmName[0x08];     // 0xD5
    BYTE _8[0x111];             // 0xDD
    BYTE nCharClass;            // 0x1EE
    BYTE nCharFlags;            // 0x1EF
    BYTE nMaxDiff;              // 0x1F0
    BYTE _9[0x1F];              // 0x1F1
    BYTE CreatedGameDifficulty; // 0x210
    void* _10;                  // 0x211
    BYTE _11[0x15];             // 0x215
    WORD _12;                   // 0x22A
    BYTE _13;                   // 0x22C
    char szRealmName2[0x18];    // 0x22D
    char szGamePass[0x18];      // 0x245
    char szGameDesc[0x104];     // 0x25D
    char channelname[0x20];     //+0x35f
    BYTE _14[0x40];             //+0x37f
    BYTE charlevel;             //+0x3bf
    BYTE ladderflag;            //+0x3c0
    DWORD passhash;             //+0x3c1
    BYTE passlength;            //+0x3c5
};

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
    DWORD _4[4];           // 0x1B0
    ActMisc* pMisc;     // 0x1B4
    DWORD _5[8];        // 0x1BC
    WORD  dwLevelNo;    // 0x1D0
    BYTE  unk[6];
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
    DWORD _2[6];        // 0x0C
    Room2* pRoom2Next;       // 0x24
    DWORD dwRoomFlags;       // 0x28
    DWORD dwRoomsNear;       // 0x2C
    Room1* pRoom1;           // 0x30
    RoomTile* pRoomTiles;    // 0x4C
    Level* pLevel;           // 0x58
    DWORD dwPosX;            // 0x34
    DWORD dwPosY;            // 0x38
    DWORD dwSizeX;           // 0x3C
    DWORD dwSizeY;           // 0x40
    DWORD _3;                // 0x44
    DWORD dwPresetType;      // 0x48
    DWORD _4[2];             // 0x50
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
    DWORD _1[2];
    void  *pDunno;
    DWORD _3;
    DWORD dwMapSeed;
    Room1* pRoom1;
    DWORD dwAct;
    DWORD _2[9];
    ActMisc* pMisc;
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


typedef  struct UnitAny  Player;
struct UnitAny {
    DWORD dwType;      // 0x00
    DWORD dwTxtFileNo; // 0x04
    DWORD _1;          // 0x08
    DWORD dwUnitId;    // 0x0C
    DWORD dwMode;      // 0x10
    union {
        PlayerData* pPlayerData;
        void* pItemData;
        void* pMonsterData;
        void* pObjectData;
        // TileData *pTileData doesn't appear to exist anymore
    };               // 0x14
    DWORD dwAct;     // 0x18
    Act* pAct;       // 0x1C
    DWORD dwSeed[2]; // 0x20
    DWORD _2;        // 0x28
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


#endif
