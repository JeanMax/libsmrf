#include "seed.h"

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

    if (!memread(game->pid, (PTR)game->room2.pLevel, sizeof(Level),
                 find_level_callback, &game->level)) {
        LOG_ERROR("Can't find level");
        return FALSE;
    }
    log_Level(&game->level);

    /* Level level_bis; */
    /* memcpy(&level_bis, &game->level, sizeof(Level)); */
    /* while (level_bis.pNextLevel) { */
    /*     if (!memread(game->pid, (PTR)level_bis.pNextLevel, sizeof(Level), */
    /*                  find_level_callback, &level_bis)) { */
    /*         LOG_WARNING("Can't find level_bis"); */
    /*            //TODO: some of the level_bis.pNextLevel are invalid?! */
    /*         break; */
    /*     } */

    /*     log_Level(&level_bis); */
    /*     if (game->room2.dwPosX + game->room2.dwSizeX >= level_bis.dwPosX */
    /*         && game->room2.dwPosX <= level_bis.dwPosX + level_bis.dwSizeX */

    /*         && game->room2.dwPosY + game->room2.dwSizeY >= level_bis.dwPosY */
    /*         && game->room2.dwPosY <= level_bis.dwPosY + level_bis.dwSizeY) { */

    /*         LOG_DEBUG("Found alternative area: old: %u - new: %u", */
    /*                   game->level.dwLevelNo, level_bis.dwLevelNo); */
    /*         memcpy(&game->level, &level_bis, sizeof(Level)); */
    /*         break; */
    /*     } */
    /* } */



    Room2 lvl_room2;
    if (!memread(game->pid, (PTR)game->level.pRoom2First, sizeof(Room2),
                 find_room2_callback, &lvl_room2)) {
        LOG_ERROR("Can't find lvl.room2");
        return FALSE;
    }
    log_Room2(&lvl_room2);
    LOG_INFO("(%x, %x) *", lvl_room2.dwPosX, lvl_room2.dwPosY);


    // iterate room2.next
    Room1 r1;
    int i = 0;
    while ((PTR)lvl_room2.pRoom2Next
           && is_valid_ptr((PTR)lvl_room2.pRoom2Next)) {
        if (!memread(game->pid, (PTR)lvl_room2.pRoom2Next, sizeof(Room2),
                     find_room2_callback, &lvl_room2)) {
            LOG_WARNING("Can't find lvl_room2.next");
            break;
        }
        i++;
        /* log_Room2(&lvl_room2); */
        LOG_INFO("(%x, %x)", lvl_room2.dwPosX, lvl_room2.dwPosY);


        if (!memread(game->pid, (PTR)lvl_room2.pRoom1, sizeof(Room1),
                     find_room1_callback, &r1)) {
            LOG_WARNING("Can't find lvl_room2.room1");
            continue;
        }
        LOG_DEBUG("(%x, %x) *", r1.dwXStart, r1.dwYStart);

        int j = 0;
        while ((PTR)r1.pRoomNext
               && is_valid_ptr((PTR)r1.pRoomNext)) {
            if (!memread(game->pid, (PTR)r1.pRoomNext, sizeof(Room1),
                         find_room1_callback, &r1)) {
                LOG_WARNING("Can't find room1.next");
                break;
            }
            j++;
            /* log_Room1(&r1); */
            LOG_DEBUG("(%x, %x)", r1.dwXStart, r1.dwYStart);
        }
        LOG_DEBUG("%d room1 found", j);

    }
    LOG_INFO("%d room2 found", i);


    /* LOG_INFO("SEED: %d", game->act.dwMapSeed); */
    /* LOG_INFO("COORD: %d %d", game->path.xPos, game->path.yPos); */
    /* LOG_INFO("AREA: %d", game->level.dwLevelNo); */
    LOG_INFO("{"
             /* "\"player_name\": \"%s\", " */
             "\"seed\": %d, "
             "\"x\": %d, "
             "\"y\": %d, "
             "\"area_name\": \"%s\", "
             "\"area\": (%d)"
             "}",
             /* game->player_data.szName, */
             game->act.dwMapSeed,
             game->path.xPos,
             game->path.yPos,
             AREAS[game->level.dwLevelNo],
             game->level.dwLevelNo);

    return TRUE;
}

static BOOL main_loop(BOOL loop)
{
    GameState game = {0};
    BOOL successful_update;

    do {
        successful_update = init(&game) && update(&game);
        if (loop) {
            sleep(1);
        }
    } while (loop);

    return successful_update;
}



static void usage(const char *exe)
{
    LOG_INFO("Usage: %s [OPTION]...\n"
             "List information about the D2R game state.\n"
             "\n"
             "  -l, --loop    refresh info in an endless loop\n"
             "      --help    display this help and exit",
             exe);
}

int main(int argc, const char **argv)
{
    BOOL loop = FALSE;

    for (const char *exe = *argv++; argc > 1 && *argv; argv++) {
        if (!strcmp(*argv, "--loop") || !strcmp(*argv, "-l")) {
            loop = TRUE;
        } else if (!strcmp(*argv, "--help")) {
            usage(exe);
            return EXIT_SUCCESS;
        } else {
            usage(exe);
            return EXIT_FAILURE;
        }
    }

    if (!main_loop(loop)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
