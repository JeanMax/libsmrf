#include "parse.h"

#include "proc.h"  // is_valid_ptr / memread
#include "util/list.h"
#include "util/malloc.h"

Level      *g_levels[MAX_LEVEL] = {0};


static void free_preset_list(PresetUnit *ptr)
{
    PresetUnit *prev = NULL;
    while (ptr) {
        prev = ptr;
        ptr = ptr->pNext;
        FREE(prev);
    }
}

static void free_room1_list(Room1 *ptr)
{
    Room1 *prev = NULL;
    while (ptr) {
        if (ptr->Coll) {
            FREE(ptr->Coll);
        }
        // done in the Htable refresh/cleanup
        /* free_unit_list(ptr->pUnitFirst); */

        prev = ptr;
        ptr = ptr->pNext;
        FREE(prev);
    }
}

static void free_room2_list(Room2 *ptr)
{
    Room2 *prev = NULL;
    while (ptr) {
        free_room1_list(ptr->pRoom1);
        free_preset_list(ptr->pPreset);

        prev = ptr;
        ptr = ptr->pNext;
        FREE(prev);
    }
}

void free_all_levels(void)
{
    for (int i = 0; i < MAX_LEVEL; i++) {
        // we have our own poor-man hash table for storing them
        /* free_level_list(g_levels[i]); */
        if (g_levels[i]) {
            free_room2_list(g_levels[i]->pRoom2First);
            FREE(g_levels[i]);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

static Room1 *room1_in_list(Room1 *r1, Room1 *r1_list)
{
    if (r1) {
        while (r1_list) {
            if (r1->dwPosX == r1_list->dwPosX
                && r1->dwPosY == r1_list->dwPosY
                && r1->dwSizeY == r1_list->dwSizeY
                && r1->dwSizeY == r1_list->dwSizeY
                && r1->dwPosXBig == r1_list->dwPosXBig
                && r1->dwPosYBig == r1_list->dwPosYBig
                && r1->dwSizeYBig == r1_list->dwSizeYBig
                && r1->dwSizeYBig == r1_list->dwSizeYBig) {
                return r1_list;
            }
            r1_list = r1_list->pNext;
        }
    }
    return NULL;
}

static Room2 *room2_in_list(Room2 *r2, Room2 *r2_list)
{
    if (r2) {
        while (r2_list) {
            if (r2->dwPosX == r2_list->dwPosX
                && r2->dwPosY == r2_list->dwPosY
                && r2->dwSizeY == r2_list->dwSizeY
                && r2->dwSizeY == r2_list->dwSizeY) {
                return r2_list;
            }
            r2_list = r2_list->pNext;
        }
    }
    return NULL;
}

static PresetUnit *preset_in_list(PresetUnit *pu, PresetUnit *pu_list)
{
    if (pu) {
        while (pu_list) {
            if (pu->dwPosX == pu_list->dwPosX
                && pu->dwPosY == pu_list->dwPosY
                && pu->dwType == pu_list->dwType
                && pu->dwTxtFileNo == pu_list->dwTxtFileNo) {
                return pu_list;
            }
            pu_list = pu_list->pNext;
        }
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////

static PresetUnit *parse_preset_list(ptr_t pu_addr, PresetUnit *pu_first)
{
    PresetUnit pu;
    PresetUnit *pu_prev = NULL, *pu_new;

    LAST_LINK(pu_first, pu_prev);
    while (is_valid_ptr(pu_addr)) {
        if (!memread(pu_addr, sizeof(PresetUnit),
                     find_PresetUnit_callback, &pu)) {
            LOG_WARNING("Can't find preset");
            break;
        }
        /* log_PresetUnit(&pu); */

        pu_new = preset_in_list(&pu, pu_first);
        if (!pu_new) {
            DUPE(pu_new, &pu, sizeof(PresetUnit));
            pu_new->pNext = NULL;
            ADD_LINK(pu_first, pu_prev, pu_new);
        }

        pu_addr = (ptr_t)pu.pNext;
    }

    return pu_first;
}

static Room1 *parse_room1_list(ptr_t r1_addr, Room1 *r1_first)
{
    Room1 r1;
    Room1 *r1_prev = NULL, *r1_new;

    LAST_LINK(r1_first, r1_prev);
    while (is_valid_ptr(r1_addr)) {
        if (!memread(r1_addr, sizeof(Room1),
                     find_Room1_callback, &r1)) {
            LOG_WARNING("Can't find room1");
            break;
        }
        /* log_Room1(&r1); */

        r1_new = room1_in_list(&r1, r1_first);
        if (!r1_new) {
            DUPE(r1_new, &r1, sizeof(Room1));
            r1_new->pNext = NULL;
            r1_new->Coll = NULL;
            r1_new->pUnitFirst = NULL;
            ADD_LINK(r1_first, r1_prev, r1_new);
        }

        r1_new->pUnitFirst = parse_unit_list((ptr_t)r1.pUnitFirst);
        /* r1_new->Coll = parse_collmap((ptr_t)r1.Coll); */

        r1_addr = (ptr_t)r1.pNext;
    }

    return r1_first;
}

static Room2 *parse_room2_list(ptr_t r2_addr, Room2 *r2_first)
{
    Room2 r2;
    Room2 *r2_prev = NULL, *r2_new;

    LAST_LINK(r2_first, r2_prev);
    while (is_valid_ptr(r2_addr)) {
        if (!memread(r2_addr, sizeof(Room2),
                     find_Room2_callback, &r2)) {
            LOG_WARNING("Can't find room2");
            break;
        }
        /* log_Room2(&r2); */

        r2_new = room2_in_list(&r2, r2_first);
        if (!r2_new) {
            DUPE(r2_new, &r2, sizeof(Room2));
            r2_new->pNext = NULL;
            r2_new->pRoom1 = NULL;
            r2_new->pPreset = NULL;
            ADD_LINK(r2_first, r2_prev, r2_new);
        }

        r2_new->pRoom1 = parse_room1_list((ptr_t)r2.pRoom1,
                                          r2_new->pRoom1);
        r2_new->pPreset = parse_preset_list((ptr_t)r2.pPreset,
                                            r2_new->pPreset);

        r2_addr = (ptr_t)r2.pNext;
    }

    return r2_first;
}

static Level *parse_level(ptr_t level_addr)
{
    Level *level_new;
    Level level;

    if (!is_valid_ptr(level_addr)
        ||!memread(level_addr, sizeof(Level),
                   find_Level_callback, &level)) {
        LOG_WARNING("Can't find level");
        return NULL;
    }
    /* log_Level(&level); */
    /* LOG_DEBUG("found Level ptr at %16jx", level_addr); /\* DEBUG *\/ */

    level_new = g_levels[level.dwLevelNo];
    if (!level_new) {
        DUPE(level_new, &level, sizeof(Level));
        level_new->pRoom2First = NULL;
        g_levels[level.dwLevelNo] = level_new;
    }
    level_new->pRoom2First = parse_room2_list((ptr_t)level.pRoom2First,
                                              level_new->pRoom2First);

    return level_new;
}

Level *parse_level_list(ptr_t level_addr)
{
    Level *level_first = NULL, *level_new;
    ptr_t next_level_addr = level_addr;

    /* // follow linked list */
    /* do { */
    /*     level_new = parse_level(next_level_addr); */
    /*     if (!level_new) { */
    /*         break; */
    /*     } */
    /*     if (!level_first) { */
    /*         level_first = level_new; */
    /*     } */
    /*     next_level_addr = (ptr_t)level_new->pNext; */
    /* } while (is_valid_ptr(next_level_addr)); */


    // just check current ptr
    level_new = parse_level(next_level_addr);
    if (!level_new) {
        LOG_ERROR("Can't parse 1st Level ptr");
        return NULL;
    }
    level_first = level_new;

    // consider it's a level[], go to next cell
    next_level_addr = level_addr + sizeof(Level);
    do {
        level_new = parse_level(next_level_addr);
        if (!level_new) {
            break;
        }
        next_level_addr += sizeof(Level);
    } while (is_valid_ptr(next_level_addr));

    // consider it's a level[], go to prev cell
    next_level_addr = level_addr - sizeof(Level);
    do {
        level_new = parse_level(next_level_addr);
        if (!level_new) {
            break;
        }
        next_level_addr -= sizeof(Level);
    } while (is_valid_ptr(next_level_addr));

    if (!level_first) {
        LOG_ERROR("Can't parse Level list");
    }
    return level_first;
}
