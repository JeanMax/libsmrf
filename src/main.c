#include "vmread.h"

#define PAGE_LENGTH (4096 * 8)
unsigned char g_read_buf[PAGE_LENGTH];
Player g_player;

#define is_valid_ptr(ptr, start_address, length) \
       ((unsigned long)ptr >= start_address      \
     && (unsigned long)ptr < start_address + length)


char *memsearch(const void *mem, const void *search, size_t mem_len, size_t search_len)
{
    char *ptr = (char *)mem;

    while (mem_len >= search_len) {
        if (!memcmp(ptr, search, search_len)) {
            return ptr;
        }
        ptr++;
        mem_len--;
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
            /* fprintf(stderr, "bullshit %s in: %s\n", *kw, memory_info_str); /\* DEBUG *\/ */
            return 1;
        }
    }
    return 0;
}



int is_valid_player_data(void *ptr, unsigned long start_address, size_t length)
{
    if ((unsigned long)ptr + sizeof(PlayerData) > start_address + length) {
        fprintf(stderr, "nop %lu %lu %lu %lu\n",
                (unsigned long)ptr, sizeof(PlayerData), start_address, length);
        return 0;
    }

    PlayerData *player_data = (PlayerData *)ptr;
    return is_valid_ptr(player_data->pNormalQuest, start_address, length)
        && is_valid_ptr(player_data->pNightmareQuest, start_address, length)
        && is_valid_ptr(player_data->pHellQuest, start_address, length)
        && is_valid_ptr(player_data->pNormalWaypoint, start_address, length)
        && is_valid_ptr(player_data->pNightmareWaypoint, start_address, length)
        && is_valid_ptr(player_data->pHellWaypoint, start_address, length);
}

int is_valid_player(void *ptr, unsigned long start_address, size_t length)
{
    if ((unsigned long)ptr + sizeof(Player) > start_address + length) {
        fprintf(stderr, "nop %lu %lu %lu %lu\n",
                (unsigned long)ptr, sizeof(Player), start_address, length);
        return 0;
    }

    Player *player = (Player *)ptr;

    if (!is_valid_ptr(player->pPath, start_address, length)) {
        fprintf(stderr, "LKAJFLKAZJFL\n");
    }

    return is_valid_ptr(player->pAct, start_address, length)
        && is_valid_ptr(player->pPath, start_address, length);
        /* && player->dwAct < 5; */
}

int is_valid_act(void *ptr, unsigned long start_address, size_t length)
{
    if ((unsigned long)ptr + sizeof(Act) > start_address + length) {
        fprintf(stderr, "nop %lu %lu %lu %lu\n",
                (unsigned long)ptr, sizeof(Act), start_address, length);
        return 0;
    }

    Act *act = (Act *)ptr;
    if (!is_valid_ptr(act->pRoom1, start_address, length)) {
        fprintf(stderr, "zgeg room %08lx \n", (unsigned long)act->pRoom1);
    }
    if (!is_valid_ptr(act->pMisc, start_address, length)) {
        fprintf(stderr, "zgeg misc %08lx \n", (unsigned long)act->pMisc);
    }
    if (act->dwAct > 4) {
        fprintf(stderr, "zgeg act %08lx \n", (unsigned long)act->dwAct);
    }

    return is_valid_ptr(act->pRoom1, start_address, length)
        && (is_valid_ptr(act->pMisc, start_address, length) || !act->pMisc)
        && act->dwAct < 5;
}



unsigned long find_player_data_ptr(FILE* pMemFile, unsigned long start_address, size_t length)
{
    char *match;
    size_t read_len = 0;
    size_t page_len = MIN(PAGE_LENGTH, length);

    fseeko(pMemFile, start_address, SEEK_SET);
    for (unsigned long address = start_address;
         address < start_address + length;
         address += page_len) {

        read_len = fread(&g_read_buf, 1, page_len, pMemFile);

        match = memsearch((char *)g_read_buf, "TrapSmurf", read_len, strlen("TrapSmurf"));
        if (match) {
            fprintf(stderr, "\nSmurf match: %08lx\n", address); /* DEBUG */
            if (is_valid_player_data(match, start_address, length)) {
                /* PlayerData *player_data = (PlayerData *)match; */
                fprintf(stderr, "Valid player_data! %08lx\n", start_address + ((unsigned long)match - (unsigned long)g_read_buf));
                /* if (address != 0x7fffd4530000 && address != 0x7fffd4540000) */
                return start_address + ((unsigned long)match - (unsigned long)g_read_buf);
            }
            /* fwrite(&g_read_buf, 1, read_len, stdout); */
        }

        if (read_len < page_len) {
            fprintf(stderr, "WARNING: something went wrong with fread (read_len: %lu)\n",
                   read_len);
            break;
        }
    }

    fprintf(stderr, ".");   /* DEBUG */
    return 0;
}

Player *find_player_ptr(FILE* pMemFile, unsigned long start_address, size_t length,
                        unsigned long player_data_addr)
{
    char *match;
    size_t read_len = 0;
    size_t page_len = MIN(PAGE_LENGTH, length);
    Player *player = 0;

    fprintf(stderr, "add %08lx\n", player_data_addr);

    /* player_data_addr = __bswap_64(player_data_addr); */
    fseeko(pMemFile, start_address, SEEK_SET);
    for (unsigned long address = start_address;
         address < start_address + length;
         address += page_len) {

        read_len = fread(&g_read_buf, 1, page_len, pMemFile);

        /* match = memsearch(g_read_buf, &player_data_addr, read_len, sizeof(unsigned long)); */

        match = memsearch(g_read_buf, "\x99\x4a\xd3\xff\x7f", read_len, 5);
        if (match) {
            player = (Player *)(match - 4 * sizeof(DWORD));
            fprintf(stderr, "\nplayer_data match: %08lx \n", *(unsigned long *)(match + 2)); /* DEBUG */
            fprintf(stderr, "\nplayer_data path: %08lx \n", (unsigned long)player->pPath); /* DEBUG */
            if (is_valid_player(player, start_address, length)) {
                fprintf(stderr, "YAY act %08lx\n", (unsigned long)player->pAct); /* DEBUG */
                memcpy(&g_player, player, sizeof(Player));
                /* return &g_player; */
            }
        }

        if (read_len < page_len) {
            fprintf(stderr, "WARNING: something went wrong with fread (read_len: %lu)\n",
                   read_len);
            break;
        }
    }

    return 0;
}

DWORD find_seed(FILE* pMemFile, unsigned long start_address, size_t length,
                unsigned long act_addr)
{
    char *match;
    size_t read_len = 0;
    size_t page_len = MIN(PAGE_LENGTH, length);
    char fuck[] = "\x8d\xa8\xd2\x43\x00";

    /* memcpy(fuck, player_data, sizeof(fuck) - 1); */
    fprintf(stderr, "\nseed theoric addr: %08lx\n", act_addr + sizeof(DWORD) * 3); /* DEBUG */

    fseeko(pMemFile, start_address, SEEK_SET);
    for (unsigned long address = start_address;
         address < start_address + length;
         address += page_len) {

        read_len = fread(&g_read_buf, 1, page_len, pMemFile);

        match = memsearch(g_read_buf, fuck, read_len, strlen(fuck));
        if (match) {
            fprintf(stderr, "\nseed match: %08lx\n", address + ((unsigned long)match - (unsigned long)g_read_buf)); /* DEBUG */
            is_valid_act(match - sizeof(DWORD) * 3, start_address, length);
            fprintf(stderr, "\nseed diff: %lu\n", (act_addr + sizeof(DWORD) * 3) - (address + ((unsigned long)match - (unsigned long)g_read_buf)));
        }

        if (read_len < page_len) {
            fprintf(stderr, "WARNING: something went wrong with fread (read_len: %lu)\n",
                   read_len);
            break;
        }
    }






    /* size_t read_len = 0; */
    /* size_t page_len = MIN(PAGE_LENGTH, length - (act_addr - start_address)); */

    /* fseeko(pMemFile, act_addr + 256, SEEK_SET); */
    /* read_len = fread(&g_read_buf, 1, page_len, pMemFile); */

    /* fprintf(stderr, "zob %08lx %08lx %08lx %08lx \n", *g_read_buf, *(g_read_buf + 8), *(g_read_buf + 16), *(g_read_buf + 24)); */

    /* if (is_valid_act(g_read_buf, act_addr, page_len)) { */
    /*     return ((Act *)g_read_buf)->dwMapSeed; */
    /* } */
    return 0;
}



int main(int argc, char **argv)
{
    /* unsigned long z = 3; */
    /* z=__bswap_64(z); */
    /* if (memsearch("\x42\x42\x42\x42\x42\x42\x42\x42\x42\x04\x00\x00\x00\x00\x00\x00\x00\x03\x42\x42\x42\x42\x42\x42\x42\x42", &z, 25, 8)) { */
    /*      fprintf(stderr, "zboub!!!\n"); /\* DEBUG *\/ */
    /* } */
    /* fprintf(stderr, "zgegy: %08lx\n", z); */



    if (argc != 2) {
        printf("%s <pid>\n", argv[0]);
        return 1;
    }
    /* int pid = atoi(argv[1]); */

    /* long ptraceResult = ptrace(PTRACE_ATTACH, pid, NULL, NULL); */
    /* if (ptraceResult < 0) { */
    /*     fprintf(stderr, "Unable to attach to the pid specified\n"); */
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
    unsigned long start_address;
    unsigned long end_address;

    unsigned long player_data_addr = 0;
    while (fgets(line, 256, pMapsFile) != NULL) {

        sscanf(line, "%16lx-%16lx\n", &start_address, &end_address);
        if (start_address >= 0x7fff00000000 && end_address <= 0x7fffffffffff
            && end_address - start_address > 0x1000000
            && is_rw_memory(line) && !is_bullshit_memory(line)) {
            /* fprintf(stderr, "%s\n", line); /\* DEBUG *\/ */
            player_data_addr = find_player_data_ptr(pMemFile,
                                                    start_address,
                                                    end_address - start_address);
            if (player_data_addr){
                fprintf(stderr, "%s\n", line); /* DEBUG */
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
        (void)player;
    }

    DWORD seed = 0;
    /* if (player) { */
        seed = find_seed(pMemFile,
                         start_address,
                         end_address - start_address,
                         player_data_addr);
                         /* (unsigned long)g_player.pAct); */
    /* } */

    if (seed) {
        fprintf(stderr, "SEED: %d\n", seed);
    }

    fclose(pMemFile);

    /* ptrace(PTRACE_CONT, pid, NULL, NULL); */
    /* ptrace(PTRACE_DETACH, pid, NULL, NULL); */

    fprintf(stderr, "\n");
    return 0;
}
