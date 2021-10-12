#include "seed.h"
#include <dirent.h>

#define PAGE_LENGTH 0x1000
#define MAX_PATH 128

#define MAX_MAPS     0x1000
PTR g_maps_range[MAX_MAPS][2] = {0};


////////////////////////////////////////////////////////////////////////////////

BYTE *memsearch(const void *mem, const void *search, size_t mem_len, size_t search_len)
{
    BYTE *ptr = (BYTE *)mem;
#ifdef DEBUG_MODE
    if (!IS_ALIGNED(ptr)) {
        LOG_ERROR("unaligned memsearch: %lu bytes left to align (%08lx)",
                  (PTR)ptr % sizeof(PTR), (PTR)ptr);
        /* return NULL; */
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

////////////////////////////////////////////////////////////////////////////////

int memread(pid_t pid, PTR start_address, size_t length,
            t_read_callback *on_page_read, void *data)
{
    char path[MAX_PATH];
    static BYTE read_buf[PAGE_LENGTH];

    sprintf(path, "/proc/%d/mem", pid);
    FILE *mem_file = fopen(path, "r"); //TODO: keep it open?
    if (!mem_file) {
        return 0;
    }

    size_t read_len = 0;
    size_t page_len = MIN(PAGE_LENGTH, length);

    fseeko(mem_file, start_address, SEEK_SET);
    for (PTR address = start_address;
         address < start_address + length;
         address += page_len) {

        read_len = fread(&read_buf, 1, page_len, mem_file);
        if (!on_page_read(read_buf, read_len, address, data)) {
            fclose(mem_file);
            return 2;
        }
        if (read_len < page_len) {
            LOG_WARNING("something went wrong with fread (read_len: %lu)",
                        read_len);
            break;
        }
    }

    fclose(mem_file);
    return 1;
}

BOOL memreadall(pid_t pid, t_read_callback *on_page_read, void *data)
{
    for (int i = 0; i < MAX_MAPS && g_maps_range[i][0]; i++) {
        if (memread(pid,
                    g_maps_range[i][0],
                    g_maps_range[i][1] - g_maps_range[i][0],
                    on_page_read,
                    data) == 2) {
            fprintf(stderr, "\n");   /* DEBUG */
            return TRUE;
        }
    }
    fprintf(stderr, ".");   /* DEBUG */
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

BOOL is_valid_ptr(PTR ptr)
{
    if (!IS_ALIGNED(ptr)) {
        return FALSE;
    }

    for (int i = 0; i < MAX_MAPS && g_maps_range[i][0]; i++) {
        if (ptr >= g_maps_range[i][0]
            && ptr <= g_maps_range[i][1]) {
            return TRUE;
        }
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

static BOOL is_rw_memory(const char *memory_info_str)
{
    while (*memory_info_str && *memory_info_str != ' ') {
        memory_info_str++;
    }
    return memory_info_str[1] == 'r' &&  memory_info_str[2] == 'w';
}

static BOOL is_bullshit_memory(const char *memory_info_str)
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
            return TRUE;
        }
    }
    return FALSE;
}

BOOL readmaps(pid_t pid)
{
    char path[MAX_PATH], read_buf[PAGE_LENGTH];

    sprintf(path, "/proc/%d/maps", pid);
    FILE *maps_file = fopen(path, "r");
    if (!maps_file) {
        return FALSE;
    }

    PTR start_address;
    PTR end_address;
    int i = 0;
    while (fgets(read_buf, PAGE_LENGTH, maps_file)) {
        sscanf(read_buf, "%16lx-%16lx\n", &start_address, &end_address);
        if (start_address >= MIN_MAP_LEN && end_address <= MAX_MAP_ADDR
            && end_address - start_address > MIN_MAP_LEN
            && is_rw_memory(read_buf)
            && !is_bullshit_memory(read_buf)) {
            g_maps_range[i][0] = start_address;
            g_maps_range[i][1] = end_address;
            i++;
        }
    }

    g_maps_range[i][0] = 0;
    g_maps_range[i][1] = 0;

    fclose(maps_file);
    return i ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////

pid_t pidof(const char*pname)
{
    FILE *comm_file;
    struct dirent *entry;
    char path[MAX_PATH], read_buf[PAGE_LENGTH];
    pid_t ret = 0;

    DIR *proc_dir = opendir("/proc/");
    if (!proc_dir) {
        perror("Fail");
        return ret;
    }

    while ((entry = readdir(proc_dir))) {
        strcpy(path, "/proc/");
        strcat(path, entry->d_name);
        strcat(path, "/comm");

        comm_file = fopen(path, "r");
        if (comm_file) {
            fscanf(comm_file, "%s", read_buf);
            if (!strcmp(read_buf, pname)) {
                ret = atoi(entry->d_name);
                fclose(comm_file);
                closedir(proc_dir);
                return ret;
            }
            fclose(comm_file);
        }
    }

    closedir(proc_dir);
    return ret;
}
