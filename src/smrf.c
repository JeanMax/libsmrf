#include "smrf.h"

#include "smrf/parse.h"
#include "smrf/proc.h"
#include "smrf/util/malloc.h"
#include "smrf/util/list.h"
#include "smrf/util/log.h"

#define DEFAULT_D2R_WINDOW_TITLE "Diablo II: Resurrected"
#define WINDOW_TITLE_OR_DEFAULT(game) (*game->window_title_setting ? game->window_title_setting : DEFAULT_D2R_WINDOW_TITLE)



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
    memset(game, 0, sizeof(GameState));
    pthread_mutex_unlock(&game->mutex);
    pthread_mutex_destroy(&game->mutex);
}

static void reset_game_state(GameState *game)
{
    static char status_bak[STATUS_MAX]; //TODO: dont

    pthread_mutex_lock(&game->mutex);
    free_all_levels();
    hdelall(g_unit_table);
    memcpy(status_bak, game->status, STATUS_MAX);
    memset((char *)game + PLAYER_DATA_NAME_MAX,
           0, sizeof(GameState) - PLAYER_DATA_NAME_MAX);
    memcpy(game->status, status_bak, STATUS_MAX);
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

inline static void *search_player_callback(byte *buf, size_t buf_len, ptr_t address, void *data)
{
    byte *b = buf;
    Player *player;
    UnitWithAddr *uwa = (UnitWithAddr *)data;

    while (buf_len >= sizeof(Player)) {
        player = (Player *)b;
        if (is_valid_Player(player)) {
            ptr_t here = address + (ptr_t)(b - buf);
            LOG_INFO("found maybe-Player ptr at %16jx", here); /* DEBUG */
            /* log_Player(player);                          /\* DEBUG *\/ */

            if (deep_validate_Player(player)) {
                memcpy(&uwa->unit, player, sizeof(Player));
                uwa->unit_addr[0] = here;
                return data; // done
            }
        }
        b += sizeof(ptr_t);
        buf_len -= sizeof(ptr_t);
    }

    return NULL; // keep reading
}

////////////////////////////////////////////////////////////////////////////////

static bool update_unit_callback(void *node_value, void *data)
{
    (void)data;
    UnitWithAddr *uwa = node_value;

    /* if (uwa->unit.dwType == 0 && uwa->unit.dwTxtFileNo == 0x1) { */
    /*     return FALSE; */
    /* } */

    MonsterData mdata;
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

        if (u.dwType == UNIT_MONSTER && u.wIsCorpse == 1) {  // remove dead monsters
            hdel(g_unit_table, uwa->unit.dwUnitId);
            return FALSE;
        }

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
        }

        return FALSE; // yay
    }

    // here we can assume all addr were invalid
    hdel(g_unit_table, uwa->unit.dwUnitId);

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

static bool update_player(GameState *game, Player *player, bool need_full_search)
{
    UnitWithAddr uwa = {0};  //TODO: now it's a addr-list... rename and use something appropriate here

    if (!need_full_search && game->_player_addr) {
        UPDATE_STATUS(game, "Updating Player...");
        if (!memread((ptr_t)game->_player_addr, sizeof(Player),
                     find_Player_callback, player)) {
            LOG_ERROR("Can't refresh player");
            return FALSE;
        }
        return TRUE;
    }

    UPDATE_STATUS(game, "Out of game...");

    void *search_aborted = memreadall(FALSE, search_player_callback, &uwa);
    if (!search_aborted) {
        LOG_ERROR("Can't find Player ptr");
        return FALSE;
    }
    UPDATE_STATUS(game, "In Game...");  // TODO: move that before

    /* pthread_mutex_lock(&game->mutex); */
    game->_player_addr = uwa.unit_addr[0];  //TODO: remove from GameState
    /* pthread_mutex_unlock(&game->mutex); */

    memcpy(player, &uwa.unit, sizeof(Player));
    log_Player(player);
    LOG_INFO("Woop woop! Found Player ptr at %16jx", uwa.unit_addr); /* DEBUG */
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
    static PlayerContent pc = {0};  // static because big
    static PlayerContent tmp = {0};  // static because big
    static Player player = {0};  // static because big

    bool need_full_search = FALSE;
    if (!update_game_window(game, &need_full_search)
        || !update_player(game, &player, need_full_search)) {
        reset_game_state(game);
        return FALSE;
    }

    UPDATE_STATUS(game, "Updating Game...");
    if (!memread((ptr_t)player.pPlayerData, sizeof(PlayerData),
                 find_PlayerData_callback, &tmp.player_data)) {
        LOG_ERROR("Can't find playerData");
        reset_game_state(game);
        return FALSE;
    }
    log_PlayerData(&tmp.player_data);

    if (!memread((ptr_t)player.pPath, sizeof(Path),
                 find_Path_callback, &tmp.path)) {
        LOG_ERROR("Can't find path");
        return FALSE;
    }
    log_Path(&tmp.path);

    if (!memread((ptr_t)tmp.path.pRoom1, sizeof(Room1),
                 find_Room1_callback, &tmp.room1)) {
        LOG_ERROR("Can't find room1");
        return FALSE;
    }
    /* log_Room1(&tmp.room1); */

    if (!memread((ptr_t)tmp.room1.pRoom2, sizeof(Room2),
                 find_Room2_callback, &tmp.room2)) {
        LOG_ERROR("Can't find room2");
        return FALSE;
    }
    /* log_Room2(&tmp.room2); */

    pthread_mutex_lock(&game->mutex);
    UPDATE_STATUS(game, "Updating Units...");
    LOG_DEBUG("before: %d units in table", g_unit_table->length);
    hiter(g_unit_table, update_unit_callback, NULL);
    LOG_DEBUG("after: %d units in table", g_unit_table->length);

    game->all_levels = g_levels;
    UPDATE_STATUS(game, "Updating Map...");
    game->level = parse_level_list((ptr_t)tmp.room2.pLevel); //TODO: pass Game
    tmp.room2.pLevel = game->level;
    memcpy(&pc, &tmp, sizeof(PlayerContent));
    memcpy(&game->player, &player, sizeof(Player));
    game->player.pPlayerData = &pc.player_data;
    game->player.pPath = &pc.path;
    game->player.pPath->pRoom1 = &pc.room1;
    game->player.pPath->pRoom1->pRoom2 = &pc.room2;
    UPDATE_STATUS(game, "Fresh.");
    pthread_mutex_unlock(&game->mutex);

    return TRUE;
}
