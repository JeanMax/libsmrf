#!/usr/bin/env python3

import pandas as pd

TXT_DIR = "./txt/D2R/"
SDK_SRC_DIR = "./src/sdk/"
SDK_INC_DIR = "./include/smrf/sdk/"



################################################################################


def align(i, size=8):
    return i - i % size + size


def upcasify(s):
    return s.upper().replace(" ", "_").replace("-", "_").replace("'", "")


def dict_replace(s, d):
    for k, v in d.items():
        s = s.replace(k, v)
    return s


################################################################################


def read_lvl(filename):
    df = pd.read_csv(
        filename,
        delimiter="\t",
        skiprows=[110],  # "Expansion"
        false_values=["Null"],  # if you want the first "Null" level
        index_col="Name",
        dtype={
            "Id": pd.Int32Dtype(),
            "Act": pd.Int32Dtype(),
            "Waypoint": pd.Int32Dtype(),
            # could be interesting:
            # MonLvl MonLvl(N) MonLvl(H)
            # MonLvlEx MonLvlEx(N) MonLvlEx(H)
        }
    )
    return df.loc[:, ["Id", "*StringName", "Act", "Waypoint"]]


def read_act(filename):
    df = pd.read_csv(
        filename,
        delimiter="\t",
        index_col="start",
        dtype={
            "act": pd.Int32Dtype(),
        }
    )
    return df.loc[:, [
        "act", "town",
        "waypoint1", "waypoint2", "waypoint3",
        "waypoint4", "waypoint5", "waypoint6",
        "waypoint7", "waypoint8", "waypoint9",
    ]]


def read_mon(filename):
    df = pd.read_csv(
        filename,
        delimiter="\t",
        skiprows=[411],  # "Expansion"
        index_col="*hcIdx",
        dtype={
            "*hcIdx": pd.Int32Dtype(),
            "inTown": pd.Int32Dtype(),
            "killable": pd.Int32Dtype(),
            "enabled": pd.Int32Dtype(),
            "threat": pd.Int32Dtype(),
            "neverCount": pd.Int32Dtype(),
            "petIgnore": pd.Int32Dtype(),
            "inventory": pd.Int32Dtype(),
            "interact": pd.Int32Dtype(),
            "npc": pd.Int32Dtype(),
            "primeevil": pd.Int32Dtype(),
            "boss": pd.Int32Dtype(),
            "zoo": pd.Int32Dtype(),
            # "demon": pd.Int32Dtype(),
            # "flying": pd.Int32Dtype(),
            # "lUndead": pd.Int32Dtype(),
            # "hUndead": pd.Int32Dtype(),
            "ResDm": pd.Int32Dtype(),
            "ResMa": pd.Int32Dtype(),
            "ResFi": pd.Int32Dtype(),
            "ResLi": pd.Int32Dtype(),
            "ResCo": pd.Int32Dtype(),
            "ResPo": pd.Int32Dtype(),
            "ResDm(N)": pd.Int32Dtype(),
            "ResMa(N)": pd.Int32Dtype(),
            "ResFi(N)": pd.Int32Dtype(),
            "ResLi(N)": pd.Int32Dtype(),
            "ResCo(N)": pd.Int32Dtype(),
            "ResPo(N)": pd.Int32Dtype(),
            "ResDm(H)": pd.Int32Dtype(),
            "ResMa(H)": pd.Int32Dtype(),
            "ResFi(H)": pd.Int32Dtype(),
            "ResLi(H)": pd.Int32Dtype(),
            "ResCo(H)": pd.Int32Dtype(),
            "ResPo(H)": pd.Int32Dtype(),
        }
    )
    return df.loc[:, [
        "Id", "NameStr",
        "inTown", "killable", "enabled", "threat", "neverCount", "petIgnore",
        "inventory", "interact", "npc", "primeevil", "boss", "zoo",
        "ResDm", "ResMa", "ResFi", "ResLi", "ResCo", "ResPo",
        "ResDm(N)", "ResMa(N)", "ResFi(N)", "ResLi(N)", "ResCo(N)", "ResPo(N)",
        "ResDm(H)", "ResMa(H)", "ResFi(H)", "ResLi(H)", "ResCo(H)", "ResPo(H)",
    ]].fillna(0)


def read_super(filename):
    df = pd.read_csv(
        filename,
        delimiter="\t",
        skiprows=[43],  # "Expansion"
        index_col="hcIdx",
        dtype={
            "hcIdx": pd.Int32Dtype(),
        }
    )
    return df.loc[:, ["Name", "Class"]]


def read_obj(filename):
    df = pd.read_csv(
        filename,
        delimiter="\t",
        skiprows=[411],  # "Expansion"
        index_col="*ID",
        dtype={
            "*ID": pd.Int32Dtype(),
            "IsAttackable0": pd.Int32Dtype(),
            "Lockable": pd.Int32Dtype(),
            "SizeX": pd.Int32Dtype(),
            "SizeY": pd.Int32Dtype(),
            "IsDoor": pd.Int32Dtype(),
            "BlocksVis": pd.Int32Dtype(),
            "BlockMissile": pd.Int32Dtype(),
            "OpenWarp": pd.Int32Dtype(),
            "AutoMap": pd.Int32Dtype(),
            "SubClass": pd.Int32Dtype(),
        },
    )
    return df.loc[:, [
        "Name", "Class", "*Description", "IsAttackable0", "Lockable",
        "SizeX", "SizeY", "IsDoor", "BlocksVis",
        "BlockMissile", "OpenWarp", "AutoMap", "SubClass"
    ]]


def read_tile(filename):
    df = pd.read_csv(
        filename,
        delimiter="\t",
        skiprows=[72],  # "Expansion"
        index_col=False,
        dtype={
            "Id": pd.Int32Dtype(),
            "NoInteract": pd.Int32Dtype(),
            "UniqueId": pd.Int32Dtype(),
        }
    )
    return df.loc[:, ["Name", "NoInteract", "Id", "UniqueId"]]

def read_missile(filename):
    df = pd.read_csv(
        filename,
        delimiter="\t",
        index_col="*ID",
        dtype={
            "*ID": pd.Int32Dtype(),
            # there's a lot more infos in there (damages for instance)
        }
    )
    return df.loc[:, ["Missile"]]

################################################################################


def generate_act_enum_content(df):
    return ",\n    ".join([
        f'ACT_{r["act"]}'
        for _, r in df.iterrows()
    ]).replace("ACT_1", "ACT_1 = 0")


def generate_waypoint_enum_content(df):
    return ",\n    ".join([
        "WP_" + upcasify(r["*StringName"])
        for _, r in df.sort_values(by="Waypoint").iterrows()
        if r["Waypoint"] != 255
    ]).replace("ROGUE_ENCAMPMENT", "ROGUE_ENCAMPMENT = 0")


def generate_level_enum_content(df):
    def replace_dupe(r):
        if r["Id"] == 136:
            return "UBER_TRISTRAM"
        if r["Id"] in [47, 48, 49, 92, 91]:
            return r["*StringName"].replace("Sewers", f'Sewers Act {r["Act"] + 1}')
        return r["*StringName"]
    s = ",\n    ".join([
        upcasify(f'LVL_{replace_dupe(r)}')
        for _, r in df.iterrows()
    ]).replace("LVL_NULL", "NO_LEVEL = 0")
    for i in range(1, 8):
        s = s.replace("TAL_RASHAS_TOMB,", f"TAL_RASHAS_TOMB_{i},", count=1)
    return s


def generate_monster_enum_content(df):
    return ",\n    ".join([
        upcasify(f'MON_{r["Id"]}')
        for _, r in df.iterrows()
    ]).replace("MON_SKELETON1", "MON_SKELETON1 = 0")


def generate_super_enum_content(df):
    return ",\n    ".join([
        upcasify(f'SUPER_{r["Name"]}')
        for _, r in df.iterrows()
    ]).replace("SUPER_BISHIBOSH", "SUPER_BISHIBOSH = 0")


def generate_object_enum_content(df):
    return ",\n    ".join([
        upcasify(f'OBJ_{r["Class"]}')
        for _, r in df.iterrows()
    ]).replace("OBJ_NULL", "NO_OBJECT = 0")


def generate_unique_tile_enum_content(df):
    return ",\n    ".join([
        upcasify(f'UNIQUE_TILE_{r["Name"]}')
        for _, r in df.iterrows()
    ]).replace("CLIFF_L", "CLIFF_L = 0")


def generate_tile_enum_content(df):
    return ",\n    ".join([
        upcasify(f'TILE_{r["Name"]}')
        for _, r in df.iterrows()
    ]).replace("CLIFF_L", "CLIFF_L = 0")


def generate_missile_enum_content(df):
    return ",\n    ".join([
        upcasify(f'MISS_{r["Missile"]}')
        for _, r in df.iterrows()
    ]).replace("MISS_ARROW,", "MISS_ARROW = 0,")


################################################################################


def generate_act_struct_content(level_df, act_df):
    roman_dict = {
        "1": "I",
        "2": "II",
        "3": "III",
        "4": "IV",
        "5": "V",
    }
    def wp_name(row, wp_idx):
        return "LVL_" + upcasify(level_df.loc[
            row[f"waypoint{wp_idx}"], "*StringName"
        ])
    return ",\n    ".join([
        f'{{.id={act_df.index.get_loc(i)}, '
        f'.name="Act {dict_replace(str(r["act"]), roman_dict)}", .wp={{\n        '
        f'{",\n        ".join([wp_name(r, j) for j in range(1, 4 if r["act"] == 4 else 10)])}'
        f'\n    }}}}'
        for i, r in act_df.iterrows()
    ])


def generate_level_struct_content(df):
    act_dict = {
        "4": "ACT_5",
        "3": "ACT_4",
        "2": "ACT_3",
        "1": "ACT_2",
        "0": "ACT_1",
    }
    def wp_name(wp_idx):
        if wp_idx == 255:
            return "NO_WAYPOINT"
        return "WP_" + upcasify(
            df.sort_values(by="Waypoint").iloc[wp_idx]["*StringName"]
        )
    return ",\n    ".join([
        f'{{.id={df.index.get_loc(i)}, '
        f'.name="{r["*StringName"]}",{" " * (27 - len(r["*StringName"]))} '
        f'.act={dict_replace(str(r["Act"]), act_dict)}, .wp={wp_name(r["Waypoint"])}}}'
        for i, r in df.iterrows()
    ])


def generate_monster_struct_content(df):
    threat_dict = {
            "14": "THREAT_PRIME_EVIL",
            "13": "THREAT_BLOOD_RAVEN",
            "12": "THREAT_SHAMAN",
            "11": "THREAT_MINION",
            "10": "THREAT_MONSTER",
            "9": "THREAT_TURRET",
            "8": "THREAT_FAMILIAR",
            "7": "THREAT_CATAPULT",
            "6": "THREAT_DOOR",
            "4": "THREAT_TRAPPED_SOUL",
            "2": "THREAT_WALL",
            "1": "THREAT_BLOCK",
            "0": "NO_THREAT",
    }
    return ",\n    ".join([
        f'{{.id={i},\n     '
        f'.name="{r["NameStr"]}",{" " * (19 - len(r["NameStr"]))} '
        f'.classId="{r["Id"]}",\n     '
        f'.inTown={r["inTown"]},     '
        f'.killable={r["killable"]},  '
        f'.enabled={r["enabled"]},   '
        f'.threat={dict_replace(str(r["threat"]), threat_dict)},\n     '
        f'.neverCount={r["neverCount"]}, '
        f'.petIgnore={r["petIgnore"]}, '
        f'.inventory={r["inventory"]}, '
        f'.interact={r["interact"]},\n     '
        f'.npc={r["npc"]},        '
        f'.primeevil={r["primeevil"]}, '
        f'.boss={r["boss"]},      '
        f'.zoo={r["zoo"]},\n     '
        f'.res={{'
        f'{{{r["ResDm"]}, {r["ResMa"]}, {r["ResFi"]}, {r["ResLi"]}, {r["ResCo"]}, {r["ResPo"]}}},\n           '
        f'{{{r["ResDm(N)"]}, {r["ResMa(N)"]}, {r["ResFi(N)"]}, {r["ResLi(N)"]}, {r["ResCo(N)"]}, {r["ResPo(N)"]}}},\n           '
        f'{{{r["ResDm(H)"]}, {r["ResMa(H)"]}, {r["ResFi(H)"]}, {r["ResLi(H)"]}, {r["ResCo(H)"]}, {r["ResPo(H)"]}}}}}}}'
        for i, r in df.iterrows()
    ])


def generate_super_struct_content(df):
    return ",\n    ".join([
        f'{{.id={i}, '
        f'.name="{r["Name"]}",{" " * (24 - len(r["Name"]))} '
        f'.monster={upcasify(f'MON_{r["Class"]}')}}}'
        for i, r in df.iterrows()
    ])


def generate_object_struct_content(df):
    subclass_dict = {
        "128": "SUB_SECRET_DOOR",
        "64": "SUB_WAYPOINT",
        "32": "SUB_WELL",
        "16": "SUB_ARCANE",
        "8": "SUB_CONTAINER",
        "4": "SUB_PORTAL",
        "2": "SUB_OBELISK",
        "1": "SUB_SHRINE",
        "0": "NO_SUBCLASS",
    }
    automap_dict = {
        "1468": "MAP_ANYA",
        "1467": "MAP_ALTAR",
        "694": "MAP_STAIR_LEFT",
        "693": "MAP_STAIR_RIGHT",
        "427": "MAP_TOME",
        "339": "MAP_PORTAL",
        "319": "MAP_BANK",
        "318": "MAP_CHEST",
        "316": "MAP_MALUS",
        "315": "MAP_GIBDIN",
        "314": "MAP_CAIN_STONE",
        "313": "MAP_INIFUSS_TREE",
        "310": "MAP_SHRINE",
        "309": "MAP_WELL",
        "307": "MAP_WAYPOINT",
        "306": "MAP_DIABLO_SEAL",
        "305": "MAP_COMPELLING_ORB",
        "304": "MAP_SEWER",
        "223": "MAP_TRAP_DOOR",
        "0": "NO_AUTOMAP",
    }
    return ",\n    ".join([
        f'{{.id={i},\n     '
        f'.name="{r["Name"]}",{" " * (23 - len(r["Name"]))} '
        f'.classId="{r["Class"]}",\n     '
        f'.desc="{r["*Description"]}",\n     '
        f'.sizeX={r["SizeX"]},        '
        f'.sizeY={r["SizeY"]},       '
        f'.isDoor={r["IsDoor"]},       '
        f'.blocksVis={r["BlocksVis"]},\n     '
        f'.blockMissile={r["BlockMissile"]}, '
        f'.openWarp={r["OpenWarp"]},    '
        f'.isAttackable={r["IsAttackable0"]}, '
        f'.lockable={r["Lockable"]},\n     '
        f'.subClass={dict_replace(str(r["SubClass"]), subclass_dict)}, '
        f'.autoMap={dict_replace(str(r["AutoMap"]), automap_dict)}}}'
        for i, r in df.iterrows()
    ])


def generate_unique_tile_struct_content(df):
    return ",\n    ".join([
        f'{{.id={i},         '
        f'.name="{r["Name"]}",\n     '
        f'.noInteract={r["NoInteract"]}, '
        f'.notUniqueId=TILE_{upcasify(r["Name"])}}}'
        for i, r in df.iterrows()
    ])


def generate_tile_struct_content(df):
    return ",\n    ".join([
        f'{{.id={i}, '
        f'.uid=UNIQUE_TILE_{upcasify(df[df["Id"] == i].iloc[0]["Name"])}}}'
        for i in df["Id"].unique()
    ])


def generate_missile_struct_content(df):
    return ",\n    ".join([
        f'{{.id={i}, '
        f'.name="{r["Missile"]}"}}'
        for i, r in df.iterrows()
    ])


################################################################################


def str_level_inc(level_df, act_df):
    MAX_LEVEL = len(level_df)
    MAX_LEVEL_NAME = align(level_df["*StringName"].apply(len).max())
    MAX_ACT = len(act_df)
    return f"""#ifndef _LEVEL_H
#define _LEVEL_H

enum ActId  {{
    {generate_act_enum_content(act_df)}
}};
typedef enum ActId  ActId;


enum WaypointId {{
    NO_WAYPOINT = 0xff,
    {generate_waypoint_enum_content(level_df)}
}};
typedef enum WaypointId  WaypointId;


enum LevelId {{
    {generate_level_enum_content(level_df)}
}};
typedef enum LevelId  LevelId;


#define MAX_ACT {MAX_ACT}
#define MAX_ACT_NAME 8
#define MAX_ACT_WP 9

struct ActInfo {{
    ActId id;
    char name[MAX_ACT_NAME];
    LevelId wp[MAX_ACT_WP];
}};
typedef struct ActInfo  ActInfo;

extern const ActInfo ACT_INFO[MAX_ACT];


#define MAX_LEVEL {MAX_LEVEL}
#define MAX_LEVEL_NAME {MAX_LEVEL_NAME}

struct LevelInfo {{
    LevelId id;
    char name[MAX_LEVEL_NAME];
    ActId act;
    WaypointId wp;
}};
typedef struct LevelInfo  LevelInfo;

extern const LevelInfo LEVEL_INFO[MAX_LEVEL];


#endif
"""

def str_level_src(level_df, act_df):
    return f"""#include "sdk/level.h"

const ActInfo ACT_INFO[MAX_ACT] = {{
    {generate_act_struct_content(level_df, act_df)}
}};


const LevelInfo LEVEL_INFO[MAX_LEVEL] = {{
    {generate_level_struct_content(level_df)}
}};
"""


def str_player_inc():
    return f"""#ifndef _PLAYER_H
#define _PLAYER_H

enum PlayerId  {{
    PLAYER_AMA = 0,
    PLAYER_SOR,
    PLAYER_NEC,
    PLAYER_PAL,
    PLAYER_BAR,
    PLAYER_DRU,
    PLAYER_ASS,
    MAX_PLAYER,
}};
typedef enum PlayerId  PlayerId;


#define MAX_PLAYER_NAME 16
#define MAX_PLAYER_CLASS 8

struct PlayerInfo {{
    PlayerId id;
    char name[MAX_PLAYER_NAME];
    char classId[MAX_PLAYER_CLASS];
}};
typedef struct PlayerInfo  PlayerInfo;

extern const PlayerInfo PLAYER_INFO[MAX_PLAYER];


#endif
{""}"""

def str_player_src():
    return f"""#include "sdk/player.h"

const PlayerInfo PLAYER_INFO[MAX_PLAYER] = {{
    {{.id=PLAYER_AMA, .name="Amazon",      .classId="ama"}},
    {{.id=PLAYER_SOR, .name="Sorceress",   .classId="sor"}},
    {{.id=PLAYER_NEC, .name="Necromancer", .classId="nec"}},
    {{.id=PLAYER_PAL, .name="Paladin",     .classId="pal"}},
    {{.id=PLAYER_BAR, .name="Barbarian",   .classId="bar"}},
    {{.id=PLAYER_DRU, .name="Druid",       .classId="dru"}},
    {{.id=PLAYER_ASS, .name="Assassin",    .classId="ass"}},
}};
{""}"""


def str_monster_inc(monster_df, super_df):
    MAX_MONSTER = len(monster_df)
    MAX_MONSTER_NAME = align(monster_df["NameStr"].apply(len).max())
    MAX_MONSTER_CLASS = align(monster_df["Id"].apply(len).max())
    MAX_SUPER = len(super_df)
    MAX_SUPER_NAME = align(super_df["Name"].apply(len).max())
    return f"""#ifndef _MONSTER_H
#define _MONSTER_H

enum DifficultyId {{
    DIFF_NORMAL = 0,
    DIFF_NIGHTMARE,
    DIFF_HELL,
    MAX_DIFFICULTY
}};
typedef enum DifficultyId DifficultyId;  //TODO: move


enum ResistanceId {{
    RES_PHYSICAL = 0,
    RES_MAGIC,
    RES_FIRE,
    RES_LIGHT,
    RES_COLD,
    RES_POISON,
    MAX_RESISTANCE
}};
typedef enum ResistanceId ResistanceId;


enum ThreatId {{
    NO_THREAT = 0,
    THREAT_BLOCK,
    THREAT_WALL,
    THREAT_UNUSED_3,
    THREAT_TRAPPED_SOUL,
    THREAT_UNUSED_5,
    THREAT_DOOR,
    THREAT_CATAPULT,
    THREAT_FAMILIAR,
    THREAT_TURRET,
    THREAT_MONSTER,
    THREAT_MINION,
    THREAT_SHAMAN,
    THREAT_BLOOD_RAVEN,
    THREAT_PRIME_EVIL,
}};
typedef enum ThreatId ThreatId;


enum SuperId  {{
    {generate_super_enum_content(super_df)}
}};
typedef enum SuperId  SuperId;


enum MonsterId  {{
    {generate_monster_enum_content(monster_df)}
}};
typedef enum MonsterId  MonsterId;


#define MAX_SUPER {MAX_SUPER}
#define MAX_SUPER_NAME {MAX_SUPER_NAME}

struct SuperInfo {{
    SuperId id;
    char name[MAX_SUPER_NAME];
    MonsterId monster;
}};
typedef struct SuperInfo  SuperInfo;

extern const SuperInfo SUPER_INFO[MAX_SUPER];


#define MAX_MONSTER {MAX_MONSTER}
#define MAX_MONSTER_NAME {MAX_MONSTER_NAME}
#define MAX_MONSTER_CLASS {MAX_MONSTER_CLASS}

struct MonsterInfo {{
    MonsterId id;
    char name[MAX_MONSTER_NAME];
    char classId[MAX_MONSTER_CLASS];
    unsigned char inTown;
    unsigned char killable;
    unsigned char enabled;
    unsigned char threat;  // ThreatId
    unsigned char neverCount;
    unsigned char petIgnore;
    unsigned char inventory;
    unsigned char interact;
    unsigned char npc;
    unsigned char primeevil;
    unsigned char boss;
    unsigned char zoo;
    int res[MAX_DIFFICULTY][MAX_RESISTANCE];
}};
typedef struct MonsterInfo  MonsterInfo;

extern const MonsterInfo MONSTER_INFO[MAX_MONSTER];


#endif
"""

def str_monster_src(monster_df, super_df):
    return f"""#include "sdk/monster.h"

const SuperInfo SUPER_INFO[MAX_SUPER] = {{
    {generate_super_struct_content(super_df)}
}};


const MonsterInfo MONSTER_INFO[MAX_MONSTER] = {{
    {generate_monster_struct_content(monster_df)}
}};
"""


def str_object_inc(object_df):
    MAX_OBJ = len(object_df)
    MAX_OBJ_NAME = align(object_df["Name"].apply(len).max())
    MAX_OBJ_CLASS = align(object_df["Class"].apply(len).max())
    MAX_OBJ_DESC = align(object_df["*Description"].apply(len).max())
    return f"""#ifndef _OBJECT_H
#define _OBJECT_H

enum ObjectSubclass {{
    NO_SUBCLASS = 0,
    SUB_SHRINE = 1 << 0,
    SUB_OBELISK = 1 << 1,
    SUB_PORTAL = 1 << 2,
    SUB_CONTAINER = 1 << 3,
    SUB_ARCANE = 1 << 4,
    SUB_WELL = 1 << 5,
    SUB_WAYPOINT = 1 << 6,
    SUB_SECRET_DOOR = 1 << 7,
}};
typedef enum ObjectSubclass  ObjectSubclass;


enum AutoMapId {{
    NO_AUTOMAP = 0,
    MAP_TRAP_DOOR = 223,
    MAP_SEWER = 304,
    MAP_COMPELLING_ORB = 305,
    MAP_DIABLO_SEAL = 306,
    MAP_WAYPOINT = 307,
    MAP_WELL = 309,
    MAP_SHRINE = 310,
    MAP_INIFUSS_TREE = 313,
    MAP_CAIN_STONE = 314,
    MAP_GIBDIN = 315,
    MAP_MALUS = 316,
    MAP_CHEST = 318,
    MAP_BANK = 319,
    MAP_PORTAL = 339,
    MAP_TOME = 427,
    MAP_STAIR_RIGHT = 693,
    MAP_STAIR_LEFT = 694,
    MAP_ALTAR = 1467,
    MAP_ANYA = 1468,
}};
typedef enum AutoMapId  AutoMapId;


enum ObjectId  {{
    {generate_object_enum_content(object_df)}
}};
typedef enum ObjectId  ObjectId;


#define MAX_OBJECT {MAX_OBJ}
#define MAX_OBJECT_NAME {MAX_OBJ_NAME}
#define MAX_OBJECT_CLASS {MAX_OBJ_CLASS}
#define MAX_OBJECT_DESC {MAX_OBJ_DESC}

struct ObjectInfo {{
    ObjectId id;
    char name[MAX_OBJECT_NAME];
    char classId[MAX_OBJECT_CLASS];
    char desc[MAX_OBJECT_DESC];
    unsigned char sizeX;
    unsigned char sizeY;
    unsigned char isDoor;
    unsigned char blocksVis;
    unsigned char blockMissile;
    unsigned char openWarp;
    unsigned char isAttackable;
    unsigned char lockable;
    ObjectSubclass subClass;
    AutoMapId autoMap;
}};
typedef struct ObjectInfo  ObjectInfo;

extern const ObjectInfo OBJECT_INFO[MAX_OBJECT];


#endif
"""

def str_object_src(object_df):
    return f"""#include "sdk/object.h"

const ObjectInfo OBJECT_INFO[MAX_OBJECT] = {{
    {generate_object_struct_content(object_df)}
}};
"""


def str_tile_inc(tile_df):
    MAX_UNIQUE_TILE = len(tile_df)
    MAX_TILE = tile_df["Id"].max() + 1
    MAX_TILE_NAME = align(tile_df["Name"].apply(len).max())
    return f"""#ifndef _TILE_H
#define _TILE_H

enum UniqueTileId  {{
    {generate_unique_tile_enum_content(tile_df)}
}};
typedef enum UniqueTileId  UniqueTileId;


enum TileId  {{
    {generate_tile_enum_content(tile_df)}
}};
typedef enum TileId  TileId;


#define MAX_UNIQUE_TILE {MAX_UNIQUE_TILE}
#define MAX_TILE_NAME {MAX_TILE_NAME}

struct UniqueTileInfo {{
    UniqueTileId id;
    char name[MAX_TILE_NAME];
    int noInteract;
    TileId notUniqueId;
}};
typedef struct UniqueTileInfo  UniqueTileInfo;

extern const UniqueTileInfo UNIQUE_TILE_INFO[MAX_UNIQUE_TILE];


#define MAX_TILE {MAX_TILE}

struct TileInfo {{
    TileId id;
    UniqueTileId uid;
}};
typedef struct TileInfo  TileInfo;

extern const TileInfo TILE_INFO[MAX_TILE];


#endif
"""

def str_tile_src(tile_df):
    return f"""#include "sdk/tile.h"

const UniqueTileInfo UNIQUE_TILE_INFO[MAX_UNIQUE_TILE] = {{
    {generate_unique_tile_struct_content(tile_df)}
}};


const TileInfo TILE_INFO[MAX_TILE] = {{
    {generate_tile_struct_content(tile_df)}
}};
"""


def str_missile_inc(missile_df):
    MAX_MISSILE = len(missile_df)
    MAX_MISSILE_NAME = align(missile_df["Missile"].apply(len).max())
    return f"""#ifndef _MISSILE_H
#define _MISSILE_H

enum MissileId  {{
    {generate_missile_enum_content(missile_df)}
}};
typedef enum MissileId  MissileId;


#define MAX_MISSILE {MAX_MISSILE}
#define MAX_MISSILE_NAME {MAX_MISSILE_NAME}

struct MissileInfo {{
    MissileId id;
    char name[MAX_MISSILE_NAME];
}};
typedef struct MissileInfo  MissileInfo;

extern const MissileInfo MISSILE_INFO[MAX_MISSILE];


#endif
"""

def str_missile_src(missile_df):
    return f"""#include "sdk/missile.h"

const MissileInfo MISSILE_INFO[MAX_MISSILE] = {{
    {generate_missile_struct_content(missile_df)}
}};
"""


################################################################################


level_df = read_lvl(TXT_DIR + "levels.txt")
act_df = read_act(TXT_DIR + "actinfo.txt")
with open(SDK_INC_DIR + "level.h", "w") as f:
    f.write(
        str_level_inc(level_df, act_df)
    )
with open(SDK_SRC_DIR + "level.c", "w") as f:
    f.write(
        str_level_src(level_df, act_df)
    )

monster_df = read_mon(TXT_DIR + "monstats.txt")
super_df = read_super(TXT_DIR + "superuniques.txt")
with open(SDK_INC_DIR + "monster.h", "w") as f:
    f.write(
        str_monster_inc(monster_df, super_df)
    )
with open(SDK_SRC_DIR + "monster.c", "w") as f:
    f.write(
        str_monster_src(monster_df, super_df)
    )

object_df = read_obj(TXT_DIR + "objects.txt")
with open(SDK_INC_DIR + "object.h", "w") as f:
    f.write(
        str_object_inc(object_df)
    )
with open(SDK_SRC_DIR + "object.c", "w") as f:
    f.write(
        str_object_src(object_df)
    )

# super lazy, I should parse playerclass.txt instead
with open(SDK_INC_DIR + "player.h", "w") as f:
    f.write(
        str_player_inc()
    )
with open(SDK_SRC_DIR + "player.c", "w") as f:
    f.write(
        str_player_src()
    )

tile_df = read_tile(TXT_DIR + "lvlwarp.txt")
with open(SDK_INC_DIR + "tile.h", "w") as f:
    f.write(
        str_tile_inc(tile_df)
    )
with open(SDK_SRC_DIR + "tile.c", "w") as f:
    f.write(
        str_tile_src(tile_df)
    )

missile_df = read_missile(TXT_DIR + "missiles.txt")
with open(SDK_INC_DIR + "missile.h", "w") as f:
    f.write(
        str_missile_inc(missile_df)
    )
with open(SDK_SRC_DIR + "missile.c", "w") as f:
    f.write(
        str_missile_src(missile_df)
    )



#TODO:
# weapons.txt
# armor.txt
# belts.txt
# books.txt
# gems.txt
# misc.txt (weapon -> armor -> misc)
# runes.txt
# setitems.txt
# sets.txt
# uniqueitems.txt

# itemratio.txt
# ItemStatCost.txt
# ItemTypes.txt

# MagicPrefix.txt
# MagicSuffix.txt

# Properties.txt
# QualityItems.txt

# RarePrefix.txt
# RareSuffix.txt

# uniqueprefix.txt
# uniquesuffix.txt

# automagic.txt

# cubemain.txt
# runeword: d2go
