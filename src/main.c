#include "smrf.h"

#define MAX_PLAYER_DATA 256

const char *AREAS[] = {
    "None",
    "Rogue Encampment",
    "Blood Moor",
    "Cold Plains",
    "Stony Field",
    "Dark Wood",
    "Black Marsh",
    "Tamoe Highland",
    "Den Of Evil",
    "Cave Level 1",
    "Underground Passage Level 1",
    "Hole Level 1",
    "Pit Level 1",
    "Cave Level 2",
    "Underground Passage Level 2",
    "Hole Level 2",
    "Pit Level 2",
    "Burial Grounds",
    "Crypt",
    "Mausoleum",
    "Forgotten Tower",
    "Tower Cellar Level 1",
    "Tower Cellar Level 2",
    "Tower Cellar Level 3",
    "Tower Cellar Level 4",
    "Tower Cellar Level 5",
    "Monastery Gate",
    "Outer Cloister",
    "Barracks",
    "Jail Level 1",
    "Jail Level 2",
    "Jail Level 3",
    "Inner Cloister",
    "Cathedral",
    "Catacombs Level 1",
    "Catacombs Level 2",
    "Catacombs Level 3",
    "Catacombs Level 4",
    "Tristram",
    "Moo Moo Farm",
    "Lut Gholein",
    "Rocky Waste",
    "Dry Hills",
    "Far Oasis",
    "Lost City",
    "Valley Of Snakes",
    "Canyon Of The Magi",
    "Sewers Level 1",
    "Sewers Level 2",
    "Sewers Level 3",
    "Harem Level 1",
    "Harem Level 2",
    "Palace Cellar Level 1",
    "Palace Cellar Level 2",
    "Palace Cellar Level 3",
    "Stony Tomb Level 1",
    "Halls Of The Dead Level 1",
    "Halls Of The Dead Level 2",
    "Claw Viper Temple Level 1",
    "Stony Tomb Level 2",
    "Halls Of The Dead Level 3",
    "Claw Viper Temple Level 2",
    "Maggot Lair Level 1",
    "Maggot Lair Level 2",
    "Maggot Lair Level 3",
    "Ancient Tunnels",
    "Tal Rashas Tomb #1",
    "Tal Rashas Tomb #2",
    "Tal Rashas Tomb #3",
    "Tal Rashas Tomb #4",
    "Tal Rashas Tomb #5",
    "Tal Rashas Tomb #6",
    "Tal Rashas Tomb #7",
    "Duriels Lair",
    "Arcane Sanctuary",
    "Kurast Docktown",
    "Spider Forest",
    "Great Marsh",
    "Flayer Jungle",
    "Lower Kurast",
    "Kurast Bazaar",
    "Upper Kurast",
    "Kurast Causeway",
    "Travincal",
    "Spider Cave",
    "Spider Cavern",
    "Swampy Pit Level 1",
    "Swampy Pit Level 2",
    "Flayer Dungeon Level 1",
    "Flayer Dungeon Level 2",
    "Swampy Pit Level 3",
    "Flayer Dungeon Level 3",
    "Sewers Level 1",
    "Sewers Level 2",
    "Ruined Temple",
    "Disused Fane",
    "Forgotten Reliquary",
    "Forgotten Temple",
    "Ruined Fane",
    "Disused Reliquary",
    "Durance Of Hate Level 1",
    "Durance Of Hate Level 2",
    "Durance Of Hate Level 3",
    "The Pandemonium Fortress",
    "Outer Steppes",
    "Plains Of Despair",
    "City Of The Damned",
    "River Of Flame",
    "Chaos Sanctuary",
    "Harrogath",
    "Bloody Foothills",
    "Frigid Highlands",
    "Arreat Plateau",
    "Crystalline Passage",
    "Frozen River",
    "Glacial Trail",
    "Drifter Cavern",
    "Frozen Tundra",
    "Ancient's Way",
    "Icy Cellar",
    "Arreat Summit",
    "Nihlathak's Temple",
    "Halls Of Anguish",
    "Halls Of Pain",
    "Halls Of Vaught",
    "Abaddon",
    "Pit Of Acheron",
    "Infernal Pit",
    "Worldstone Keep Level 1",
    "Worldstone Keep Level 2",
    "Worldstone Keep Level 3",
    "Throne Of Destruction",
    "The Worldstone Chamber",
    "Matron's Den",
    "Fogotten Sands",
    "Furnace of Pain",
    "Tristram"
};  //TODO: move that

Level *g_levels[0xff] = {0};


//TODO: add enum for callback return

static BOOL search_player_data_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    BYTE *b = buf;
    PlayerData *player_data;
    while (buf_len >= sizeof(PlayerData)) {
        player_data = (PlayerData *)b;
        if (is_valid_PlayerData(player_data)) {
            PTR here = address + (PTR)(b - buf);
/* #ifdef NDEBUG */
/*             LOG_DEBUG("Valid player_data! %16lx", here); */
/*             log_PlayerData(player_data); */
/* #endif */
            PTR *addr_list = (PTR *)data;
            while (*addr_list) {
                addr_list++;
            }
            *addr_list = here;
            /* return 2; */
            /* return FALSE; */
        }
        b += sizeof(PTR);
        buf_len -= sizeof(PTR);
    }

    return TRUE; // keep reading
}

static BOOL search_player_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{

    BYTE *b = buf;
    PTR *player_data_addr = (PTR *)data;
    PTR *p;
    Player *player;

    while (buf_len >= sizeof(Player)) {
        player = (Player *)b;
        for (p = player_data_addr; *p; p++) {
            if ((PTR)player->pPlayerData == *p && is_valid_Player(player)) {
                PTR here = address + (PTR)(b - buf);
                LOG_DEBUG("Valid player! %08lx", here);
                *(PTR *)data = here;
                memcpy(((PTR *)data) + 1, b, sizeof(Player));
                return FALSE;
            }
        }
        b += sizeof(PTR);
        buf_len -= sizeof(PTR);
    }

    return TRUE; // keep reading
}

static BOOL find_player_data_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    (void)buf_len, (void)address;
    if (!is_valid_PlayerData((PlayerData *)buf)) {
        LOG_WARNING("Invalid PlayerData %16lx", address);
        log_PlayerData((PlayerData *)buf);
        return TRUE;
    }
    memcpy(data, buf, sizeof(PlayerData));
    return FALSE;
}

static BOOL find_player_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    (void)buf_len, (void)address;
    if (!is_valid_Player((Player *)buf)) {
        LOG_WARNING("Invalid player %16lx", address);
        log_Player((Player *)buf);
        return TRUE;
    }
    memcpy(data, buf, sizeof(Player));
    return FALSE;
}

static BOOL find_act_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    (void)buf_len, (void)address;
    if (!is_valid_Act((Act *)buf)) {
        LOG_WARNING("Invalid act %16lx", address);
        log_Act((Act *)buf);
        return TRUE;
    }
    memcpy(data, buf, sizeof(Act));
    return FALSE;
}

static BOOL find_path_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    (void)buf_len, (void)address;
    if (!is_valid_Path((Path *)buf)) {
        LOG_WARNING("Invalid path %16lx", address);
        log_Path((Path *)buf);
        return TRUE;
    }
    memcpy(data, buf, sizeof(Path));
    return FALSE;
}

static BOOL find_room2_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    (void)buf_len, (void)address;
    if (!is_valid_Room2((Room2 *)buf)) {
        LOG_WARNING("Invalid room2 %16lx", address);
        log_Room2((Room2 *)buf);
        return TRUE;
    }
    memcpy(data, buf, sizeof(Room2));
    return FALSE;
}

static BOOL find_room1_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    (void)buf_len, (void)address;
    if (!is_valid_Room1((Room1 *)buf)) {
        LOG_WARNING("Invalid room1 %16lx", address);
        log_Room1((Room1 *)buf);
        return TRUE;
    }
    memcpy(data, buf, sizeof(Room1));
    return FALSE;
}

static BOOL find_collmap_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    (void)address;
    /* hex_dump(buf, sizeof(CollMap)); /\* DEBUG *\/ */
    if (!is_valid_CollMap((CollMap *)buf)) {
        LOG_WARNING("Invalid collmap %16lx", address);
        log_CollMap((CollMap *)buf);
        return TRUE;
    }
    memcpy(data, buf, buf_len);
    return FALSE;
}

static BOOL find_preset_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    (void)buf_len, (void)address;
    if (!is_valid_PresetUnit((PresetUnit *)buf)) {
        LOG_WARNING("Invalid preset %16lx", address);
        log_PresetUnit((PresetUnit *)buf);
        return TRUE;
    }
    memcpy(data, buf, sizeof(PresetUnit));
    return FALSE;
}


static BOOL find_level_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    (void)buf_len, (void)address;
    if (!is_valid_Level((Level *)buf)) {
        LOG_WARNING("Invalid level %16lx", address);
        log_Level((Level *)buf);
        return TRUE;
    }
    memcpy(data, buf, sizeof(Level));
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

static PresetUnit *parse_preset_list(pid_t pid, PTR pu_addr)
{
    PresetUnit pu;
    PresetUnit *pu_first = NULL, *pu_prev, *pu_new;
    int i = 0;                  /* DEBUG */

    while (is_valid_ptr(pu_addr)) {
        if (!memread(pid, pu_addr, sizeof(PresetUnit),
                     find_preset_callback, &pu)) {
            LOG_WARNING("Can't find preset");
            break;
        }
        i++;              /* DEBUG */
        /* log_PresetUnit(&pu); */
        /* LOG_DEBUG("(%x, %x) *", pu.dwPosX, pu.dwPosY); */

        DUPE(pu_new, &pu, sizeof(PresetUnit));
        pu_new->pPresetNext = NULL;

        if (!pu_first) {
            pu_first = pu_new;
        } else  {
            pu_prev->pPresetNext = pu_new;
        }
        pu_prev = pu_new;
        pu_addr = (PTR)pu.pPresetNext;
    }

    LOG_DEBUG("%d preset found", i);

    return pu_first;
}

static CollMap *parse_collmap(pid_t pid, PTR col_addr)
{
    CollMap col;

    if (!is_valid_ptr(col_addr)) {
        return NULL;
    }
    if (!memread(pid, col_addr, sizeof(CollMap),
                 find_collmap_callback, &col)) {
        LOG_WARNING("Can't find collmap");
        return NULL;
    }

    if ((PTR)col.pMapStart != col_addr + ((PTR)&col.coll - (PTR)&col)) {
        LOG_ERROR("collmap isn't contiguous");
        exit(EXIT_FAILURE);
    }

    size_t map_size = sizeof(DWORD) * col.dwSizeGameY * col.dwSizeGameX;
    CollMap *ret;
    MALLOC(ret, sizeof(CollMap) + map_size);

    if (!memread(pid, col_addr, sizeof(CollMap) + map_size,
                 find_collmap_callback, ret)) {
        LOG_WARNING("Can't find collmap.coll");
        return NULL;
    }

    return ret;
}

static Room1 *parse_room1_list(pid_t pid, PTR r1_addr)
{
    Room1 r1;
    Room1 *r1_first = NULL, *r1_prev, *r1_new;
    int i = 0;                  /* DEBUG */

    while (is_valid_ptr(r1_addr)) {
        if (!memread(pid, r1_addr, sizeof(Room1),
                     find_room1_callback, &r1)) {
            LOG_WARNING("Can't find room1");
            break;
        }
        i++;              /* DEBUG */
        /* log_Room1(&r1); */
        /* LOG_DEBUG("(%x, %x) *", r1.dwXStart, r1.dwYStart); */

        DUPE(r1_new, &r1, sizeof(Room1));
        r1_new->pRoomNext = NULL;
        r1_new->Coll = parse_collmap(pid, (PTR)r1.Coll);

        if (!r1_first) {
            r1_first = r1_new;
        } else  {
            r1_prev->pRoomNext = r1_new;
        }
        r1_prev = r1_new;
        r1_addr = (PTR)r1.pRoomNext;
    }

    LOG_DEBUG("%d room1 found", i);

    return r1_first;
}

static Room2 *parse_room2_list(pid_t pid, PTR r2_addr)
{
    Room2 r2;
    Room2 *r2_first = NULL, *r2_prev, *r2_new;
    int i = 0;                  /* DEBUG */

    while (is_valid_ptr(r2_addr)) {
        if (!memread(pid, r2_addr, sizeof(Room2),
                     find_room2_callback, &r2)) {
            LOG_WARNING("Can't find room2");
            break;
        }
        i++;              /* DEBUG */
        /* log_Room2(&r2); */
        /* LOG_DEBUG("(%x, %x)", r2.dwPosX, r2.dwPosY); */

        DUPE(r2_new, &r2, sizeof(Room2));
        r2_new->pRoom2Next = NULL;
        r2_new->pRoom1 = parse_room1_list(pid, (PTR)r2.pRoom1);
        r2_new->pPreset = parse_preset_list(pid, (PTR)r2.pPreset);

        if (!r2_first) {
            r2_first = r2_new;
        } else  {
            r2_prev->pRoom2Next = r2_new;
        }
        r2_prev = r2_new;
        r2_addr = (PTR)r2.pRoom2Next;
    }

    LOG_DEBUG("%d room2 found", i);

    return r2_first;
}

static void free_preset(PresetUnit *preset)
{
    if (!preset) {
        return;
    }
    free_preset(preset->pPresetNext);
    free(preset);
}

static void free_room1(Room1 *room1)
{
    if (!room1) {
        return;
    }
    free_room1(room1->pRoomNext);
    free(room1);
}

static void free_room2(Room2 *room2)
{
    if (!room2) {
        return;
    }
    free_room1(room2->pRoom1);
    free_preset(room2->pPreset);
    free_room2(room2->pRoom2Next);
    free(room2);
}

static void free_level(Level *level)
{
    if (!level) {
        return;
    }
    free_room2(level->pRoom2First);
    free(level);
}

static Level *parse_map(pid_t pid, Level *level)
{
    if (g_levels[level->dwLevelNo]) {
        /* return g_levels[level->dwLevelNo]; */
        free_level(g_levels[level->dwLevelNo]); //TODO: just update room1/preset
        g_levels[level->dwLevelNo] = NULL;
    }

    Level *new_level;
    DUPE(new_level, level, sizeof(Level));

    new_level->pRoom2First = parse_room2_list(pid, (PTR)level->pRoom2First);

    g_levels[level->dwLevelNo] = new_level;
    return new_level;
}

static BOOL init(GameState *game)
{
    game->pid = pidof("D2R.exe");
    if (!game->pid) {
        LOG_ERROR("Can't find D2R.exe");

        return FALSE;
    }

    if (!readmaps(game->pid)) {
        LOG_ERROR("Can't read maps");
        return FALSE;
    }

    if (game->player_addr) {
        if (memread(game->pid, (PTR)game->player_addr, sizeof(Player),
                    find_player_callback, &game->player)) {
            return TRUE;
        } else {
            LOG_ERROR("Can't refresh player");
        }
    }

    PTR player_data_addr[MAX_PLAYER_DATA] = {0};
    memreadall(game->pid, TRUE, search_player_data_callback, &player_data_addr);
    if (!*player_data_addr) {
        LOG_ERROR("Can't find PlayerData ptr");
        return FALSE;
    }

#ifdef NDEBUG
    int i;
    for (i = 0; i < MAX_PLAYER_DATA && player_data_addr[i]; i++) {}
    LOG_WARNING("player_data found: %d", i);
#endif

    if (!memreadall(game->pid, FALSE, search_player_callback, &player_data_addr)) {
        LOG_ERROR("Can't find Player ptr");
        return FALSE;
    }
    game->player_addr = *(PTR *)player_data_addr;
    memcpy(&game->player, ((PTR *)player_data_addr + 1), sizeof(Player));
    log_Player(&game->player);

    return TRUE;
}

static BOOL update(GameState *game)
{
    if (!memread(game->pid, (PTR)game->player.pPlayerData, sizeof(PlayerData),
                 find_player_data_callback, &game->player_data)) {
        LOG_ERROR("Can't find playerData");
        return FALSE;
    }
    log_PlayerData(&game->player_data);

    if (!memread(game->pid, (PTR)game->player.pPath, sizeof(Path),
                 find_path_callback, &game->path)) {
        LOG_ERROR("Can't find path");
        return FALSE;
    }
    log_Path(&game->path);

    if (!memread(game->pid, (PTR)game->player.pAct, sizeof(Act),
                 find_act_callback, &game->act)) {
        LOG_ERROR("Can't find act");
        return FALSE;
    }
    log_Act(&game->act);

    if (!memread(game->pid, (PTR)game->path.pRoom1, sizeof(Room1),
                 find_room1_callback, &game->room1)) {
        LOG_ERROR("Can't find room1");
        return FALSE;
    }
    log_Room1(&game->room1);

    if (!memread(game->pid, (PTR)game->room1.pRoom2, sizeof(Room2),
                 find_room2_callback, &game->room2)) {
        LOG_ERROR("Can't find room2");
        return FALSE;
    }
    log_Room2(&game->room2);

    Level level;
    if (!memread(game->pid, (PTR)game->room2.pLevel, sizeof(Level),
                 find_level_callback, &level)) {
        LOG_ERROR("Can't find level");
        return FALSE;
    }
    log_Level(&level);

    game->level = parse_map(game->pid, &level);

    LOG_INFO("{"
             "\"seed\": %d, "
             "\"x\": %d, "
             "\"y\": %d, "
             "\"area_name\": \"%s\", "
             "\"area\": (%d)"
             "}",
             game->act.dwMapSeed,
             game->path.xPos,
             game->path.yPos,
             AREAS[game->level->dwLevelNo],
             game->level->dwLevelNo);

    return TRUE;
}

GameState *refresh(void)
{
    static GameState game = {0};

    if (init(&game) && update(&game)) {
        return &game;
    }
    return NULL;
}

/* static BOOL main_loop(BOOL loop) */
/* { */
/*     GameState game = {0}; */
/*     BOOL successful_update; */

/*     do { */
/*         successful_update = init(&game) && update(&game); */
/*         if (loop) { */
/*             sleep(1); */
/*         } */
/*     } while (loop); */

/*     return successful_update; */
/* } */

/* static void usage(const char *exe) */
/* { */
/*     LOG_INFO("Usage: %s [OPTION]...\n" */
/*              "List information about the D2R game state.\n" */
/*              "\n" */
/*              "  -l, --loop    refresh info in an endless loop\n" */
/*              "      --help    display this help and exit", */
/*              exe); */
/* } */

/* int main(int argc, const char **argv) */
/* { */
/*     BOOL loop = FALSE; */

/*     for (const char *exe = *argv++; argc > 1 && *argv; argv++) { */
/*         if (!strcmp(*argv, "--loop") || !strcmp(*argv, "-l")) { */
/*             loop = TRUE; */
/*         } else if (!strcmp(*argv, "--help")) { */
/*             usage(exe); */
/*             return EXIT_SUCCESS; */
/*         } else { */
/*             usage(exe); */
/*             return EXIT_FAILURE; */
/*         } */
/*     } */

/*     if (!main_loop(loop)) { */
/*         return EXIT_FAILURE; */
/*     } */

/*     return EXIT_SUCCESS; */
/* } */
