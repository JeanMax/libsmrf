#ifndef _SEED_H
#define _SEED_H

#define _GNU_SOURCE

#include "d2structs.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef NO_COLORS
# define CLR_BLACK ""
# define CLR_RED ""
# define CLR_GREEN ""
# define CLR_YELLOW ""
# define CLR_BLUE ""
# define CLR_MAGENTA ""
# define CLR_CYAN ""
# define CLR_WHITE ""
# define CLR_RESET ""
#else
# define CLR_BLACK   "\033[30;01m"
# define CLR_RED     "\033[31;01m"
# define CLR_GREEN   "\033[32;01m"
# define CLR_YELLOW  "\033[33;01m"
# define CLR_BLUE    "\033[34;01m"
# define CLR_MAGENTA "\033[35;01m"
# define CLR_CYAN    "\033[36;01m"
# define CLR_WHITE   "\033[37;01m"
# define CLR_RESET   "\033[0m"
#endif

#ifdef DEBUG_MODE
# define LOG_DEBUG(str, ...) \
    fprintf(stderr, CLR_MAGENTA "[DEBUG]: " CLR_RESET str "\n", ##__VA_ARGS__)
# define LOG_INFO(str, ...) \
    printf(CLR_BLUE "[INFO]: " CLR_RESET str "\n", ##__VA_ARGS__)
#else
# define LOG_DEBUG(str, ...) do {} while (0)
# define LOG_INFO(str, ...) printf(str "\n", ##__VA_ARGS__)
#endif
#define LOG_WARNING(str, ...) \
    fprintf(stderr, CLR_YELLOW "[WARNING]: " CLR_RESET str "\n", ##__VA_ARGS__)
#define LOG_ERROR(str, ...) \
    fprintf(stderr, CLR_RED "[ERROR]: " CLR_RESET str "\n", ##__VA_ARGS__)


#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


// proc.c
typedef BOOL t_read_callback(BYTE *buf, size_t buf_len, PTR address, void *data);

#define IS_ALIGNED(ptr) !((PTR)(ptr) % sizeof(PTR))

#define MAP_PREFIX 0x7fff
#define MIN_MAP_LEN  0x10000
#define MIN_MAP_ADDR 0x10000000

#define fast_is_valid_ptr(ptr) \
    (IS_ALIGNED(ptr) && (PTR)(ptr) >> 32 == MAP_PREFIX)
BOOL is_valid_ptr(PTR ptr);

BYTE *memsearch(const void *mem, const void *search, size_t mem_len, size_t search_len);
int memread(pid_t pid, PTR start_address, size_t length,
            t_read_callback *on_page_read, void *data);
BOOL memreadall(pid_t pid, BOOL only_valid, t_read_callback *on_page_read, void *data);
BOOL readmaps(pid_t pid);
pid_t pidof(const char*pname);


// d2structs.c
void log_data(void *ptr, size_t len);

void log_BnetData(BnetData *ptr);
void log_Level(Level *ptr);
void log_Room1(Room1 *ptr);
void log_Room2(Room2 *ptr);
void log_Path(Path *ptr);
void log_Act(Act *ptr);
void log_PlayerData(PlayerData *ptr);
void log_Player(Player *ptr);

BOOL is_valid_BnetData(BnetData *ptr);
BOOL is_valid_Level(Level *ptr);
BOOL is_valid_Room1(Room1 *ptr);
BOOL is_valid_Room2(Room2 *ptr);
BOOL is_valid_Path(Path *ptr);
BOOL is_valid_Act(Act *ptr);
BOOL is_valid_PlayerData(PlayerData *ptr);
BOOL is_valid_Player(Player *ptr);

#endif
