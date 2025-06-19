#ifndef _PARSE_H
#define _PARSE_H

#include "d2structs.h"
#include "util/hash.h"

// parse_level.c
extern Level  *g_levels[MAX_LEVEL];

void free_all_levels(void);
Level *parse_level_list(ptr_t level_addr);


// parse_unit.c
extern Htable *g_unit_table;  //TODO: berk

#define MAX_UNIT_ADDR 8

typedef  struct UnitWithAddr  UnitWithAddr;
struct UnitWithAddr {
    UnitAny unit;
    ptr_t unit_addr[MAX_UNIT_ADDR];
};

UnitAny *parse_unit_list(ptr_t u_addr);

Player *parse_unit_table(ptr_t ut_addr);

void delete_unit_callback(size_t unused_key, void *node_value);
bool update_unit_callback(void *node_value, void *data);

bool deep_validate_Player(Player *maybe_player);

#endif
