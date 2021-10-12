#include "seed.h"


inline static char byte_to_char(BYTE b)
{
	return (b >= 127 || b < 32) ? '.' : (char)b;
}

void log_data(void *ptr, size_t len)
{
    unsigned int i, j, end;

	for (i = 0; i < len; i += 8) {
		end = (len - i < 8) ? len - i : 8;
		for (j = i; j < i + end; j++) {
			fprintf(stderr, "%02x ", ((BYTE *)ptr)[j]);
		}
		for (j = 8; j > end; j--) {
			fprintf(stderr, "   ");
		}
		fprintf(stderr, "   ");
		for (j = i; j < i + end; j++) {
			fprintf(stderr, "%c", byte_to_char(((BYTE *)ptr)[j]));
		}
		fprintf(stderr, "\n");
	}
}

////////////////////////////////////////////////////////////////////////////////

void log_Path(Path *ptr)
{
    fprintf(stderr,
            "struct " CLR_GREEN "Path" CLR_RESET " {\n"
            "    WORD xOffset: %04x\n"
            "    WORD xPos: %04x\n"
            "    WORD yOffset: %04x\n"
            "    WORD yPos: %04x\n"
            "    DWORD _1[2]: %08x %08x\n"
            "    WORD xTarget: %04x\n"
            "    WORD yTarget: %04x\n"
            "    DWORD _2[2]: %08x %08x\n"
            "    Room1* pRoom1: %08lx\n"
            "    Room1* pRoomUnk: %08lx\n"
            "    DWORD _3[3]: %08x %08x %08x\n"
            "    UnitAny* pUnit: %08lx\n"
            "    DWORD dwFlags: %08x\n"
            "    DWORD _4: %08x\n"
            "    DWORD dwPathType: %08x\n"
            "    DWORD dwPrevPathType: %08x\n"
            "    DWORD dwUnitSize: %08x\n"
            "    DWORD _5[4]: %08x %08x %08x %08x\n"
            "    UnitAny* pTargetUnit: %08lx\n"
            "    DWORD dwTargetType: %08x\n"
            "    DWORD dwTargetId: %08x\n"
            "    BYTE bDirection: %02x\n"
            "}\"n",
            ptr->xOffset,
            ptr->xPos,
            ptr->yOffset,
            ptr->yPos,
            ptr->_1[0], ptr->_1[1],
            ptr->xTarget,
            ptr->yTarget,
            ptr->_2[0], ptr->_2[1],
            (PTR)ptr->pRoom1,
            (PTR)ptr->pRoomUnk,
            ptr->_3[0], ptr->_3[1], ptr->_3[2],
            (PTR)ptr->pUnit,
            ptr->dwFlags,
            ptr->_4,
            ptr->dwPathType,
            ptr->dwPrevPathType,
            ptr->dwUnitSize,
            ptr->_5[0], ptr->_5[1], ptr->_5[2], ptr->_5[3],
            (PTR)ptr->pTargetUnit,
            ptr->dwTargetType,
            ptr->dwTargetId,
            ptr->bDirection);
}

void log_Act(Act *ptr)
{
    fprintf(stderr,
            "struct " CLR_GREEN "Act" CLR_RESET " {\n"
            "    DWORD _1[3]: %08x %08x %08x\n"
            "    DWORD dwMapSeed: %08x\n"
            "    Room1* pRoom1: %08lx\n"
            "    DWORD dwAct: %08x\n"
            "    DWORD _2[12]: %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n"
            "    ActMisc* pMisc: %08lx\n"
            "}\n",
            ptr->_1[0], ptr->_1[1], ptr->_1[2],
            ptr->dwMapSeed,
            (PTR)ptr->pRoom1,
            ptr->dwAct,
            ptr->_2[0], ptr->_2[1], ptr->_2[2], ptr->_2[3], ptr->_2[4], ptr->_2[5], ptr->_2[6], ptr->_2[7], ptr->_2[8], ptr->_2[9], ptr->_2[10], ptr->_2[11],
            (PTR)ptr->pMisc);
}

void log_PlayerData(PlayerData *ptr)
{
    fprintf(stderr,
            "struct " CLR_GREEN "PlayerData" CLR_RESET " {\n"
            "    char szName[0x10]: %s\n"
            "    QuestInfo* pNormalQuest: %08lx\n"
            "    QuestInfo* pNightmareQuest: %08lx\n"
            "    QuestInfo* pHellQuest: %08lx\n"
            "    Waypoint* pNormalWaypoint: %08lx\n"
            "    Waypoint* pNightmareWaypoint: %08lx\n"
            "    Waypoint* pHellWaypoint: %08lx\n"
            "}\n",
            ptr->szName,
            (PTR)ptr->pNormalQuest,
            (PTR)ptr->pNightmareQuest,
            (PTR)ptr->pHellQuest,
            (PTR)ptr->pNormalWaypoint,
            (PTR)ptr->pNightmareWaypoint,
            (PTR)ptr->pHellWaypoint);
}

void log_Player(Player *ptr)
{
    fprintf(stderr,
            "struct " CLR_GREEN "Player" CLR_RESET " {\n"
            "    DWORD dwType: %08x\n"
            "    DWORD dwTxtFileNo: %08x\n"
            "    DWORD dwUnitId: %08x\n"
            "    DWORD dwMode: %08x\n"
            "    PlayerData* pPlayerData: %08lx\n"
            "    DWORD _1: %08x\n"
            "    DWORD dwAct: %08x\n"
            "    Act* pAct: %08lx\n"
            "    DWORD dwSeed[2]: %08x, %08x\n"
            "    DWORD _2: %08x\n"
            "    Path* pPath: %08lx\n"
            "}\n",
            ptr->dwType,
            ptr->dwTxtFileNo,
            ptr->dwUnitId,
            ptr->dwMode,
            (PTR)ptr->pPlayerData,
            ptr->_1,
            ptr->dwAct,
            (PTR)ptr->pAct,
            ptr->dwSeed[0], ptr->dwSeed[1],
            ptr->_2,
            (PTR)ptr->pPath);
}

////////////////////////////////////////////////////////////////////////////////

inline BOOL is_valid_Path(Path *ptr)
{
    return IS_ALIGNED(ptr)
        && is_valid_ptr((PTR)ptr->pRoom1)
        && is_valid_ptr((PTR)ptr->pUnit)
        && is_valid_ptr((PTR)ptr->pTargetUnit);
}

inline BOOL is_valid_Act(Act *ptr)
{
    return IS_ALIGNED(ptr)
        && is_valid_ptr((PTR)ptr->pRoom1)
        && is_valid_ptr((PTR)ptr->pMisc);/* || !ptr->pMisc)
                                            && ptr->dwAct < 5;*/
}

inline BOOL is_valid_PlayerData(PlayerData *ptr)
{
    return IS_ALIGNED(ptr)
        && is_valid_ptr((PTR)ptr->pNormalQuest)
        && is_valid_ptr((PTR)ptr->pNightmareQuest)
        && is_valid_ptr((PTR)ptr->pHellQuest)
        && is_valid_ptr((PTR)ptr->pNormalWaypoint)
        && is_valid_ptr((PTR)ptr->pNightmareWaypoint)
        && is_valid_ptr((PTR)ptr->pHellWaypoint);
}

inline BOOL is_valid_Player(Player *ptr)
{
    return IS_ALIGNED(ptr)
        && is_valid_ptr((PTR)ptr->pPlayerData)
        && is_valid_ptr((PTR)ptr->pAct)
        && is_valid_ptr((PTR)ptr->pPath);
}
