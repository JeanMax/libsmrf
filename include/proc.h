#ifndef _PROC_H
#define _PROC_H

#include <string.h>

#include "util/types.h"
#include "util/log.h"


typedef  struct MapAddress  MapAddress;
struct MapAddress {
    ptr_t start;
    ptr_t end;
};


typedef bool t_read_callback(byte *buf, size_t buf_len, ptr_t address, void *data);


// TODO: let's hope nobody sees that
#define IS_VALID_MAP_PREFIX(ptr) ((ptr_t)(ptr) >> 40 == 0x7f  \
                                || (ptr_t)(ptr) >> 36 == 0x7f \
                                || (ptr_t)(ptr) >> 32 == 0x7f \
                                || (ptr_t)(ptr) >> 24 == 0x7f)

#define fast_is_valid_ptr(ptr) (IS_ALIGNED(ptr) && IS_VALID_MAP_PREFIX(ptr))
bool is_valid_ptr(ptr_t ptr);

byte *memsearch(const void *mem, const void *search, size_t mem_len, size_t search_len);
int memread(pid_t pid, ptr_t start_address, size_t length,
            t_read_callback *on_page_read, void *data);
bool memreadall(pid_t pid, bool only_valid, t_read_callback *on_page_read, void *data);
bool readmaps(pid_t pid);
pid_t pidof(const char *pname);

#endif
