/*
D2R.exe + 0x2055E40 == pPlayerUnit
                       pPlayerUnit + 0x20 == pAct
                                             pAct + 0x14 == map seed int32_t.
*/

#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

/* #include "d2struct.h" */

#include <stdint.h>
typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

#include <byteswap.h>
/* #define SWAP16(x) ((x >> 8) | (x << 8)) */

/* #define SWAP32(x) (((x >> 24) & 0x000000ff)     \ */
/*                  | ((x << 8)  & 0x00ff0000)     \ */
/*                  | ((x >> 8)  & 0x0000ff00)     \ */
/*                  | ((x << 24) & 0xff000000)) */


typedef  struct UnitAny  UnitAny;

typedef  struct CollMap  CollMap;
struct CollMap {
    DWORD dwPosGameX;  // 0x00
    DWORD dwPosGameY;  // 0x04
    DWORD dwSizeGameX; // 0x08
    DWORD dwSizeGameY; // 0x0C
    DWORD dwPosRoomX;  // 0x10
    DWORD dwPosRoomY;  // 0x14
    DWORD dwSizeRoomX; // 0x18
    DWORD dwSizeRoomY; // 0x1C
    WORD* pMapStart;   // 0x20
    WORD* pMapEnd;     // 0x22
};

typedef  struct PresetUnit  PresetUnit;
struct PresetUnit {
    DWORD _1;                // 0x00
    DWORD dwTxtFileNo;       // 0x04
    DWORD dwPosX;            // 0x08
    PresetUnit* pPresetNext; // 0x0C
    DWORD _3;                // 0x10
    DWORD dwType;            // 0x14
    DWORD dwPosY;            // 0x18
};

typedef  struct Room2  Room2;
typedef  struct ActMisc  ActMisc;
typedef  struct Level  Level;
struct Level {
    DWORD _1[4];        // 0x00
    Room2* pRoom2First; // 0x10
    DWORD _2[2];        // 0x14
    DWORD dwPosX;       // 0x1C
    DWORD dwPosY;       // 0x20
    DWORD dwSizeX;      // 0x24
    DWORD dwSizeY;      // 0x28
    DWORD _3[96];       // 0x2C
    Level* pNextLevel;  // 0x1AC
    DWORD _4;           // 0x1B0
    ActMisc* pMisc;     // 0x1B4
    DWORD _5[6];        // 0x1BC
    DWORD dwLevelNo;    // 0x1D0
    DWORD _6[3];        // 0x1D4
    union {
        DWORD RoomCenterX[9];
        DWORD WarpX[9];
    }; // 0x1E0
    union {
        DWORD RoomCenterY[9];
        DWORD WarpY[9];
    };                   // 0x204
    DWORD dwRoomEntries; // 0x228
};

typedef  struct RoomTile  RoomTile;
struct RoomTile {
    Room2* pRoom2;   // 0x00
    RoomTile* pNext; // 0x04
    DWORD _2[2];     // 0x08
    DWORD* nNum;     // 0x10
};

typedef  struct Room1  Room1;
struct Room1 {
    Room1** pRoomsNear;  // 0x00
    DWORD _1[3];         // 0x04
    Room2* pRoom2;       // 0x10
    DWORD _2[3];         // 0x14
    CollMap* Coll;       // 0x20
    DWORD dwRoomsNear;   // 0x24
    DWORD _3[9];         // 0x28
    DWORD dwXStart;      // 0x4C
    DWORD dwYStart;      // 0x50
    DWORD dwXSize;       // 0x54
    DWORD dwYSize;       // 0x58
    DWORD _4[6];         // 0x5C
    UnitAny* pUnitFirst; // 0x74
    DWORD _5;            // 0x78
    Room1* pRoomNext;    // 0x7C
};

struct Room2 {
    DWORD _1[2];        // 0x00
    Room2** pRoom2Near; // 0x08
    DWORD _2[5];        // 0x0C
    struct {
        DWORD dwRoomNumber;  // 0x00
        DWORD _1;            // 0x04
        DWORD* pdwSubNumber; // 0x08
    } * pType2Info;          // 0x20
    Room2* pRoom2Next;       // 0x24
    DWORD dwRoomFlags;       // 0x28
    DWORD dwRoomsNear;       // 0x2C
    Room1* pRoom1;           // 0x30
    DWORD dwPosX;            // 0x34
    DWORD dwPosY;            // 0x38
    DWORD dwSizeX;           // 0x3C
    DWORD dwSizeY;           // 0x40
    DWORD _3;                // 0x44
    DWORD dwPresetType;      // 0x48
    RoomTile* pRoomTiles;    // 0x4C
    DWORD _4[2];             // 0x50
    Level* pLevel;           // 0x58
    PresetUnit* pPreset;     // 0x5C
};

typedef  struct Path  Path;
struct Path {
    WORD xOffset;         // 0x00
    WORD xPos;            // 0x02
    WORD yOffset;         // 0x04
    WORD yPos;            // 0x06
    DWORD _1[2];          // 0x08
    WORD xTarget;         // 0x10
    WORD yTarget;         // 0x12
    DWORD _2[2];          // 0x14
    Room1* pRoom1;        // 0x1C
    Room1* pRoomUnk;      // 0x20
    DWORD _3[3];          // 0x24
    UnitAny* pUnit;       // 0x30
    DWORD dwFlags;        // 0x34
    DWORD _4;             // 0x38
    DWORD dwPathType;     // 0x3C
    DWORD dwPrevPathType; // 0x40
    DWORD dwUnitSize;     // 0x44
    DWORD _5[4];          // 0x48
    UnitAny* pTargetUnit; // 0x58
    DWORD dwTargetType;   // 0x5C
    DWORD dwTargetId;     // 0x60
    BYTE bDirection;      // 0x64
};


typedef  struct Act  Act;
struct Act {
    DWORD _1[3];     // 0x00
    DWORD dwMapSeed; // 0x0C
    Room1* pRoom1;   // 0x10
    DWORD dwAct;     // 0x14
    DWORD _2[12];    // 0x18
    ActMisc* pMisc;  // 0x48
};

struct ActMisc {
    DWORD _1[37];           // 0x00
    DWORD dwStaffTombLevel; // 0x94
    DWORD _2[245];          // 0x98
    Act* pAct;              // 0x46C
    DWORD _3[3];            // 0x470
    Level* pLevelFirst;     // 0x47C
};


typedef  struct QuestInfo  QuestInfo;
struct QuestInfo {
    void* pBuffer; // 0x00
    DWORD _1;      // 0x04
};

typedef  struct Waypoint  Waypoint;
struct Waypoint {
    BYTE flags; // 0x00
};

typedef  struct PlayerData  PlayerData;
struct PlayerData {
    char szName[0x10];            // 0x00
    QuestInfo* pNormalQuest;      // 0x10
    QuestInfo* pNightmareQuest;   // 0x14
    QuestInfo* pHellQuest;        // 0x18
    Waypoint* pNormalWaypoint;    // 0x1c
    Waypoint* pNightmareWaypoint; // 0x20
    Waypoint* pHellWaypoint;      // 0x24
};


typedef  struct Player  Player;
struct Player {
    DWORD dwType;
    DWORD dwTxtFileNo;
    DWORD dwUnitId;
    DWORD dwMode;
    PlayerData* pPlayerData;
    DWORD _1;
    DWORD dwAct;
    Act* pAct;
    DWORD dwSeed[2];
    DWORD _2;
    Path* pPath;
};


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


#define is_valid_ptr(ptr, start_address, length) \
       ((unsigned long)ptr >= start_address                \
     && (unsigned long)ptr < start_address + length)

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

#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define PAGE_LENGTH (4096 * 8)
unsigned char g_read_buf[PAGE_LENGTH];


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
                PlayerData *player_data = (PlayerData *)match;
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

Player g_player;
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
            fprintf(stderr, "\nplayer_data act: %08lx \n", player->pPath); /* DEBUG */
            if (is_valid_player(player, start_address, length)) {
                fprintf(stderr, "YAY act %08lx\n", player->pAct); /* DEBUG */
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


int is_valid_act(void *ptr, unsigned long start_address, size_t length)
{
    if ((unsigned long)ptr + sizeof(Act) > start_address + length) {
        fprintf(stderr, "nop %lu %lu %lu %lu\n",
                (unsigned long)ptr, sizeof(Act), start_address, length);
        return 0;
    }

    Act *act = (Act *)ptr;
    if (!is_valid_ptr(act->pRoom1, start_address, length)) {
        fprintf(stderr, "zgegr %08lx \n", act->pRoom1);
    }
    if (!is_valid_ptr(act->pMisc, start_address, length)) {
        fprintf(stderr, "zgegm %08lx \n", act->pMisc);
    }
    if (act->dwAct > 4) {
        fprintf(stderr, "zgega %08lx \n", act->pMisc);
    }

    return is_valid_ptr(act->pRoom1, start_address, length)
        && (is_valid_ptr(act->pMisc, start_address, length) || !act->pMisc)
        && act->dwAct < 5;
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
            fprintf(stderr, "\nseed diff: %d\n", (act_addr + sizeof(DWORD) * 3) - (address + ((unsigned long)match - (unsigned long)g_read_buf)));
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




int main(int argc, char **argv)
{
    unsigned long z = 3;
    z=__bswap_64(z);
    if (memsearch("\x42\x42\x42\x42\x42\x42\x42\x42\x42\x04\x00\x00\x00\x00\x00\x00\x00\x03\x42\x42\x42\x42\x42\x42\x42\x42", &z, 25, 8)) {
         fprintf(stderr, "zboub!!!\n"); /* DEBUG */
    }
    fprintf(stderr, "zgegy: %08lx\n", z);



    if (argc != 2) {
        printf("%s <pid>\n", argv[0]);
        return 1;
    }
    int pid = atoi(argv[1]);

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
