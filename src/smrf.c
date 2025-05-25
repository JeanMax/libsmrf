#include "smrf.h"

#include "smrf/parse.h"
#include "smrf/proc.h"
#include "smrf/util/malloc.h"
#include "smrf/util/list.h"
#include "smrf/util/log.h"

#define DEFAULT_D2R_WINDOW_TITLE "Diablo II: Resurrected"

static void unit_deleter(size_t unused_key, void *node_value)
{
    UnitWithAddr *uwa = node_value;
    (void)unused_key;

    /* if (uwa->unit.pAct) { */
    /*     FREE(uwa->unit.pAct); */
    /* } */
    if (uwa->unit.pPath) {
        FREE(uwa->unit.pPath);
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
    pthread_mutex_lock(&game->mutex);
    free_all_levels();
    hdelall(g_unit_table);
    memset((char *)game + PLAYER_DATA_NAME_MAX,
           0, sizeof(GameState) - PLAYER_DATA_NAME_MAX);
    /* UPDATE_STATUS(game, "Reset..."); */
    pthread_mutex_unlock(&game->mutex);
}

////////////////////////////////////////////////////////////////////////////////

//TODO: add enum for callback return
inline static bool search_player_callback(byte *buf, size_t buf_len, ptr_t address, void *data)
{
    static int i = 0;           /* DEBUG */
    byte *b = buf;
    Player *player;
    PlayerList **maybe_player_list = (PlayerList **)data;

    while (buf_len >= sizeof(Player)) {
        player = (Player *)b;
        if (is_valid_Player(player)) {
            ptr_t here = address + (ptr_t)(b - buf);
            LOG_INFO("found maybe-Player ptr at %16jx [%d]", here, i); /* DEBUG */
            log_Player(player);                          /* DEBUG */

            PlayerList *pl;
            MALLOC(pl, sizeof(PlayerList));
            memcpy(&pl->player, b, sizeof(Player));
            pl->player_addr = here;
            pl->idx = i++;      /* DEBUG */
            PUSH_LINK(*maybe_player_list, pl);
        }
        b += sizeof(ptr_t);
        buf_len -= sizeof(ptr_t);
    }

    return TRUE; // keep reading
}

static void free_maybe_player(PlayerList *ptr)
{
    PlayerList *prev;

    while (ptr) {
        prev = ptr;
        ptr = ptr->pNext;
        FREE(prev);
    }
}

////////////////////////////////////////////////////////////////////////////////

static bool update_unit_callback(void *node_value, void *data)
{
    (void)data;
    UnitWithAddr *uwa = node_value;

    /* if (uwa->unit.dwType == 0 && uwa->unit.dwTxtFileNo == 0x1) { */
    /*     return FALSE; */
    /* } */

    /* static Act act; */
    static Path path;
    static UnitAny u;
    UnitAny *next;

    if (!memread(uwa->unit_addr, sizeof(UnitAny),
                 find_UnitAny_callback, &u)) {
        LOG_WARNING("Can't refresh unit");
        hdel(g_unit_table, uwa->unit.dwUnitId);
        return FALSE;
    }
    /* log_UnitAny(&u); */

    if (u.dwType == 1 && u.wIsCorpse == 0x1) {  // remove dead monsters
        hdel(g_unit_table, uwa->unit.dwUnitId);
        return FALSE;
    }

    next = uwa->unit.pNext;

    /* if (u.pAct && is_valid_ptr((ptr_t)u.pAct)) { */
    /*     if (!memread((ptr_t)u.pAct, sizeof(Act), */
    /*                  find_Act_callback, &act)) {  //TODO: this fails a lot */
    /*         LOG_WARNING("Can't resfresh unit's Act, removing from Htable"); */
    /*         hdel(g_unit_table, uwa->unit.dwUnitId); */
    /*         return FALSE; */
    /*     } */
    /*     DUPE(u.pAct, &act, sizeof(Act)); */
    /* } */
    /* if (uwa->unit.pAct) { */
    /*     FREE(uwa->unit.pAct); */
    /* } */

    if (u.pPath && is_valid_ptr((ptr_t)u.pPath)) {
        if (!memread((ptr_t)u.pPath, sizeof(Path),
                     find_Path_callback, &path)) {  //TODO: this fails a lot
            LOG_WARNING("Can't resfresh unit's Path, removing from Htable");
            hdel(g_unit_table, uwa->unit.dwUnitId);
            return FALSE;
        }
        DUPE(u.pPath, &path, sizeof(Path));
    }
    if (uwa->unit.pPath) {
        FREE(uwa->unit.pPath);
    }

    memcpy(&uwa->unit, &u, sizeof(UnitAny));
    uwa->unit.pNext = next;

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

static bool deep_validate_Player(Player *maybe_player)
{
    //TODO: this is done twice, it's stupid
    static PlayerData player_data;
    /* static Act act; */
    static Path path;
    static Room1 room1;
    static Room2 room2;
    static Level level;

    if (!maybe_player->pPlayerData
        || !memread((ptr_t)maybe_player->pPlayerData, sizeof(PlayerData),
                    find_PlayerData_callback, &player_data)) {
        /* LOG_WARNING("Can't validate PlayerData"); */
        return FALSE;
    }
    log_PlayerData(&player_data);
    log_Player(maybe_player);                          /* DEBUG */
    log_UnitAny(maybe_player);                          /* DEBUG */


    /* if (!maybe_player->pAct */
    /*     || !memread((ptr_t)maybe_player->pAct, sizeof(Act), */
    /*                 find_Act_callback, &act)) { */
    /*     LOG_WARNING("Can't validate Act"); */
    /*     return FALSE; */
    /* } */
    /* log_Act(&act); */

    if (!maybe_player->pPath
        || !memread((ptr_t)maybe_player->pPath, sizeof(Path),
                    find_Path_callback, &path)) {
        LOG_WARNING("Can't validate Path");
        return FALSE;
    }
    log_Path(&path);

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

static bool update_player(GameState *game, Player *player, bool need_full_search)
{
    static PlayerList *maybe_player_list = NULL;

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
    free_maybe_player(maybe_player_list);
    maybe_player_list = NULL;
    memreadall(FALSE, search_player_callback, &maybe_player_list);
    if (!maybe_player_list) {
        LOG_ERROR("Can't find any Player ptr");
        return FALSE;
    }

    UPDATE_STATUS(game, "In Game...");  // TODO: move that before
    PlayerList *pl;
    UPDATE_STATUS(game, "Validating Player...");
    for (pl = maybe_player_list; pl; pl = pl->pNext) {
        if (deep_validate_Player(&pl->player)) {
            LOG_DEBUG("YAY");
            /* continue;               /\* DEBUG *\/ */
            /* exit(EXIT_SUCCESS);     /\* DEBUG *\/ */
            break;
        }
    }
    if (!pl) {
        LOG_ERROR("Can't find any valid Player");
        /* exit(EXIT_FAILURE);     /\* DEBUG *\/ */
        return FALSE;
    }

    /* pthread_mutex_lock(&game->mutex); */
    game->_player_addr = pl->player_addr;  //TODO: remove from GameState
    /* pthread_mutex_unlock(&game->mutex); */

    memcpy(player, &pl->player, sizeof(Player));
    log_Player(player);
    LOG_INFO("Woop woop! Found Player ptr at %16jx [%d]",
             (ptr_t)pl->player_addr, pl->idx); /* DEBUG */
    return TRUE;
}

static bool update_game_window(GameState *game, bool *need_full_search)
{
    static pid_t prev_pid = 0;
    static bool need_pid_refresh = TRUE;
    static char prev_title_name[WINDOW_TITLE_MAX] = {0};

    if (strcmp(prev_title_name, game->window_title_setting)) {
        need_pid_refresh = TRUE;
        strcpy(prev_title_name, game->window_title_setting);
    }

    UPDATE_STATUS(game, "Searching for D2R...");
    pid_t pid = readmaps(*game->window_title_setting ?
                         game->window_title_setting : DEFAULT_D2R_WINDOW_TITLE,
                         need_pid_refresh);
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
    static PlayerContent pc = {0};
    static PlayerContent tmp = {0};
    static Player player = {0};

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

    /* UPDATE_STATUS(game, "Updating Act..."); */
    /* if (!memread((ptr_t)player.pAct, sizeof(Act), */
    /*              find_Act_callback, &tmp.act)) { */
    /*     LOG_ERROR("Can't find act"); */
    /*     memset(&tmp.act, 0, sizeof(Act)); */
    /*     /\* return FALSE; *\/ */
    /* } */
    /* log_Act(&tmp.act); */

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
    game->level = parse_level_list((ptr_t)tmp.room2.pLevel);
    tmp.room2.pLevel = game->level;
    memcpy(&pc, &tmp, sizeof(PlayerContent));
    memcpy(&game->player, &player, sizeof(Player));
    game->player.pPlayerData = &pc.player_data;
    /* game->player.pAct = &pc.act; */
    game->player.pPath = &pc.path;
    game->player.pPath->pRoom1 = &pc.room1;
    game->player.pPath->pRoom1->pRoom2 = &pc.room2;
    UPDATE_STATUS(game, "Fresh.");
    pthread_mutex_unlock(&game->mutex);

    return TRUE;
}
