#ifndef _PROC_H
#define _PROC_H

#include <string.h>
#include <unistd.h> // geteuid

#include "util/types.h"

typedef  struct MapAddress  MapAddress;
struct MapAddress {
    ptr_t start;
    ptr_t end;
};


typedef void *t_read_callback(byte *buf, size_t buf_len, ptr_t address, void *data);

#define QUICK_MAX_MAP_LEN 0x8000000
#define is_valid_ptr__quick(ptr) (IS_ALIGNED(ptr)           \
                                  && ptr >= 0x1000          \
                                  && ptr <= 0x7fffffffffff)


bool is_valid_ptr(ptr_t ptr);

byte *memsearch(const void *mem, const void *search, size_t mem_len, size_t search_len);
void *memread(ptr_t start_address, size_t length,
              t_read_callback *on_page_read, void *data);
void *memreadall(bool quick, t_read_callback *on_page_read, void *data);
pid_t readmaps(const char *win_name, bool refresh_win);  //TODO: rename

pid_t pid_of_cmd(const char *cmd_name);
pid_t pid_of_window(const char *win_name);

#endif
