#include "d2structs.h"

void log_Level(Level *ptr)
{
    (void)ptr;
    LOG_DEBUG("struct " CLR_GREEN "Level" CLR_RESET " {\n"
              "    dword _1[4]: %08x %08x %08x %08x\n"
              "    Room2* pRoom2First: %16jx\n"
              "    void* pDunno: %16jx\n"
              "    dword _2: %08x\n"
              "    dword dwPosX: %08x\n"
              "    dword dwPosY: %08x\n"
              "    dword dwSizeX: %08x\n"
              "    dword dwSizeY: %08x\n"
              "    dword _3[96]: %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n"
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
              ptr->_1[0], ptr->_1[1], ptr->_1[2], ptr->_1[3],
              (ptr_t)ptr->pRoom2First,
              (ptr_t)ptr->pDunno,
              ptr->_2,
              ptr->dwPosX,
              ptr->dwPosY,
              ptr->dwSizeX,
              ptr->dwSizeY,
              ptr->_3[0], ptr->_3[1], ptr->_3[2], ptr->_3[3], ptr->_3[4], ptr->_3[5], ptr->_3[6], ptr->_3[7], ptr->_3[8], ptr->_3[9], ptr->_3[10], ptr->_3[11], ptr->_3[12], ptr->_3[13], ptr->_3[14], ptr->_3[15], ptr->_3[16], ptr->_3[17], ptr->_3[18], ptr->_3[19], ptr->_3[20], ptr->_3[21], ptr->_3[22], ptr->_3[23], ptr->_3[24], ptr->_3[25], ptr->_3[26], ptr->_3[27], ptr->_3[28], ptr->_3[29], ptr->_3[30], ptr->_3[31], ptr->_3[32], ptr->_3[33], ptr->_3[34], ptr->_3[35], ptr->_3[36], ptr->_3[37], ptr->_3[38], ptr->_3[39], ptr->_3[40], ptr->_3[41], ptr->_3[42], ptr->_3[43], ptr->_3[44], ptr->_3[45], ptr->_3[46], ptr->_3[47], ptr->_3[48], ptr->_3[49], ptr->_3[50], ptr->_3[51], ptr->_3[52], ptr->_3[53], ptr->_3[54], ptr->_3[55], ptr->_3[56], ptr->_3[57], ptr->_3[58], ptr->_3[59], ptr->_3[60], ptr->_3[61], ptr->_3[62], ptr->_3[63], ptr->_3[64], ptr->_3[65], ptr->_3[66], ptr->_3[67], ptr->_3[68], ptr->_3[69], ptr->_3[70], ptr->_3[71], ptr->_3[72], ptr->_3[73], ptr->_3[74], ptr->_3[75], ptr->_3[76], ptr->_3[77], ptr->_3[78], ptr->_3[79], ptr->_3[80], ptr->_3[81], ptr->_3[82], ptr->_3[83], ptr->_3[84], ptr->_3[85], ptr->_3[86], ptr->_3[87], ptr->_3[88], ptr->_3[89], ptr->_3[90], ptr->_3[91], ptr->_3[92], ptr->_3[93], ptr->_3[94], ptr->_3[95],
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
              "    dword _2[2]: %08x %08x\n"
              "    Room1* pRoom1: %16jx\n"
              "    Room1* pRoomUnk: %16jx\n"
              "    dword _3[3]: %08x %08x %08x\n"
              "    UnitAny* pUnit: %16jx\n"
              "    dword dwFlags: %08x\n"
              "    dword _4: %08x\n"
              "    dword dwPathType: %08x\n"
              "    dword dwPrevPathType: %08x\n"
              "    dword dwUnitSize: %08x\n"
              "    dword _5[4]: %08x %08x %08x %08x\n"
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
              ptr->_2[0], ptr->_2[1],
              (ptr_t)ptr->pRoom1,
              (ptr_t)ptr->pRoomUnk,
              ptr->_3[0], ptr->_3[1], ptr->_3[2],
              (ptr_t)ptr->pUnit,
              ptr->dwFlags,
              ptr->_4,
              ptr->dwPathType,
              ptr->dwPrevPathType,
              ptr->dwUnitSize,
              ptr->_5[0], ptr->_5[1], ptr->_5[2], ptr->_5[3],
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
    hex_dump(ptr, sizeof(Act)); /* DEBUG */
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
    hex_dump(ptr, sizeof(ActMisc));
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

////////////////////////////////////////////////////////////////////////////////

static bool is_valid_player_name_str(const char *b, size_t len)
{
    const char *start = b;
    while ((size_t)(b - start) < len) {
        if (*b && !(isupper((int)*b) || islower((int)*b) || *b == '_')) {
            return FALSE;
        }
        // check if right side is 0 padded
        if (!*b) {
            while ((size_t)(b - start) < len) {
                if (*b) {
                    return FALSE;
                }
                b++;
            }
            return *start;
        }
        b++;
    }
    return *start; // forbid empty string
}

inline bool is_valid_Level(Level *ptr)
{
    return IS_ALIGNED(ptr)
        && (!ptr->pRoom2First || is_valid_ptr((ptr_t)ptr->pRoom2First))
        /* && is_valid_ptr((ptr_t)ptr->pDunno) */
        && (!ptr->pNext || is_valid_ptr((ptr_t)ptr->pNext));
        /* && (!ptr->pMisc || is_valid_ptr((ptr_t)ptr->pMisc)); */
}

inline bool is_valid_Room2(Room2 *ptr)
{
    return IS_ALIGNED(ptr)
        /* && is_valid_ptr((ptr_t)ptr->pRoom2Near) */
        && is_valid_ptr((ptr_t)ptr->pRoomTiles)
        && is_valid_ptr((ptr_t)ptr->pLevel)
        && (!ptr->pPreset || is_valid_ptr((ptr_t)ptr->pPreset));
        ;
}

inline bool is_valid_Room1(Room1 *ptr)
{
    return IS_ALIGNED(ptr)
        && (!ptr->pRoomsNear || is_valid_ptr((ptr_t)ptr->pRoomsNear))
        && (!ptr->pRoom2 || is_valid_ptr((ptr_t)ptr->pRoom2))
        && (!ptr->pDunno2 || is_valid_ptr((ptr_t)ptr->pDunno2))
        /* && is_valid_ptr((ptr_t)ptr->pDunno3) */
        && (!ptr->Coll || is_valid_ptr((ptr_t)ptr->Coll))
        /* && is_valid_ptr((ptr_t)ptr->pUnitFirst) */
        /* && is_valid_ptr((ptr_t)ptr->pNext) */
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

    return IS_ALIGNED(ptr)
        && (!ptr->pNext || is_valid_ptr((ptr_t)ptr->pNext));
}

inline bool is_valid_Path(Path *ptr)
{
    return IS_ALIGNED(ptr)
        && is_valid_ptr((ptr_t)ptr->pRoom1)
        && (!ptr->pTargetUnit || is_valid_ptr((ptr_t)ptr->pRoomUnk))
        && is_valid_ptr((ptr_t)ptr->pUnit)
        && (!ptr->pTargetUnit || is_valid_ptr((ptr_t)ptr->pTargetUnit));
}

inline bool is_valid_Act(Act *ptr)
{
    return IS_ALIGNED(ptr)
        && is_valid_ptr((ptr_t)ptr->pDunno);
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

inline bool is_valid_PlayerData(PlayerData *ptr)
{
    return IS_ALIGNED(ptr)
        && is_valid_stack_ptr((ptr_t)ptr->pNormalQuest)
        && is_valid_stack_ptr((ptr_t)ptr->pNightmareQuest)
        && is_valid_stack_ptr((ptr_t)ptr->pHellQuest)
        && is_valid_stack_ptr((ptr_t)ptr->pNormalWaypoint)
        && is_valid_stack_ptr((ptr_t)ptr->pNightmareWaypoint)
        && is_valid_stack_ptr((ptr_t)ptr->pHellWaypoint)
        && is_valid_player_name_str(ptr->szName, PLAYER_DATA_NAME_MAX);
}

inline bool is_valid_Player(Player *ptr)
{
    return IS_ALIGNED(ptr)
        && is_valid_stack_ptr((ptr_t)ptr->pPlayerData)
        && is_valid_ptr((ptr_t)ptr->pAct)
        && is_valid_ptr((ptr_t)ptr->pPath)
        && ptr->dwAct < 5;
}
