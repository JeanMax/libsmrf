#ifndef _PARSE_H
#define _PARSE_H

#include "d2structs.h"
#include "d2sdk.h"
#include "util/hash.h"

#define MAX_UNIT_ADDR 8

typedef  struct UnitWithAddr  UnitWithAddr;
struct UnitWithAddr {
    UnitAny unit;
    ptr_t unit_addr[MAX_UNIT_ADDR];
};

extern Level  *g_levels[MAX_LEVEL];

void free_all_levels(void);
Level *parse_level_list(ptr_t level_addr);


#endif
