import pandas as pd


def read_lvl(filename):
    df = pd.read_csv(
        filename,
        delimiter='\t',
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
        delimiter='\t',
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
        delimiter='\t',
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
        delimiter='\t',
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
        "Name", "Class", "*Description", "IsAttackable0",
        "SizeX", "SizeY", "IsDoor", "BlocksVis",
        "BlockMissile", "OpenWarp", "AutoMap", "SubClass"
    ]]


################################################################################


def upcasify(s):
    return s.upper().replace(" ", "_").replace("-", "_").replace("'", "")


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
        upcasify(replace_dupe(r))
        for _, r in df.iterrows()
    ]).replace("NULL", "NO_LEVEL = 0")
    for i in range(1, 8):
        s = s.replace("TAL_RASHAS_TOMB,", f"TAL_RASHAS_TOMB_{i},", count=1)
    return s


def generate_monster_enum_content(df):
    return ",\n    ".join([
        upcasify(f'MON_{r["Id"]}')
        for _, r in df.iterrows()
    ])


def generate_super_enum_content(df):
    return ",\n    ".join([
        upcasify(f'SUPER_{r["Name"]}')
        for _, r in df.iterrows()
    ])


def generate_object_enum_content(df):
    return ",\n    ".join([
        upcasify(f'OBJ_{r["Class"]}')
        for _, r in df.iterrows()
    ])


################################################################################


def generate_act_struct_content(level_df, act_df):
    def wp_name(row, wp_idx):
        return upcasify(level_df.loc[
            row[f"waypoint{wp_idx}"], "*StringName"
        ])
    def romanify(s):
        return s.replace("1", "I").replace("2", "II").replace("3", "III").replace("4", "IV").replace("5", "V")
    return ",\n    ".join([
        f'{{.name="Act {romanify(str(r["act"]))}", .wp={{\n        '
        f'{",\n        ".join([wp_name(r, i) for i in range(1, 4 if r["act"] == 4 else 10)])}'
        f'\n    }}}}'
        for _, r in act_df.iterrows()
    ])


def generate_level_struct_content(df):
    def actify(s):
        return s.replace("4", "ACT_5").replace("3", "ACT_4").replace("2", "ACT_3").replace("1", "ACT_2").replace("0", "ACT_1")
    def wp_name(wp_idx):
        if wp_idx == 255:
            return "NO_WAYPOINT"
        return "WP_" + upcasify(
            df.sort_values(by="Waypoint").iloc[wp_idx]["*StringName"]
        )
    return ",\n    ".join([
        f'{{.name="{r["*StringName"]}",{" " * (27 - len(r["*StringName"]))} '
        f'.act={actify(str(r["Act"]))}, .wp={wp_name(r["Waypoint"])}}}'
        for _, r in df.iterrows()
    ])


def generate_monster_struct_content(df):
    return ",\n    ".join([
        f'{{'
        f'.name="{r["NameStr"]}",{" " * (24 - len(r["NameStr"]))} '
        f'.class="{r["Id"]}",\n     '
        f'.inTown={r["inTown"]},     '
        f'.killable={r["killable"]},  '
        f'.enabled={r["enabled"]},   '
        f'.threat={r["threat"]},\n     '
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
        for _, r in df.iterrows()
    ])


def generate_super_struct_content(df):
    return ",\n    ".join([
        f'{{.name="{r["Name"]}",{" " * (24 - len(r["Name"]))} '
        f'.monster={upcasify(f'MON_{r["Class"]}')}}}'
        for _, r in df.iterrows()
    ])


def generate_object_struct_content(df):
    return ",\n    ".join([
        f'{{'
        f'.name="{r["Name"]}",{" " * (23 - len(r["Name"]))} '
        f'.class="{r["Class"]}",\n     '
        f'.desc="{r["*Description"]}",\n     '
        f'.sizeX={r["SizeX"]},        '
        f'.sizeY={r["SizeY"]},       '
        f'.isDoor={r["IsDoor"]},       '
        f'.blocksVis={r["BlocksVis"]},\n     '
        f'.blockMissile={r["BlockMissile"]}, '
        f'.openWarp={r["OpenWarp"]},    '
        f'.isAttackable={r["IsAttackable0"]}, '
        f'.subClass={r["SubClass"]}, '
        f'.autoMap={r["AutoMap"]}}}'
        for _, r in df.iterrows()
    ])


################################################################################

def align(i, size=8):
    if i % size:
        i = i - i % size + size
    return i

################################################################################


level_df = read_lvl('./sdk/D2R/levels.txt')
MAX_LEVEL = len(level_df)
MAX_LEVEL_NAME = align(level_df["*StringName"].apply(len).max())

act_df = read_act('./sdk/D2R/actinfo.txt')
MAX_ACT = len(act_df)

mon_df = read_mon('./sdk/D2R/monstats.txt')
MAX_MONSTER = len(mon_df)
MAX_MONSTER_NAME = align(mon_df["NameStr"].apply(len).max())
MAX_MONSTER_CLASS = align(mon_df["Id"].apply(len).max())

super_df = read_super('./sdk/D2R/superuniques.txt')
MAX_SUPER = len(super_df)
MAX_SUPER_NAME = align(super_df["Name"].apply(len).max())

obj_df = read_obj('./sdk/D2R/objects.txt')
MAX_OBJ = len(obj_df)
MAX_OBJ_NAME = align(obj_df["Name"].apply(len).max())
MAX_OBJ_CLASS = align(obj_df["Class"].apply(len).max())
MAX_OBJ_DESC = align(obj_df["*Description"].apply(len).max())


print(f"""////////////////////////////////////////////////////////////////////////////////
// level.h
////////////////////////////////////////////////////////////////////////////////
typedef enum ActId  ActId;
enum ActId  {{
    {generate_act_enum_content(act_df)}
}};


typedef enum WaypointId  WaypointId;
enum WaypointId {{
    NO_WAYPOINT = 0xff,
    {generate_waypoint_enum_content(level_df)}
}};


typedef enum LevelId  LevelId;
enum LevelId {{
    {generate_level_enum_content(level_df)}
}};


#define MAX_ACT {MAX_ACT}
#define MAX_ACT_NAME 8
#define MAX_ACT_WP 9

typedef struct ActInfo  ActInfo;
struct ActInfo {{
    /* ActId id; */
    char name[MAX_ACT_NAME];
    LevelId wp[MAX_ACT_WP];
}};

extern const ActInfo ACT_INFO[MAX_ACT];


#define MAX_LEVEL {MAX_LEVEL}
#define MAX_LEVEL_NAME {MAX_LEVEL_NAME}

typedef struct LevelInfo  LevelInfo;
struct LevelInfo {{
    /* LevelId id; */
    char name[MAX_LEVEL_NAME];
    ActId act;
    WaypointId wp;
}};

extern const LevelInfo LEVEL_INFO[MAX_LEVEL];



////////////////////////////////////////////////////////////////////////////////
// level.c
////////////////////////////////////////////////////////////////////////////////
const ActInfo ACT_INFO[MAX_ACT] = {{
    {generate_act_struct_content(level_df, act_df)}
}};


const LevelInfo LEVEL_INFO[MAX_LEVEL] = {{
    {generate_level_struct_content(level_df)}
}};



////////////////////////////////////////////////////////////////////////////////
// monster.h
////////////////////////////////////////////////////////////////////////////////
typedef enum DifficultyId DifficultyId;  //TODO: move
enum DifficultyId {{
    DIFF_NORMAL = 0,
    DIFF_NIGHTMARE,
    DIFF_HELL,
    MAX_DIFFICULTY
}};


typedef enum ResistanceId ResistanceId;
enum ResistanceId {{
    RES_PHYSICAL = 0,
    RES_MAGIC,
    RES_FIRE,
    RES_LIGHT,
    RES_COLD,
    RES_POISON,
    MAX_RESISTANCE
}};


typedef enum SuperId  SuperId;
enum SuperId  {{
    {generate_super_enum_content(super_df)}
}};


typedef enum MonsterId  MonsterId;
enum MonsterId  {{
    {generate_monster_enum_content(mon_df)}
}};


#define MAX_SUPER {MAX_SUPER}
#define MAX_SUPER_NAME {MAX_SUPER_NAME}

typedef struct SuperInfo  SuperInfo;
struct SuperInfo {{
    /* SuperId id; */
    char name[MAX_SUPER_NAME];
    MonsterId monster;
}};

extern const SuperInfo SUPER_INFO[MAX_SUPER];


#define MAX_MONSTER {MAX_MONSTER}
#define MAX_MONSTER_NAME {MAX_MONSTER_NAME}
#define MAX_MONSTER_CLASS {MAX_MONSTER_CLASS}

typedef struct MonsterInfo  MonsterInfo;
struct MonsterInfo {{
    /* MonsterId id; */
    char name[MAX_MONSTER_NAME];
    char class[MAX_MONSTER_CLASS];
    unsigned char inTown;
    unsigned char killable;
    unsigned char enabled;
    unsigned char threat;
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

extern const MonsterInfo MONSTER_INFO[MAX_MONSTER];



////////////////////////////////////////////////////////////////////////////////
// monster.c
////////////////////////////////////////////////////////////////////////////////
const SuperInfo SUPER_INFO[MAX_SUPER] = {{
    {generate_super_struct_content(super_df)}
}};


const MonsterInfo MONSTER_INFO[MAX_MONSTER] = {{
    {generate_monster_struct_content(mon_df)}
}};



////////////////////////////////////////////////////////////////////////////////
// object.h
////////////////////////////////////////////////////////////////////////////////
typedef enum ObjectSubclass ObjectSubclass;
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


typedef enum ObjectId  ObjectId;
enum ObjectId  {{
    {generate_object_enum_content(obj_df)}
}};


#define MAX_OBJECT {MAX_OBJ}
#define MAX_OBJECT_NAME {MAX_OBJ_NAME}
#define MAX_OBJECT_CLASS {MAX_OBJ_CLASS}
#define MAX_OBJECT_DESC {MAX_OBJ_DESC}

typedef struct ObjectInfo  ObjectInfo;
struct ObjectInfo {{
    /* ObjectId id; */
    char name[MAX_OBJECT_NAME];
    char class[MAX_OBJECT_CLASS];
    char desc[MAX_OBJECT_DESC];
    unsigned char sizeX;
    unsigned char sizeY;
    unsigned char isDoor;
    unsigned char blocksVis;
    unsigned char blockMissile;
    unsigned char openWarp;
    unsigned char isAttackable;
    unsigned char subClass;
    int autoMap;
}};

extern const ObjectInfo OBJECT_INFO[MAX_OBJECT];



////////////////////////////////////////////////////////////////////////////////
// object.c
////////////////////////////////////////////////////////////////////////////////
const ObjectInfo OBJECT_INFO[MAX_OBJECT] = {{
    {generate_object_struct_content(obj_df)}
}};

""")
