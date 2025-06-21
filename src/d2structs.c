#include "d2structs.h"

#include "proc.h"  // is_valid_ptr
#include <ctype.h>  // isupper/islower

void log_GameInfo(GameInfo *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "GameInfo" CLR_RESET " {\n"
              "    char gameName[GAME_INFO_STR_MAX]: %s\n"
              "    qword inGame: %16jx\n"
              "    void *_dunno1: %16jx\n"
              "    qword _dunno2: %16jx\n"
              "    byte _dunno3a: %02x\n"
              "    byte _zero1[6]: %02x %02x %02x %02x %02x %02x\n"
              "    byte _dunno3b: %02x\n"
              "    char gamePass[GAME_INFO_STR_MAX]: %s\n"
              "    qword inGameBis: %16jx\n"
              "    void *_dunno1Bis: %16jx\n"
              "    qword _dunno2Bis: %16jx\n"
              "    byte _dunno3aBis: %02x\n"
              "    byte _zero2[6]: %02x %02x %02x %02x %02x %02x\n"
              "    byte _dunno3bBis: %02x\n"
              "    qword _dunno1Ter: %16jx\n"
              "    qword _dunno2Ter: %16jx\n"
              "    qword _dunno3Ter: %16jx\n"
              "    qword inGameTer: %16jx\n"
              "    dword something1: %08x\n"
              "    dword expansionInfo: %08x\n"
              "    dword hardcoreInfo: %08x\n"
              "    dword ladderInfo: %08x\n"
              "    dword difficultyId: %08x\n"
              "    dword something2: %08x\n"
              "    qword _dunnoLast: %16jx\n"
              "}",
              ptr->gameName,
              ptr->inGame,
              ptr->_dunno1,
              ptr->_dunno2,
              ptr->_dunno3a,
              ptr->_zero1[0], ptr->_zero1[1], ptr->_zero1[2], ptr->_zero1[3], ptr->_zero1[4], ptr->_zero1[5],
              ptr->_dunno3b,
              ptr->gamePass,
              ptr->inGameBis,
              ptr->_dunno1Bis,
              ptr->_dunno2Bis,
              ptr->_dunno3aBis,
              ptr->_zero2[0], ptr->_zero2[1], ptr->_zero2[2], ptr->_zero2[3], ptr->_zero2[4], ptr->_zero2[5],
              ptr->_dunno3bBis,
              ptr->_dunno1Ter,
              ptr->_dunno2Ter,
              ptr->_dunno3Ter,
              ptr->inGameTer,
              ptr->something1,
              ptr->expansionInfo,
              ptr->hardcoreInfo,
              ptr->ladderInfo,
              ptr->difficultyId,
              ptr->something2,
              ptr->_dunnoLast);
    /* hex_dump(ptr, sizeof(GameInfo)); /\* DEBUG *\/ */
}

void log_Inventory(Inventory *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "Inventory" CLR_RESET " {\n"
              "    dword dwSignature: %08x\n"
              "    dword _pad1: %08x\n"
              "    UnitAny* pOwner: %16jx\n"
              "    UnitAny* pFirstItem: %16jx\n"
              "    UnitAny* pLastItem: %16jx\n"
              "    void* pInventoryGrid: %16jx\n"
              "    byte _1[8]: %02x %02x %02x %02x %02x %02x %02x %02x\n"
              "    word wIsMainClassic: %04x\n"
              "    byte _2[2]: %02x %02x\n"
              "    dword dwWeaponId: %08x\n"
              "    void* pIsMain: %16jx\n"
              "    UnitAny* pCursorItem: %16jx\n"
              "    dword dwOwnerId: %08x\n"
              "    dword dwFilledSockets: %08x\n"
              "    byte _4[32]: %02x %02x %02x %02x %02x %02x %02x %02x %02x "
              "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x "
              "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n"
              "    word wIsMainXpac: %04x\n"
              "}",
              ptr->dwSignature,
              ptr->_pad1,
              ptr->pOwner,
              ptr->pFirstItem,
              ptr->pLastItem,
              ptr->pInventoryGrid,
              ptr->_1[0], ptr->_1[1], ptr->_1[2], ptr->_1[3], ptr->_1[4], ptr->_1[5], ptr->_1[6], ptr->_1[7],
              ptr->wIsMainClassic,
              ptr->_2[0], ptr->_2[1],
              ptr->dwWeaponId,
              ptr->pIsMain,
              ptr->pCursorItem,
              ptr->dwOwnerId,
              ptr->dwFilledSockets,
              ptr->_4[0], ptr->_4[1], ptr->_4[2], ptr->_4[3], ptr->_4[4], ptr->_4[5], ptr->_4[6], ptr->_4[7], ptr->_4[8], ptr->_4[9], ptr->_4[10], ptr->_4[11], ptr->_4[12], ptr->_4[13], ptr->_4[14], ptr->_4[15], ptr->_4[16], ptr->_4[17], ptr->_4[18], ptr->_4[19], ptr->_4[20], ptr->_4[21], ptr->_4[22], ptr->_4[23], ptr->_4[24], ptr->_4[25], ptr->_4[26], ptr->_4[27], ptr->_4[28], ptr->_4[29], ptr->_4[30], ptr->_4[31],
              ptr->wIsMainXpac);
}

void log_Level(Level *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "Level" CLR_RESET " {\n"
              "    Level* pLevelBis: %16jx\n"
              "    dword _1[4]: %08x %08x\n"
              "    Room2* pRoom2First: %16jx\n"
              "    void* pDunno: %16jx\n"
              "    dword _2: %08x\n"
              "    dword dwPosX: %08x\n"
              "    dword dwPosY: %08x\n"
              "    dword dwSizeX: %08x\n"
              "    dword dwSizeY: %08x\n"
              "    dword _3[16]: %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n"
              "    Level* pNext: %16jx\n"
              "    dword _4: %08x %08x %08x %08x\n"
              "    ActMisc* pMisc: %16jx\n"
              "    dword _5[6]: %08x %08x %08x %08x %08x %08x\n"
              "    dword dwLevelNo: %08x\n"
              "    dword _6[3]: %08x %08x %08x\n"
              "    dword RoomCenterX[9]: %08x %08x %08x %08x %08x %08x %08x %08x %08x\n"
              "    dword RoomCenterY[9]: %08x %08x %08x %08x %08x %08x %08x %08x %08x\n"
              "    dword dwRoomEntries: %08x\n"
              "}",
              ptr->pLevelBis,
              ptr->_1[0], ptr->_1[1],
              (ptr_t)ptr->pRoom2First,
              (ptr_t)ptr->pDunno,
              ptr->_2,
              ptr->dwPosX,
              ptr->dwPosY,
              ptr->dwSizeX,
              ptr->dwSizeY,
              ptr->_3[0], ptr->_3[1], ptr->_3[2], ptr->_3[3], ptr->_3[4], ptr->_3[5], ptr->_3[6], ptr->_3[7], ptr->_3[8], ptr->_3[9], ptr->_3[10], ptr->_3[11], ptr->_3[12], ptr->_3[13], ptr->_3[14], ptr->_3[15],
              (ptr_t)ptr->pNext,
              ptr->_4[0], ptr->_4[1], ptr->_4[2], ptr->_4[3],
              (ptr_t)ptr->pMisc,
              ptr->_5[0], ptr->_5[1], ptr->_5[2], ptr->_5[3], ptr->_5[4], ptr->_5[5],
              ptr->dwLevelNo,
              ptr->_6[0], ptr->_6[1], ptr->_6[2],
              ptr->RoomCenterX[0], ptr->RoomCenterX[1], ptr->RoomCenterX[2], ptr->RoomCenterX[3], ptr->RoomCenterX[4], ptr->RoomCenterX[5], ptr->RoomCenterX[6], ptr->RoomCenterX[7], ptr->RoomCenterX[8],
              ptr->RoomCenterY[0], ptr->RoomCenterY[1], ptr->RoomCenterY[2], ptr->RoomCenterY[3], ptr->RoomCenterY[4], ptr->RoomCenterY[5], ptr->RoomCenterY[6], ptr->RoomCenterY[7], ptr->RoomCenterY[8],
              ptr->dwRoomEntries);
}

void log_Room2(Room2 *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "Room2" CLR_RESET " {\n"
              "    dword _1[2]: %08x %08x\n"
              "    Room2** pRoom2Near: %16jx\n"
              "    dword _2[6]: %08x %08x %08x %08x %08x %08x\n"
              "    void* pDunno1: %16jx\n"
              "    dword dwRoomFlags: %08x\n"
              "    dword dwRoomsNear: %08x\n"
              "    void* pDunno2: %16jx\n"
              "    RoomTile* pRoomTiles: %16jx\n"
              "    Room2* pNext: %16jx\n"
              "    void* pDunno3: %16jx\n"
              "    Room1* pRoom1: %16jx\n"
              "    dword dwPosX: %08x\n"
              "    dword dwPosY: %08x\n"
              "    dword dwSizeX: %08x\n"
              "    dword dwSizeY: %08x\n"
              "    dword _3: %08x\n"
              "    dword dwPresetType: %08x\n"
              "    dword _4[2]: %08x %08x %08x %08x %08x %08x\n"
              "    Level* pLevel: %16jx\n"
              "    PresetUnit* pPreset: %16jx\n"
              "}",
              ptr->_1[0], ptr->_1[1],
              (ptr_t)ptr->pRoom2Near,
              ptr->_2[0], ptr->_2[1], ptr->_2[2], ptr->_2[3], ptr->_2[4], ptr->_2[5],
              (ptr_t)ptr->pDunno1,
              ptr->dwRoomFlags,
              ptr->dwRoomsNear,
              (ptr_t)ptr->pDunno2,
              (ptr_t)ptr->pRoomTiles,
              (ptr_t)ptr->pNext,
              (ptr_t)ptr->pDunno3,
              (ptr_t)ptr->pRoom1,
              ptr->dwPosX,
              ptr->dwPosY,
              ptr->dwSizeX,
              ptr->dwSizeY,
              ptr->_3,
              ptr->dwPresetType,
              ptr->_4[0], ptr->_4[1], ptr->_4[2], ptr->_4[3], ptr->_4[4], ptr->_4[5],
              (ptr_t)ptr->pLevel,
              (ptr_t)ptr->pPreset);
}

void log_Room1(Room1 *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "Room1" CLR_RESET " {\n"
              "    Room1** pRoomsNear: %16jx\n"
              "    void* pDunno1: %16jx\n"
              "    dword _1[2]: %08x %08x\n"
              "    Room2* pRoom2: %16jx\n"
              "    dword _2[2]: %08x %08x\n"
              "    void* pDunno2: %16jx\n"
              "    dword dwRoomsNear: %08x\n"
              "    dword _2bis: %08x\n"
              "    CollMap* Coll: %16jx\n"
              "    void* pDunno2bis: %16jx\n"
              "    void* pDunno3: %16jx\n"
              "    dword _3[12]: %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n"
              "    dword dwPosXBig: %08x\n"
              "    dword dwPosYBig: %08x\n"
              "    dword dwSizeXBig: %08x\n"
              "    dword dwSizeYBig: %08x\n"
              "    dword dwPosX: %08x\n"
              "    dword dwPosY: %08x\n"
              "    dword dwSizeX: %08x\n"
              "    dword dwSizeY: %08x\n"
              "    dword _4[2]: %08x %08x\n"
              "    UnitAny* pUnitFirst: %16jx\n"
              "    Room1* pNext: %16jx\n"
              "    dword _5[2]: %08x %08x\n"
              "    dword _6[2]: %08x %08x\n"
              "}",
              (ptr_t)ptr->pRoomsNear,
              (ptr_t)ptr->pDunno1,
              ptr->_1[0], ptr->_1[1],
              (ptr_t)ptr->pRoom2,
              ptr->_2[0], ptr->_2[1],
              (ptr_t)ptr->Coll,
              ptr->dwRoomsNear,
              ptr->_2bis,
              (ptr_t)ptr->pDunno2,
              (ptr_t)ptr->pDunno2bis,
              (ptr_t)ptr->pDunno3,
              ptr->_3[0], ptr->_3[1], ptr->_3[2], ptr->_3[3], ptr->_3[4], ptr->_3[5], ptr->_3[6], ptr->_3[7], ptr->_3[8], ptr->_3[9], ptr->_3[10], ptr->_3[11],
              ptr->dwPosXBig,
              ptr->dwPosYBig,
              ptr->dwSizeXBig,
              ptr->dwSizeYBig,
              ptr->dwPosX,
              ptr->dwPosY,
              ptr->dwSizeX,
              ptr->dwSizeY,
              ptr->_4[0], ptr->_4[1],
              (ptr_t)ptr->pUnitFirst,
              (ptr_t)ptr->pNext,
              ptr->_5[0], ptr->_5[1],
              ptr->_6[0], ptr->_6[1]);
}

void log_CollMap(CollMap *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "CollMap" CLR_RESET " {\n"
              "    dword dwPosGameX: %08x\n"
              "    dword dwPosGameY: %08x\n"
              "    dword dwSizeGameX: %08x\n"
              "    dword dwSizeGameY: %08x\n"
              "    dword dwPosRoomX: %08x\n"
              "    dword dwPosRoomY: %08x\n"
              "    dword dwSizeRoomX: %08x\n"
              "    dword dwSizeRoomY: %08x\n"
              "    word* pMapStart: %16jx\n"
              "}",
              ptr->dwPosGameX,
              ptr->dwPosGameY,
              ptr->dwSizeGameX,
              ptr->dwSizeGameY,
              ptr->dwPosRoomX,
              ptr->dwPosRoomY,
              ptr->dwSizeRoomX,
              ptr->dwSizeRoomY,
              (ptr_t)ptr->pMapStart);
}

void log_PresetUnit(PresetUnit *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "PresetUnit" CLR_RESET " {\n"
              "    dword _1: %08x\n"
              "    dword dwTxtFileNo: %08x\n"
              "    dword dwPosX: %08x\n"
              "    dword _2: %08x\n"
              "    PresetUnit* pNext: %16jx\n"
              "    dword _3: %08x\n"
              "    dword _4: %08x\n"
              "    dword dwType: %08x\n"
              "    dword dwPosY: %08x\n"
              "}",
              ptr->_1,
              ptr->dwTxtFileNo,
              ptr->dwPosX,
              ptr->_2,
              (ptr_t)ptr->pNext,
              ptr->_3,
              ptr->_4,
              ptr->dwType,
              ptr->dwPosY);
}

void log_Path(Path *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "Path" CLR_RESET " {\n"
              "    word xOffset: %04x\n"
              "    word xPos: %04x\n"
              "    word yOffset: %04x\n"
              "    word yPos: %04x\n"
              "    dword _1[2]: %08x %08x\n"
              "    word xTarget: %04x\n"
              "    word yTarget: %04x\n"
              /* "    dword _2[2]: %08x %08x\n" */
              "    Room1* pRoom1_bis: %16jx\n"
              "    Room1* pRoom1: %16jx\n"
              "    Room1* pRoomUnk: %16jx\n"
              "    dword _3[4]: %08x %08x %08x %08x\n"
              "    UnitAny* pUnit: %16jx\n"
              "    dword dwFlags: %08x\n"
              "    dword _4: %08x\n"
              "    dword dwPathType: %08x\n"
              "    dword dwPrevPathType: %08x\n"
              "    dword dwUnitSize: %08x\n"
              "    dword _5[5]: %08x %08x %08x %08x %08x\n"
              "    UnitAny* pTargetUnit: %16jx\n"
              "    dword dwTargetType: %08x\n"
              "    dword dwTargetId: %08x\n"
              "    byte bDirection: %02x\n"
              "}",
              ptr->xOffset,
              ptr->xPos,
              ptr->yOffset,
              ptr->yPos,
              ptr->_1[0], ptr->_1[1],
              ptr->xTarget,
              ptr->yTarget,
              /* ptr->_2[0], ptr->_2[1], */
              (ptr_t)ptr->pRoom1_bis,
              (ptr_t)ptr->pRoom1,
              (ptr_t)ptr->pRoomUnk,
              ptr->_3[0], ptr->_3[1], ptr->_3[2], ptr->_3[3],
              (ptr_t)ptr->pUnit,
              ptr->dwFlags,
              ptr->_4,
              ptr->dwPathType,
              ptr->dwPrevPathType,
              ptr->dwUnitSize,
              ptr->_5[0], ptr->_5[1], ptr->_5[2], ptr->_5[3], ptr->_5[4],
              (ptr_t)ptr->pTargetUnit,
              ptr->dwTargetType,
              ptr->dwTargetId,
              ptr->bDirection);
}

void log_Act(Act *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "Act" CLR_RESET " {\n"
              "    dword _1[2]: %08x %08x\n"
              "    void  *pDunno: %16jx\n"
              "    dword _3: %08x\n"
              "    dword dwMapSeed: %08x\n"
              "    Room1* pRoom1: %16jx\n"
              "    dword dwAct: %08x\n"
              "    dword _2[9]: %08x %08x %08x %08x %08x %08x %08x %08x %08x\n"
              "    ActMisc* pMisc: %16jx\n"
              "}",
              ptr->_1[0], ptr->_1[1],
              (ptr_t)ptr->pDunno,
              ptr->_3,
              ptr->dwMapSeed,
              (ptr_t)ptr->pRoom1,
              ptr->dwAct,
              ptr->_2[0], ptr->_2[1], ptr->_2[2], ptr->_2[3], ptr->_2[4], ptr->_2[5], ptr->_2[6], ptr->_2[7], ptr->_2[8],
              (ptr_t)ptr->pMisc);
    /* hex_dump(ptr, sizeof(Act)); /\* DEBUG *\/ */
}

void log_ActMisc(ActMisc *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "ActMisc" CLR_RESET " {\n"
              "    dword _1[37]: %08x\n"
              "    dword dwStaffTombLevel: %08x\n"
              "    dword _2[245]: %08x\n"
              "    dword _pad: %08x\n"
              "    Act* pAct: %16jx\n"
              "    dword _3[3]: %08x\n"
              "    Level* pLevelFirst: %16jx\n"
              "}",
              ptr->_1[0],
              ptr->dwStaffTombLevel,
              ptr->_2[0],
              ptr->_pad,
              (ptr_t)ptr->pAct,
              ptr->_3[0],
              (ptr_t)ptr->pLevelFirst);
    /* hex_dump(ptr, sizeof(ActMisc)); */
}

void log_PlayerData(PlayerData *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "PlayerData" CLR_RESET " {\n"
              "    char szName[0x10]: %.16s\n"
              "    QuestInfo* pNormalQuest: %16jx\n"
              "    QuestInfo* pNightmareQuest: %16jx\n"
              "    QuestInfo* pHellQuest: %16jx\n"
              "    Waypoint* pNormalWaypoint: %16jx\n"
              "    Waypoint* pNightmareWaypoint: %16jx\n"
              "    Waypoint* pHellWaypoint: %16jx\n"
              "}",
              ptr->szName,
              (ptr_t)ptr->pNormalQuest,
              (ptr_t)ptr->pNightmareQuest,
              (ptr_t)ptr->pHellQuest,
              (ptr_t)ptr->pNormalWaypoint,
              (ptr_t)ptr->pNightmareWaypoint,
              (ptr_t)ptr->pHellWaypoint);
}

void log_MissileData(MissileData *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "MissileData" CLR_RESET " {\n"
              /* "    byte _1[64]: %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n" */
              "}"
              /* ptr->_1[0], ptr->_1[1], ptr->_1[2], ptr->_1[3], ptr->_1[4], ptr->_1[5], ptr->_1[6], ptr->_1[7], ptr->_1[8], ptr->_1[9], ptr->_1[10], ptr->_1[11], ptr->_1[12], ptr->_1[13], ptr->_1[14], ptr->_1[15], ptr->_1[16], ptr->_1[17], ptr->_1[18], ptr->_1[19], ptr->_1[20], ptr->_1[21], ptr->_1[22], ptr->_1[23], ptr->_1[24], ptr->_1[25], ptr->_1[26], ptr->_1[27], ptr->_1[28], ptr->_1[29], ptr->_1[30], ptr->_1[31], ptr->_1[32], ptr->_1[33], ptr->_1[34], ptr->_1[35], ptr->_1[36], ptr->_1[37], ptr->_1[38], ptr->_1[39], ptr->_1[40], ptr->_1[41], ptr->_1[42], ptr->_1[43], ptr->_1[44], ptr->_1[45], ptr->_1[46], ptr->_1[47], ptr->_1[48], ptr->_1[49], ptr->_1[50], ptr->_1[51], ptr->_1[52], ptr->_1[53], ptr->_1[54], ptr->_1[55], ptr->_1[56], ptr->_1[57], ptr->_1[58], ptr->_1[59], ptr->_1[60], ptr->_1[61], ptr->_1[62], ptr->_1[63] */
              );
    hex_dump(ptr, sizeof(MissileData));
}

void log_MonsterData(MonsterData *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "MonsterData" CLR_RESET " {\n"
              "    void* pDunno: %16jx\n"
              "    dword _1[4]: %08x %08x %08x %08x\n"
              "    dword dwOwnerId: %08x\n"
              "    dword _2[8]: %08x %08x %08x %08x %08x %08x %08x %08x\n"
              "    word wIsUnique: %04x\n"
              "    byte fType: %02x\n"
              "}",
              ptr->pDunno,
              ptr->_1[0], ptr->_1[1], ptr->_1[2], ptr->_1[3],
              ptr->dwOwnerId,
              ptr->_2[0], ptr->_2[1], ptr->_2[2], ptr->_2[3], ptr->_2[4], ptr->_2[5], ptr->_2[6], ptr->_2[7],
              ptr->wIsUnique,
              ptr->fType);
    /* hex_dump(ptr, sizeof(MonsterData)); */
}

void log_Player(Player *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "Player" CLR_RESET " {\n"
              "    dword dwType: %08x\n"
              "    dword dwTxtFileNo: %08x\n"
              "    dword dwUnitId: %08x\n"
              "    dword dwMode: %08x\n"
              "    PlayerData* pPlayerData: %16jx\n"
              "    dword dwAct: %08x\n"
              "    Act* pAct: %16jx\n"
              "    dword dwSeed[2]: %08x, %08x\n"
              "    Path* pPath: %16jx\n"
              "}",
              ptr->dwType,
              ptr->dwTxtFileNo,
              ptr->dwUnitId,
              ptr->dwMode,
              (ptr_t)ptr->pPlayerData,
              /* ptr->_1, */
              ptr->dwAct,
              (ptr_t)ptr->pAct,
              ptr->dwSeed[0], ptr->dwSeed[1],
              (ptr_t)ptr->pPath);
}

void log_UnitAny(UnitAny *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "UnitAny" CLR_RESET " {\n"
              "    dword dwType: %08x\n"
              "    dword dwTxtFileNo: %08x\n"
              "    dword dwUnitId: %08x\n"
              "    dword dwMode: %08x\n"
              "    void* pUnitData: %16jx\n"
              "    dword dwAct: %08x\n"
              "    dword _pad1: %08x\n"
              "    Act* pAct: %16jx\n"
              "    dword dwSeed[2]: %08x, %08x\n"
              "    dword _dunno1[2]: %08x, %08x\n"
              "    Path* pPath: %16jx\n"
              "    dword _dunno2[18]: %08x %08x %08x %08x %08x %08x %08x %08x  "
                                     "%08x %08x %08x %08x %08x %08x %08x %08x  "
                                     "%08x %08x\n"
              "    void* pStats: %16jx\n"
              "    void* pInventory: %16jx\n"
              "    dword _dunno3[11]: %08x %08x %08x %08x %08x %08x %08x %08x  "
                                     "%08x %08x %08x\n"
              "    word xPos: %08x\n"
              "    word yPos: %08x\n"
              "    dword _dunno4[14]: %08x %08x %08x %08x %08x %08x %08x %08x  "
                                     "%08x %08x %08x %08x %08x %08x\n"
              "    void* pSkills: %16jx\n"
              "    dword _dunno5[20]: %08x %08x %08x %08x %08x %08x %08x %08x  "
                                     "%08x %08x %08x %08x %08x %08x %08x %08x  "
                                     "%08x %08x %08x %08x\n"
              "    UnitAny* pNext: %16jx\n"
              "    void*    pRoomNext: %16jx\n"
              "    dword _dunno6[5]: %08x %08x %08x %08x %08x\n"
              "    dword dwPlayerClass: %08x\n"
              "    dword _dunno7[11]: %08x %08x %08x %08x %08x %08x %08x %08x  "
                                     "%08x %08x %08x\n"
              "    word _pad2: %04x\n"
              "    byte wIsCorpse: %02x\n"
              "    byte _pad3: %02x\n"
              "}",
              ptr->dwType,
              ptr->dwTxtFileNo,
              ptr->dwUnitId,
              ptr->dwMode,
              (ptr_t)ptr->pPlayerData,
              ptr->dwAct,
              ptr->_pad1,
              (ptr_t)ptr->pAct,
              ptr->dwSeed[0], ptr->dwSeed[1],
              ptr->_dunno1[0], ptr->_dunno1[1],
              (ptr_t)ptr->pPath,
              ptr->_dunno2[0], ptr->_dunno2[1], ptr->_dunno2[2], ptr->_dunno2[3], ptr->_dunno2[4], ptr->_dunno2[5], ptr->_dunno2[6], ptr->_dunno2[7], ptr->_dunno2[8], ptr->_dunno2[9], ptr->_dunno2[10], ptr->_dunno2[11], ptr->_dunno2[12], ptr->_dunno2[13], ptr->_dunno2[14], ptr->_dunno2[15], ptr->_dunno2[16], ptr->_dunno2[17],
              (ptr_t)ptr->pStats,
              (ptr_t)ptr->pInventory,
              ptr->_dunno3[0], ptr->_dunno3[1], ptr->_dunno3[2], ptr->_dunno3[3], ptr->_dunno3[4], ptr->_dunno3[5], ptr->_dunno3[6], ptr->_dunno3[7], ptr->_dunno3[8], ptr->_dunno3[9], ptr->_dunno3[10],
              ptr->xPos,
              ptr->yPos,
              ptr->_dunno4[0], ptr->_dunno4[1], ptr->_dunno4[2], ptr->_dunno4[3], ptr->_dunno4[4], ptr->_dunno4[5], ptr->_dunno4[6], ptr->_dunno4[7], ptr->_dunno4[8], ptr->_dunno4[9], ptr->_dunno4[10], ptr->_dunno4[11], ptr->_dunno4[12], ptr->_dunno4[13],
              (ptr_t)ptr->pSkills,
              ptr->_dunno5[0], ptr->_dunno5[1], ptr->_dunno5[2], ptr->_dunno5[3], ptr->_dunno5[4], ptr->_dunno5[5], ptr->_dunno5[6], ptr->_dunno5[7], ptr->_dunno5[8], ptr->_dunno5[9], ptr->_dunno5[10], ptr->_dunno5[11], ptr->_dunno5[12], ptr->_dunno5[13], ptr->_dunno5[14], ptr->_dunno5[15], ptr->_dunno5[16], ptr->_dunno5[17], ptr->_dunno5[18], ptr->_dunno5[19],
              (ptr_t)ptr->pNext,
              (ptr_t)ptr->pRoomNext,
              ptr->_dunno6[0], ptr->_dunno6[1], ptr->_dunno6[2], ptr->_dunno6[3], ptr->_dunno6[4],
              ptr->dwPlayerClass,
              ptr->_dunno7[0], ptr->_dunno7[1], ptr->_dunno7[2], ptr->_dunno7[3], ptr->_dunno7[4], ptr->_dunno7[5], ptr->_dunno7[6], ptr->_dunno7[7], ptr->_dunno7[8], ptr->_dunno7[9], ptr->_dunno7[10],
              ptr->_pad2,
              ptr->wIsCorpse,
              ptr->_pad2);
}

void log_UnitHashTable(UnitHashTable *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "UnitHashTable" CLR_RESET " {\n"
              "    UnitAny *table[%d]: %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx %16jx\n"
              "}",
              UNIT_HASH_TABLE_LEN,
              ptr->table[0], ptr->table[1], ptr->table[2], ptr->table[3], ptr->table[4], ptr->table[5], ptr->table[6], ptr->table[7], ptr->table[8], ptr->table[9], ptr->table[10], ptr->table[11], ptr->table[12], ptr->table[13], ptr->table[14], ptr->table[15], ptr->table[16], ptr->table[17], ptr->table[18], ptr->table[19], ptr->table[20], ptr->table[21], ptr->table[22], ptr->table[23], ptr->table[24], ptr->table[25], ptr->table[26], ptr->table[27], ptr->table[28], ptr->table[29], ptr->table[30], ptr->table[31], ptr->table[32], ptr->table[33], ptr->table[34], ptr->table[35], ptr->table[36], ptr->table[37], ptr->table[38], ptr->table[39], ptr->table[40], ptr->table[41], ptr->table[42], ptr->table[43], ptr->table[44], ptr->table[45], ptr->table[46], ptr->table[47], ptr->table[48], ptr->table[49], ptr->table[50], ptr->table[51], ptr->table[52], ptr->table[53], ptr->table[54], ptr->table[55], ptr->table[56], ptr->table[57], ptr->table[58], ptr->table[59], ptr->table[60], ptr->table[61], ptr->table[62], ptr->table[63], ptr->table[64], ptr->table[65], ptr->table[66], ptr->table[67], ptr->table[68], ptr->table[69], ptr->table[70], ptr->table[71], ptr->table[72], ptr->table[73], ptr->table[74], ptr->table[75], ptr->table[76], ptr->table[77], ptr->table[78], ptr->table[79], ptr->table[80], ptr->table[81], ptr->table[82], ptr->table[83], ptr->table[84], ptr->table[85], ptr->table[86], ptr->table[87], ptr->table[88], ptr->table[89], ptr->table[90], ptr->table[91], ptr->table[92], ptr->table[93], ptr->table[94], ptr->table[95], ptr->table[96], ptr->table[97], ptr->table[98], ptr->table[99], ptr->table[100], ptr->table[101], ptr->table[102], ptr->table[103], ptr->table[104], ptr->table[105], ptr->table[106], ptr->table[107], ptr->table[108], ptr->table[109], ptr->table[110], ptr->table[111], ptr->table[112], ptr->table[113], ptr->table[114], ptr->table[115], ptr->table[116], ptr->table[117], ptr->table[118], ptr->table[119], ptr->table[120], ptr->table[121], ptr->table[122], ptr->table[123], ptr->table[124], ptr->table[125], ptr->table[126], ptr->table[127]);
}

////////////////////////////////////////////////////////////////////////////////

static inline bool is_valid_string(const char *b, size_t len, bool check_right)
{
    const char *start = b;
    while ((size_t)(b - start) < len) {
        if (*b && !(isprint((int)*b) || *b == ' ')) {
            return FALSE;  //TODO: it actually accepts wide char :|
        }
        // check if right side is 0 padded
        if (!*b) {
            if (check_right) {
                while ((size_t)(b - start) < len) {
                    if (*b) {
                        return FALSE;
                    }
                    b++;
                }
            }
            return *start;
        }
        b++;
    }
    return *start; // forbid empty string
}

inline bool is_valid_GameInfo(GameInfo *ptr)
{
    return IS_ALIGNED(ptr)
        && (ptr->inGame == 0 || ptr->inGame == 1)
        && ptr->inGame == ptr->inGameBis
        && ptr->inGame == ptr->inGameTer
        && ptr->_dunnoLast == 0
        && is_valid_ptr__quick((ptr_t)ptr->_dunno1)
        && is_valid_ptr__quick((ptr_t)ptr->_dunno1Bis)
        && ptr->_dunno1 + 0x60 == ptr->_dunno1Bis
        && ptr->_dunno1Ter
        && ptr->_dunno3a
        && ptr->_dunno3b
        && ptr->_dunno3aBis
        && ptr->_dunno3bBis
        && (ptr->something1 == GAME_FLAG_UNSET || ptr->something1 == GAME_FLAG_OFF || ptr->something1 == GAME_FLAG_ON)
        && (ptr->something2 == GAME_FLAG_UNSET || ptr->something2 == GAME_FLAG_OFF || ptr->something2 == GAME_FLAG_ON)
        && (ptr->expansionInfo == GAME_FLAG_UNSET || ptr->expansionInfo == GAME_FLAG_OFF || ptr->expansionInfo == GAME_FLAG_ON)
        && (ptr->hardcoreInfo == GAME_FLAG_UNSET || ptr->hardcoreInfo == GAME_FLAG_OFF || ptr->hardcoreInfo == GAME_FLAG_ON)
        && (ptr->ladderInfo == GAME_FLAG_UNSET || ptr->ladderInfo == GAME_FLAG_OFF || ptr->ladderInfo == GAME_FLAG_ON)
        && (ptr->difficultyId == DIFF_NORMAL || ptr->difficultyId == DIFF_NIGHTMARE || ptr->difficultyId == DIFF_HELL)
        && (!*ptr->gameName || is_valid_string(ptr->gameName, GAME_INFO_STR_MAX, FALSE))
        && (!*ptr->gamePass || is_valid_string(ptr->gamePass, GAME_INFO_STR_MAX, FALSE))
        ;
}

inline bool is_valid_Inventory(Inventory *ptr)  //TODO
{
    return IS_ALIGNED(ptr);
}

inline bool is_valid_Level(Level *ptr)
{
    static dword zero[86] = {0};
    /* log_Level(ptr);                          /\* DEBUG *\/ */
#ifdef NDEBUG
    /* if (memcmp(ptr->_zero, zero, sizeof(dword) * 80)) { */
        /* LOG_WARNING("not zero'ed"); */
        /* LOG_WARNING("dword _zero[80]: %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n", */
        /*             ptr->_zero[0], ptr->_zero[1], ptr->_zero[2], ptr->_zero[3], ptr->_zero[4], ptr->_zero[5], ptr->_zero[6], ptr->_zero[7], ptr->_zero[8], ptr->_zero[9], ptr->_zero[10], ptr->_zero[11], ptr->_zero[12], ptr->_zero[13], ptr->_zero[14], ptr->_zero[15], ptr->_zero[16], ptr->_zero[17], ptr->_zero[18], ptr->_zero[19], ptr->_zero[20], ptr->_zero[21], ptr->_zero[22], ptr->_zero[23], ptr->_zero[24], ptr->_zero[25], ptr->_zero[26], ptr->_zero[27], ptr->_zero[28], ptr->_zero[29], ptr->_zero[30], ptr->_zero[31], ptr->_zero[32], ptr->_zero[33], ptr->_zero[34], ptr->_zero[35], ptr->_zero[36], ptr->_zero[37], ptr->_zero[38], ptr->_zero[39], ptr->_zero[40], ptr->_zero[41], ptr->_zero[42], ptr->_zero[43], ptr->_zero[44], ptr->_zero[45], ptr->_zero[46], ptr->_zero[47], ptr->_zero[48], ptr->_zero[49], ptr->_zero[50], ptr->_zero[51], ptr->_zero[52], ptr->_zero[53], ptr->_zero[54], ptr->_zero[55], ptr->_zero[56], ptr->_zero[57], ptr->_zero[58], ptr->_zero[59], ptr->_zero[60], ptr->_zero[61], ptr->_zero[62], ptr->_zero[63], ptr->_zero[64], ptr->_zero[65], ptr->_zero[66], ptr->_zero[67], ptr->_zero[68], ptr->_zero[69], ptr->_zero[70], ptr->_zero[71], ptr->_zero[72], ptr->_zero[73], ptr->_zero[74], ptr->_zero[75], ptr->_zero[76], ptr->_zero[77], ptr->_zero[78], ptr->_zero[79]); */
    /* } */
#endif
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pRoom2First): %d", */
    /*             !ptr->pRoom2First || is_valid_ptr__quick((ptr_t)ptr->pRoom2First) ); */
    /* /\* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pDunno): %d", *\/ */
    /* /\*             is_valid_ptr__quick((ptr_t)ptr->pDunno) ); *\/ */
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pNext): %d", */
    /*             !ptr->pNext || is_valid_ptr__quick((ptr_t)ptr->pNext) ); */
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pMisc): %d", */
    /*             !ptr->pMisc || is_valid_ptr__quick((ptr_t)ptr->pMisc) ); */
    return IS_ALIGNED(ptr)
        && ptr->dwPosX <= 0xffff
        && ptr->dwPosY <= 0xffff
        && ptr->dwSizeX <= 0xff
        && ptr->dwSizeY <= 0xff
        && ptr->dwLevelNo < MAX_LEVEL  // TODO: it can get weird
        && ptr->dwRoomEntries <= 0xf
        && (!ptr->pRoom2First || is_valid_ptr__quick((ptr_t)ptr->pRoom2First))
        /* && is_valid_ptr__quick((ptr_t)ptr->pDunno) */
        && (!ptr->pNext || is_valid_ptr__quick((ptr_t)ptr->pNext))
        /* && (!ptr->pMisc || is_valid_ptr__quick((ptr_t)ptr->pMisc)) */
        && !memcmp(ptr->_zero, zero, sizeof(dword) * 80);
}

inline bool is_valid_Room2(Room2 *ptr)
{
    return IS_ALIGNED(ptr)
        /* && is_valid_ptr((ptr_t)ptr->pRoom2Near) */
        /* && (!ptr->pRoomTiles || is_valid_ptr((ptr_t)ptr->pRoomTiles)) */
        && is_valid_ptr((ptr_t)ptr->pLevel)
        && (!ptr->pPreset || is_valid_ptr((ptr_t)ptr->pPreset))
        && (!ptr->pNext || is_valid_ptr((ptr_t)ptr->pNext))
        ;
}

inline bool is_valid_Room1(Room1 *ptr)
{
    /* log_Room1(ptr);            /\* DEBUG *\/ */
    return IS_ALIGNED(ptr)
        /* && (!ptr->pRoomsNear || is_valid_ptr((ptr_t)ptr->pRoomsNear)) */
        && (!ptr->pRoom2 || is_valid_ptr((ptr_t)ptr->pRoom2))
        /* && (!ptr->pDunno2 || is_valid_ptr((ptr_t)ptr->pDunno2)) */
        /* && is_valid_ptr((ptr_t)ptr->pDunno3) */
        /* && (!ptr->Coll || is_valid_ptr((ptr_t)ptr->Coll)) */
        && (!ptr->pUnitFirst || is_valid_ptr((ptr_t)ptr->pUnitFirst))
        && (!ptr->pNext || is_valid_ptr((ptr_t)ptr->pNext))
        ;
}

inline bool is_valid_CollMap(CollMap *ptr)
{
    if (!is_valid_ptr((ptr_t)ptr->pMapStart)) {
        LOG_WARNING("invalid pMapStart");
    }

    return IS_ALIGNED(ptr) && is_valid_ptr((ptr_t)ptr->pMapStart);
}

inline bool is_valid_PresetUnit(PresetUnit *ptr)
{
    if (ptr->pNext && !is_valid_ptr((ptr_t)ptr->pNext)) {
        LOG_WARNING("invalid pNext");
    }

    //TODO: validate dwType / dwTxtFileNo

    return IS_ALIGNED(ptr)
        && (!ptr->pNext || is_valid_ptr((ptr_t)ptr->pNext));
}

inline bool is_valid_Path(Path *ptr)
{
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pRoom1): %d", */
    /*             is_valid_ptr__quick((ptr_t)ptr->pRoom1)); */
    /* log_Path(ptr); /\* DEBUG *\/ */
    return IS_ALIGNED(ptr)
        && is_valid_ptr__quick((ptr_t)ptr->pRoom1)
        /* && (!ptr->pRoomUnk || is_valid_ptr((ptr_t)ptr->pRoomUnk)) */
        /* && (!ptr->pUnit || is_valid_ptr((ptr_t)ptr->pUnit)) */
        /* && (!ptr->pTargetUnit || is_valid_ptr((ptr_t)ptr->pTargetUnit)); */
        ;
}

inline bool is_valid_Act(Act *ptr)
{
    log_Act(ptr); /* DEBUG */
    return IS_ALIGNED(ptr);
        /* && is_valid_ptr((ptr_t)ptr->pDunno); */
        /* && is_valid_ptr((ptr_t)ptr->pRoom1) */
        /* && (!ptr->pMisc || is_valid_ptr((ptr_t)ptr->pMisc)) */
        /* && ptr->dwAct < 5; */
}

inline bool is_valid_ActMisc(ActMisc *ptr)
{
    return IS_ALIGNED(ptr)
        && is_valid_ptr((ptr_t)ptr->pAct)
        && is_valid_ptr((ptr_t)ptr->pLevelFirst)
        && ptr->dwStaffTombLevel < 12; //TODO
}

inline bool is_valid_MissileData(MissileData *ptr)
{
    return IS_ALIGNED(ptr);
    /* && ptr->fType < TODO; */
}

inline bool is_valid_MonsterData(MonsterData *ptr)
{
    return IS_ALIGNED(ptr);
    /* && ptr->fType < TODO; */
}

inline bool is_valid_PlayerData(PlayerData *ptr)
{
    /* log_PlayerData(ptr);     /\* DEBUG *\/ */
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pNormalQuest): %d", */
    /*          is_valid_ptr__quick((ptr_t)ptr->pNormalQuest)); */
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pNightmareQuest): %d", */
    /*          is_valid_ptr__quick((ptr_t)ptr->pNightmareQuest) ); */
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pHellQuest): %d", */
    /*          is_valid_ptr__quick((ptr_t)ptr->pHellWaypoint)); */
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pNormalWaypoint): %d", */
    /*          is_valid_ptr__quick((ptr_t)ptr->pNormalWaypoint)); */
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pNightmareWaypoint): %d", */
    /*          is_valid_ptr__quick((ptr_t)ptr->pNightmareWaypoint)); */
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pHellWaypoint): %d", */
    /*          is_valid_ptr__quick((ptr_t)ptr->pHellWaypoint)); */
    /* LOG_WARNING("is_valid_string(ptr->szName, PLAYER_DATA_NAME_MAX, TRUE): %d", */
    /*          is_valid_string(ptr->szName, PLAYER_DATA_NAME_MAX)); */
    return IS_ALIGNED(ptr)
        && is_valid_ptr__quick((ptr_t)ptr->pNormalQuest)
        && is_valid_ptr__quick((ptr_t)ptr->pNightmareQuest)
        && is_valid_ptr__quick((ptr_t)ptr->pHellQuest)
        && is_valid_ptr__quick((ptr_t)ptr->pNormalWaypoint)
        && is_valid_ptr__quick((ptr_t)ptr->pNightmareWaypoint)
        && is_valid_ptr__quick((ptr_t)ptr->pHellWaypoint)
        && is_valid_string(ptr->szName, PLAYER_DATA_NAME_MAX, TRUE)
        ;
}

inline bool is_valid_Missile(Missile *ptr)
{
    /* log_UnitAny(ptr);                          /\* DEBUG *\/ */
    return IS_ALIGNED(ptr)
        && ptr->dwType == UNIT_MISSILE
        /* && ptr->dwTxtFileNo < MAX_MONSTER */
        && ptr->dwUnitId != 0
        /* && ptr->dwAct < 5 */
        /* && ptr->dwPlayerClass < CLASS_MAX */
        /* && !ptr->xPos */
        /* && !ptr->yPos */
        && is_valid_ptr__quick((ptr_t)ptr->pMissileData)
        && is_valid_ptr__quick((ptr_t)ptr->pPath)
        && (!ptr->pNext || is_valid_ptr__quick((ptr_t)ptr->pNext))
        ;
}

inline bool is_valid_Monster(Monster *ptr)
{
    /* log_UnitAny(ptr);                          /\* DEBUG *\/ */
    return IS_ALIGNED(ptr)
        && ptr->dwType == UNIT_MONSTER
        /* && ptr->dwTxtFileNo < MAX_MONSTER */
        && ptr->dwUnitId != 0
        && ptr->dwAct < 5
        /* && ptr->dwPlayerClass < CLASS_MAX */
        /* && !ptr->xPos */
        /* && !ptr->yPos */
        && is_valid_ptr__quick((ptr_t)ptr->pMonsterData)
        && is_valid_ptr__quick((ptr_t)ptr->pPath)
        && (!ptr->pNext || is_valid_ptr__quick((ptr_t)ptr->pNext))
        ;
}

inline bool is_valid_Player(Player *ptr)
{
    /* log_UnitAny(ptr);                          /\* DEBUG *\/ */
    /* LOG_WARNING("is_valid_ptr__quick((ptr_t)ptr->pPlayerData): %d", */
    /*             is_valid_ptr__quick((ptr_t)ptr->pPlayerData) ); */
    /* LOG_WARNING("is_valid_ptr((ptr_t)ptr->pAct): %d", */
    /*             is_valid_ptr((ptr_t)ptr->pAct) ); */
    /* LOG_WARNING("is_valid_ptr((ptr_t)ptr->pPath): %d", */
    /*             is_valid_ptr((ptr_t)ptr->pPath) ); */
    /* LOG_WARNING("tr->dwAct < 5: %d", ptr->dwAct < 5); */
    return IS_ALIGNED(ptr)
        && ptr->dwType == UNIT_PLAYER
        && ptr->dwTxtFileNo == ptr->dwPlayerClass
        && ptr->dwUnitId != 0
        && ptr->dwAct < 5
        && ptr->dwPlayerClass < MAX_PLAYER
        /* && !ptr->xPos */
        /* && !ptr->yPos */
        && is_valid_ptr__quick((ptr_t)ptr->pPlayerData)
        && (!ptr->pAct || is_valid_ptr__quick((ptr_t)ptr->pAct))
        && is_valid_ptr__quick((ptr_t)ptr->pPath)
        && is_valid_ptr__quick((ptr_t)ptr->pStats)
        && is_valid_ptr__quick((ptr_t)ptr->pInventory)
        && is_valid_ptr__quick((ptr_t)ptr->pSkills)
        && (!ptr->pNext || is_valid_ptr__quick((ptr_t)ptr->pNext))
        && (!ptr->pRoomNext || is_valid_ptr__quick((ptr_t)ptr->pRoomNext))
        ;
}

inline bool is_valid_UnitAny(UnitAny *ptr)
{
    if (!(IS_ALIGNED(ptr)
          && ptr->dwType < MAX_UNIT
          && ptr->dwUnitId != 0
          && (!ptr->pNext || is_valid_ptr__quick((ptr_t)ptr->pNext))
          /* && is_valid_ptr((ptr_t)ptr->pAct) */
          && is_valid_ptr__quick((ptr_t)ptr->pPath))) {
        return FALSE;
    }

    switch (ptr->dwType) {
        case UNIT_PLAYER:
            return is_valid_Player(ptr);
        case UNIT_MONSTER:
            return is_valid_Monster(ptr);
        /* case UNIT_OBJECT: */
        /*     return is_valid_object(ptr); //TODO */
        case UNIT_MISSILE:
            return is_valid_Missile(ptr); //TODO
        /* case UNIT_ITEM: */
        /*     return is_valid_item(ptr); //TODO */
        /* case UNIT_TILE: */
        /*     return is_valid_TILE(ptr); //TODO */
    }

    return TRUE;                /* DEBUG */
}

inline bool is_valid_UnitHashTable(UnitHashTable *ptr)
{
    if (!IS_ALIGNED(ptr)) {
        return FALSE;
    }

    int player_count = 0;
    int monster_count = 0;

    for (UnitType u_type = 0; u_type < MAX_UNIT; u_type++) {
        for (ptr_t i = 0; i < UNIT_HASH_TABLE_LEN; i++) {
            UnitAny *u = ptr->table[u_type * UNIT_HASH_TABLE_LEN + i];
            if (u) {
                if (!is_valid_ptr((ptr_t)u)) {
                    return FALSE;
                }

                UnitAny un;
                if (!memread((ptr_t)u, sizeof(UnitAny),
                             find_UnitAny_callback, &un)) {
                    return FALSE;
                }

                if (u_type == UNIT_MONSTER) {
                    monster_count++;
                } else if (u_type == UNIT_PLAYER) {
                    player_count++;
                }
            }
        }
    }

    LOG_DEBUG("%d monsters / %d players in table",
             monster_count, player_count);

    return player_count && monster_count;
}
