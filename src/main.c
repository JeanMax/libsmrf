#include "seed.h"

#define MAX_PLAYER_DATA 256


static BOOL find_player_data_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    BYTE *b = buf;
    PlayerData *player_data;
    while (buf_len >= sizeof(PlayerData)) {
        player_data = (PlayerData *)b;
        if (is_valid_PlayerData(player_data)) {
            PTR here = address + (PTR)(b - buf);
#ifdef DEBUG_MODE
            LOG_DEBUG("Valid player_data! %16lx", here);
            log_PlayerData(player_data);
#endif
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

static BOOL find_player_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
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
                log_Player(player);
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


int main(void)
{
    pid_t pid = pidof("D2R.exe");
    if (!pid) {
        LOG_ERROR("Can't find D2R.exe");
        return EXIT_FAILURE;
    }

    if (!readmaps(pid)) {
        LOG_ERROR("Can't read maps");
        return EXIT_FAILURE;
    }

    PTR player_data_addr[MAX_PLAYER_DATA] = {0};
    memreadall(pid, TRUE, find_player_data_callback, &player_data_addr);
    if (!player_data_addr[0]) {
        LOG_ERROR("Can't find PlayerData ptr");
        return EXIT_FAILURE;
    }

#ifdef DEBUG_MODE
    int i;
    for (i = 0; i < MAX_PLAYER_DATA && player_data_addr[i]; i++) {}
    LOG_WARNING("player_data found: %d", i);
#endif

    if (!memreadall(pid, FALSE, find_player_callback, &player_data_addr)) {
        LOG_ERROR("Can't find Player ptr");
        return EXIT_FAILURE;
    }
    PTR player_addr = *(PTR *)player_data_addr;
    Player player;
    memcpy(&player, ((PTR *)player_data_addr + 1), sizeof(Player));
    LOG_INFO("pPlayer: %08lx", player_addr);
    /* log_Player(&player); */

    Path path;
    if (!memread(pid,
                 (PTR)player.pPath,
                 sizeof(Path),
                 find_path_callback,
                 &path)) {
        LOG_ERROR("Can't find path");
        return EXIT_FAILURE;
    }
    log_Path(&path);

    Act act;
    if (!memread(pid,
                 (PTR)player.pAct,
                 sizeof(Act),
                 find_act_callback,
                 &act)) {
        LOG_ERROR("Can't find act");
        return EXIT_FAILURE;
    }
    log_Act(&act);

    Room1 room1;
    if (!memread(pid,
                 (PTR)act.pRoom1,
                 sizeof(Room1),
                 find_room1_callback,
                 &room1)) {
        LOG_ERROR("Can't find room1");
        return EXIT_FAILURE;
    }
    log_Room1(&room1);

    Room2 room2;
    if (!memread(pid,
                 (PTR)room1.pRoom2,
                 sizeof(Room2),
                 find_room2_callback,
                 &room2)) {
        LOG_ERROR("Can't find room2");
        return EXIT_FAILURE;
    }
    log_Room2(&room2);

    BYTE *b = (BYTE *)&room2 + 72;
    Level level;
    LOG_DEBUG("lvl ptr at %16lx", *(PTR *)b);
    if (memread(pid,
                *(PTR *)b,
                sizeof(Level),
                find_level_callback,
                &level)) {
        LOG_ERROR("WOOP WOOP");
                /* return EXIT_FAILURE; */
    }

    /* Level level; */
    /* if (!memread(pid, */
    /*              (PTR)room2.pLevel, */
    /*              sizeof(Level), */
    /*              find_level_callback, */
    /*              &level)) { */
    /*     LOG_ERROR("Can't find level"); */
    /*     return EXIT_FAILURE; */
    /* } */
    log_Level(&level);


    return EXIT_SUCCESS;
}
