#ifndef _D2SDK_H
#define _D2SDK_H

#include "util/types.h"

inline dword act_from_area(dword area)
{
    return area < 40 ? 1 :
           (area < 75 ? 2 :
            (area < 103 ? 3 :
             (area < 109 ? 4 : 5)));
}

// PRESETS
inline bool is_shrine(dword id)
{
    return id == 2   // Shrine
        || id == 77  // Healthorama
        || id == 81  // Forest Altar
        || id == 83  // Shrine
        || id == 84  // HealingWell
        || id == 85  // HealthShrine
        || id == 86  // Shrine
        || id == 93  // ManaShrine
        || id == 96  // HealthShrine
        || id == 97  // MagicShrine
        || id == 109 // Shrine HealthR
        || id == 111 // Fountain 1
        || id == 113 // Fountain 2 Well
        || id == 115 // Fountain 3
        || id == 116 // Snakewoman Shrine
        || id == 118 // Fountain 4
        || id == 120 // Healthshrine Dungeun
        || id == 123 // Innershrinehell2
        || id == 124 // Innershrinehell3
        || id == 130 // Pool Wilderness
        || id == 132 // Cathedralwell Inside
        || id == 133 // Shrine
        || id == 134 // Shrine
        || id == 135 // Shrine
        || id == 136 // Shrine
        || id == 137 // Desertwell Well Desert Tom
        || id == 138 // Cavewell Caves
        || id == 150 // Shrine1
        || id == 151 // Shrine4
        || id == 164 // Mana Well1
        || id == 165 // Mana Well2
        || id == 166 // Mana Well3 Tomb
        || id == 167 // Mana Well4 Harom
        || id == 168 // Mana Well5
        || id == 170 // Jungle Healwell
        || id == 172 // Health Well/shrine Desert
        || id == 173 // Mana Well/shrine Desert
        || id == 184 // Jungle Shrine2
        || id == 190 // Jungle Shrine3
        || id == 191 // Jungle Shrine4
        || id == 197 // Jungleshrine
        || id == 199 // Mafistoshrine
        || id == 200 // Mafistoshrine
        || id == 201 // Mafistoshrine
        || id == 202 // Mafistomana
        || id == 206 // Mafistohealth
        || id == 226 // Outershrinehell
        || id == 231 // Outershrinehell2
        || id == 232 // Outershrinehell3
        || id == 236 // Health Wellforhell
        || id == 249 // HellManaWell
        || id == 260 // Holyshrine
        || id == 275 // Health Shrine
        || id == 276 // Mana Shrine
        || id == 277 // Magic Shrine
        || id == 278 // Mana Shrine
        || id == 279 // Magic Shrine
        || id == 280 // Healthwell
        || id == 281 // Manawell
        || id == 282 // Shrine
        || id == 299 // MagicShrine
        || id == 300 // MagicShrine
        || id == 301 // HealthWell
        || id == 302 // ManaWell
        || id == 303 // MagicShrine
        || id == 319 // HealthWell
        || id == 320 // HealthWell
        || id == 322 // TombWell
        || id == 325 // MagicShrine
        || id == 343 // ManaWell
        || id == 344 // HealthWell
        || id == 361 // MagicShrine
        || id == 414 // Shrine
        || id == 415 // Shrine
        || id == 421 // Shrine 3
        || id == 422 // Mana Shrine
        || id == 423 // Health Shrine
        || id == 426 // Well
        || id == 427 // Shrine2
        || id == 428 // Shrine2
        || id == 464 // Health Shrine
        || id == 465 // Mana Shrine
        || id == 472 // IcecaveShrine2
        || id == 479 // IcecaveShrine2
        || id == 483 // ManaShrine
        || id == 484 // BaalHealthShrine
        || id == 488 // BaalMagicShrine
        || id == 491 // ManaShrine
        || id == 492 // HealthShrine
        || id == 493 // Well
        || id == 495 // Shrine3
        || id == 497 // Shrine3
        || id == 498 // BaalWell
        || id == 499 // BaalMagicShrine2
        || id == 503 // BaalMagicShrine3
        || id == 509 // TempleMagicShrine
        || id == 512 // TempleMagicShrine2
        || id == 513 // TempleWell
        || id == 519 // IcecaveWell
        || id == 520 // TempleMagicShrine3
        || id == 521 // TempleHealthShrine
        || id == 522 // TempleManaShrine
        || id == 539; // TempleWaypoint
        // || id == 12  // Rogue Fountain
        // || id == 31  // Fountain
}

inline bool is_quest(dword id)
{
    return id == 8   // Tower Tome
        || id == 17  // StoneAlpha
        || id == 18  // StoneBeta
        || id == 19  // StoneGamma
        || id == 20  // StoneDelta
        || id == 21  // StoneLambda
        || id == 22  // StoneTheta
        || id == 30  // Inifuss Tree
        || id == 108 // Malus
        || id == 152 // Holder For Horadric Staff
        || id == 193 // Lam Esen's Tome
        || id == 251 // Gidbinn Altar
        || id == 252 // Gidbinn Decoy
        || id == 354 // Horadric Cube Chest
        || id == 355 // Horadric Scroll Chest
        || id == 356 // Staff Of Kings Chest
        || id == 357 // Horazon's Journal
        || id == 376 // Forge Hell
        || id == 392 // Diablo Seal1
        || id == 393 // Diablo Seal2
        || id == 394 // Diablo Seal3
        || id == 395 // Diablo Seal4
        || id == 396 // Diablo Seal5
        || id == 404 // Compelling Orb
        || id == 405 // Khalim Chest
        || id == 406 // Khalim Chest
        || id == 407 // Khalim Chest
        || id == 458 // Temple Altar
        || id == 460 // Drehya Outside Town
        || id == 462 // Nihlathak Outside Town
        // || id == 523 //  Blacksmith // it's larzuk forge eh
        || id == 473 // Caged Fellow
        || id == 546 // AncientsAltar
        || id == 558 // Frozen Anya
        || id == 742; // tristram portal
}

inline bool is_waypoint(dword id)
{
    return id == 119 // Waypoint Portal
        || id == 145 // Waypointi Inner Hell
        || id == 156 // Waypoint
        || id == 157 // WildernessWaypoint
        || id == 237 // Act3Waypoint Town
        || id == 238 // Waypointh
        || id == 288 // Waypoint Celler
        || id == 323 // SewerWaypoint
        || id == 324 // TravincalWaypoint
        || id == 398 // PandamoniaWaypoint
        || id == 429 // Waypoint
        || id == 398 // PandamoniaWaypoint
        || id == 402 // VallyWaypoint
        || id == 494 // BaalWaypoint
        || id == 496 // WildernessWaypoint
        || id == 511; // IcecaveWaypoint
}

inline bool is_transit(dword id)
{
    return id == 59  // Town Portal
        || id == 74  // Trap Door
        || id == 60  // Permanent Town Portal
        || id == 194 // Stairsl
        || id == 195 // Stairsr
        // || id == 244 // Sewers
        // || id == 246 // Sewers
        || id == 262 // Cathedral
        || id == 263 // Jail
        || id == 264 // Jail
        || id == 265 // Jail
        || id == 298 // Arcane Portal
        || id == 342 // Hellgate
        || id == 366 // Sewer Quest Stairs
        || id == 367 // Sewer Quest Lever
        || id == 386 // Stairsr
        || id == 547 // Worldstone Level1
        || id == 557 // Baal's Lair
        || id == 563 // Worldstone Chamber
        || id == 564 // GlacialCaves Level1
        || id == 565 // Last Cinematic
        || id == 566 // Harrogath LastPortal
        || id == 569; // ThroneOfDest Portal
        // || id == 189 // Cain Portal
        // || id == 268 // Wirt's Body
        // || id == 377 // Portal To Guild
        // || id == 451 // Hellgate
}

inline bool is_interesting_preset(dword id)
{
    return is_quest(id)
        || is_shrine(id)
        || is_waypoint(id)
        || is_transit(id)
        || id == 267; // Bank
}

inline bool is_uninteresting_unit(dword id)
{
    return id == 149 // rock
        || id == 152 // rogue
        || id == 179 // cow
        || id == 195 // lut gholein stuff
        || id == 196 // lut gholein tree
        || id == 197 // lut gholein stuff
        || id == 203 // lut gholein npc
        || id == 205 // lut gholein npc
        || id == 294 // kurast stuff
        || id == 296 // kurast stuff
        || id == 359 // iron wolf
        || id == 822 // fallen stuff
        || id == 823 // fallen stuff

        // || id == 513 // drehya
        // || id == 563 // baal tentacle
        // || id == 564 // baal tentacle
        // || id == 565 // baal tentacle
        // || id == 566 // baal tentacle
        || id == 567 // baal tentacle
        || id == 568 // injuried barb
        || id == 569 // injuried barb
        || id == 570 // injuried barb
        // || id == 571 // baal crab clone
        // || id == 575 // worldstone effect
        ;
}

inline bool is_backward_tile(dword id)
{
    return id == 5 //Cave Down
        || id == 8 //Crypt Up
        || id == 11 //Tower to Wilderness
        || id == 13 //Jail Up
        || id == 16 //Catacombs to Cathedral
        || id == 17 //Catacombs Up
        || id == 21 //Sewer Dock to Town
        || id == 22 //Sewer Up
        || id == 25 //Harem to Town
        || id == 26 //Harem Up 1
        || id == 27 //Harem Up 2
        || id == 30 //Basement Up 1
        || id == 31 //Basement Up 2
        || id == 45 //Tomb Up
        || id == 48 //Lair Up
        || id == 52 //Spider to Jungle
        || id == 55 //Dungeon Up
        || id == 58 //Sewer Up L
        || id == 59 //Sewer Up R
        || id == 62 //Temple Up L
        || id == 63 //Temple Up R
        || id == 65 //Mephisto Up L
        || id == 66 //Mephisto Up R
        || id == 70 //Lava to Mesa
        || id == 73 //Ice Caves Up
        || id == 78 //Temple Up
        || id == 81; //Baal Temple Up
}


#define MAX_AREA 137
extern const char *AREAS[MAX_AREA];

enum collision_flag {
    COL_NONE = 0x0000,
    COL_BLOCKWALK = 0x0001,
    COL_BLOCKLINEOFSIGHT = 0x0002,
    COL_WALL = 0x0004,
    COL_BLOCKPLAYER = 0x0008,
    COL_ALTERNATETILE = 0x0010,
    COL_BLANK = 0x0020,
    COL_MISSILE = 0x0040,
    COL_PLAYER = 0x0080,
    COL_NPCLOCATION = 0x0100,
    COL_ITEM = 0x0200,
    COL_OBJECT = 0x0400,
    COL_CLOSEDDOOR = 0x0800,
    COL_NPCCOLLISION = 0x1000,
    COL_FRIENDLYNPC = 0x2000,
    COL_UNKNOWN = 0x4000,
    COL_DEADBODY = 0x8000, // also portal
    COL_THICKENEDWALL = 0xfefe,
    COL_AVOID = 0xffff
};

#endif
