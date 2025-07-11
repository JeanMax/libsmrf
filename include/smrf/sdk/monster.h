#ifndef _MONSTER_H
#define _MONSTER_H

enum DifficultyId {
    DIFF_NORMAL = 0,
    DIFF_NIGHTMARE,
    DIFF_HELL,
    MAX_DIFFICULTY
};
typedef enum DifficultyId DifficultyId;  //TODO: move


enum ResistanceId {
    RES_PHYSICAL = 0,
    RES_MAGIC,
    RES_FIRE,
    RES_LIGHT,
    RES_COLD,
    RES_POISON,
    MAX_RESISTANCE
};
typedef enum ResistanceId ResistanceId;


enum ThreatId {
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
};
typedef enum ThreatId ThreatId;


enum SuperId  {
    SUPER_BISHIBOSH = 0,
    SUPER_BONEBREAK,
    SUPER_COLDCROW,
    SUPER_RAKANISHU,
    SUPER_TREEHEAD_WOODFIST,
    SUPER_GRISWOLD,
    SUPER_THE_COUNTESS,
    SUPER_PITSPAWN_FOULDOG,
    SUPER_FLAMESPIKE_THE_CRAWLER,
    SUPER_BONEASH,
    SUPER_RADAMENT,
    SUPER_BLOODWITCH_THE_WILD,
    SUPER_FANGSKIN,
    SUPER_BEETLEBURST,
    SUPER_LEATHERARM,
    SUPER_COLDWORM_THE_BURROWER,
    SUPER_FIRE_EYE,
    SUPER_DARK_ELDER,
    SUPER_THE_SUMMONER,
    SUPER_ANCIENT_KAA_THE_SOULLESS,
    SUPER_THE_SMITH,
    SUPER_WEB_MAGE_THE_BURNING,
    SUPER_WITCH_DOCTOR_ENDUGU,
    SUPER_STORMTREE,
    SUPER_SARINA_THE_BATTLEMAID,
    SUPER_ICEHAWK_RIFTWING,
    SUPER_ISMAIL_VILEHAND,
    SUPER_GELEB_FLAMEFINGER,
    SUPER_BREMM_SPARKFIST,
    SUPER_TOORC_ICEFIST,
    SUPER_WYAND_VOIDFINGER,
    SUPER_MAFFER_DRAGONHAND,
    SUPER_WINGED_DEATH,
    SUPER_THE_TORMENTOR,
    SUPER_TAINTBREEDER,
    SUPER_RIFTWRAITH_THE_CANNIBAL,
    SUPER_INFECTOR_OF_SOULS,
    SUPER_LORD_DE_SEIS,
    SUPER_GRAND_VIZIER_OF_CHAOS,
    SUPER_THE_COW_KING,
    SUPER_CORPSEFIRE,
    SUPER_THE_FEATURE_CREEP,
    SUPER_SIEGE_BOSS,
    SUPER_ANCIENT_BARBARIAN_1,
    SUPER_ANCIENT_BARBARIAN_2,
    SUPER_ANCIENT_BARBARIAN_3,
    SUPER_AXE_DWELLER,
    SUPER_BONESAW_BREAKER,
    SUPER_DAC_FARREN,
    SUPER_MEGAFLOW_RECTIFIER,
    SUPER_EYEBACK_UNLEASHED,
    SUPER_THREASH_SOCKET,
    SUPER_PINDLESKIN,
    SUPER_SNAPCHIP_SHATTER,
    SUPER_ANODIZED_ELITE,
    SUPER_VINVEAR_MOLECH,
    SUPER_SHARP_TOOTH_SAYER,
    SUPER_MAGMA_TORQUER,
    SUPER_BLAZE_RIPPER,
    SUPER_FROZENSTEIN,
    SUPER_NIHLATHAK,
    SUPER_BAAL_SUBJECT_1,
    SUPER_BAAL_SUBJECT_2,
    SUPER_BAAL_SUBJECT_3,
    SUPER_BAAL_SUBJECT_4,
    SUPER_BAAL_SUBJECT_5
};
typedef enum SuperId  SuperId;


enum MonsterId  {
    MON_SKELETON1 = 0,
    MON_SKELETON2,
    MON_SKELETON3,
    MON_SKELETON4,
    MON_SKELETON5,
    MON_ZOMBIE1,
    MON_ZOMBIE2,
    MON_ZOMBIE3,
    MON_ZOMBIE4,
    MON_ZOMBIE5,
    MON_BIGHEAD1,
    MON_BIGHEAD2,
    MON_BIGHEAD3,
    MON_BIGHEAD4,
    MON_BIGHEAD5,
    MON_FOULCROW1,
    MON_FOULCROW2,
    MON_FOULCROW3,
    MON_FOULCROW4,
    MON_FALLEN1,
    MON_FALLEN2,
    MON_FALLEN3,
    MON_FALLEN4,
    MON_FALLEN5,
    MON_BRUTE2,
    MON_BRUTE3,
    MON_BRUTE4,
    MON_BRUTE5,
    MON_BRUTE1,
    MON_SANDRAIDER1,
    MON_SANDRAIDER2,
    MON_SANDRAIDER3,
    MON_SANDRAIDER4,
    MON_SANDRAIDER5,
    MON_GORGON1,
    MON_GORGON2,
    MON_GORGON3,
    MON_GORGON4,
    MON_WRAITH1,
    MON_WRAITH2,
    MON_WRAITH3,
    MON_WRAITH4,
    MON_WRAITH5,
    MON_CORRUPTROGUE1,
    MON_CORRUPTROGUE2,
    MON_CORRUPTROGUE3,
    MON_CORRUPTROGUE4,
    MON_CORRUPTROGUE5,
    MON_BABOON1,
    MON_BABOON2,
    MON_BABOON3,
    MON_BABOON4,
    MON_BABOON5,
    MON_GOATMAN1,
    MON_GOATMAN2,
    MON_GOATMAN3,
    MON_GOATMAN4,
    MON_GOATMAN5,
    MON_FALLENSHAMAN1,
    MON_FALLENSHAMAN2,
    MON_FALLENSHAMAN3,
    MON_FALLENSHAMAN4,
    MON_FALLENSHAMAN5,
    MON_QUILLRAT1,
    MON_QUILLRAT2,
    MON_QUILLRAT3,
    MON_QUILLRAT4,
    MON_QUILLRAT5,
    MON_SANDMAGGOT1,
    MON_SANDMAGGOT2,
    MON_SANDMAGGOT3,
    MON_SANDMAGGOT4,
    MON_SANDMAGGOT5,
    MON_CLAWVIPER1,
    MON_CLAWVIPER2,
    MON_CLAWVIPER3,
    MON_CLAWVIPER4,
    MON_CLAWVIPER5,
    MON_SANDLEAPER1,
    MON_SANDLEAPER2,
    MON_SANDLEAPER3,
    MON_SANDLEAPER4,
    MON_SANDLEAPER5,
    MON_PANTHERWOMAN1,
    MON_PANTHERWOMAN2,
    MON_PANTHERWOMAN3,
    MON_PANTHERWOMAN4,
    MON_SWARM1,
    MON_SWARM2,
    MON_SWARM3,
    MON_SWARM4,
    MON_SCARAB1,
    MON_SCARAB2,
    MON_SCARAB3,
    MON_SCARAB4,
    MON_SCARAB5,
    MON_MUMMY1,
    MON_MUMMY2,
    MON_MUMMY3,
    MON_MUMMY4,
    MON_MUMMY5,
    MON_UNRAVELER1,
    MON_UNRAVELER2,
    MON_UNRAVELER3,
    MON_UNRAVELER4,
    MON_UNRAVELER5,
    MON_CHAOSHORDE1,
    MON_CHAOSHORDE2,
    MON_CHAOSHORDE3,
    MON_CHAOSHORDE4,
    MON_VULTURE1,
    MON_VULTURE2,
    MON_VULTURE3,
    MON_VULTURE4,
    MON_MOSQUITO1,
    MON_MOSQUITO2,
    MON_MOSQUITO3,
    MON_MOSQUITO4,
    MON_WILLOWISP1,
    MON_WILLOWISP2,
    MON_WILLOWISP3,
    MON_WILLOWISP4,
    MON_ARACH1,
    MON_ARACH2,
    MON_ARACH3,
    MON_ARACH4,
    MON_ARACH5,
    MON_THORNHULK1,
    MON_THORNHULK2,
    MON_THORNHULK3,
    MON_THORNHULK4,
    MON_VAMPIRE1,
    MON_VAMPIRE2,
    MON_VAMPIRE3,
    MON_VAMPIRE4,
    MON_VAMPIRE5,
    MON_BATDEMON1,
    MON_BATDEMON2,
    MON_BATDEMON3,
    MON_BATDEMON4,
    MON_BATDEMON5,
    MON_FETISH1,
    MON_FETISH2,
    MON_FETISH3,
    MON_FETISH4,
    MON_FETISH5,
    MON_CAIN1,
    MON_GHEED,
    MON_AKARA,
    MON_CHICKEN,
    MON_KASHYA,
    MON_RAT,
    MON_ROGUE1,
    MON_HELLMETEOR,
    MON_CHARSI,
    MON_WARRIV1,
    MON_ANDARIEL,
    MON_BIRD1,
    MON_BIRD2,
    MON_BAT,
    MON_CR_ARCHER1,
    MON_CR_ARCHER2,
    MON_CR_ARCHER3,
    MON_CR_ARCHER4,
    MON_CR_ARCHER5,
    MON_CR_LANCER1,
    MON_CR_LANCER2,
    MON_CR_LANCER3,
    MON_CR_LANCER4,
    MON_CR_LANCER5,
    MON_SK_ARCHER1,
    MON_SK_ARCHER2,
    MON_SK_ARCHER3,
    MON_SK_ARCHER4,
    MON_SK_ARCHER5,
    MON_WARRIV2,
    MON_ATMA,
    MON_DROGNAN,
    MON_FARA,
    MON_COW,
    MON_MAGGOTBABY1,
    MON_MAGGOTBABY2,
    MON_MAGGOTBABY3,
    MON_MAGGOTBABY4,
    MON_MAGGOTBABY5,
    MON_CAMEL,
    MON_BLUNDERBORE1,
    MON_BLUNDERBORE2,
    MON_BLUNDERBORE3,
    MON_BLUNDERBORE4,
    MON_MAGGOTEGG1,
    MON_MAGGOTEGG2,
    MON_MAGGOTEGG3,
    MON_MAGGOTEGG4,
    MON_MAGGOTEGG5,
    MON_ACT2MALE,
    MON_ACT2FEMALE,
    MON_ACT2CHILD,
    MON_GREIZ,
    MON_ELZIX,
    MON_GEGLASH,
    MON_JERHYN,
    MON_LYSANDER,
    MON_ACT2GUARD1,
    MON_ACT2VENDOR1,
    MON_ACT2VENDOR2,
    MON_CROWNEST1,
    MON_CROWNEST2,
    MON_CROWNEST3,
    MON_CROWNEST4,
    MON_MESHIF1,
    MON_DURIEL,
    MON_BONEFETISH1,
    MON_BONEFETISH2,
    MON_BONEFETISH3,
    MON_BONEFETISH4,
    MON_BONEFETISH5,
    MON_DARKGUARD1,
    MON_DARKGUARD2,
    MON_DARKGUARD3,
    MON_DARKGUARD4,
    MON_DARKGUARD5,
    MON_BLOODMAGE1,
    MON_BLOODMAGE2,
    MON_BLOODMAGE3,
    MON_BLOODMAGE4,
    MON_BLOODMAGE5,
    MON_MAGGOT,
    MON_SARCOPHAGUS,
    MON_RADAMENT,
    MON_FIREBEAST,
    MON_ICEGLOBE,
    MON_LIGHTNINGBEAST,
    MON_POISONORB,
    MON_FLYINGSCIMITAR,
    MON_ZEALOT1,
    MON_ZEALOT2,
    MON_ZEALOT3,
    MON_CANTOR1,
    MON_CANTOR2,
    MON_CANTOR3,
    MON_CANTOR4,
    MON_MEPHISTO,
    MON_DIABLO,
    MON_CAIN2,
    MON_CAIN3,
    MON_CAIN4,
    MON_FROGDEMON1,
    MON_FROGDEMON2,
    MON_FROGDEMON3,
    MON_SUMMONER,
    MON_TYRAEL1,
    MON_ASHEARA,
    MON_HRATLI,
    MON_ALKOR,
    MON_ORMUS,
    MON_IZUAL,
    MON_HALBU,
    MON_TENTACLE1,
    MON_TENTACLE2,
    MON_TENTACLE3,
    MON_TENTACLEHEAD1,
    MON_TENTACLEHEAD2,
    MON_TENTACLEHEAD3,
    MON_MESHIF2,
    MON_CAIN5,
    MON_NAVI,
    MON_BLOODRAVEN,
    MON_BUG,
    MON_SCORPION,
    MON_ROGUE2,
    MON_ROGUEHIRE,
    MON_ROGUE3,
    MON_GARGOYLETRAP,
    MON_SKMAGE_POIS1,
    MON_SKMAGE_POIS2,
    MON_SKMAGE_POIS3,
    MON_SKMAGE_POIS4,
    MON_FETISHSHAMAN1,
    MON_FETISHSHAMAN2,
    MON_FETISHSHAMAN3,
    MON_FETISHSHAMAN4,
    MON_FETISHSHAMAN5,
    MON_LARVA,
    MON_MAGGOTQUEEN1,
    MON_MAGGOTQUEEN2,
    MON_MAGGOTQUEEN3,
    MON_MAGGOTQUEEN4,
    MON_MAGGOTQUEEN5,
    MON_CLAYGOLEM,
    MON_BLOODGOLEM,
    MON_IRONGOLEM,
    MON_FIREGOLEM,
    MON_FAMILIAR,
    MON_ACT3MALE,
    MON_BABOON6,
    MON_ACT3FEMALE,
    MON_NATALYA,
    MON_VILEMOTHER1,
    MON_VILEMOTHER2,
    MON_VILEMOTHER3,
    MON_VILECHILD1,
    MON_VILECHILD2,
    MON_VILECHILD3,
    MON_FINGERMAGE1,
    MON_FINGERMAGE2,
    MON_FINGERMAGE3,
    MON_REGURGITATOR1,
    MON_REGURGITATOR2,
    MON_REGURGITATOR3,
    MON_DOOMKNIGHT1,
    MON_DOOMKNIGHT2,
    MON_DOOMKNIGHT3,
    MON_QUILLBEAR1,
    MON_QUILLBEAR2,
    MON_QUILLBEAR3,
    MON_QUILLBEAR4,
    MON_QUILLBEAR5,
    MON_SNAKE,
    MON_PARROT,
    MON_FISH,
    MON_EVILHOLE1,
    MON_EVILHOLE2,
    MON_EVILHOLE3,
    MON_EVILHOLE4,
    MON_EVILHOLE5,
    MON_TRAP_FIREBOLT,
    MON_TRAP_HORZMISSILE,
    MON_TRAP_VERTMISSILE,
    MON_TRAP_POISONCLOUD,
    MON_TRAP_LIGHTNING,
    MON_ACT2GUARD2,
    MON_INVISOSPAWNER,
    MON_DIABLOCLONE,
    MON_SUCKERNEST1,
    MON_SUCKERNEST2,
    MON_SUCKERNEST3,
    MON_SUCKERNEST4,
    MON_ACT2HIRE,
    MON_MINISPIDER,
    MON_BONEPRISON1,
    MON_BONEPRISON2,
    MON_BONEPRISON3,
    MON_BONEPRISON4,
    MON_BONEWALL,
    MON_COUNCILMEMBER1,
    MON_COUNCILMEMBER2,
    MON_COUNCILMEMBER3,
    MON_TURRET1,
    MON_TURRET2,
    MON_TURRET3,
    MON_HYDRA1,
    MON_HYDRA2,
    MON_HYDRA3,
    MON_TRAP_MELEE,
    MON_SEVENTOMBS,
    MON_DOPPLEZON,
    MON_VALKYRIE,
    MON_ACT2GUARD3,
    MON_ACT3HIRE,
    MON_MEGADEMON1,
    MON_MEGADEMON2,
    MON_MEGADEMON3,
    MON_NECROSKELETON,
    MON_NECROMAGE,
    MON_GRISWOLD,
    MON_COMPELLINGORB,
    MON_TYRAEL2,
    MON_DARKWANDERER,
    MON_TRAP_NOVA,
    MON_SPIRITMUMMY,
    MON_LIGHTNINGSPIRE,
    MON_FIRETOWER,
    MON_SLINGER1,
    MON_SLINGER2,
    MON_SLINGER3,
    MON_SLINGER4,
    MON_ACT2GUARD4,
    MON_ACT2GUARD5,
    MON_SKMAGE_COLD1,
    MON_SKMAGE_COLD2,
    MON_SKMAGE_COLD3,
    MON_SKMAGE_COLD4,
    MON_SKMAGE_FIRE1,
    MON_SKMAGE_FIRE2,
    MON_SKMAGE_FIRE3,
    MON_SKMAGE_FIRE4,
    MON_SKMAGE_LTNG1,
    MON_SKMAGE_LTNG2,
    MON_SKMAGE_LTNG3,
    MON_SKMAGE_LTNG4,
    MON_HELLBOVINE,
    MON_WINDOW1,
    MON_WINDOW2,
    MON_SLINGER5,
    MON_SLINGER6,
    MON_FETISHBLOW1,
    MON_FETISHBLOW2,
    MON_FETISHBLOW3,
    MON_FETISHBLOW4,
    MON_FETISHBLOW5,
    MON_MEPHISTOSPIRIT,
    MON_SMITH,
    MON_TRAPPEDSOUL1,
    MON_TRAPPEDSOUL2,
    MON_JAMELLA,
    MON_IZUALGHOST,
    MON_FETISH11,
    MON_MALACHAI,
    MON_HEPHASTO,
    MON_WAKEOFDESTRUCTION,
    MON_CHARGEBOLTSENTRY,
    MON_LIGHTNINGSENTRY,
    MON_BLADECREEPER,
    MON_INVISOPET,
    MON_INFERNOSENTRY,
    MON_DEATHSENTRY,
    MON_SHADOWWARRIOR,
    MON_SHADOWMASTER,
    MON_DRUIDHAWK,
    MON_SPIRITWOLF,
    MON_FENRIS,
    MON_SPIRITOFBARBS,
    MON_HEARTOFWOLVERINE,
    MON_OAKSAGE,
    MON_PLAGUEPOPPY,
    MON_CYCLEOFLIFE,
    MON_VINECREATURE,
    MON_DRUIDBEAR,
    MON_EAGLE,
    MON_WOLF,
    MON_BEAR,
    MON_BARRICADEDOOR1,
    MON_BARRICADEDOOR2,
    MON_PRISONDOOR,
    MON_BARRICADETOWER,
    MON_REANIMATEDHORDE1,
    MON_REANIMATEDHORDE2,
    MON_REANIMATEDHORDE3,
    MON_REANIMATEDHORDE4,
    MON_REANIMATEDHORDE5,
    MON_SIEGEBEAST1,
    MON_SIEGEBEAST2,
    MON_SIEGEBEAST3,
    MON_SIEGEBEAST4,
    MON_SIEGEBEAST5,
    MON_SNOWYETI1,
    MON_SNOWYETI2,
    MON_SNOWYETI3,
    MON_SNOWYETI4,
    MON_WOLFRIDER1,
    MON_WOLFRIDER2,
    MON_WOLFRIDER3,
    MON_MINION1,
    MON_MINION2,
    MON_MINION3,
    MON_MINION4,
    MON_MINION5,
    MON_MINION6,
    MON_MINION7,
    MON_MINION8,
    MON_SUICIDEMINION1,
    MON_SUICIDEMINION2,
    MON_SUICIDEMINION3,
    MON_SUICIDEMINION4,
    MON_SUICIDEMINION5,
    MON_SUICIDEMINION6,
    MON_SUICIDEMINION7,
    MON_SUICIDEMINION8,
    MON_SUCCUBUS1,
    MON_SUCCUBUS2,
    MON_SUCCUBUS3,
    MON_SUCCUBUS4,
    MON_SUCCUBUS5,
    MON_SUCCUBUSWITCH1,
    MON_SUCCUBUSWITCH2,
    MON_SUCCUBUSWITCH3,
    MON_SUCCUBUSWITCH4,
    MON_SUCCUBUSWITCH5,
    MON_OVERSEER1,
    MON_OVERSEER2,
    MON_OVERSEER3,
    MON_OVERSEER4,
    MON_OVERSEER5,
    MON_MINIONSPAWNER1,
    MON_MINIONSPAWNER2,
    MON_MINIONSPAWNER3,
    MON_MINIONSPAWNER4,
    MON_MINIONSPAWNER5,
    MON_MINIONSPAWNER6,
    MON_MINIONSPAWNER7,
    MON_MINIONSPAWNER8,
    MON_IMP1,
    MON_IMP2,
    MON_IMP3,
    MON_IMP4,
    MON_IMP5,
    MON_CATAPULT1,
    MON_CATAPULT2,
    MON_CATAPULT3,
    MON_CATAPULT4,
    MON_FROZENHORROR1,
    MON_FROZENHORROR2,
    MON_FROZENHORROR3,
    MON_FROZENHORROR4,
    MON_FROZENHORROR5,
    MON_BLOODLORD1,
    MON_BLOODLORD2,
    MON_BLOODLORD3,
    MON_BLOODLORD4,
    MON_BLOODLORD5,
    MON_LARZUK,
    MON_DREHYA,
    MON_MALAH,
    MON_NIHLATHAK,
    MON_QUAL_KEHK,
    MON_CATAPULTSPOTTER1,
    MON_CATAPULTSPOTTER2,
    MON_CATAPULTSPOTTER3,
    MON_CATAPULTSPOTTER4,
    MON_CAIN6,
    MON_TYRAEL3,
    MON_ACT5BARB1,
    MON_ACT5BARB2,
    MON_BARRICADEWALL1,
    MON_BARRICADEWALL2,
    MON_NIHLATHAKBOSS,
    MON_DREHYAICED,
    MON_EVILHUT,
    MON_DEATHMAULER1,
    MON_DEATHMAULER2,
    MON_DEATHMAULER3,
    MON_DEATHMAULER4,
    MON_DEATHMAULER5,
    MON_ACT5POW,
    MON_ACT5BARB3,
    MON_ACT5BARB4,
    MON_ANCIENTSTATUE1,
    MON_ANCIENTSTATUE2,
    MON_ANCIENTSTATUE3,
    MON_ANCIENTBARB1,
    MON_ANCIENTBARB2,
    MON_ANCIENTBARB3,
    MON_BAALTHRONE,
    MON_BAALCRAB,
    MON_BAALTAUNT,
    MON_PUTRIDDEFILER1,
    MON_PUTRIDDEFILER2,
    MON_PUTRIDDEFILER3,
    MON_PUTRIDDEFILER4,
    MON_PUTRIDDEFILER5,
    MON_PAINWORM1,
    MON_PAINWORM2,
    MON_PAINWORM3,
    MON_PAINWORM4,
    MON_PAINWORM5,
    MON_BUNNY,
    MON_BAALHIGHPRIEST,
    MON_VENOMLORD,
    MON_BAALCRABSTAIRS,
    MON_ACT5HIRE1,
    MON_ACT5HIRE2,
    MON_BAALTENTACLE1,
    MON_BAALTENTACLE2,
    MON_BAALTENTACLE3,
    MON_BAALTENTACLE4,
    MON_BAALTENTACLE5,
    MON_INJUREDBARB1,
    MON_INJUREDBARB2,
    MON_INJUREDBARB3,
    MON_BAALCLONE,
    MON_BAALMINION1,
    MON_BAALMINION2,
    MON_BAALMINION3,
    MON_WORLDSTONEEFFECT,
    MON_SK_ARCHER6,
    MON_SK_ARCHER7,
    MON_SK_ARCHER8,
    MON_SK_ARCHER9,
    MON_SK_ARCHER10,
    MON_BIGHEAD6,
    MON_BIGHEAD7,
    MON_BIGHEAD8,
    MON_BIGHEAD9,
    MON_BIGHEAD10,
    MON_GOATMAN6,
    MON_GOATMAN7,
    MON_GOATMAN8,
    MON_GOATMAN9,
    MON_GOATMAN10,
    MON_FOULCROW5,
    MON_FOULCROW6,
    MON_FOULCROW7,
    MON_FOULCROW8,
    MON_CLAWVIPER6,
    MON_CLAWVIPER7,
    MON_CLAWVIPER8,
    MON_CLAWVIPER9,
    MON_CLAWVIPER10,
    MON_SANDRAIDER6,
    MON_SANDRAIDER7,
    MON_SANDRAIDER8,
    MON_SANDRAIDER9,
    MON_SANDRAIDER10,
    MON_DEATHMAULER6,
    MON_QUILLRAT6,
    MON_QUILLRAT7,
    MON_QUILLRAT8,
    MON_VULTURE5,
    MON_THORNHULK5,
    MON_SLINGER7,
    MON_SLINGER8,
    MON_SLINGER9,
    MON_CR_ARCHER6,
    MON_CR_ARCHER7,
    MON_CR_LANCER6,
    MON_CR_LANCER7,
    MON_CR_LANCER8,
    MON_BLUNDERBORE5,
    MON_BLUNDERBORE6,
    MON_SKMAGE_FIRE5,
    MON_SKMAGE_FIRE6,
    MON_SKMAGE_LTNG5,
    MON_SKMAGE_LTNG6,
    MON_SKMAGE_COLD5,
    MON_SKMAGE_POIS5,
    MON_SKMAGE_POIS6,
    MON_PANTHERWOMAN5,
    MON_PANTHERWOMAN6,
    MON_SANDLEAPER6,
    MON_SANDLEAPER7,
    MON_WRAITH6,
    MON_WRAITH7,
    MON_WRAITH8,
    MON_SUCCUBUS6,
    MON_SUCCUBUS7,
    MON_SUCCUBUSWITCH6,
    MON_SUCCUBUSWITCH7,
    MON_SUCCUBUSWITCH8,
    MON_WILLOWISP5,
    MON_WILLOWISP6,
    MON_WILLOWISP7,
    MON_FALLEN6,
    MON_FALLEN7,
    MON_FALLEN8,
    MON_FALLENSHAMAN6,
    MON_FALLENSHAMAN7,
    MON_FALLENSHAMAN8,
    MON_SKELETON6,
    MON_SKELETON7,
    MON_BATDEMON6,
    MON_BATDEMON7,
    MON_BLOODLORD6,
    MON_BLOODLORD7,
    MON_SCARAB6,
    MON_SCARAB7,
    MON_FETISH6,
    MON_FETISH7,
    MON_FETISH8,
    MON_FETISHBLOW6,
    MON_FETISHBLOW7,
    MON_FETISHBLOW8,
    MON_FETISHSHAMAN6,
    MON_FETISHSHAMAN7,
    MON_FETISHSHAMAN8,
    MON_BABOON7,
    MON_BABOON8,
    MON_UNRAVELER6,
    MON_UNRAVELER7,
    MON_UNRAVELER8,
    MON_UNRAVELER9,
    MON_ZEALOT4,
    MON_ZEALOT5,
    MON_CANTOR5,
    MON_CANTOR6,
    MON_VILEMOTHER4,
    MON_VILEMOTHER5,
    MON_VILECHILD4,
    MON_VILECHILD5,
    MON_SANDMAGGOT6,
    MON_MAGGOTBABY6,
    MON_MAGGOTEGG6,
    MON_MINION9,
    MON_MINION10,
    MON_MINION11,
    MON_ARACH6,
    MON_MEGADEMON4,
    MON_MEGADEMON5,
    MON_IMP6,
    MON_IMP7,
    MON_BONEFETISH6,
    MON_BONEFETISH7,
    MON_FINGERMAGE4,
    MON_FINGERMAGE5,
    MON_REGURGITATOR4,
    MON_VAMPIRE6,
    MON_VAMPIRE7,
    MON_VAMPIRE8,
    MON_REANIMATEDHORDE6,
    MON_DKFIG1,
    MON_DKFIG2,
    MON_DKMAG1,
    MON_DKMAG2,
    MON_MUMMY6,
    MON_UBERMEPHISTO,
    MON_UBERDIABLO,
    MON_UBERIZUAL,
    MON_UBERANDARIEL,
    MON_UBERDURIEL,
    MON_UBERBAAL,
    MON_DEMONSPAWNER,
    MON_DEMONHOLE,
    MON_MEGADEMON6,
    MON_DKMAG3,
    MON_IMP8,
    MON_SWARM5,
    MON_SANDMAGGOT7,
    MON_ARACH7,
    MON_SCARAB8,
    MON_SUCCUBUS8,
    MON_SUCCUBUSWITCH9,
    MON_CORRUPTROGUE6,
    MON_CR_ARCHER8,
    MON_CR_LANCER9,
    MON_OVERSEER6,
    MON_SKELETON8,
    MON_SK_ARCHER11,
    MON_SKMAGE_FIRE7,
    MON_SKMAGE_LTNG7,
    MON_SKMAGE_COLD6,
    MON_SKMAGE_POIS7,
    MON_VAMPIRE9,
    MON_WRAITH9,
    MON_WILLOWISP8,
    MON_HORSE,
    MON_COWKING,
    MON_DARKELDER,
    MON_PIG,
    MON_SEAGULL,
    MON_SUICIDEMINION9,
    MON_SUICIDEMINION10,
    MON_SUICIDEMINION11
};
typedef enum MonsterId  MonsterId;


#define MAX_SUPER 66
#define MAX_SUPER_NAME 32

struct SuperInfo {
    SuperId id;
    char name[MAX_SUPER_NAME];
    MonsterId monster;
};
typedef struct SuperInfo  SuperInfo;

extern const SuperInfo SUPER_INFO[MAX_SUPER];


#define MAX_MONSTER 742
#define MAX_MONSTER_NAME 32
#define MAX_MONSTER_CLASS 24

struct MonsterInfo {
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
};
typedef struct MonsterInfo  MonsterInfo;

extern const MonsterInfo MONSTER_INFO[MAX_MONSTER];


#endif
