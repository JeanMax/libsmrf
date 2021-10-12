#include "seed.h"

#define PAGE_LENGTH (4096 * 8)
unsigned char g_read_buf[PAGE_LENGTH];
Player g_player;

    /* (!((PTR)ptr % sizeof(WORD))                      \ */
#define is_valid_ptr(ptr, start_address, length)                \
    ((PTR)ptr >= start_address                        \
     && (PTR)ptr < start_address + length)




char *memsearch(const void *mem, const void *search, size_t mem_len, size_t search_len)
{
    char *ptr = (char *)mem;
#ifdef DEBUG_MODE
    if ((PTR)ptr % sizeof(PTR)) {
        LOG_ERROR("unaligned memsearch: %lu bytes left to align (%08lx)",
                  (PTR)ptr % sizeof(PTR), (PTR)ptr);
    }
#endif

    while (mem_len >= search_len) {
        if (!memcmp(ptr, search, search_len)) {
            return ptr;
        }
        /* ptr++; */
        /* mem_len--; */
        /* TODO: use aligned version once you know everything's ok */
        ptr += sizeof(DWORD);
        mem_len -= sizeof(DWORD);
    }
    return NULL;
}

int is_rw_memory(const char *memory_info_str)
{
    while (*memory_info_str && *memory_info_str != ' ') {
        memory_info_str++;
    }
    return memory_info_str[1] == 'r' &&  memory_info_str[2] == 'w';
}

int is_bullshit_memory(const char *memory_info_str)
{
    static char *forbidden_kw[] = {
        "nvidia",
        "wine",
        "pulse",
        ".so",
        ".dll",
        "deleted",
        NULL
    };

    for (char **kw = forbidden_kw; *kw; kw++) {
        if (strstr(memory_info_str, *kw)) {
            /* LOG_DEBUG("bullshit %s in: %s", *kw, memory_info_str); /\* DEBUG *\/ */
            return 1;
        }
    }
    return 0;
}



int is_valid_player_data(void *ptr, PTR start_address, size_t length)
{
    if ((PTR)ptr + sizeof(PlayerData) > start_address + length) {
        LOG_ERROR("nop %lu %lu %lu %lu",
                  (PTR)ptr, sizeof(PlayerData), start_address, length);
        return 0;
    }

    PlayerData *player_data = (PlayerData *)ptr;

    if ((PTR)ptr % sizeof(PTR)) {
        LOG_ERROR("unaligned player_data: %lu bytes left to align (%08lx)",
                  (PTR)ptr % sizeof(PTR),
                  (PTR)ptr);
        return 0;
    }

    return is_valid_ptr(player_data->pNormalQuest, start_address, length)
        && is_valid_ptr(player_data->pNightmareQuest, start_address, length)
        && is_valid_ptr(player_data->pHellQuest, start_address, length)
        && is_valid_ptr(player_data->pNormalWaypoint, start_address, length)
        && is_valid_ptr(player_data->pNightmareWaypoint, start_address, length)
        && is_valid_ptr(player_data->pHellWaypoint, start_address, length);
}

int is_valid_player(void *ptr, PTR start_address, size_t length)
{
    if ((PTR)ptr + sizeof(Player) > start_address + length) {
        LOG_ERROR("nop %lu %lu %lu %lu",
                (PTR)ptr, sizeof(Player), start_address, length);
        return 0;
    }

    Player *player = (Player *)ptr;

    /* if (!is_valid_ptr(player->pPath, start_address, length)) { */
    /*     LOG_WARNING("LKAJFLKAZJFL"); /\* DEBUG *\/ */
    /* } */

    return is_valid_ptr(player->pAct, start_address, length)
        && is_valid_ptr(player->pPath, start_address, length);
        /* && player->dwAct < 5; */
}

int is_valid_act(void *ptr, PTR start_address, size_t length)
{
    if ((PTR)ptr + sizeof(Act) > start_address + length) {
        LOG_ERROR("nop %lu %lu %lu %lu",
                (PTR)ptr, sizeof(Act), start_address, length);
        return 0;
    }

    Act *act = (Act *)ptr;
    if (!is_valid_ptr(act->pRoom1, start_address, length)) {
        LOG_DEBUG("zgeg room %08lx ", (PTR)act->pRoom1);
    }
    if (!is_valid_ptr(act->pMisc, start_address, length)) {
        LOG_DEBUG("zgeg misc %08lx ", (PTR)act->pMisc);
    }
    if (act->dwAct > 4) {
        LOG_DEBUG("zgeg act %08lx ", (PTR)act->dwAct);
    }

    return is_valid_ptr(act->pRoom1, start_address, length)
        && (is_valid_ptr(act->pMisc, start_address, length) || !act->pMisc)
        && act->dwAct < 5;
}



PTR find_player_data_ptr(FILE* pMemFile, PTR start_address, size_t length)
{
    char *match;
    size_t read_len = 0;
    size_t page_len = MIN(PAGE_LENGTH, length);

    fseeko(pMemFile, start_address, SEEK_SET);
    for (PTR address = start_address;
         address < start_address + length;
         address += page_len) {

        read_len = fread(&g_read_buf, 1, page_len, pMemFile);

        match = memsearch((char *)g_read_buf, "TrapSmurf", read_len, strlen("TrapSmurf"));
        if (match) {
            fprintf(stderr, "\n");   /* DEBUG */
            LOG_DEBUG("Smurf match: %08lx", address); /* DEBUG */
            if (is_valid_player_data(match, start_address, length)) {
                /* PlayerData *player_data = (PlayerData *)match; */
                LOG_DEBUG("Valid player_data! %08lx",
                        address + ((PTR)match - (PTR)g_read_buf));
                return address + ((PTR)match - (PTR)g_read_buf);
            }
            /* fwrite(&g_read_buf, 1, read_len, stdout); */
        }

        if (read_len < page_len) {
            LOG_WARNING("something went wrong with fread (read_len: %lu)",
                        read_len);
            break;
        }
    }

    fprintf(stderr, ".");   /* DEBUG */
    return 0;
}



char *playersearch(const void *mem, PTR player_data_addr, size_t mem_len,
                   PTR start_address, size_t length)
{
    char *ptr = (char *)mem;
    size_t search_len = sizeof(Player);
    Player *player;

        /* && player->dwAct < 5; */

    /* LOG_WARNING("% %lu", */
    /*             start_address, length); */
    while (mem_len >= search_len) {
        player = (Player *)ptr;
        if (is_valid_ptr(player->pPlayerData, start_address, length)
                && is_valid_ptr(player->pAct, start_address, length)
                && is_valid_ptr(player->pPath, start_address, length)) {

            /* LOG_DEBUG("valid player ptr found: %08lx", */
            /*           (PTR)player->pPlayerData); */
            if ((PTR)player->pPlayerData == player_data_addr) {
                LOG_ERROR("WOOP WOOP %08lx", (PTR)player->pPlayerData);
            }
            size_t diff_pla = ABS(player_data_addr - (PTR)player->pPlayerData);
            size_t diff_act = ABS((PTR)player->pAct - (0x7fffd7436b94 - sizeof(DWORD) * 3));
            if ((PTR)player->pPlayerData == player_data_addr) {
                LOG_ERROR("WOOP WOOP %08lx", (PTR)player->pPlayerData);
            }
            if (diff_act < 1024) {
                LOG_ERROR("ALMOST %08lx - diff: %lu %lu - act_addr: %08lx - act: %d - path: %d",
                          (PTR)player->pPlayerData,
                          diff_pla, diff_act,
                          (PTR)player->pAct + sizeof(DWORD) * 3,
                          is_valid_ptr(player->pAct, start_address, length),
                          is_valid_ptr(player->pPath, start_address, length));
            }
         }
            /* return ptr; */
        /* } */

        /* ptr++; */
        /* mem_len--; */
        /* TODO: use aligned version once you know everything's ok */
        ptr += sizeof(PTR);
        mem_len -= sizeof(PTR);
    }
    return NULL;
}


Player *find_player_ptr(FILE* pMemFile, PTR start_address, size_t length,
                        PTR player_data_addr)
{
    (void)player_data_addr;
    char *match;
    size_t read_len = 0;
    size_t page_len = MIN(PAGE_LENGTH, length);
    Player *player = 0;

    LOG_DEBUG("add %08lx", player_data_addr);

    /* player_data_addr = __bswap_64(player_data_addr); */
    fseeko(pMemFile, start_address, SEEK_SET);
    for (PTR address = start_address;
         address < start_address + length;
         address += page_len) {

        read_len = fread(&g_read_buf, 1, page_len, pMemFile);

        /* match = memsearch(g_read_buf, &player_data_addr, read_len, sizeof(PTR)); */

        /* match = memsearch(g_read_buf, "\xd0\xa3\x47\xd7\xff\x7f", read_len, 6); */
        playersearch(g_read_buf, player_data_addr, read_len, start_address, length);
        /* if (match) { */
        /*     player = (Player *)(match - 4 * sizeof(DWORD)); */
        /*     LOG_DEBUG("player_data match: %08lx ", *(PTR *)(match + 2)); /\* DEBUG *\/ */
        /*     LOG_DEBUG("player_data path: %08lx ", (PTR)player->pPath); /\* DEBUG *\/ */
        /*     if (is_valid_player(player, start_address, length)) { */
        /*         LOG_DEBUG("YAY act %08lx", (PTR)player->pAct); /\* DEBUG *\/ */
        /*         memcpy(&g_player, player, sizeof(Player)); */
        /*         /\* return &g_player; *\/ */
        /*     } */
        /* } */

        if (read_len < page_len) {
            LOG_WARNING("something went wrong with fread (read_len: %lu)",
                        read_len);
            break;
        }
    }

    return 0;
}

DWORD find_seed(FILE* pMemFile, PTR start_address, size_t length,
                PTR act_addr)
{
    (void)act_addr;
    char *match;
    size_t read_len = 0;
    size_t page_len = MIN(PAGE_LENGTH, length);
    char fuck[] = "\x53\xf7\xdf\x57\x00";

    /* memcpy(fuck, player_data, sizeof(fuck) - 1); */
    LOG_DEBUG("seed theoric addr: %08lx", act_addr + sizeof(DWORD) * 3); /* DEBUG */

    fseeko(pMemFile, start_address, SEEK_SET);
    for (PTR address = start_address;
         address < start_address + length;
         address += page_len) {

        read_len = fread(&g_read_buf, 1, page_len, pMemFile);

        match = memsearch(g_read_buf, fuck, read_len, strlen(fuck));
        if (match) {
            LOG_DEBUG("seed match: %08lx", address + ((PTR)match - (PTR)g_read_buf)); /* DEBUG */
            is_valid_act(match - sizeof(DWORD) * 3, start_address, length);
            LOG_DEBUG("seed diff: %lu", (address + ((PTR)match - (PTR)g_read_buf)) - (act_addr + sizeof(DWORD) * 3));
        }

        if (read_len < page_len) {
            LOG_WARNING("something went wrong with fread (read_len: %lu)",
                        read_len);
            break;
        }
    }






    /* size_t read_len = 0; */
    /* size_t page_len = MIN(PAGE_LENGTH, length - (act_addr - start_address)); */

    /* fseeko(pMemFile, act_addr + 256, SEEK_SET); */
    /* read_len = fread(&g_read_buf, 1, page_len, pMemFile); */

    /* LOG_DEBUG("zob %08lx %08lx %08lx %08lx ", *g_read_buf, *(g_read_buf + 8), *(g_read_buf + 16), *(g_read_buf + 24)); */

    /* if (is_valid_act(g_read_buf, act_addr, page_len)) { */
    /*     return ((Act *)g_read_buf)->dwMapSeed; */
    /* } */
    return 0;
}



int main(int argc, char **argv)
{
    /* PTR z = 3; */
    /* z=__bswap_64(z); */
    /* if (memsearch("\x42\x42\x42\x42\x42\x42\x42\x42\x42\x04\x00\x00\x00\x00\x00\x00\x00\x03\x42\x42\x42\x42\x42\x42\x42\x42", &z, 25, 8)) { */
    /*      LOG_DEBUG("zboub!!!"); /\* DEBUG *\/ */
    /* } */
    /* LOG_DEBUG("zgegy: %08lx", z); */



    if (argc != 2) {
        LOG_ERROR("%s <pid>", argv[0]);
        return 1;
    }
    /* int pid = atoi(argv[1]); */

    /* long ptraceResult = ptrace(PTRACE_ATTACH, pid, NULL, NULL); */
    /* if (ptraceResult < 0) { */
    /*     LOG_DEBUG("Unable to attach to the pid specified"); */
    /*     return 2; */
    /* } */
    /* wait(NULL); */

    char mapsFilename[1024];
    sprintf(mapsFilename, "/proc/%s/maps", argv[1]);
    FILE* pMapsFile = fopen(mapsFilename, "r");

    char memFilename[1024];
    sprintf(memFilename, "/proc/%s/mem", argv[1]);
    FILE* pMemFile = fopen(memFilename, "r");

    char line[256];
    PTR start_address;
    PTR end_address;

    PTR player_data_addr = 0;
    while (fgets(line, 256, pMapsFile) != NULL) {

        sscanf(line, "%16lx-%16lx\n", &start_address, &end_address);
        if (start_address >= 0x7fff00000000 && end_address <= 0x7fffffffffff
            && end_address - start_address > 0x1000000
            && is_rw_memory(line) && !is_bullshit_memory(line)) {
            /* LOG_DEBUG("%s", line); /\* DEBUG *\/ */
            player_data_addr = find_player_data_ptr(pMemFile,
                                                    start_address,
                                                    end_address - start_address);
            if (player_data_addr){
                LOG_DEBUG("%s", line); /* DEBUG */
                break;
            }
        }
    }
    fclose(pMapsFile);

    Player *player = 0;
    if (player_data_addr) {
        player = find_player_ptr(pMemFile,
                                 start_address,
                                 end_address - start_address,
                                 player_data_addr);
    }

    DWORD seed = 0;
    /* if (player) { */
        seed = find_seed(pMemFile,
                         start_address,
                         end_address - start_address,
                         player_data_addr);
                         /* (PTR)g_player.pAct); */
    /* } */

    if (seed) {
        LOG_DEBUG("SEED: %d", seed);
    }

    fclose(pMemFile);

    /* ptrace(PTRACE_CONT, pid, NULL, NULL); */
    /* ptrace(PTRACE_DETACH, pid, NULL, NULL); */

    return 0;
}



/* TODO

 * in d2structs.c:
   * print_X
   * is_valid_X
 * in mem.c:
   * generic read
   * generic search (calling read)
   * store list of alloc

 */
