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


typedef bool t_read_callback(byte *buf, size_t buf_len, ptr_t address, void *data);


#define is_valid_ptr__quick(ptr) (IS_ALIGNED(ptr)           \
                                  && ptr >= 0x100000        \
                                  && ptr <= 0x7fffffffffff)


bool is_valid_ptr(ptr_t ptr);

byte *memsearch(const void *mem, const void *search, size_t mem_len, size_t search_len);
int memread(pid_t pid, ptr_t start_address, size_t length,
            t_read_callback *on_page_read, void *data);
bool memreadall(pid_t pid, bool quick, t_read_callback *on_page_read, void *data);
bool readmaps(pid_t pid);
pid_t pidof(const char *pname);

#endif
