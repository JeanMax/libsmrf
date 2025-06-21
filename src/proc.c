#include "proc.h"
#include "smrf.h"  // WINDOW_TITLE_MAX

#include "util/log.h"

#ifndef _WIN32
# include <dirent.h>
# include <ctype.h> // isspace
#else
# include <windows.h>
# include <tlhelp32.h>
# include <winuser.h>
# include <psapi.h>

# define MAX_MODULE 0x100
#endif

#include <stdio.h> // fseek


#define PAGE_LENGTH 0x1800  // sizeof UnitHashTable ... if bigger you miss the GameInfo
#ifndef PATH_MAX
# define PATH_MAX 0x100
#endif

#define READ_BUF_LEN 0x100

#define MAX_MAPS     0x4000
static MapAddress g_maps_range[MAX_MAPS] = {0};  //TODO: this is ugly

static pid_t g_pid = 0;
#ifndef _WIN32
static FILE *g_mem_file = NULL;
#else
static HANDLE g_process = NULL;
#endif

static MapAddress g_base_addr = {0};  //TODO: this is ugly

#define D2R_EXE "D2R.exe"
#define DELICIOUS_COOKIE "/tmp/smrf.cookie"


static void reset_global_state(void)  // TODO: :|
{
    g_pid = 0;
    g_base_addr.start = 0;
    g_base_addr.end = 0;
#ifndef _WIN32
    if (g_mem_file) {
        fclose(g_mem_file);
        g_mem_file = NULL;
    }
#else
    if (g_process) {
        CloseHandle(g_process);
        g_process = NULL;
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////

void *memread(ptr_t start_address, size_t length,
              t_read_callback *on_page_read, void *data)
{
#ifndef _WIN32
    if (!g_mem_file) {
        char path[PATH_MAX];
        sprintf(path, "/proc/%d/mem", g_pid);
        g_mem_file = fopen(path, "r");
        if (!g_mem_file) {
            LOG_ERROR("Can't read proc memory");
            reset_global_state();
            return NULL;
        }
    }
#else
    if (!g_process) {
        LOG_ERROR("Can't read proc memory");
        reset_global_state();
        return NULL;
    }
#endif
    byte read_buf[PAGE_LENGTH];
    size_t read_len = 0;
    size_t page_len = MIN(PAGE_LENGTH, length);
    void *ret = NULL;

    for (ptr_t address = start_address;
         address < start_address + length;
         address += page_len) {

        //TODO: if something is between 2 pages, you won't find it

#ifndef _WIN32
        fseek(g_mem_file, (long)address, SEEK_SET);
        read_len = fread(&read_buf, sizeof(byte), page_len, g_mem_file);
#else
        ReadProcessMemory(g_process, (void *)address, &read_buf, page_len, &read_len);
        // Toolhelp32ReadProcessMemory(g_pid, (void *)address, &read_buf, page_len, &read_len);
#endif
        if (read_len < page_len) {
            LOG_WARNING("something went wrong with memread (read_len: %zu / %zu)",
                        read_len, page_len);
        }
        ret = on_page_read(read_buf, read_len, address, data);
        if (ret || read_len < page_len) {
            break;
        }
    }

    return ret;
}

void *memreadbase(ptr_t start_address, size_t length,
                  t_read_callback *on_page_read, void *data)
{
    return memread(g_base_addr.start + start_address, length, on_page_read, data);
}

void *memreadall(bool quick, t_read_callback *on_page_read, void *data)
{
    void *ret = NULL;

    int i;
    for (i = 0; i < MAX_MAPS && g_maps_range[i].start; i++) {}
    for (i--; i >= 0; i--) {
    /* for (int i = 0; i < MAX_MAPS && g_maps_range[i].start; i++) { */
        ptr_t start = g_maps_range[i].start;
        size_t length = g_maps_range[i].end - g_maps_range[i].start;
        LOG_INFO("map at %12jx - %12jx,     size: %8jx",
                 start, g_maps_range[i].end, length); /* DEBUG */
        if (quick &&
            (length >= QUICK_MAX_MAP_LEN || !is_valid_ptr__quick(start))) {
            LOG_INFO("quick skip: %12jx", start); /* DEBUG */
            continue;
        }
        ret = memread(start, length, on_page_read, data);
        if (ret) {
#ifdef NDEBUG
            fprintf(stderr, "\n");   /* DEBUG */
#endif
            return ret;
        }
#ifdef NDEBUG
        if (i && !(i % 10)) {
            fprintf(stderr, ".");   /* DEBUG */
        }
#endif
    }

    return ret;
}

void *memreadallbase(t_read_callback *on_page_read, void *data)
{
    size_t length = g_base_addr.end - g_base_addr.start;

    LOG_INFO("base at %12jx - %12jx,     size: %8jx",
             g_base_addr.start, g_base_addr.end, length); /* DEBUG */

    return memread(g_base_addr.start, length, on_page_read, data);
}


////////////////////////////////////////////////////////////////////////////////

inline bool is_valid_ptr(ptr_t ptr)
{
    if (!ptr || !IS_ALIGNED(ptr)) {
        return FALSE;
    }

    for (int i = 0; i < MAX_MAPS && g_maps_range[i].end; i++) {
        if (ptr >= g_maps_range[i].start
            && ptr <= g_maps_range[i].end) {
            return TRUE;
        }
    }
    return FALSE;
}

inline bool is_valid_ptr__base(ptr_t ptr)
{
    if (!ptr || !IS_ALIGNED(ptr)) {
        return FALSE;
    }
    ptr += g_base_addr.start;
    return ptr >= g_base_addr.start && ptr <= g_base_addr.end;
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
    char *desc = strrchr(memory_info_str, ' ');
    if (!desc) {
        return TRUE;  // shouldn't happen
    }

    while (*desc && isspace(*desc)) {
        desc++;
    }
    if (!*desc) {
        return FALSE;  // empty desc in info (high addresses, not sure what is it)
    }

    // it seems you don't even need heap
    if (!strcmp(desc, "[heap]\n")) {
        return FALSE;
    }

    // it seems you don't even need stack
    if (!strcmp(desc, "[stack]\n")) {
        return FALSE;
    }

    return TRUE; // probably dll
}
#endif

static bool readbase(void)
{
#ifndef _WIN32
    if (!system("pid=$(pgrep -f '^C:.*" D2R_EXE "'); "
                "objdump -P header \"$(readlink -f /proc/$pid/cwd/" D2R_EXE ")\" "
                "| grep -E 'Image Base|Size Of Image' "
                "| cut -d: -f2 "
                "| xargs > " DELICIOUS_COOKIE)) {
        FILE *cookie = fopen(DELICIOUS_COOKIE, "r");
        if (cookie) {
            ptr_t img_start, img_size;
            char read_buf[READ_BUF_LEN];
            fgets(read_buf, READ_BUF_LEN, cookie);
            sscanf(read_buf, "%16jx %16jx\n", &img_start, &img_size);
            g_base_addr.start = img_start + 0x100000000;
            g_base_addr.end = g_base_addr.start + img_size;
            /* LOG_DEBUG("found base at: %16jx %16jx %16jx", */
            /*           g_base_addr.start, g_base_addr.end + g_base_addr.start, g_base_addr.end); */
            g_base_addr.end += 0x800000; //TODO: find out why and how much
            fclose(cookie);
            if (system("rm -f " DELICIOUS_COOKIE)) {
                LOG_WARNING("readbase: can't rm cookie " DELICIOUS_COOKIE);
            }
            return TRUE;
        }
    }

#else
    MODULEINFO modInfo = {0};
    HMODULE hMods[MAX_MODULE];
    DWORD cbNeeded;
    if (!EnumProcessModules(g_process, hMods, sizeof(hMods), &cbNeeded)) {
        LOG_ERROR("Can't EnumProcessModules");
        return FALSE;
    }
    for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
        TCHAR szModName[MAX_PATH];
        if (!GetModuleFileNameEx(g_process, hMods[i], szModName,
                                sizeof(szModName) / sizeof(TCHAR))) {
            continue;
        }
        if (!strstr(szModName, D2R_EXE)) {
            continue;
        }
        if (!GetModuleInformation(g_process, hMods[i],
                                  &modInfo, sizeof(modInfo))) {
            LOG_ERROR("Can't GetModuleInformation");
            return FALSE;
        }
        /* LOG_DEBUG("found base at: %16jx %16jx %16jx", */
        /*           modInfo.lpBaseOfDll, modInfo.SizeOfImage, modInfo.EntryPoint); */

        g_base_addr.start = (ptr_t)modInfo.lpBaseOfDll;
        g_base_addr.end = (ptr_t)modInfo.lpBaseOfDll + modInfo.SizeOfImage;
        g_base_addr.end += 0x800000; //TODO: find out why and how much
        return TRUE;
    }

#endif

    LOG_ERROR("Can't find base");
    return FALSE;
}

pid_t readmaps(const char *win_name, bool refresh_win)
{
    if (refresh_win) {
        reset_global_state();
    }

    if (!g_pid) {
        /* g_pid = pid_of_cmd("D2R.exe"); */
        g_pid = pid_of_window(win_name);
    }
    if (!g_pid) {
        LOG_ERROR("Can't find D2R window '%s'", win_name);
        return FALSE;
    }

    int i = 0;
#ifndef _WIN32
    char path[PATH_MAX], read_buf[READ_BUF_LEN];
    memset(&g_maps_range, 0, sizeof(g_maps_range));

    sprintf(path, "/proc/%d/maps", g_pid);
    FILE *maps_file = fopen(path, "r");
    if (!maps_file) {
        reset_global_state();
        return FALSE;
    }

    ptr_t start_address;
    ptr_t end_address;
    while (fgets(read_buf, READ_BUF_LEN, maps_file)) {
        sscanf(read_buf, "%16jx-%16jx\n", &start_address, &end_address);
        if (is_rw_memory(read_buf) && !is_bullshit_memory(read_buf)) {
            if (i == MAX_MAPS) {
                LOG_ERROR("Too many mappings, abort.");
                exit(EXIT_FAILURE);
            }
            g_maps_range[i].start = start_address;
            g_maps_range[i].end = end_address;
            i++;
        }
    }
    fclose(maps_file);
#else
    if (!g_process) {
        g_process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                0, g_pid);
    }
    if (!g_process) {
        reset_global_state();
        return FALSE;
    }

    SYSTEM_INFO sysInfo = {0};
    GetSystemInfo(&sysInfo);

    ptr_t addr_min = (ptr_t)sysInfo.lpMinimumApplicationAddress;
    ptr_t addr_max = (ptr_t)sysInfo.lpMaximumApplicationAddress;

    MEMORY_BASIC_INFORMATION info;
    for (ptr_t ptr = addr_min;
         VirtualQueryEx(g_process, (void *)ptr, &info, sizeof(info)) && ptr < addr_max;
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
            i++;
        }
    }
#endif
    /* LOG_DEBUG("%d maps found", i); /\* DEBUG *\/ */
    g_maps_range[i].start = 0;
    g_maps_range[i].end = 0;

    if (!g_base_addr.start && !readbase()) {
        reset_global_state();
        return FALSE;
    }

    return i ? g_pid : FALSE;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
static pid_t g_temp_pid = {0};
static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
    char buffer[WINDOW_TITLE_MAX];
    int written = GetWindowTextA(hwnd, buffer, WINDOW_TITLE_MAX);
    if (written && strstr(buffer, (char *)lparam) != NULL) {
        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        g_temp_pid = pid;
        return FALSE;
    }

    return TRUE;
}
#endif

pid_t pid_of_window(const char *win_name)
{
#ifndef _WIN32
    char buf[WINDOW_TITLE_MAX];
    pid_t pid = 0;

    snprintf(buf, WINDOW_TITLE_MAX,
             "wmctrl -lp | grep -m1 '%s' | cut -d' ' -f4 2>/dev/null > " DELICIOUS_COOKIE,
             win_name);  // I'm sorry I'm fucking lazy

    if (!system(buf)) {
        FILE *cookie = fopen(DELICIOUS_COOKIE, "r");
        if (cookie) {
            if (fgets(buf, WINDOW_TITLE_MAX, cookie)) {
                pid = atoi(buf);
            }
            fclose(cookie);
        }
        if (system("rm -f " DELICIOUS_COOKIE)) {
            LOG_WARNING("pid_of_window: can't rm cookie " DELICIOUS_COOKIE);
        }
    }
    return pid;
#else
    g_temp_pid = 0;
    EnumWindows(EnumWindowsProc, (ptr_t)win_name);
    return g_temp_pid;
#endif
}
