#include "smrf.h"

#ifndef _WIN32
# include <dirent.h>
#else
# include <windows.h>
# include <tlhelp32.h>
#endif

#define PAGE_LENGTH 0x1000
#ifndef PATH_MAX
# define PATH_MAX 0x100
#endif

#define MAX_MAPS     0x4000
static MapAddress g_maps_range[MAX_MAPS] = {0};  //TODO: this is ugly
MapAddress g_stack = {0};


////////////////////////////////////////////////////////////////////////////////

byte *memsearch(const void *mem, const void *search, size_t mem_len, size_t search_len)
{
    byte *ptr = (byte *)mem;
#ifdef NDEBUG
    if (!IS_ALIGNED(ptr)) {
        LOG_ERROR("unaligned memsearch: %zu bytes left to align (%16jx)",
                  (ptr_t)ptr % sizeof(ptr_t), (ptr_t)ptr);
        /* return NULL; */
    }
#endif

    while (mem_len >= search_len) {
        if (!memcmp(ptr, search, search_len)) {
            return ptr;
        }
        ptr += sizeof(ptr_t);
        mem_len -= sizeof(ptr_t);
   }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////

int memread(pid_t pid, ptr_t start_address, size_t length,
            t_read_callback *on_page_read, void *data)
{
#ifndef _WIN32
    char path[PATH_MAX];

    sprintf(path, "/proc/%d/mem", pid);
    FILE *mem_file = fopen(path, "r");
    if (!mem_file) {
        LOG_ERROR("Can't read proc memory");
        if (geteuid()) {
            LOG_ERROR("Try again as root!");
        }
        exit(EXIT_FAILURE);
    }
    fseeko(mem_file, (long)start_address, SEEK_SET);
#else
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                 0, pid);
    if (!process) {
        LOG_ERROR("Can't read proc memory (try again as admin?)");
        exit(EXIT_FAILURE);
    }
#endif
    static byte read_buf[PAGE_LENGTH];
    size_t read_len = 0;
    size_t page_len = MIN(PAGE_LENGTH, length);
    int callback_ret, ret = 0;

    for (ptr_t address = start_address;
         address < start_address + length;
         address += page_len) {

#ifndef _WIN32
        read_len = fread(&read_buf, 1, page_len, mem_file);
#else
        ReadProcessMemory(process, (void *)address, &read_buf, page_len, &read_len);
        // Toolhelp32ReadProcessMemory(pid, (void *)address, &read_buf, page_len, &read_len);
#endif
        if (!(callback_ret = on_page_read(read_buf, read_len, address, data))) {
            ret = 2;
            break;
        }
        if (callback_ret > 1) {
            ret = 2; //TODO: enum
        }
        if (read_len < page_len) {
            if (read_len) {
                LOG_WARNING("something went wrong with memread (read_len: %zu)",
                            (size_t)read_len);
            }
            break;
        }
    }

#ifndef _WIN32
    fclose(mem_file);
#else
    CloseHandle(process);
#endif
    return ret;
}

bool memreadall(pid_t pid, bool stack_only, t_read_callback *on_page_read, void *data)
{
    int i;
    for (i = 0; i < MAX_MAPS && g_maps_range[i].start; i++) {}
    for (i--; i >= 0; i--) {
    /* for (int i = 0; i < MAX_MAPS && g_maps_range[i].start; i++) { */
        ptr_t start = g_maps_range[i].start;
        size_t length = g_maps_range[i].end - g_maps_range[i].start;
        /* LOG_WARNING("map at %16jx, size: %16jx", start, length); /\* DEBUG *\/ */
        if (stack_only && !is_valid_stack_ptr(start)) {
            continue;
        }
        if (memread(pid, start, length, on_page_read, data) == 2) {
#ifdef NDEBUG
            fprintf(stderr, "\n");   /* DEBUG */
#endif
            return TRUE;
        }
#ifdef NDEBUG
        fprintf(stderr, ".");   /* DEBUG */
#endif
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

bool is_valid_ptr(ptr_t ptr)
{
    if (!IS_ALIGNED(ptr)) {
        return FALSE;
    }

    for (int i = 0; i < MAX_MAPS && g_maps_range[i].start; i++) {
        if (ptr >= g_maps_range[i].start
            && ptr <= g_maps_range[i].end) {
            return TRUE;
        }
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

#ifndef _WIN32
static bool is_rw_memory(const char *memory_info_str)
{
    while (*memory_info_str && *memory_info_str != ' ') {
        memory_info_str++;
    }
    return memory_info_str[1] == 'r' &&  memory_info_str[2] == 'w';
}

static bool is_bullshit_memory(const char *memory_info_str)
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
#endif

bool readmaps(pid_t pid)
{
    int i = 0;
    g_stack.end = 0;
#ifndef _WIN32
    char path[PATH_MAX], read_buf[PAGE_LENGTH];
    bzero(&g_maps_range, sizeof(g_maps_range));

    sprintf(path, "/proc/%d/maps", pid);
    FILE *maps_file = fopen(path, "r");
    if (!maps_file) {
        return FALSE;
    }

    ptr_t start_address;
    ptr_t end_address;
    while (fgets(read_buf, PAGE_LENGTH, maps_file)) {
        sscanf(read_buf, "%16jx-%16jx\n", &start_address, &end_address);
        if (strstr(read_buf, "[stack]")) {
            g_stack.start = start_address;
            g_stack.end = MAX(g_stack.end, end_address);
            continue;
        }
        if (is_rw_memory(read_buf) && !is_bullshit_memory(read_buf)) {
            if (i == MAX_MAPS) {
                LOG_ERROR("Too many mappings, abort.");
                exit(EXIT_FAILURE);
            }
            g_maps_range[i].start = start_address;
            g_maps_range[i].end = end_address;
            g_stack.end = MAX(g_stack.end, end_address);
            i++;
        }
    }
    fclose(maps_file);
#else
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                 0, pid);
    if (!process) {
        return FALSE;
    }

    SYSTEM_INFO sysInfo = {0};
    GetSystemInfo(&sysInfo);

    ptr_t addr_min = (ptr_t)sysInfo.lpMinimumApplicationAddress;
    ptr_t addr_max = (ptr_t)sysInfo.lpMaximumApplicationAddress;

    MEMORY_BASIC_INFORMATION info;
    for (ptr_t ptr = addr_min;
         VirtualQueryEx(process, (void *)ptr, &info, sizeof(info)) && ptr < addr_max;
         ptr += info.RegionSize) {
        if (info.State == MEM_COMMIT && info.Type == MEM_PRIVATE
                && !(info.Protect & PAGE_GUARD) && !(info.Protect & PAGE_NOACCESS)
                && (info.Protect & PAGE_READWRITE) ) {
            if (i == MAX_MAPS) {
                LOG_ERROR("Too many mappings, abort.");
                exit(EXIT_FAILURE);
            }
            g_maps_range[i].start = ptr;
            g_maps_range[i].end = ptr + info.RegionSize;
            g_stack.end = MAX(g_stack.end, g_maps_range[i].end);
            i++;
        }
    }
    CloseHandle(process);
    g_stack.start = addr_min;
    for (BYTE off = 1; g_stack.end >> off; off++) {
        if (g_stack.end >> off == 0x7f) {
            g_stack.start = g_stack.end >> off << off;
            break;
        }
    }
#endif
    LOG_DEBUG("stack: %16jx - %16jx", g_stack.start, g_stack.end); /* DEBUG */
    LOG_DEBUG("%d maps found", i); /* DEBUG */
    g_maps_range[i].start = 0;
    g_maps_range[i].end = 0;
    return i ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////

pid_t pidof(const char *pname)
{
#ifndef _WIN32
    FILE *comm_file;
    struct dirent *entry;
    char path[PATH_MAX], read_buf[PAGE_LENGTH];
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
#else
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (!Process32First(processesSnapshot, &processInfo)) {
        return 0;
    }
    do {
        if (!strcmp(pname, processInfo.szExeFile)) {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    } while (Process32Next(processesSnapshot, &processInfo));

    CloseHandle(processesSnapshot);
    return 0;
#endif
}
