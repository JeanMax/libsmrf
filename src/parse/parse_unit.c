#include "parse.h"

#include "proc.h"  // is_valid_ptr / memread
#include "util/list.h"
#include "util/malloc.h"

#define HKEY(u) ( (size_t)(u).dwUnitId | ((size_t)(u).dwType << 32) )

Htable     *g_unit_table = {0};  //TODO: move that to GameState (and remove private fields there)


inline void delete_unit_callback(size_t unused_key, void *node_value)
{
    UnitWithAddr *uwa = node_value;
    (void)unused_key;

    if (uwa->unit.pPath) {
        FREE(uwa->unit.pPath);
    }
    if (uwa->unit.dwType == UNIT_MONSTER && uwa->unit.pMonsterData) {
        FREE(uwa->unit.pMonsterData);
    }
    FREE(uwa);
}

////////////////////////////////////////////////////////////////////////////////

bool update_unit_callback(void *node_value, void *data)
{
    (void)data;
    UnitWithAddr *uwa = node_value;

    /* if (uwa->unit.dwType == 0 && uwa->unit.dwTxtFileNo == 0x1) { */
    /*     return FALSE; */
    /* } */

    Path path;
    UnitAny u;
    UnitAny *next;
    MonsterData *m;
    Path *p;

    for (int i = 0; i < MAX_UNIT_ADDR; i++) {
        if (!is_valid_ptr(uwa->unit_addr[i])) {
            uwa->unit_addr[i] = 0;
            continue;
        }

        if (!memread(uwa->unit_addr[i], sizeof(UnitAny),
                     find_UnitAny_callback, &u)) {
            LOG_WARNING("Can't refresh unit at addr %16jx", uwa->unit_addr[i]);
            uwa->unit_addr[i] = 0;
            continue;
        }
        /* log_UnitAny(&u); */

        if (!(u.dwUnitId == uwa->unit.dwUnitId
              && u.dwType == uwa->unit.dwType)) {  // well, shit
            LOG_WARNING("Wrong unit refresh at addr %16jx", uwa->unit_addr[i]);
            uwa->unit_addr[i] = 0;
            continue;
        }

        /* if (u.dwType == UNIT_MONSTER && u.wIsCorpse == 1) {  // remove dead monsters */
        /*     hdel(g_unit_table, uwa->unit.dwUnitId); */
        /*     return FALSE; */
        /* } */

        next = uwa->unit.pNext;
        p = uwa->unit.pPath;
        m = uwa->unit.pMonsterData;
        memcpy(&uwa->unit, &u, sizeof(UnitAny));
        uwa->unit.pNext = next;
        uwa->unit.pPath = p;
        uwa->unit.pMonsterData = m;

        if (u.pPath) {
            if (!is_valid_ptr((ptr_t)u.pPath)
                || !memread((ptr_t)u.pPath, sizeof(Path),
                            find_Path_callback, &path)) {
                LOG_WARNING("Can't resfresh unit's Path at addr %16jx", uwa->unit_addr[i]);
                uwa->unit_addr[i] = 0;
                continue;
            }
            DUPE(u.pPath, &path, sizeof(Path));
        }
        if (uwa->unit.pPath) {
            FREE(uwa->unit.pPath);
        }
        uwa->unit.pPath = u.pPath;

        if (u.dwType == UNIT_MONSTER) {
            MonsterData mdata;
            if (u.pMonsterData) {
                if (!is_valid_ptr((ptr_t)u.pMonsterData)
                    || !memread((ptr_t)u.pMonsterData, sizeof(MonsterData),
                                find_MonsterData_callback, &mdata)) {
                    LOG_WARNING("Can't resfresh unit's MonsterData at addr %16jx", uwa->unit_addr[i]);
                    uwa->unit_addr[i] = 0;
                    continue;
                }
                DUPE(u.pMonsterData, &mdata, sizeof(MonsterData));
            }
            if (uwa->unit.pMonsterData) {
                FREE(uwa->unit.pMonsterData);
            }
            uwa->unit.pMonsterData = u.pMonsterData;

            /* LOG_DEBUG("Successful monster refresh at %08x", uwa->unit_addr); /\* DEBUG *\/ */
        } else if (u.dwType == UNIT_PLAYER) {
            PlayerData pdata;
            if (u.pPlayerData) {
                if (!is_valid_ptr((ptr_t)u.pPlayerData)
                    || !memread((ptr_t)u.pPlayerData, sizeof(PlayerData),
                                find_PlayerData_callback, &pdata)) {
                    LOG_WARNING("Can't resfresh unit's PlayerData at addr %16jx", uwa->unit_addr[i]);
                    uwa->unit_addr[i] = 0;
                    continue;
                }
                DUPE(u.pPlayerData, &pdata, sizeof(PlayerData));
            }
            if (uwa->unit.pPlayerData) {
                FREE(uwa->unit.pPlayerData);
            }
            uwa->unit.pPlayerData = u.pPlayerData;

            /* LOG_DEBUG("Successful player refresh at %08x", uwa->unit_addr); /\* DEBUG *\/ */
        }


        return FALSE; // yay
    }

    // here we can assume all addr were invalid
    hdel(g_unit_table, uwa->unit.dwUnitId);

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

bool deep_validate_Player(Player *maybe_player)
{
    Inventory inventory;
    PlayerData player_data;
    Path path;
    Room1 room1;
    Room2 room2;
    Level level;


    if (!maybe_player->pPlayerData
        || !memread((ptr_t)maybe_player->pPlayerData, sizeof(PlayerData),
                    find_PlayerData_callback, &player_data)) {
        /* LOG_WARNING("Can't validate PlayerData"); */
        return FALSE;
    }
    log_PlayerData(&player_data);

    if (!maybe_player->pInventory
        || !memread((ptr_t)maybe_player->pInventory, sizeof(Inventory),
                    find_Inventory_callback, &inventory)) {
        LOG_WARNING("Can't validate Inventory");
        return FALSE;
    }
    log_Inventory(&inventory);

    if (!maybe_player->pPath
        || !memread((ptr_t)maybe_player->pPath, sizeof(Path),
                    find_Path_callback, &path)) {
        LOG_WARNING("Can't validate Path");
        return FALSE;
    }
    log_UnitAny(maybe_player);                          /* DEBUG */
    log_Path(&path);

    LOG_DEBUG("main: classic=%d xpac=%d ptr=%16jx [%s]",
              inventory.wIsMainClassic, inventory.wIsMainXpac, inventory.pIsMain,
              player_data.szName); /* DEBUG */
    if (!inventory.pIsMain || inventory.wIsMainClassic != 0x7) {
    /* if (!inventory.wIsMainXpac || inventory.wIsMainClassic == 1) { */
        LOG_WARNING("Not the main character");
        return FALSE;
    }

    if (!path.pRoom1
        || !memread((ptr_t)path.pRoom1, sizeof(Room1),
                    find_Room1_callback, &room1)) {
        LOG_WARNING("Can't validate Room1");
        return FALSE;
    }
    log_Room1(&room1);

    if (!room1.pRoom2
        || !memread((ptr_t)room1.pRoom2, sizeof(Room2),
                    find_Room2_callback, &room2)) {
        LOG_WARNING("Can't validate Room2");
        return FALSE;
    }
    log_Room2(&room2);

    if (!room2.pLevel
        || !memread((ptr_t)room2.pLevel, sizeof(Level),
                    find_Level_callback, &level)) {
        LOG_WARNING("Can't validate Level");
        return FALSE;
    }
    log_Level(&level);

    if (!level.pRoom2First
        || !memread((ptr_t)level.pRoom2First, sizeof(Room2),
                    find_Room2_callback, &room2)) {
        LOG_WARNING("Can't validate Room2First");
        return FALSE;
    }
    log_Room2(&room2);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////

Player *parse_unit_table(ptr_t ut_addr)
{
    UnitHashTable ut;
    Player *player = NULL;

    if (!is_valid_ptr__base(ut_addr)
        || !memreadbase(ut_addr, sizeof(UnitHashTable),
                    find_UnitHashTable_callback, &ut)) {
        LOG_ERROR("Can't parse unit table %d", is_valid_ptr__base(ut_addr));
        return NULL;
    }

    for (UnitType u_type = 0; u_type < MAX_UNIT; u_type++) {
        for (ptr_t i = 0; i < UNIT_HASH_TABLE_LEN; i++) {
            UnitAny *u = ut.table[u_type * UNIT_HASH_TABLE_LEN + i];
            if (!u) {
                continue;
            }

            UnitAny *parsed_unit = parse_unit_list((ptr_t)u);
            if (!parsed_unit) {
                LOG_WARNING("Couldn't parse unit from UnitHashTable");
                continue;
            }

            UnitAny un;
            if (u_type == UNIT_PLAYER && !player
                && memread((ptr_t)u, sizeof(Player),
                           find_Player_callback, &un)
                && deep_validate_Player(&un)) {
                    player = parsed_unit;
            }
        }
    }

    return player;
}

////////////////////////////////////////////////////////////////////////////////

static UnitAny *parse_unit(ptr_t u_addr, UnitAny **u_last, UnitAny **u_first)
{
    static MonsterData mdata;
    static PlayerData pdata;
    static UnitAny u;
    static Path path;

    if (!memread(u_addr, sizeof(UnitAny),
                 find_UnitAny_callback, &u)) {
        LOG_WARNING("Can't update unit");
        return NULL;
    }
    /* log_UnitAny(&u); */

    UnitAny *ret = u.pNext;

    /* if (u.dwType == UNIT_MONSTER && u.wIsCorpse == 1) {  // remove dead monsters */
    /*     return ret; */
    /* } */

    UnitWithAddr *uwa = hget(g_unit_table, HKEY(u));
    if (uwa) {
        for (int i = 0; i < MAX_UNIT_ADDR; i++) {
            if (u_addr == uwa->unit_addr[i]) {  // already stored
                return ret;
            }
        }
        for (int i = 0; i < MAX_UNIT_ADDR; i++) {
            if (!uwa->unit_addr[i]) {  // store new addr
                uwa->unit_addr[i] = u_addr;
                return ret;
            }
        }
        LOG_DEBUG("Can't store new addr for unit %x", u.dwUnitId);
        return ret;
    }

    if (!u.pPath || !is_valid_ptr((ptr_t)u.pPath)
        || !memread((ptr_t)u.pPath, sizeof(Path),
                    find_Path_callback, &path)) {
        /* LOG_WARNING("Can't update unit's Path %d", */
        /*             u.pPath && is_valid_ptr((ptr_t)u.pPath));  //TODO: this fails a lot */
        /* log_Path(&path);    /\* DEBUG *\/ */
        /* log_UnitAny(&u);    /\* DEBUG *\/ */
        return ret;
    }

    if (u.dwType == UNIT_MONSTER) {
        if (!u.pMonsterData || !is_valid_ptr((ptr_t)u.pMonsterData)
            || !memread((ptr_t)u.pMonsterData, sizeof(MonsterData),
                        find_MonsterData_callback, &mdata)) {
            LOG_WARNING("Can't update unit's MonsterData");  //TODO: this fails a lot
            /* log_MonsterData(&mdata);    /\* DEBUG *\/ */
            /* log_UnitAny(&u);    /\* DEBUG *\/ */
            return ret;
        }
        DUPE(u.pMonsterData, &mdata, sizeof(MonsterData));
        /* log_MonsterData(&mdata);    /\* DEBUG *\/ */
        /* log_UnitAny(&u);    /\* DEBUG *\/ */
    } else if (u.dwType == UNIT_PLAYER) {
        if (!u.pPlayerData || !is_valid_ptr((ptr_t)u.pPlayerData)
            || !memread((ptr_t)u.pPlayerData, sizeof(PlayerData),
                        find_PlayerData_callback, &pdata)) {
            LOG_WARNING("Can't update unit's PlayerData");  //TODO: this fails a lot
            /* log_PlayerData(&pdata);    /\* DEBUG *\/ */
            /* log_UnitAny(&u);    /\* DEBUG *\/ */
            return ret;
        }
        DUPE(u.pPlayerData, &pdata, sizeof(PlayerData));
        /* log_PlayerData(&pdata);    /\* DEBUG *\/ */
        /* log_UnitAny(&u);    /\* DEBUG *\/ */
    }

    //TODO: parse inventory

    DUPE(u.pPath, &path, sizeof(Path));
    /* DUPE(u.pAct, &act, sizeof(Act)); */

    ZALLOC(uwa, sizeof(UnitWithAddr)); //TODO: leak (just a few? looks like you loose a node)
    memcpy(&uwa->unit, &u, sizeof(UnitAny));
    uwa->unit_addr[0] = u_addr;
    hset(g_unit_table, HKEY(u), uwa);

    uwa->unit.pNext = NULL;
    ADD_LINK(*u_first, *u_last, &uwa->unit);

    return ret;
}

UnitAny *parse_unit_list(ptr_t u_addr)
{
    UnitAny *u_last = NULL, *u_first = NULL;

    while (is_valid_ptr(u_addr)) {
        u_addr = (ptr_t)(void *)parse_unit(u_addr,
                                           &u_first, &u_last);
    }

    return u_first;
}
