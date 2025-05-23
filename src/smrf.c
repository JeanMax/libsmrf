#include "smrf.h"

#include "smrf/parse.h"
#include "smrf/proc.h"
#include "smrf/util/list.h"
#include "smrf/util/log.h"


char       *g_player_name_setting = NULL;  //TODO: remove, probably useless now

//TODO: add enum for callback return

inline static bool is_valid_name(const char *name)
{
    if (!g_player_name_setting || !*g_player_name_setting) {
        return is_valid_player_name_str(name, PLAYER_DATA_NAME_MAX);
    }
    return !strcmp(name, g_player_name_setting);
}

inline static bool search_player_callback(byte *buf, size_t buf_len, ptr_t address, void *data)
{
#ifdef NDEBUG
    static int i = 0;
#endif
    byte *b = buf;
    Player *player;
    PlayerList **maybe_player_list = (PlayerList **)data;

    while (buf_len >= sizeof(Player)) {
        player = (Player *)b;
        if (is_valid_Player(player)) {
            ptr_t here = address + (ptr_t)(b - buf);
            LOG_DEBUG("found maybe-Player ptr at %16jx [%d]", here, i++); /* DEBUG */
            log_Player(player);                          /* DEBUG */

            PlayerList *pl;
            MALLOC(pl, sizeof(PlayerList));
            memcpy(&pl->player, b, sizeof(Player));
            pl->player_addr = here;
            PUSH_LINK(*maybe_player_list, pl);
        }
        b += sizeof(ptr_t);
        buf_len -= sizeof(ptr_t);
    }

    return TRUE; // keep reading
}

static void free_maybe_player(PlayerList *ptr)
{
    PlayerList *prev = NULL;
    while (ptr) {
        prev = ptr;
        ptr = ptr->pNext;
        FREE(prev);
    }
}

////////////////////////////////////////////////////////////////////////////////

static bool deep_validate_Player(pid_t pid, Player *maybe_player)
{
    static PlayerData player_data;
    /* static Act act; */
    static Path path;
    static Room1 room1;
    static Room2 room2;
    static Level level;

    if (!maybe_player->pPlayerData
        || !memread(pid, (ptr_t)maybe_player->pPlayerData, sizeof(PlayerData),
                    find_PlayerData_callback, &player_data)
        || !is_valid_name(player_data.szName)) {
        /* LOG_WARNING("Can't validate PlayerData"); */
        return FALSE;
    }
    log_PlayerData(&player_data);
    log_Player(maybe_player);                          /* DEBUG */
    log_UnitAny(maybe_player);                          /* DEBUG */


    /* if (!maybe_player->pAct */
    /*     || !memread(pid, (ptr_t)maybe_player->pAct, sizeof(Act), */
    /*                 find_Act_callback, &act)) { */
    /*     LOG_WARNING("Can't validate Act"); */
    /*     return FALSE; */
    /* } */
    /* log_Act(&act); */

    if (!maybe_player->pPath
        || !memread(pid, (ptr_t)maybe_player->pPath, sizeof(Path),
                    find_Path_callback, &path)) {
        LOG_WARNING("Can't validate Path");
        return FALSE;
    }
    /* log_Path(&path); */

    if (!path.pRoom1
        || !memread(pid, (ptr_t)path.pRoom1, sizeof(Room1),
                    find_Room1_callback, &room1)) {
        LOG_WARNING("Can't validate Room1");
        return FALSE;
    }
    log_Room1(&room1);

    if (!room1.pRoom2
        || !memread(pid, (ptr_t)room1.pRoom2, sizeof(Room2),
                    find_Room2_callback, &room2)) {
        LOG_WARNING("Can't validate Room2");
        return FALSE;
    }
    log_Room2(&room2);

    if (!room2.pLevel
        || !memread(pid, (ptr_t)room2.pLevel, sizeof(Level),
                    find_Level_callback, &level)) {
        LOG_WARNING("Can't validate Level");
        return FALSE;
    }
    log_Level(&level);

    if (!level.pRoom2First
        || !memread(pid, (ptr_t)level.pRoom2First, sizeof(Room2),
                    find_Room2_callback, &room2)) {
        LOG_WARNING("Can't validate Room2First");
        return FALSE;
    }
    log_Room2(&room2);

    return TRUE;
}

static bool update_player(GameState *game, Player *player)
{
    static PlayerList *maybe_player_list = NULL;

    UPDATE_STATUS(game, "Searching for D2R.exe...");
    pid_t pid = pidof("D2R.exe");
    if (!pid) {
        LOG_ERROR("Can't find D2R.exe");
        return FALSE;
    }

    UPDATE_STATUS(game, "Reading D2R memory...");
    if (!readmaps(pid)) {
        LOG_ERROR("Can't read maps");
        return FALSE;
    }

    if (pid == game->_pid && game->_player_addr) {
        UPDATE_STATUS(game, "Updating Player...");
        if (memread(pid, (ptr_t)game->_player_addr, sizeof(Player),
                    find_Player_callback, player)) {
            return TRUE;
        } else {
            LOG_ERROR("Can't refresh player");
            return FALSE;       /* DEBUG */
        }
    }

    UPDATE_STATUS(game, "Out of game...");
    free_maybe_player(maybe_player_list);
    maybe_player_list = NULL;
    memreadall(pid, FALSE, search_player_callback, &maybe_player_list);
    if (!maybe_player_list) {
        LOG_ERROR("Can't find any Player ptr");
        return FALSE;
    }

    UPDATE_STATUS(game, "In Game...");
    PlayerList *pl;
    UPDATE_STATUS(game, "Validating Player...");
    int i = 0;
    for (pl = maybe_player_list; pl; pl = pl->pNext, i++) {
        if (deep_validate_Player(pid, &pl->player)) {
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
    game->_pid = pid;
    game->_player_addr = pl->player_addr;
    /* pthread_mutex_unlock(&game->mutex); */

    memcpy(player, &pl->player, sizeof(Player));
    log_Player(player);
    LOG_INFO("Woop woop! Found Player ptr at %16jx [%d]", (ptr_t)pl->player_addr, i); /* DEBUG */
    return TRUE;
}

bool update_game_state(GameState *game)
{
    static PlayerContent pc = {0};
    static PlayerContent tmp = {0};
    static Player player = {0};

    if (!update_player(game, &player)) {
        destroy_game_state(game);
        return FALSE;
    }

    UPDATE_STATUS(game, "Updating PlayerData...");
    if (!memread(game->_pid, (ptr_t)player.pPlayerData, sizeof(PlayerData),
                 find_PlayerData_callback, &tmp.player_data)) {
        LOG_ERROR("Can't find playerData");
        destroy_game_state(game);
        return FALSE;
    }
    log_PlayerData(&tmp.player_data);

    /* UPDATE_STATUS(game, "Updating Act..."); */
    /* if (!memread(game->_pid, (ptr_t)player.pAct, sizeof(Act), */
    /*              find_Act_callback, &tmp.act)) { */
    /*     LOG_ERROR("Can't find act"); */
    /*     bzero(&tmp.act, sizeof(Act)); */
    /*     /\* return FALSE; *\/ */
    /* } */
    /* log_Act(&tmp.act); */

    UPDATE_STATUS(game, "Updating Path...");
    if (!memread(game->_pid, (ptr_t)player.pPath, sizeof(Path),
                 find_Path_callback, &tmp.path)) {
        //TODO: handle this the smart way
        // -> don't go to the 1st player of the list :/
        LOG_ERROR("Can't find path");
        return FALSE;
    }
    log_Path(&tmp.path);

    UPDATE_STATUS(game, "Updating Room1...");
    if (!memread(game->_pid, (ptr_t)tmp.path.pRoom1, sizeof(Room1),
                 find_Room1_callback, &tmp.room1)) {
        LOG_ERROR("Can't find room1");
        return FALSE;
    }
    /* log_Room1(&tmp.room1); */

    UPDATE_STATUS(game, "Updating Room2...");
    if (!memread(game->_pid, (ptr_t)tmp.room1.pRoom2, sizeof(Room2),
                 find_Room2_callback, &tmp.room2)) {
        LOG_ERROR("Can't find room2");
        return FALSE;
    }
    /* log_Room2(&tmp.room2); */

    pthread_mutex_lock(&game->mutex);
    game->all_levels = g_levels;
    game->level = parse_level_list(game->_pid, (ptr_t)tmp.room2.pLevel);
    tmp.room2.pLevel = game->level;
    memcpy(&pc, &tmp, sizeof(PlayerContent));
    memcpy(&game->player, &player, sizeof(Player));
    game->player.pPlayerData = &pc.player_data;
    /* game->player.pAct = &pc.act; */
    game->player.pPath = &pc.path;
    game->player.pPath->pRoom1 = &pc.room1;
    game->player.pPath->pRoom1->pRoom2 = &pc.room2;
    strcpy(game->status, "Fresh.");
    pthread_mutex_unlock(&game->mutex);

    return TRUE;
}

void init_game_state(GameState *game)
{
    bzero(game, sizeof(GameState));
    g_player_name_setting = game->player_name_setting;
    strcpy(game->status, "Loading...");
    pthread_mutex_init(&game->mutex, NULL);
}

void destroy_game_state(GameState *game)
{
    pthread_mutex_lock(&game->mutex);
    free_all_levels();
    game->level = NULL;
    pthread_mutex_unlock(&game->mutex);
    pthread_mutex_destroy(&game->mutex);
    memset((char *)game + PLAYER_DATA_NAME_MAX,
           0, sizeof(GameState) - PLAYER_DATA_NAME_MAX);
}
