#include "seed.h"

#define PLAYER_NAME "TrapSmurf"
/* #define PLAYER_NAME "Smurfette" */
/* #define PLAYER_NAME "Jeanette" */
/* #define PLAYER_NAME "_online" */

static BOOL find_player_data_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    BYTE *match;

    match = memsearch((char *)buf, PLAYER_NAME, buf_len, strlen(PLAYER_NAME));
    if (match) {
        PTR here = address + (PTR)(match - buf);
        LOG_DEBUG("");
        LOG_DEBUG("Smurf match: %16lx", here); /* DEBUG */
        log_data(match - 16, 256);
        if (is_valid_PlayerData((PlayerData *)match)) {
            LOG_DEBUG("Valid player_data! %08lx", here);
            log_PlayerData((PlayerData *)match);
            PTR *addr_list = (PTR *)data;
            while (*addr_list) {
                addr_list++;
            }
            *addr_list = here;
            return 2;
        }
    }

    (void)address;
    return TRUE; // keep reading
}

static BOOL find_player_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    BYTE *b = buf;
    PTR player_data_addr = *(PTR *)data;
    Player *player;

    while (buf_len >= sizeof(Player)) {
        player = (Player *)b;
        if ((PTR)player->pPlayerData == player_data_addr && is_valid_Player(player)) {
            PTR here = address + (PTR)(b - buf);
            LOG_DEBUG("Valid player! %08lx", here);
            log_Player(player);
            *(PTR *)data = here;;
            memcpy(((PTR *)data) + 1, b, sizeof(Player));
            return FALSE;
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

/*
static BOOL hack_seed_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    BYTE *match;

    match = memsearch((char *)buf, data, buf_len, 4);
    if (match) {
        LOG_DEBUG("Seed match: %08lx", address);
        Act *act = (Act *)(match - sizeof(DWORD) * 3);
        if (is_valid_Act(act)) {
            PTR addr = address + (PTR)((match - sizeof(DWORD) * 3) - buf);
            LOG_DEBUG("Valid hact at %16lx - diff  %16lx",
                      addr);
            log_Act(act);
        }
            //return FALSE;
    }

    (void)address;
    return TRUE; // keep reading
}
*/


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

    PTR player_data_addr[32] = {0};
    if (!memreadall(pid, TRUE, find_player_data_callback, &player_data_addr)) {
        LOG_ERROR("Can't find PlayerData ptr");
        return EXIT_FAILURE;
    }
    /* LOG_INFO("pPlayerData: %08lx", player_data_addr[0]); */

    BYTE zboub[sizeof(Player) + sizeof(PTR)];
    for (int i = 31; i >= 0; i--) {
        if (!player_data_addr[i]) {
            continue;
        }
        *(PTR *)zboub = player_data_addr[i];
        if (memreadall(pid, FALSE, find_player_callback, &zboub)) {
            break;
        } else if (!i) {
            LOG_ERROR("Can't find Player ptr");
            return EXIT_FAILURE;
        }
    }
    PTR player_addr = *(PTR *)zboub;
    Player player;
    memcpy(&player, ((PTR *)zboub + 1), sizeof(Player));
    LOG_INFO("pPlayer: %08lx", player_addr);
    /* log_Player(&player);    /\* DEBUG *\/ */


    Act act;
    if (!memread(pid,
                 (PTR)player.pAct + sizeof(PTR),
                 sizeof(Act),
                 find_seed_callback,
                 &act)) {
        LOG_ERROR("Can't find seed");
        /* return EXIT_FAILURE; */
    }
    log_Act(&act);


    /* PTR seed = (PTR)player.pAct; */
    /* if (!memreadall(pid, find_seed_callback, &seed)) { */
    /*     LOG_ERROR("Can't find seed"); */
    /*     return EXIT_FAILURE; */
    /* } */

    /* BYTE seed_str[] = "\x5d\x15\xa6\x47"; */
    /* if (!memreadall(pid, hack_seed_callback, &seed_str)) { */
    /*     LOG_ERROR("Can't hack seed"); */
    /*     /\* return EXIT_FAILURE; *\/ */
    /* } */


    return EXIT_SUCCESS;
}
