#ifndef _PROC_H
#define _PROC_H

#include <string.h>
#include <unistd.h> // geteuid

#include "util/types.h"
#include "util/log.h"


typedef  struct MapAddress  MapAddress;
struct MapAddress {
    ptr_t start;
    ptr_t end;
};


typedef bool t_read_callback(byte *buf, size_t buf_len, ptr_t address, void *data);


extern MapAddress g_stack;

#define is_valid_stack_ptr(ptr) (IS_ALIGNED(ptr)            \
                                 && ptr >= g_stack.start    \
                                 && ptr < g_stack.end)
bool is_valid_ptr(ptr_t ptr);

byte *memsearch(const void *mem, const void *search, size_t mem_len, size_t search_len);
int memread(pid_t pid, ptr_t start_address, size_t length,
            t_read_callback *on_page_read, void *data);
bool memreadall(pid_t pid, bool stack_only, t_read_callback *on_page_read, void *data);
bool readmaps(pid_t pid);
pid_t pidof(const char *pname);

#endif
