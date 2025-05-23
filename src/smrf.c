#include "smrf.h"

#define MAX_PLAYER_DATA 0x1000

Level      *g_levels[MAX_AREA] = {0};
char       *g_player_name_setting = NULL;

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

////////////////////////////////////////////////////////////////////////////////

static void free_maybe_player(PlayerList *pl)
{
    if (!pl) {
        return;
    }
    free_maybe_player(pl->pNext);
    FREE(pl);
}

static void free_preset(PresetUnit *preset)
{
    if (!preset) {
        return;
    }
    free_preset(preset->pNext);
    FREE(preset);
}

static void free_room1(Room1 *room1)
{
    if (!room1) {
        return;
    }
    if (room1->Coll) {
        FREE(room1->Coll);
    }
    free_room1(room1->pNext);
    FREE(room1);
}

static void free_room2(Room2 *room2)
{
    if (!room2) {
        return;
    }
    free_room1(room2->pRoom1);
    free_preset(room2->pPreset);
    free_room2(room2->pNext);
    FREE(room2);
}

static void free_level(Level *level)
{
    if (!level) {
        return;
    }
    free_room2(level->pRoom2First);
    FREE(level);
}

static void free_all_levels(void)
{
    for (int i = 0; i < MAX_AREA; i++) {
        free_level(g_levels[i]);
        g_levels[i] = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////

static Room1 *room1_in_list(Room1 *r1, Room1 *r1_list)
{
    if (r1) {
        while (r1_list) {
            if (r1->dwPosX == r1_list->dwPosX
                && r1->dwPosY == r1_list->dwPosY
                && r1->dwSizeY == r1_list->dwSizeY
                && r1->dwSizeY == r1_list->dwSizeY
                && r1->dwPosXBig == r1_list->dwPosXBig
                && r1->dwPosYBig == r1_list->dwPosYBig
                && r1->dwSizeYBig == r1_list->dwSizeYBig
                && r1->dwSizeYBig == r1_list->dwSizeYBig) {
                return r1_list;
            }
            r1_list = r1_list->pNext;
        }
    }
    return NULL;
}

static Room2 *room2_in_list(Room2 *r2, Room2 *r2_list)
{
    if (r2) {
        while (r2_list) {
            if (r2->dwPosX == r2_list->dwPosX
                && r2->dwPosY == r2_list->dwPosY
                && r2->dwSizeY == r2_list->dwSizeY
                && r2->dwSizeY == r2_list->dwSizeY
                && r2->dwPresetType == r2_list->dwPresetType) {
                return r2_list;
            }
            r2_list = r2_list->pNext;
        }
    }
    return NULL;
}

static PresetUnit *preset_in_list(PresetUnit *pu, PresetUnit *pu_list)
{
    if (pu) {
        while (pu_list) {
            if (pu->dwPosX == pu_list->dwPosX
                && pu->dwPosY == pu_list->dwPosY
                && pu->dwType == pu_list->dwType
                && pu->dwTxtFileNo == pu_list->dwTxtFileNo) {
                return pu_list;
            }
            pu_list = pu_list->pNext;
        }
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////

static PresetUnit *parse_preset_list(pid_t pid, ptr_t pu_addr, PresetUnit *pu_first)
{
    PresetUnit pu;
    PresetUnit *pu_prev = NULL, *pu_new;
    /* int i = 0;                  /\* DEBUG *\/ */

    LAST_LINK(pu_first, pu_prev);
    while (is_valid_ptr(pu_addr)) {
        if (!memread(pid, pu_addr, sizeof(PresetUnit),
                     find_PresetUnit_callback, &pu)) {
            LOG_WARNING("Can't find preset");
            break;
        }
        /* i++;              /\* DEBUG *\/ */
        /* log_PresetUnit(&pu); */
        /* LOG_DEBUG("(%x, %x) *", pu.dwPosX, pu.dwPosY); */

        pu_new = preset_in_list(&pu, pu_first);
        if (!pu_new) {
            DUPE(pu_new, &pu, sizeof(PresetUnit));
            pu_new->pNext = NULL;
            ADD_LINK(pu_first, pu_prev, pu_new);
        }

        pu_addr = (ptr_t)pu.pNext;
    }

    /* LOG_DEBUG("%d preset found", i); */

    return pu_first;
}

static Room1 *parse_room1_list(pid_t pid, ptr_t r1_addr, Room1 *r1_first)
{
    Room1 r1;
    Room1 *r1_prev = NULL, *r1_new;
    /* int i = 0;                  /\* DEBUG *\/ */

    LAST_LINK(r1_first, r1_prev);
    while (is_valid_ptr(r1_addr)) {
        if (!memread(pid, r1_addr, sizeof(Room1),
                     find_Room1_callback, &r1)) {
            LOG_WARNING("Can't find room1");
            break;
        }
        /* i++;              /\* DEBUG *\/ */
        /* log_Room1(&r1); */
        /* DEBUG */

        /* ptr_t unit_addr = (ptr_t)r1.pUnitFirst; */
        /* if (!is_valid_ptr(unit_addr)) { */
        /*  if (unit_addr) { */
        /*      LOG_WARNING("Invalid unit addr :/"); */
        /*  } */
        /* } else { */
        /*     Player u1; */
        /*     if (!memread(pid, unit_addr, sizeof(UnitAny), */
        /*                  find_UnitAny_callback, &u1)) { */
        /*         LOG_WARNING("Can't find unit ):"); */
        /*     } else { */
        /*      /\* if (u1.dwType == 1) //monster *\/ */
        /*      log_Player(&u1); */
        /*      find_ptr(pid, (byte *)&u1, sizeof(Player), 0); */
        /*     } */


        /*     /\* LOG_WARNING("invalid pUnitFirst"); *\/ */
        /* } */

        /* DEBUG */
        /* LOG_DEBUG("(%x, %x) *", r1.dwXStart, r1.dwYStart); */


        r1_new = room1_in_list(&r1, r1_first);
        if (!r1_new) {
            DUPE(r1_new, &r1, sizeof(Room1));
            r1_new->pNext = NULL;
            r1_new->Coll = NULL;
            ADD_LINK(r1_first, r1_prev, r1_new);
        }

        /* r1_new->Coll = parse_collmap(pid, (ptr_t)r1.Coll); */

        r1_addr = (ptr_t)r1.pNext;
    }

    /* LOG_DEBUG("%d room1 found", i); */

    return r1_first;
}

static Room2 *parse_room2_list(pid_t pid, ptr_t r2_addr, Room2 *r2_first)
{
    Room2 r2;
    Room2 *r2_prev = NULL, *r2_new;
    /* int i = 0;                  /\* DEBUG *\/ */

    LAST_LINK(r2_first, r2_prev);
    while (is_valid_ptr(r2_addr)) {
        if (!memread(pid, r2_addr, sizeof(Room2),
                     find_Room2_callback, &r2)) {
            LOG_WARNING("Can't find room2");
            break;
        }
        /* i++;              /\* DEBUG *\/ */
        /* log_Room2(&r2); */
        /* LOG_DEBUG("(%x, %x)", r2.dwPosX, r2.dwPosY); */

        r2_new = room2_in_list(&r2, r2_first);
        if (!r2_new) {
            DUPE(r2_new, &r2, sizeof(Room2));
            r2_new->pNext = NULL;
            r2_new->pRoom1 = NULL;
            r2_new->pPreset = NULL;
            ADD_LINK(r2_first, r2_prev, r2_new);
        }

        r2_new->pRoom1 = parse_room1_list(pid,
                                          (ptr_t)r2.pRoom1,
                                          r2_new->pRoom1);
        r2_new->pPreset = parse_preset_list(pid,
                                            (ptr_t)r2.pPreset,
                                            r2_new->pPreset);

        r2_addr = (ptr_t)r2.pNext;
    }

    /* LOG_DEBUG("%d room2 found", i); */

    return r2_first;
}

static Level *parse_level_list(pid_t pid, ptr_t level_addr)
{
    Level level;
    Level *level_first = NULL, *level_prev, *level_new;
    /* int i = 0;                  /\* DEBUG *\/ */

    while (is_valid_ptr(level_addr)) {
        if (!memread(pid, level_addr, sizeof(Level),
                     find_Level_callback, &level)) {
            LOG_WARNING("Can't find level");
            break;
        }
        /* i++;              /\* DEBUG *\/ */
        /* log_Level(&level); */
        /* LOG_DEBUG("(%x, %x)", level.dwPosX, level.dwPosY); */

        level_new = g_levels[level.dwLevelNo];
        if (level_new) { // not new eh
            if (!level_first) { //current lvl
                level_new->pRoom2First = parse_room2_list(pid,
                                                          (ptr_t)level.pRoom2First,
                                                          level_new->pRoom2First);
            }
        } else {
            DUPE(level_new, &level, sizeof(Level));
            level_new->pNext = NULL;
            g_levels[level.dwLevelNo] = level_new;
            level_new->pRoom2First = parse_room2_list(pid,
                                                      (ptr_t)level.pRoom2First,
                                                      NULL);
        }

        ADD_LINK(level_first, level_prev, level_new);
        level_addr = (ptr_t)level.pNext;
    }

    /* LOG_DEBUG("%d level found", i); */

    return level_first;
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
    /*     //TODO: handle this the smart way */
    /*     // -> don't go to the 1st player of the list :/ */
    /*     LOG_ERROR("Can't find act"); */
    /*     memset(&tmp.act, 0, sizeof(Act)); */
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


    /* if (memread(game->_pid, (ptr_t)player.pAct, sizeof(Act), */
    /*              find_Act_callback, &tmp.act)) { */
    /*     LOG_INFO("Found act: basic"); */
    /* } else if (memread(game->_pid, (ptr_t)player.dwSeed[0], sizeof(Act), */
    /*                    find_Act_callback, &tmp.act)) { */
    /*     LOG_INFO("Found act: seed 0"); */
    /* } else if (memread(game->_pid, (ptr_t)player.dwSeed[1], sizeof(Act), */
    /*                    find_Act_callback, &tmp.act)) { */
    /*     LOG_INFO("Found act: seed 1"); */
    /* } else if (memread(game->_pid, (ptr_t)player._dunno1[0], sizeof(Act), */
    /*                    find_Act_callback, &tmp.act)) { */
    /*     LOG_INFO("Found act: dunno 0"); */
    /* } else if (memread(game->_pid, (ptr_t)player._dunno1[0], sizeof(Act), */
    /*                    find_Act_callback, &tmp.act)) { */
    /*     LOG_INFO("Found act: dunno 1"); */
    /* } else { */
    /*     LOG_ERROR("Can't find act"); */
    /*  memset(&tmp.act, 0, sizeof(Act)); */
    /* } */
    /* log_Act(&tmp.act); */



    /* if (memread(game->_pid, (ptr_t)player.pPath, sizeof(Path), */
    /*              find_Path_callback, &tmp.path)) { */
    /*     LOG_INFO("Found path: basic"); */
    /* } else if (memread(game->_pid, (ptr_t)player.dwSeed[0], sizeof(Path), */
    /*                    find_Path_callback, &tmp.path)) { */
    /*     LOG_INFO("Found path: seed 0"); */
    /* } else if (memread(game->_pid, (ptr_t)player.dwSeed[1], sizeof(Path), */
    /*                    find_Path_callback, &tmp.path)) { */
    /*     LOG_INFO("Found path: seed 1"); */
    /* } else if (memread(game->_pid, (ptr_t)player._dunno1[0], sizeof(Path), */
    /*                    find_Path_callback, &tmp.path)) { */
    /*     LOG_INFO("Found path: dunno 0"); */
    /* } else if (memread(game->_pid, (ptr_t)player._dunno1[0], sizeof(Path), */
    /*                    find_Path_callback, &tmp.path)) { */
    /*     LOG_INFO("Found path: dunno 1"); */
    /* } else { */
    /*     LOG_ERROR("Can't find path"); */
    /*     return FALSE; */
    /* } */
    /* log_Path(&tmp.path); */



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
    pthread_mutex_unlock(&game->mutex);


    /* DEBUG */
    /* log_Act(&tmp.act); */
    /* ActMisc misc; */
    /* if (!memread(game->_pid, (ptr_t)tmp.act.pMisc, sizeof(ActMisc), */
    /*              find_ActMisc_callback, &misc)) { */
    /*     LOG_ERROR("Can't find actmisc 1"); */
    /* } */
    /* log_ActMisc(&misc); */

    /* if (!memread(game->_pid, (ptr_t)game->level->pMisc, sizeof(ActMisc), */
    /*              find_ActMisc_callback, &misc)) { */
    /*     LOG_ERROR("Can't find actmisc 2"); */
    /* } */
    /* log_ActMisc(&misc); */

    /* DEBUG */

    strcpy(game->status, "Fresh.");
    return TRUE;
}

void init_game_state(GameState *game)
{
    memset(game, 0, sizeof(GameState));
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
