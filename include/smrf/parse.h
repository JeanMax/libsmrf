#ifndef _PARSE_H
#define _PARSE_H

#include "d2structs.h"
#include "d2sdk.h"



extern Level      *g_levels[MAX_AREA];

void free_all_levels(void);
Level *parse_level_list(pid_t pid, ptr_t level_addr);


#endif
