#ifndef _D2SDK_H
#define _D2SDK_H

#include "util/types.h"

#include "sdk/level.h"
#include "sdk/player.h"
#include "sdk/monster.h"
#include "sdk/object.h"
#include "sdk/missile.h"
#include "sdk/tile.h"

inline bool is_shrine(const ObjectInfo *obj_info)
{
    return obj_info->subClass == SUB_SHRINE
        || obj_info->subClass == SUB_WELL
        || obj_info->autoMap == MAP_SHRINE
        || obj_info->autoMap == MAP_WELL;
}

inline bool is_waypoint(const ObjectInfo *obj_info)
{
    return obj_info->subClass == SUB_WAYPOINT;
}

inline bool is_portal(const ObjectInfo *obj_info)
{
    return obj_info->subClass == SUB_PORTAL
        || obj_info->autoMap == MAP_PORTAL;
}

inline bool is_special_chest(const ObjectInfo *obj_info)
{
    return obj_info->autoMap == MAP_CHEST
        /* || obj_info->id == OBJ_BANK */
        || obj_info->id == OBJ_SPECIALCHEST100
        || obj_info->id == OBJ_CONSOLATIONCHEST
        || obj_info->id == OBJ_PLACEUNIQUECHEST
        || obj_info->id == OBJ_PLACERANDOMTREASURECHEST
        || obj_info->id == OBJ_PLACEARCANETHINGAMAJIG;
}

inline bool is_chest(const ObjectInfo *obj_info)
{
    return obj_info->subClass == SUB_CONTAINER
        || is_special_chest(obj_info);
}

inline bool is_quest(const ObjectInfo *obj_info)
{
    return obj_info->id != OBJ_TOWERTOME
        && (obj_info->autoMap == MAP_TOME
        || obj_info->autoMap == MAP_CAIN_STONE
        || obj_info->autoMap == MAP_INIFUSS_TREE
        || obj_info->autoMap == MAP_MALUS
        || obj_info->autoMap == MAP_GIBDIN
        || obj_info->autoMap == MAP_DIABLO_SEAL
        || obj_info->autoMap == MAP_COMPELLING_ORB
        || obj_info->autoMap == MAP_ALTAR
        || obj_info->autoMap == MAP_ANYA
        || obj_info->autoMap == MAP_CHEST
        || obj_info->autoMap == MAP_SEWER
        || obj_info->id == OBJ_SOULSTONEFORGE // Forge Hell
        || obj_info->id == OBJ_SEVENTOMBSRECEPTACLE // Holder For Horadric Staff
        || obj_info->id == OBJ_DREHYAOUTSIDETOWN // Drehya Outside Town
        || obj_info->id == OBJ_NIHLATHAKOUTSIDETOWN // Nihlathak Outside Town
        || obj_info->id == OBJ_CAGEDWUSSIE1 // Caged Fellow
            || is_special_chest(obj_info));
}

inline bool is_transit(const ObjectInfo *obj_info)
{
    return obj_info->autoMap == MAP_TRAP_DOOR
        || obj_info->autoMap == MAP_SEWER
        || obj_info->autoMap == MAP_STAIR_LEFT
        || obj_info->autoMap == MAP_STAIR_RIGHT
        /* || obj_info->id == OBJ_ACT1CATHEDRALSHRINE */
        /* || obj_info->id == OBJ_ACT1JAILSHRINE */
        /* || obj_info->id == OBJ_ACT1JAILHEALTHSHRINE */
        /* || obj_info->id == OBJ_ACT1JAILMANASHRINE */
        || obj_info->subClass == SUB_ARCANE
        || obj_info->id == OBJ_ANCIENTSGATEWAY
        || obj_info->id == OBJ_MAYBETHELASTONE // Baal's lair
        || obj_info->id == OBJ_FINALPORTAL
        || obj_info->id == OBJ_SUMMITDOOR
        || obj_info->id == OBJ_REALLYTHELASTPORTAL
        || obj_info->id == OBJ_NOTKIDDINGLASTPORTAL
        || obj_info->id == OBJ_DUMMYEX107;  // Baal portal
}

inline bool is_npc(const MonsterInfo *mon_info)
{
    return mon_info->npc;
}

inline bool is_ally(const MonsterInfo *mon_info)
{
    return mon_info->threat == THREAT_MINION && mon_info->inTown;
}

inline bool is_a_threat(const MonsterInfo *mon_info)
{
    return mon_info->threat >= THREAT_MONSTER && !is_ally(mon_info);
}

/**
 * this is for preset type 1 with dwTxtFileNo > MAX_MONSTER + MAX_SUPER
 * -> usually boss / champions, but can be other weird preset "monstery" stuff
 */
enum WeirdPresetMonsterId { //TODO: move
    FIRST_WEIRD = MAX_MONSTER + MAX_SUPER - 1,
    DARK_WOOD_FALLEN,
    // ?
    RIVER_STALKER = FIRST_WEIRD + 11,
    SEWER_STALKER,
    // ?
    FALLEN_SHAMAN1 = FIRST_WEIRD + 18,
    FALLEN_SHAMAN2,
    // ?
    CATAPULT_IMPACT1 = FIRST_WEIRD + 22,  // don't trust the name
    DEAD_BARB,
    RATMEN_SHAMAN,
    CATAPULT_IMPACT2,  // don't trust the name
    CATAPULT_IMPACT3,  // don't trust the name
    CATAPULT_IMPACT4,  // don't trust the name
    CATAPULT_IMPACT5,  // don't trust the name
    CATAPULT_IMPACT6,  // don't trust the name
    PINDLE_FRIENDS,
    GREMLIN1,
    GREMLIN2,
    PINDLE_FRIENDS2,
    UNKNOW_MEPH_STUFF0,
    UNKNOW_MEPH_STUFF1,
    UNKNOW_MEPH_STUFF2,
    UNKNOW_MEPH_STUFF3,
};
typedef enum WeirdPresetMonsterId  WeirdPresetMonsterId;


/**
 * for beyond super monster (`dwTxtFileNo` preset type 1)
 * @see `enum WeirdPresetMonsterId`
 */
inline bool is_weird_preset_monster(dword id)
{
    return id == DARK_WOOD_FALLEN
        || id == FALLEN_SHAMAN1
        || id == FALLEN_SHAMAN2
        || id == RIVER_STALKER
        || id == SEWER_STALKER
        || id == DEAD_BARB
        || id == RATMEN_SHAMAN
        || id == CATAPULT_IMPACT1
        || id == CATAPULT_IMPACT2
        || id == CATAPULT_IMPACT3
        || id == CATAPULT_IMPACT4
        || id == CATAPULT_IMPACT5
        || id == CATAPULT_IMPACT6
        || id == PINDLE_FRIENDS
        || id == GREMLIN1
        || id == GREMLIN2
        || id == PINDLE_FRIENDS2
        /* || id == UNKNOW_MEPH_STUFF0 */
        || id == UNKNOW_MEPH_STUFF1
        || id == UNKNOW_MEPH_STUFF2
        || id == UNKNOW_MEPH_STUFF3
        ;
    /* return id == 149 // rock */
    /*     || id == 152 // rogue */
    /*     || id == 179 // cow */
    /*     || id == 195 // lut gholein stuff */
    /*     || id == 196 // lut gholein tree */
    /*     || id == 197 // lut gholein stuff */
    /*     || id == 203 // lut gholein npc */
    /*     || id == 205 // lut gholein npc */
    /*     || id == 273 // gargoyle trap */
    /*     || id == 294 // kurast stuff */
    /*     || id == 296 // kurast stuff */
    /*     || id == 327 // a2 tomb trap */
    /*     || id == 328 // a2 tomb trap */
    /*     || id == 372 // Bonepile (act2 trap) */
    /*     || id == 359 // iron wolf */
    /*     || id == 432 // a5 door */
    /*     || id == 433 // a5 door */
    /*     || id == 434 // a5 prison barb */
    /*     || id == 435 // a5 tower */
    /*     || id == 497 // a5 tower */
    /*     || id == 498 // a5 catapult */
    /*     // || id == 513 // drehya */
    /*     || id == 516 // a5 tree */
    /*     || id == 517 // a5 flames */
    /*     || id == 522 // barb */
    /*     || id == 524 // a5 door */
    /*     || id == 525 // a5 door */
    /*     || id == 528 // a5 demon hut */
    /*     // || id == 563 // baal tentacle */
    /*     // || id == 564 // baal tentacle */
    /*     // || id == 565 // baal tentacle */
    /*     // || id == 566 // baal tentacle */
    /*     || id == 567 // baal tentacle */
    /*     || id == 568 // injuried barb */
    /*     || id == 569 // injuried barb */
    /*     || id == 570 // injuried barb */
    /*     // || id == 571 // baal crab clone */
    /*     // || id == 575 // worldstone effect */
}

// for preset unit with type 5
inline bool is_backward_tile(const UniqueTileInfo *utile_info)
{
    return utile_info->notUniqueId == TILE_ACT_1_CAVE_DOWN
        || utile_info->notUniqueId == TILE_ACT_1_CRYPT_UP
        || utile_info->notUniqueId == TILE_ACT_1_TOWER_TO_WILDERNESS
        || utile_info->notUniqueId == TILE_ACT_1_WILDERNESS_TO_CAVE_FLOOR_L
        || utile_info->notUniqueId == TILE_ACT_1_JAIL_UP
        // TODO: this doesn't work well for jail
        /* || utile_info->notUniqueId == TILE_ACT_1_JAIL_DOWN */
        || utile_info->notUniqueId == TILE_ACT_1_CATACOMBS_TO_CATHEDRAL
        || utile_info->notUniqueId == TILE_ACT_1_CATACOMBS_UP
        || utile_info->notUniqueId == TILE_ACT_2_SEWER_DOCK_TO_TOWN
        || utile_info->notUniqueId == TILE_ACT_2_SEWER_UP
        || utile_info->notUniqueId == TILE_ACT_2_HAREM_TO_TOWN
        || utile_info->notUniqueId == TILE_ACT_2_HAREM_UP_1
        || utile_info->notUniqueId == TILE_ACT_2_HAREM_UP_2
        || utile_info->notUniqueId == TILE_ACT_2_BASEMENT_UP_1
        || utile_info->notUniqueId == TILE_ACT_2_BASEMENT_UP_2
        || utile_info->notUniqueId == TILE_ACT_2_TOMB_UP
        || utile_info->notUniqueId == TILE_ACT_2_LAIR_UP
        || utile_info->notUniqueId == TILE_ACT_3_SPIDER_TO_JUNGLE
        || utile_info->notUniqueId == TILE_ACT_3_DUNGEON_UP
        || utile_info->notUniqueId == TILE_ACT_3_KURAST_TO_SEWER
        || utile_info->notUniqueId == TILE_ACT_3_SEWER_UP_L
        || utile_info->notUniqueId == TILE_ACT_3_SEWER_UP_R
        || utile_info->notUniqueId == TILE_ACT_3_TEMPLE_UP_L
        || utile_info->notUniqueId == TILE_ACT_3_TEMPLE_UP_R
        || utile_info->notUniqueId == TILE_ACT_3_MEPHISTO_UP_L
        || utile_info->notUniqueId == TILE_ACT_3_MEPHISTO_UP_R
        || utile_info->notUniqueId == TILE_ACT_4_LAVA_TO_MESA
        || utile_info->notUniqueId == TILE_ACT_5_ICE_CAVES_UP_L
        || utile_info->notUniqueId == TILE_ACT_5_ICE_CAVES_DOWN_FLOOR
        || utile_info->notUniqueId == TILE_ACT_5_TEMPLE_UP
        || utile_info->notUniqueId == TILE_ACT_5_BAAL_TEMPLE_UP_L;
}


//TODO: move
enum MonsterFlag {
    MONSTER_NORMAL = 1 << 0,
    MONSTER_SUPER = 1 << 1,
    MONSTER_CHAMP = 1 << 2,
    MONSTER_BOSS = 1 << 3,
    MONSTER_MINION = 1 << 4,
    MONSTER_POSSESSED = 1 << 5,
    MONSTER_GHOSTLY = 1 << 6,
    MONSTER_MULTISHOT = 1 << 7,
    /* MONSTER_SUPER = MONSTER_BOSS | MONSTER_NORMAL, */
    //TODO: for super, check dwTxtFileNo id match a given list?
/*     int ids[] = { */
/*         58, 59, 60, 61, 62, */
/*         101, 102, 103, 104, 105, */
/*         278, 279, 280, 281, 282, */
/*         298, 299, 300, */
/*         645, 646, 647, */
/*         662, 663, 664, */
/*         667, 668, 669, 670, */
/*         675, 676 */
/*     }; */
};
typedef enum MonsterFlag  MonsterFlag;



/* enum collision_flag { */
/*     COL_NONE = 0x0000, */
/*     COL_BLOCKWALK = 0x0001, */
/*     COL_BLOCKLINEOFSIGHT = 0x0002, */
/*     COL_WALL = 0x0004, */
/*     COL_BLOCKPLAYER = 0x0008, */
/*     COL_ALTERNATETILE = 0x0010, */
/*     COL_BLANK = 0x0020, */
/*     COL_MISSILE = 0x0040, */
/*     COL_PLAYER = 0x0080, */
/*     COL_NPCLOCATION = 0x0100, */
/*     COL_ITEM = 0x0200, */
/*     COL_OBJECT = 0x0400, */
/*     COL_CLOSEDDOOR = 0x0800, */
/*     COL_NPCCOLLISION = 0x1000, */
/*     COL_FRIENDLYNPC = 0x2000, */
/*     COL_UNKNOWN = 0x4000, */
/*     COL_DEADBODY = 0x8000, // also portal */
/*     COL_THICKENEDWALL = 0xfefe, */
/*     COL_AVOID = 0xffff */
/* }; */

enum UnitType {
    UNIT_PLAYER = 0,
    UNIT_MONSTER,
    UNIT_OBJECT,
    UNIT_MISSILE,
    UNIT_ITEM,
    UNIT_TILE,
    MAX_UNIT,
};
typedef enum UnitType  UnitType;


#endif
