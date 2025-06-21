#include "smrf.h"

#include "smrf/proc.h"
#include "smrf/util/log.h"

#define DEFAULT_D2R_WINDOW_TITLE "Diablo II: Resurrected"
#define WINDOW_TITLE_OR_DEFAULT(game)                                          \
  (*game->window_title_setting ? game->window_title_setting                    \
                               : DEFAULT_D2R_WINDOW_TITLE)


void init_game_state(GameState *game)
{
    memset(game, 0, sizeof(GameState));
    UPDATE_STATUS(game, "Loading...");
    g_unit_table = hnew(0, delete_unit_callback);
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
    /* pthread_mutex_lock(&game->mutex); */
    free_all_levels();
    hdelall(g_unit_table);
    game->all_levels = NULL;
    game->level = NULL;
    game->player = NULL;
    /* game->info = NULL; */
    /* pthread_mutex_unlock(&game->mutex); */
}

////////////////////////////////////////////////////////////////////////////////

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

static void *search_game_info_callback(byte *buf, size_t buf_len, ptr_t address, void *data)
{
    (void)data;
    byte *b = buf;
    GameInfo *g;

    while (buf_len >= sizeof(GameInfo)) {
        g = (GameInfo *)b;
        if (is_valid_GameInfo(g)) {
            ptr_t here = address + (ptr_t)(b - buf);
            LOG_INFO("found GameInfo at %16jx", here); /* DEBUG */
            return (void *)here;
        }

        b += sizeof(byte);
        buf_len -= sizeof(byte);
    }

    return NULL; // keep reading
}

////////////////////////////////////////////////////////////////////////////////

static void *search_player_callback(byte *buf, size_t buf_len, ptr_t address, void *data)
{
    (void)data;
    byte *b = buf;
    UnitAny u;

    while (buf_len >= sizeof(ptr_t)) {
        Player *maybe_player = *(Player **)b;

        if (is_valid_ptr((ptr_t)maybe_player)
                && memread((ptr_t)maybe_player, sizeof(Player),
                            find_Player_callback, &u)
                && deep_validate_Player(&u)) {
            ptr_t here = address + (ptr_t)(b - buf);
            LOG_INFO("found Player at %16jx - %16jx", here, maybe_player); /* DEBUG */
            /* return (void *)here; */
            return (void *)maybe_player;
        }

        b += sizeof(ptr_t);
        buf_len -= sizeof(ptr_t);
    }

    return NULL; // keep reading
}

static Player *find_another_player_ptr(GameState *game)
{
    void *player_addr = memreadallbase(search_player_callback, NULL);
    if (!player_addr) {
        LOG_ERROR("Couldn't find another Player ptr, damn.");
        game->_ut_addr = 0;
        return NULL;
    }

    LOG_INFO("Wop wooop woop, another Player ptr found!");
    parse_unit_list((ptr_t)player_addr);
    Player p;
    if (memread((ptr_t)player_addr, sizeof(Player),
                find_Player_callback, &p)) {
        UnitWithAddr *uwa = hget(g_unit_table, p.dwUnitId);
        if (uwa) {  // alternative player adress
            return &uwa->unit;
        }
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////

static Player *update_player(GameState *game, bool need_full_search)
{
    UPDATE_STATUS(game, "Out of game...");

    if (need_full_search || !game->_ut_addr) {
        void *ut_addr = memreadallbase(search_unit_table_callback, NULL);
        if (!ut_addr) {
            LOG_ERROR("Can't find UnitHashTable ptr");
            return NULL;
        }
        game->_ut_addr = (ptr_t)ut_addr;
        LOG_INFO("Woop woop! Found UnitHashTable ptr"); /* DEBUG */
    }

    dword ut;
    if (!memread((ptr_t)game->_ut_addr, sizeof(dword),
                 find_dword_callback, &ut)) {
        LOG_ERROR("Can't read UnitHashTable ptr");
        game->_ut_addr = 0;
        return NULL;
    }

    Player *player = parse_unit_table(ut);

    if (!player && !game->player) {
        UPDATE_STATUS(game, "Weird game, trying hack...");
        LOG_ERROR("Can't find player...");
        player = find_another_player_ptr(game);
        if (!player) {
            LOG_ERROR("Hack failed, damn.");
            game->_ut_addr = 0;
            return NULL;
        }
    }

    if (player) {
        LOG_INFO("Woop woop! Found Player ptr");
        return player;
    }
    if (game->player) {  // re-using previous Player ptr
        return game->player;
    }

    LOG_ERROR("No Player ptr, (this shouldn't happen)"); /* DEBUG */
    return FALSE;
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
    if (!update_game_window(game, &need_full_search)) {
        pthread_mutex_lock(&game->mutex);
        reset_game_state(game);
        pthread_mutex_unlock(&game->mutex);
        return FALSE;
    }

    if (need_full_search || !game->_gi_addr) {
        void *gi_addr = memreadallbase(search_game_info_callback, NULL);
        if (!gi_addr) {
            LOG_ERROR("Can't find GameInfo");
            pthread_mutex_lock(&game->mutex);
            reset_game_state(game);
            game->_gi_addr = 0;
            pthread_mutex_unlock(&game->mutex);
            return FALSE;
        }
        game->_gi_addr = (ptr_t)gi_addr;
        LOG_INFO("Woop woop! Found GameInfo %16jx", gi_addr); /* DEBUG */
    }

    GameInfo info;
    if (!memread(game->_gi_addr, sizeof(GameInfo),
                 find_GameInfo_callback, &info)) {
        LOG_ERROR("Can't update GameInfo");
        pthread_mutex_lock(&game->mutex);
        reset_game_state(game);
        game->_gi_addr = 0;
        pthread_mutex_unlock(&game->mutex);
        return FALSE;
    }
    log_GameInfo(&info);

    if (!info.inGame) {
        reset_game_state(game);
        pthread_mutex_unlock(&game->mutex);
        return FALSE;
    }
    g_is_expansion = info.expansionInfo == GAME_FLAG_ON;

    pthread_mutex_lock(&game->mutex);
    Player *player = update_player(game, need_full_search);
    if (!player) {
        reset_game_state(game);
        pthread_mutex_unlock(&game->mutex);
        return FALSE;
    }

    dword uid = player->dwUnitId;

    UPDATE_STATUS(game, "Updating Units...");
    LOG_DEBUG("before: %d units in table", g_unit_table->length);
    hiter(g_unit_table, update_unit_callback, NULL);
    LOG_DEBUG("after: %d units in table", g_unit_table->length);

    UnitWithAddr *uwa = hget(g_unit_table, uid);
    if (!uwa) {
        LOG_WARNING("Player lost, searching another ptr...");
        if (!find_another_player_ptr(game)
            || !(uwa = hget(g_unit_table, uid))) {
            LOG_INFO("Player lost, assuming Out of Game");
            UPDATE_STATUS(game, "Out of game...");
            reset_game_state(game);
            pthread_mutex_unlock(&game->mutex);
            return FALSE;
        }
    }
    player = &uwa->unit;

    if (!memread((ptr_t)player->pPath->pRoom1, sizeof(Room1),
                 find_Room1_callback, &room1)) {
        LOG_ERROR("Can't find room1");
        reset_game_state(game);
        pthread_mutex_unlock(&game->mutex);
        return FALSE;
    }
    /* log_Room1(&tmp.room1); */

    if (!memread((ptr_t)room1.pRoom2, sizeof(Room2),
                 find_Room2_callback, &room2)) {
        LOG_ERROR("Can't find room2");
        reset_game_state(game);
        pthread_mutex_unlock(&game->mutex);
        return FALSE;
    }
    /* log_Room2(&tmp.room2); */


    game->player = &uwa->unit;
    /* game->info = &info; */
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
