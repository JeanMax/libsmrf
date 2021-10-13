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
            return 2;
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

static BOOL find_seed_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    (void)buf_len, (void)address;
    memcpy(data, buf, sizeof(Act));
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
        /* return EXIT_FAILURE; */
    }
    /* LOG_INFO("pPlayerData: %08lx", player_data_addr[0]); */

    /* DEBUG */
    int i;
    for (i = 0; i < MAX_PLAYER_DATA && player_data_addr[i]; i++) {}
    LOG_WARNING("player_data found: %d", i);
    /* DEBUG */

    if (!memreadall(pid, FALSE, find_player_callback, &player_data_addr)) {
        LOG_ERROR("Can't find Player ptr");
        return EXIT_FAILURE;
    }
    PTR player_addr = *(PTR *)player_data_addr;
    Player player;
    memcpy(&player, ((PTR *)player_data_addr + 1), sizeof(Player));
    LOG_INFO("pPlayer: %08lx", player_addr);
    /* log_Player(&player); */


    Act act;
    if (!memread(pid,
                 (PTR)player.pAct + sizeof(PTR),
                 sizeof(Act),
                 find_seed_callback,
                 &act)) {
        LOG_ERROR("Can't find seed");
        return EXIT_FAILURE;
    }
    log_Act(&act);

    return EXIT_SUCCESS;
}
