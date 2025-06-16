#include "smrf.h"

#include "smrf/parse.h"
#include "smrf/proc.h"
#include "smrf/util/malloc.h"
#include "smrf/util/list.h"
#include "smrf/util/log.h"

#define DEFAULT_D2R_WINDOW_TITLE "Diablo II: Resurrected"
#define WINDOW_TITLE_OR_DEFAULT(game)                                          \
  (*game->window_title_setting ? game->window_title_setting                    \
                               : DEFAULT_D2R_WINDOW_TITLE)



static void unit_deleter(size_t unused_key, void *node_value)
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

void init_game_state(GameState *game)
{
    memset(game, 0, sizeof(GameState));
    UPDATE_STATUS(game, "Loading...");
    g_unit_table = hnew(0, unit_deleter);
    pthread_mutex_init(&game->mutex, NULL);
}

void destroy_game_state(GameState *game)
{
    pthread_mutex_lock(&game->mutex);
    free_all_levels();
    hfree(&g_unit_table);
    pthread_mutex_unlock(&game->mutex);
    pthread_mutex_destroy(&game->mutex);
    memset(game, 0, sizeof(GameState));
}

static void reset_game_state(GameState *game)
{
    pthread_mutex_lock(&game->mutex);
    free_all_levels();
    hdelall(g_unit_table);
    game->all_levels = NULL;
    game->level = NULL;
    game->player = NULL;
    pthread_mutex_unlock(&game->mutex);
}

////////////////////////////////////////////////////////////////////////////////

static bool deep_validate_Player(Player *maybe_player)
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

static bool update_unit_callback(void *node_value, void *data)
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

static Player *parse_unit_table(ptr_t ut_addr)
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
                    LOG_WARNING("ZBOUB");
            }
        }
    }

    return player;
}

static void *search_unit_table_callback(byte *buf, size_t buf_len, ptr_t address, void *data)
{
    (void)data;
    byte *b = buf;
    UnitHashTable ut;

    while (buf_len >= sizeof(dword)) {
        dword maybe_ut = *(dword *)b;

        if (is_valid_ptr__base(maybe_ut)
            && memreadbase(maybe_ut, sizeof(UnitHashTable),
                           find_UnitHashTable_callback, &ut)) {
            ptr_t here = address + (ptr_t)(b - buf);
            LOG_INFO("found UT at %16jx - %16jx", here, maybe_ut); /* DEBUG */
            return (void *)here;
            /* return (void *)maybe_ut; */
        }

        b += sizeof(dword);
        buf_len -= sizeof(dword);
    }

    return NULL; // keep reading
}

////////////////////////////////////////////////////////////////////////////////

static bool update_player(GameState *game, bool need_full_search)
{
    UPDATE_STATUS(game, "Out of game...");

    if (need_full_search || !game->_ut_addr) {
        void *ut_addr = memreadallbase(search_unit_table_callback, NULL);
        if (!ut_addr) {
            LOG_ERROR("Can't find UnitHashTable ptr");
            return FALSE;
        }
        game->_ut_addr = (ptr_t)ut_addr;
        LOG_INFO("Woop woop! Found UnitHashTable ptr"); /* DEBUG */
    }

    dword ut;
    if (!memread((ptr_t)game->_ut_addr, sizeof(dword),
                 find_dword_callback, &ut)) {
        LOG_ERROR("Can't read UnitHashTable ptr");
        game->_ut_addr = 0;
        return FALSE;
    }
    Player *player = parse_unit_table(ut);
    if (!game->player) {
        if (!player) {
            LOG_ERROR("Can't find player");
            game->_ut_addr = 0;
            return FALSE;
        } else {
            pthread_mutex_lock(&game->mutex);
            game->player = player;
            pthread_mutex_unlock(&game->mutex);
            LOG_INFO("Woop woop! Found Player ptr"); /* DEBUG */
        }
    }

    return TRUE;
}

static bool update_game_window(GameState *game, bool *need_full_search)
{
    static pid_t prev_pid = 0;
    static bool need_pid_refresh = TRUE;
    static char prev_title_name[WINDOW_TITLE_MAX] = {0};

    if (strcmp(prev_title_name, WINDOW_TITLE_OR_DEFAULT(game))) {
        need_pid_refresh = TRUE;
        UPDATE_STATUS(game, "Searching for D2R...");
        strcpy(prev_title_name, WINDOW_TITLE_OR_DEFAULT(game));
    }

    pid_t pid = readmaps(WINDOW_TITLE_OR_DEFAULT(game), need_pid_refresh);
    if (!pid) {
        need_pid_refresh = TRUE;
        LOG_ERROR("Can't read maps");
        return FALSE;
    }
    need_pid_refresh = FALSE;

    if (prev_pid != pid) {
        prev_pid = pid;
        LOG_WARNING("D2R pid changed");
        *need_full_search = TRUE;
    }

    return TRUE;
}

bool update_game_state(GameState *game)
{
    static Room1 room1;
    static Room2 room2;

    bool need_full_search = FALSE;
    if (!update_game_window(game, &need_full_search)
        || !update_player(game, need_full_search)) {
        reset_game_state(game);
        return FALSE;
    }

    dword uid = game->player->dwUnitId;

    pthread_mutex_lock(&game->mutex);
    UPDATE_STATUS(game, "Updating Units...");
    LOG_DEBUG("before: %d units in table", g_unit_table->length);
    hiter(g_unit_table, update_unit_callback, NULL);
    LOG_DEBUG("after: %d units in table", g_unit_table->length);
    pthread_mutex_unlock(&game->mutex);

    UnitWithAddr *uwa = hget(g_unit_table, uid);
    if (!uwa) {
        LOG_INFO("Player lost, assuming Out of Game");
        UPDATE_STATUS(game, "Out of game...");
        reset_game_state(game);
        return FALSE;
    }

    if (!memread((ptr_t)uwa->unit.pPath->pRoom1, sizeof(Room1),
                 find_Room1_callback, &room1)) {
        LOG_ERROR("Can't find room1");
        return FALSE;
    }
    /* log_Room1(&tmp.room1); */

    if (!memread((ptr_t)room1.pRoom2, sizeof(Room2),
                 find_Room2_callback, &room2)) {
        LOG_ERROR("Can't find room2");
        return FALSE;
    }
    /* log_Room2(&tmp.room2); */


    pthread_mutex_lock(&game->mutex);
    game->player = &uwa->unit;
    game->all_levels = g_levels;
    UPDATE_STATUS(game, "Updating Map...");
    game->level = parse_level_list((ptr_t)room2.pLevel); //TODO: pass Game
    room2.pLevel = game->level;
    room1.pRoom2 = &room2;
    game->player->pPath->pRoom1 = &room1;
    UPDATE_STATUS(game, "Fresh.");
    pthread_mutex_unlock(&game->mutex);

    return TRUE;
}
