#ifndef _PARSE_H
#define _PARSE_H

#include "d2structs.h"
#include "d2sdk.h"
#include "util/hash.h"


typedef  struct UnitWithAddr  UnitWithAddr;
struct UnitWithAddr {
    UnitAny unit;
    ptr_t unit_addr;
};

extern Htable *g_unit_table;
extern Level  *g_levels[MAX_AREA];

void free_all_levels(void);
Level *parse_level_list(pid_t pid, ptr_t level_addr);


#endif
