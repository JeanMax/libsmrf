#ifndef _PROC_H
#define _PROC_H

#include <string.h>

#include "util/types.h"
#include "util/log.h"


typedef  struct MapAddress  MapAddress;
struct MapAddress {
    PTR start;
    PTR end;
};


typedef BOOL t_read_callback(BYTE *buf, size_t buf_len, PTR address, void *data);


#define MAP_PREFIX 0x7fff
/* #define MIN_MAP_LEN  0x10000 */
/* #define MIN_MAP_ADDR 0x10000000 */
#define MIN_MAP_LEN  0x1
#define MIN_MAP_ADDR 0x1

#define fast_is_valid_ptr(ptr) \
    (IS_ALIGNED(ptr) && (PTR)(ptr) >> 32 == MAP_PREFIX)
BOOL is_valid_ptr(PTR ptr);

BYTE *memsearch(const void *mem, const void *search, size_t mem_len, size_t search_len);
int memread(pid_t pid, PTR start_address, size_t length,
            t_read_callback *on_page_read, void *data);
BOOL memreadall(pid_t pid, BOOL only_valid, t_read_callback *on_page_read, void *data);
BOOL readmaps(pid_t pid);
pid_t pidof(const char*pname);

#endif
