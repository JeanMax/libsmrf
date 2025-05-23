#include "parse.h"

#include "proc.h"  // is_valid_ptr / memread
#include "util/list.h"


Level      *g_levels[MAX_AREA] = {0};


static void free_preset(PresetUnit *ptr)
{
    PresetUnit *prev = NULL;
    while (ptr) {
        prev = ptr;
        ptr = ptr->pNext;
        FREE(prev);
    }
}

static void free_unit(UnitAny *ptr)
{
    UnitAny *prev = NULL;
    while (ptr) {
        if (ptr->pAct) {
            FREE(ptr->pAct);
        }
        if (ptr->pPath) {
            FREE(ptr->pPath);
        }

        prev = ptr;
        ptr = ptr->pNext;
        FREE(prev);
    }
}

static void free_room1(Room1 *ptr)
{
    Room1 *prev = NULL;
    while (ptr) {
        if (ptr->Coll) {
            FREE(ptr->Coll);
        }
        free_unit(ptr->pUnitFirst);

        prev = ptr;
        ptr = ptr->pNext;
        FREE(prev);
    }
}

static void free_room2(Room2 *ptr)
{
    Room2 *prev = NULL;
    while (ptr) {
        free_room1(ptr->pRoom1);
        free_preset(ptr->pPreset);

        prev = ptr;
        ptr = ptr->pNext;
        FREE(prev);
    }
}

static void free_level(Level *ptr)
{
    Level *prev = NULL;
    while (ptr) {
        free_room2(ptr->pRoom2First);

        prev = ptr;
        ptr = ptr->pNext;
        FREE(prev);
    }
}

void free_all_levels(void)
{
    for (int i = 0; i < MAX_AREA; i++) {
        free_level(g_levels[i]);
        g_levels[i] = NULL;
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
                && r2->dwSizeY == r2_list->dwSizeY
                && r2->dwPresetType == r2_list->dwPresetType) {
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

static UnitAny *unit_in_list(UnitAny *u, UnitAny *u_list)
{
    if (u) {
        while (u_list) {
            if (u->dwUnitId == u_list->dwUnitId
                && u->dwType == u_list->dwType
                && u->dwTxtFileNo == u_list->dwTxtFileNo) {
                return u_list;
            }
            u_list = u_list->pNext;
        }
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////

static UnitAny *parse_unit_list(pid_t pid, ptr_t u_addr, UnitAny *u_first)
{
    UnitAny u;
    UnitAny *u_prev = NULL, *u_new = NULL;
    Act act;
    Path path;
    /* int i = 0;                  /\* DEBUG *\/ */

    LAST_LINK(u_first, u_prev);
    while (is_valid_ptr(u_addr)) {
        if (!memread(pid, u_addr, sizeof(UnitAny),
                     find_UnitAny_callback, &u)) {
            LOG_WARNING("Can't find unit");
            break;
        }
        /* i++;              /\* DEBUG *\/ */
        log_UnitAny(&u);

        if (!u.pAct || !memread(pid, (ptr_t)u.pAct, sizeof(Act),
                                find_Act_callback, &act)) {
            LOG_WARNING("Can't validate unit's Act");
            u.pAct = NULL;
        } else  {
            /* log_Act(&act); */
            DUPE(u.pAct, &act, sizeof(Act));
        }

        if (!u.pPath || !memread(pid, (ptr_t)u.pPath, sizeof(Path),
                                 find_Path_callback, &path)) {
            LOG_WARNING("Can't validate unit's Path");
            u.pPath = NULL;
        } else {
            /* log_Path(&path); */
            DUPE(u.pPath, &path, sizeof(Path));
        }

        u_addr = (ptr_t)u.pNext;  // we'll use u.pNext as tmp later

        u_new = unit_in_list(&u, u_first);  // TODO: it doesn't work if the unit change room1
        if (!u_new) {
            DUPE(u_new, &u, sizeof(UnitAny));
            u_new->pNext = NULL;
            ADD_LINK(u_first, u_prev, u_new);
        } else {
            if (u_new->pAct) {
                FREE(u_new->pAct);
            }
            if (u_new->pPath) {
                FREE(u_new->pPath);
            }
            u.pNext = u_new->pNext;
            memcpy(u_new, &u, sizeof(UnitAny));
            //TODO: for all the ones you didn't refresh and that are still in list
            //      you may want to re-read the adress of the unit...
        }

    }

    /* LOG_DEBUG("%d unit found", i); */

    return u_first;
}

static PresetUnit *parse_preset_list(pid_t pid, ptr_t pu_addr, PresetUnit *pu_first)
{
    PresetUnit pu;
    PresetUnit *pu_prev = NULL, *pu_new;
    /* int i = 0;                  /\* DEBUG *\/ */

    LAST_LINK(pu_first, pu_prev);
    while (is_valid_ptr(pu_addr)) {
        if (!memread(pid, pu_addr, sizeof(PresetUnit),
                     find_PresetUnit_callback, &pu)) {
            LOG_WARNING("Can't find preset");
            break;
        }
        /* i++;              /\* DEBUG *\/ */
        /* log_PresetUnit(&pu); */
        /* LOG_DEBUG("(%x, %x) *", pu.dwPosX, pu.dwPosY); */

        pu_new = preset_in_list(&pu, pu_first);
        if (!pu_new) {
            DUPE(pu_new, &pu, sizeof(PresetUnit));
            pu_new->pNext = NULL;
            ADD_LINK(pu_first, pu_prev, pu_new);
        }

        pu_addr = (ptr_t)pu.pNext;
    }

    /* LOG_DEBUG("%d preset found", i); */

    return pu_first;
}

static Room1 *parse_room1_list(pid_t pid, ptr_t r1_addr, Room1 *r1_first)
{
    Room1 r1;
    Room1 *r1_prev = NULL, *r1_new;
    /* int i = 0;                  /\* DEBUG *\/ */

    LAST_LINK(r1_first, r1_prev);
    while (is_valid_ptr(r1_addr)) {
        if (!memread(pid, r1_addr, sizeof(Room1),
                     find_Room1_callback, &r1)) {
            LOG_WARNING("Can't find room1");
            break;
        }
        /* i++;              /\* DEBUG *\/ */
        /* log_Room1(&r1); */
        /* DEBUG */

        r1_new = room1_in_list(&r1, r1_first);
        if (!r1_new) {
            DUPE(r1_new, &r1, sizeof(Room1));
            r1_new->pNext = NULL;
            r1_new->Coll = NULL;
            r1_new->pUnitFirst = NULL;
            ADD_LINK(r1_first, r1_prev, r1_new);
        }

        r1_new->pUnitFirst = parse_unit_list(pid,
                                            (ptr_t)r1.pUnitFirst,
                                            r1_new->pUnitFirst);

        /* r1_new->Coll = parse_collmap(pid, (ptr_t)r1.Coll); */

        r1_addr = (ptr_t)r1.pNext;
    }

    /* LOG_DEBUG("%d room1 found", i); */

    return r1_first;
}

static Room2 *parse_room2_list(pid_t pid, ptr_t r2_addr, Room2 *r2_first)
{
    Room2 r2;
    Room2 *r2_prev = NULL, *r2_new;
    /* int i = 0;                  /\* DEBUG *\/ */

    LAST_LINK(r2_first, r2_prev);
    while (is_valid_ptr(r2_addr)) {
        if (!memread(pid, r2_addr, sizeof(Room2),
                     find_Room2_callback, &r2)) {
            LOG_WARNING("Can't find room2");
            break;
        }
        /* i++;              /\* DEBUG *\/ */
        /* log_Room2(&r2); */
        /* LOG_DEBUG("(%x, %x)", r2.dwPosX, r2.dwPosY); */

        r2_new = room2_in_list(&r2, r2_first);
        if (!r2_new) {
            DUPE(r2_new, &r2, sizeof(Room2));
            r2_new->pNext = NULL;
            r2_new->pRoom1 = NULL;
            r2_new->pPreset = NULL;
            ADD_LINK(r2_first, r2_prev, r2_new);
        }

        r2_new->pRoom1 = parse_room1_list(pid,
                                          (ptr_t)r2.pRoom1,
                                          r2_new->pRoom1);
        r2_new->pPreset = parse_preset_list(pid,
                                            (ptr_t)r2.pPreset,
                                            r2_new->pPreset);

        r2_addr = (ptr_t)r2.pNext;
    }

    /* LOG_DEBUG("%d room2 found", i); */

    return r2_first;
}

Level *parse_level_list(pid_t pid, ptr_t level_addr)
{
    Level level;
    Level *level_first = NULL, *level_prev, *level_new;
    /* int i = 0;                  /\* DEBUG *\/ */

    while (is_valid_ptr(level_addr)) {
        if (!memread(pid, level_addr, sizeof(Level),
                     find_Level_callback, &level)) {
            LOG_WARNING("Can't find level");
            break;
        }
        /* i++;              /\* DEBUG *\/ */
        log_Level(&level);
        /* LOG_DEBUG("(%x, %x)", level.dwPosX, level.dwPosY); */

        level_new = g_levels[level.dwLevelNo];
        if (level_new) { // not new eh
            if (!level_first) { //current lvl
                level_new->pRoom2First = parse_room2_list(pid,
                                                          (ptr_t)level.pRoom2First,
                                                          level_new->pRoom2First);
            }
        } else {
            DUPE(level_new, &level, sizeof(Level));
            level_new->pNext = NULL;
            g_levels[level.dwLevelNo] = level_new;
            level_new->pRoom2First = parse_room2_list(pid,
                                                      (ptr_t)level.pRoom2First,
                                                      NULL);
        }

        ADD_LINK(level_first, level_prev, level_new);
        level_addr = (ptr_t)level.pNext;
    }

    /* LOG_DEBUG("%d level found", i); */

    return level_first;
}





		/* case "physical": */
		/* 	return unit.getStat(36); */
		/* case "fire": */
		/* 	return unit.getStat(39); */
		/* case "lightning": */
		/* 	return unit.getStat(41); */
		/* case "magic": */
		/* 	return unit.getStat(37); */
		/* case "cold": */
		/* 	return unit.getStat(43); */
		/* case "poison": */
        /*         return unit.getStat(45); */



		/* switch (unit.classid) { */
		/* case 179: // An evil force - cow (lol) */
		/* 	return false; */
		/* case 543: // Baal in Throne */
		/* 	if (me.area === 131) { */
		/* 		return false; */
		/* 	} */

		/* 	break; */
		/* case 110: // Vultures */
		/* case 111: */
		/* case 112: */
		/* case 113: */
		/* case 114: */
		/* case 608: */
		/* 	if (unit.mode === 8) { // Flying */
		/* 		return false; */
		/* 	} */

		/* 	break; */
		/* case 68: // Sand Maggots */
		/* case 69: */
		/* case 70: */
		/* case 71: */
		/* case 72: */
		/* case 679: */
		/* case 258: // Water Watchers */
		/* case 259: */
		/* case 260: */
		/* case 261: */
		/* case 262: */
		/* case 263: */
		/* 	if (unit.mode === 14) { // Submerged/Burrowed */
		/* 		return false; */
		/* 	} */

/* if (unit.type === 0 // player */
/*         && unit.mode !== 17) { // not dead */

/* if (unit.hp === 0 || unit.mode === 0 || unit.mode === 12) { // Dead monster */

/* ids = ["chest", "chest3", "weaponrack", "armorstand"]; */
			/* chestIds = [ */
			/* 	5, 6, 87, 104, 105, 106, 107, 143, 140, 141, 144, 146, 147, 148, 176, 177, 181, 183, 198, 240, 241, */
			/* 	242, 243, 329, 330, 331, 332, 333, 334, 335, 336, 354, 355, 356, 371, 387, 389, 390, 391, 397, 405, */
			/* 	406, 407, 413, 420, 424, 425, 430, 431, 432, 433, 454, 455, 501, 502, 504, 505, 580, 581 */
/* ]; */
			/* containers = [ */
			/* 	"chest", "loose rock", "hidden stash", "loose boulder", "corpseonstick", "casket", "armorstand", "weaponrack", "barrel", "holeanim", "tomb2", */
			/* 	"tomb3", "roguecorpse", "ratnest", "corpse", "goo pile", "largeurn", "urn", "chest3", "jug", "skeleton", "guardcorpse", "sarcophagus", "object2", */
			/* 	"cocoon", "basket", "stash", "hollow log", "hungskeleton", "pillar", "skullpile", "skull pile", "jar3", "jar2", "jar1", "bonechest", "woodchestl", */
			/* 	"woodchestr", "barrel wilderness", "burialchestr", "burialchestl", "explodingchest", "chestl", "chestr", "groundtomb", "icecavejar1", "icecavejar2", */
			/* 	"icecavejar3", "icecavejar4", "deadperson", "deadperson2", "evilurn", "tomb1l", "tomb3l", "groundtombl" */
			/* ]; */
/* unit = getUnit(2); */



	/* shrine = getUnit(2, "shrine"); */
	/* switch (Config.ScanShrines[i]) { */
	/* 			case 0: // None */
	/* 			case 1: // Refilling */
	/* 			case 2: // Health */
	/* 			case 3: // Mana */
	/* 			case 4: // Health Exchange (doesn't exist) */
	/* 			case 5: // Mana Exchange (doesn't exist) */
	/* 			case 16: // Enirhs (doesn't exist) */
	/* 			case 17: // Portal */
	/* 			case 18: // Gem */
	/* 			case 19: // Fire */
	/* 			case 20: // Monster */
	/* 			case 21: // Exploding */
	/* 			case 22: // Poison */
	/* 				this.shrineStates[i] = 0; // no state */

	/* 				break; */
	/* 			case 6: // Armor */
	/* 			case 7: // Combat */
	/* 			case 8: // Resist Fire */
	/* 			case 9: // Resist Cold */
	/* 			case 10: // Resist Lightning */
	/* 			case 11: // Resist Poison */
	/* 			case 12: // Skill */
	/* 			case 13: // Mana recharge */
	/* 			case 14: // Stamina */
	/* 			case 15: // Experience */
	/* 				// Both states and shrines are arranged in same order with armor shrine starting at 128 */
	/* 				this.shrineStates[i] = Config.ScanShrines[i] + 122; */


/* return me.getStat(12) === 99 ? 0 : (((me.getStat(13) -
 * this.totalExp[me.getStat(12)]) / this.nextExp[me.getStat(12)]) *
 * 100).toFixed(2);  // Misc.js for more */

/* for items check pickit.js checkItem() */

	/* getScarinessLevel: function (unit) { */
	/* 	var scariness = 0, ids = [58, 59, 60, 61, 62, 101, 102, 103, 104, 105, 278, 279, 280, 281, 282, 298, 299, 300, 645, 646, 647, 662, 663, 664, 667, 668, 669, 670, 675, 676]; */

	/* 	// Only handling monsters for now */
	/* 	if (unit.type !== 1) { */
	/* 		return undefined; */
	/* 	} */

	/* 	// Minion */
	/* 	if (unit.spectype & 0x08) { */
	/* 		scariness += 1; */
	/* 	} */

	/* 	// Champion */
	/* 	if (unit.spectype & 0x02) { */
	/* 		scariness += 2; */
	/* 	} */

	/* 	// Boss */
	/* 	if (unit.spectype & 0x04) { */
	/* 		scariness += 4; */
	/* 	} */

	/* 	// Summoner or the like */
	/* 	if (ids.indexOf(unit.classid) > -1) { */
	/* 		scariness += 8; */
	/* 	} */

	/* 	return scariness; */
	/* }, */
