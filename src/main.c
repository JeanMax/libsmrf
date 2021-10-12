#include "seed.h"

#define PLAYER_NAME "TrapSmurf"

static BOOL find_player_data_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    BYTE *match;

    match = memsearch((char *)buf, PLAYER_NAME, buf_len, strlen(PLAYER_NAME));
    if (match) {
        LOG_DEBUG("Smurf match: %08lx", address); /* DEBUG */
        if (is_valid_PlayerData((PlayerData *)match)) {
            LOG_DEBUG("Valid player_data! %08lx", address + (PTR)(match - buf));
            log_PlayerData((PlayerData *)match);
            *(PTR *)data = address + ((PTR)match - (PTR)buf);
            /* return FALSE; */
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
        if (
            /* (PTR)player->pPlayerData == player_data_addr && */
             is_valid_Player((Player *)b)) {

            LOG_DEBUG("Valid player! %08lx", address + (PTR)(b - buf));
            log_Player(player);
            *(PTR *)data = address + ((PTR)b - (PTR)buf);
            memcpy(((PTR *)data) + 1, b, sizeof(Player));
            /* return FALSE; */
        }

        b++;
        buf_len--;
        /* TODO: use aligned version once you know everything's ok */
        /* b += sizeof(WORD); */
        /* buf_len -= sizeof(WORD); */
    }

    return TRUE; // keep reading
}

static BOOL find_seed_callback(BYTE *buf, size_t buf_len, PTR address, void *data)
{
    BYTE *match;

    match = memsearch((char *)buf, "abcd", buf_len, sizeof(DWORD));
    if (match) {
        LOG_DEBUG("Seed match: %08lx", address); /* DEBUG */
    }

    (void)address;
    return TRUE; // keep reading

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

    PTR player_data_addr = 0;
    if (!memreadall(pid, find_player_data_callback, &player_data_addr)) {
        LOG_ERROR("Can't find PlayerData ptr");
        return EXIT_FAILURE;
    }
    LOG_INFO("pPlayerData: %08lx", player_data_addr);

    BYTE zboub[sizeof(Player) + sizeof(PTR)];
    *(PTR *)zboub = player_data_addr;
    if (!memreadall(pid, find_player_callback, &zboub)) {
        LOG_ERROR("Can't find Player ptr");
        return EXIT_FAILURE;
    }
    PTR player_addr = *(PTR *)zboub;
    Player player;
    memcpy(&player, ((PTR *)zboub + 1), sizeof(Player));
    LOG_INFO("pPlayer: %08lx", player_addr);
    /* log_Player(&player);    /\* DEBUG *\/ */


    /* Player *player = 0; */
    /* if (player_data_addr) { */
    /*     player = find_player_ptr(pMemFile, */
    /*                              start_address, */
    /*                              end_address - start_address, */
    /*                              player_data_addr); */
    /* } */

    /* DWORD seed = 0; */
    /* /\* if (player) { *\/ */
    /*     seed = find_seed(pMemFile, */
    /*                      start_address, */
    /*                      end_address - start_address, */
    /*                      player_data_addr); */
    /*                      /\* (PTR)g_player.pAct); *\/ */
    /* /\* } *\/ */

    /* if (seed) { */
    /*     LOG_DEBUG("SEED: %d", seed); */
    /* } */


    return EXIT_SUCCESS;
}
