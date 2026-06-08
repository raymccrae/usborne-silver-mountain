#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#define ROOM_NONE 0
#define ROOM_HALF_DUG_GRAVE 1
#define ROOM_GOBLIN_GRAVEYARD 2
#define ROOM_HALLOW_TOMB 3
#define ROOM_STALACTITES_AND_STALAGMITES 4
#define ROOM_MAZE_OF_TUNNELS 5
#define ROOM_VAULTED_CAVERN 6
#define ROOM_HIGH_GLASS_GATES 7
#define ROOM_PALACE_ENTRANCE_HALL 8
#define ROOM_GRARG_SENTRY_POST 9
#define ROOM_GUARD_ROOM 10
#define ROOM_MARSHY_INLET 11
#define ROOM_RUSTY_GATES 12
#define ROOM_GAMEKEEPERS_COTTAGE 13
#define ROOM_MISTY_POOL 14
#define ROOM_HIGH_WALLED_GARDEN 15
#define ROOM_INSCRIBED_CAVERN 16
#define ROOM_ORNATE_FOUNTAIN 17
#define ROOM_DANK_CORRIDOR 18
#define ROOM_LONG_GALLERY 19
#define ROOM_KITCHENS 20
#define ROOM_OLD_KILN 21
#define ROOM_OVERGROWN_TRACK 22
#define ROOM_DISUSED_WATERWHEEL 23
#define ROOM_SLUICE_GATES 24
#define ROOM_GAP_BETWEEN_BOULDERS 25
#define ROOM_PERILOUS_PATH 26
#define ROOM_SILVER_BELL 27
#define ROOM_DUNGEONS 28
#define ROOM_BANQUETING_HALL 29
#define ROOM_PALACE_BATTLEMENTS 30
#define ROOM_ISLAND_SHORE 31
#define ROOM_BEACHED_KETCH 32
#define ROOM_BARREN_COUNTRYSIDE 33
#define ROOM_SACKS_UPPER_FLOOR 34
#define ROOM_FROZEN_POND 35
#define ROOM_MOUNTAIN_HUT 36
#define ROOM_ROW_OF_CASKS 37
#define ROOM_WINE_CELLAR 38
#define ROOM_HALL_OF_TAPESTRIES 39
#define ROOM_DUSTY_LIBRARY 40
#define ROOM_ROUGH_WATER 41
#define ROOM_PLOUGHED_FIELD 42
#define ROOM_OUTSIDE_WINDMILL 43
#define ROOM_LOWER_FLOOR_MILL 44
#define ROOM_ICY_PATH 45
#define ROOM_SCREE_SLOPE 46
#define ROOM_SILVER_CHAMBER 47
#define ROOM_WIZARDS_LAIR 48
#define ROOM_MOSAIC_HALL 49
#define ROOM_SILVER_THRONE_ROOM 50
#define ROOM_MIDDLE_OF_LAKE 51
#define ROOM_ICY_LAKE_EDGE 52
#define ROOM_PITTED_TRACK 53
#define ROOM_HIGH_PINNACLE 54
#define ROOM_ABOVE_GLACIER 55
#define ROOM_HUGE_FALLEN_OAK 56
#define ROOM_SLOT_MACHINE_TURRET 57
#define ROOM_COBWEBBY_ROOM 58
#define ROOM_OGBANS_SAFE 59
#define ROOM_CUPBOARD_ROOM 60
#define ROOM_NARROW_PASSAGE 61
#define ROOM_CAVE 62
#define ROOM_WOODMANS_HUT 63
#define ROOM_WOODED_VALLEY_SIDE 64
#define ROOM_STREAM_VALLEY_BOTTOM 65
#define ROOM_DEEP_DARK_WOOD 66
#define ROOM_SHADY_HOLLOW 67
#define ROOM_ANCIENT_STONE_CIRCLE 68
#define ROOM_STABLE 69
#define ROOM_ATTIC_BEDROOM 70
#define ROOM_WELL_BOTTOM 71
#define ROOM_TOP_OF_WELL 72
#define ROOM_BURNT_OUT_CAMPFIRE 73
#define ROOM_ORCHARD 74
#define ROOM_BRIDGE_WEST_END 75
#define ROOM_BRIDGE_EAST_END 76
#define ROOM_CROSSROADS 77
#define ROOM_WINDING_ROAD 78
#define ROOM_VILLAGE 79
#define ROOM_WHITE_COTTAGE 80

#define NOUN_NONE 0
#define NOUN_COINS 1
#define NOUN_SHEET 2
#define NOUN_BOOTS 3
#define NOUN_HORSESHOE 4
#define NOUN_APPLES 5
#define NOUN_BUCKET 6
#define NOUN_AXE 7
#define NOUN_BOAT 8
#define NOUN_PHIAL 9
#define NOUN_REEDS 10
#define NOUN_BONE 11
#define NOUN_SHIELD 12
#define NOUN_PLANKS 13
#define NOUN_ROPE 14
#define NOUN_RING 15
#define NOUN_JUG 16
#define NOUN_NET 17
#define NOUN_SWORD 18
#define NOUN_SILVER_PLATE 19
#define NOUN_UNIFORM 20
#define NOUN_KEY 21
#define NOUN_SEEDS 22
#define NOUN_LAMP 23
#define NOUN_BREAD 24
#define NOUN_BROOCH 25
#define NOUN_MATCHES 26
#define NOUN_STONE_OF_DESTINY 27
#define NOUN_APPLE 28
#define NOUN_BED 29
#define NOUN_CUPBOARD 30
#define NOUN_BRIDGE 31
#define NOUN_TREES 32
#define NOUN_SAIL 33
#define NOUN_KILN 34
#define NOUN_KETCH 35
#define NOUN_BRICKS 36
#define NOUN_WINDMILL 37
#define NOUN_SACKS 38
#define NOUN_OGBANS_BOAR 39
#define NOUN_WHEEL 40
#define NOUN_PONY 41
#define NOUN_GRAVESTONES 42
#define NOUN_POOL 43
#define NOUN_GATES 44
#define NOUN_HANDLE 45
#define NOUN_HUT 46
#define NOUN_VINE 47
#define NOUN_INSCRIPTIONS 48
#define NOUN_TROLL 49
#define NOUN_RUBBLE 50
#define NOUN_HOUND 51
#define NOUN_FOUNTAIN 52
#define NOUN_CIRCLE 53
#define NOUN_MOSAICS 54
#define NOUN_BOOKS 55
#define NOUN_CASKS 56
#define NOUN_WELL 57
#define NOUN_WALLS 58
#define NOUN_RATS 59
#define NOUN_SAFE 60
#define NOUN_COBWEBS 61
#define NOUN_COIN 62
#define NOUN_BELL 63
#define NOUN_UP_SILVER_PLATE 64
#define NOUN_STONES 65
#define NOUN_KITCHENS 66
#define NOUN_GOBLET 67
#define NOUN_WINE 68
#define NOUN_GRARGS 69
#define NOUN_DOOR 70
#define NOUN_AWAKE 71
#define NOUN_GUIDE 72
#define NOUN_PROTECT 73
#define NOUN_LEAD 74
#define NOUN_HELP 75
#define NOUN_CHEST 76
#define NOUN_WATER 77
#define NOUN_STABLES 78
#define NOUN_SLUICE_GATES 79
#define NOUN_POT 80
#define NOUN_STATUE 81
#define NOUN_PINNACLE 82
#define NOUN_MUSIC 83
#define NOUN_MAGIC_WORDS 84
#define NOUN_MISTY_POOL 85
#define NOUN_WELL_BOTTOM 86
#define NOUN_OLD_KILN 87
#define NOUN_MOUNTAIN_HUT 88

#define FLAGV_UNUSED 0
#define FLAGV_COINS_HIDDEN 1
#define FLAGV_SHEET_HIDDEN 2
#define FLAGV_BOOTS_HIDDEN 3
#define FLAGV_HORSESHOE_HIDDEN 4
#define FLAGV_APPLES_HIDDEN 5
#define FLAGV_BUCKET_HIDDEN 6
#define FLAGV_AXE_HIDDEN 7
#define FLAGV_BOAT_HIDDEN 8
#define FLAGV_PHIAL_HIDDEN 9
#define FLAGV_REEDS_HIDDEN 10
#define FLAGV_BONE_HIDDEN 11
#define FLAGV_SHIELD_HIDDEN 12
#define FLAGV_PLANKS_HIDDEN 13
#define FLAGV_ROPE_HIDDEN 14
#define FLAGV_RING_HIDDEN 15
#define FLAGV_JUG_HIDDEN 16
#define FLAGV_NET_HIDDEN 17
#define FLAGV_SWORD_HIDDEN 18
#define FLAGV_SILVER_PLATE_HIDDEN 19
#define FLAGV_UNIFORM_HIDDEN 20
#define FLAGV_KEY_HIDDEN 21
#define FLAGV_SEEDS_HIDDEN 22
#define FLAGV_LAMP_HIDDEN 23
#define FLAGV_BREAD_HIDDEN 24
#define FLAGV_BROOCH_HIDDEN 25
#define FLAGV_MATCHES_HIDDEN 26
#define FLAGV_STONE_OF_DESTINY_HIDDEN 27
#define FLAGV_APPLE_HIDDEN 28
#define FLAGV_BOOTS_WORN 29
#define FLAGV_BOAT_POWERED 30
#define FLAGV_BOAT_KEPT_AFLOAT 31
#define FLAGV_BOAR_CAUGHT 32
#define FLAGV_POOL_EMPTIED 33
#define FLAGV_JUG_FULL 34
#define FLAGV_GHOST_FREED 35
#define FLAGV_PARSER_PLURAL_RETRY 36
#define FLAGV_SEEDS_PLANTED 37
#define FLAGV_VINE_GROWN 38
#define FLAGV_BOOT_WEAR_COUNTER 39
#define FLAGV_SHEET_TIED_AT_WELL 40
#define FLAGV_SAFE_CODE 41
#define FLAGV_BELL_RING_COUNT 42
#define FLAGV_HORSESHOE_LOOSENED 43
#define FLAGV_COIN_COUNT 44
#define FLAGV_RUBBLE_MOVED 45
#define FLAGV_HOUND_DISTRACTED 46
#define FLAGV_POISON_PLACED 47
#define FLAGV_GRARG_SLEEPING 48
#define FLAGV_INVENTORY_HAS_ITEMS 49
#define FLAGV_LAMP_LIT 50
#define FLAGV_UNIFORM_WORN 51
#define FLAGV_THIRD_WORD_SELECTOR 52
#define FLAGV_ROPE_TIED_AT_WELL 53
#define FLAGV_CHARM_TRIO_ASSEMBLED 54
#define FLAGV_GRARG_PATROL_WARNING 55
#define FLAGV_PLAYER_DEAD 56
#define FLAGV_THIRD_WORD_ROOM_PROTECT 57
#define FLAGV_THIRD_WORD_ROOM_LEAD 58
#define FLAGV_THIRD_WORD_ROOM_HELP 59
#define FLAGV_SAID_AWAKE 60
#define FLAGV_SAID_GUIDE 61
#define FLAGV_QUEST_COMPLETE 62
#define FLAGV_WIZARD_DEFEATED 63
#define FLAGV_TROLL_TOLL_PAID 64
#define FLAGV_RATS_GONE 65
#define FLAGV_COBWEBS_CLEARED 66
#define FLAGV_ROUGH_WATER_TURN_COUNTER 67
#define FLAGV_OGBAN_DEAD 68
#define FLAGV_SAVED_ROOM 69
#define FLAGV_WIZARD_DOOR_UNLOCKED 70

#define ROOM_COUNT ROOM_WHITE_COTTAGE
#define CARRIED_OBJECTS 28
#define NOUN_COUNT 88
#define VERB_COUNT 57
#define FLAG_COUNT 70
#define INVENTORY 0
#define GONE 81
#define LINE_LEN 39

#if defined(__GNUC__)
#define SILVERM_UNUSED __attribute__((unused))
#else
#define SILVERM_UNUSED
#endif

typedef struct {
    int x;
    int y;
    const char *name;
} Room;

typedef struct {
    int article;
    const char *name;
} Noun;

typedef struct {
    int room;
    int noun;
    int verb;
    char verb_text[32];
    char noun_text[96];
    char response[512];
    int running;
} Game;

static const char *prep_x[7] = {"", "IN", "NEAR", "BY", "ON", "", "AT"};
static const char *article_y[7] = {"", "A", "THE", "SOME", "AN", "", "A SMALL"};

static const Room rooms[ROOM_COUNT + 1] = {
    {0, 0, NULL},
    {1, 1, "HALF-DUG GRAVE"}, {1, 2, "GOBLIN GRAVEYARD"},
    {1, 1, "HALLOW TOMB"}, {2, 3, "STALACTITES AND STALAGMITES"},
    {1, 1, "MAZE OF TUNNELS"}, {1, 1, "VALUTED CAVERN"},
    {2, 3, "HIGH GLASS GATES"}, {1, 2, "ENTRANCE HALL TO THE PALACE"},
    {3, 1, "GRARG SENTRY POST"}, {1, 2, "GUARD ROOM"},
    {3, 1, "MARSHY INLET"}, {2, 3, "RUSTY GATES"},
    {1, 2, "GAMEKEEPER'S COTTAGE"}, {3, 1, "MISTY POOL"},
    {1, 1, "HIGH-WALLED GARDEN"}, {1, 4, "INSCRIBED CAVERN"},
    {3, 4, "ORNATE FOUNTAIN"}, {1, 1, "DANK CORRIDOR"},
    {1, 2, "LONG GALLERY"}, {1, 2, "KITCHENS OF THE PALACE"},
    {3, 4, "OLD KILN"}, {4, 4, "OVERGROWN TRACK"},
    {3, 1, "DISUSED WATERWHEEL"}, {3, 3, "SLUICE GATES"},
    {1, 1, "GAP BETWEEN SOME BOULDERS"}, {4, 1, "PERILOUS PATH"},
    {3, 1, "SILVER BELL IN THE ROCK"}, {1, 2, "DUNGEONS OF THE PALACE"},
    {1, 1, "BANQUETING HALL"}, {4, 2, "PALACE BATTLEMENTS"},
    {4, 4, "ISLAND SHORE"}, {3, 1, "BEACHED KETCH"},
    {1, 3, "BARREN COUNTRYSIDE"}, {3, 3, "SACKS ON THE UPPER FLOOR"},
    {4, 6, "FROZEN POND"}, {2, 1, "MOUNTAIN HUT"},
    {3, 1, "ROW OF CASKS"}, {1, 1, "WINE CELLAR"},
    {1, 2, "HALL OF TAPESTRIES"}, {1, 1, "DUSTY LIBRARY"},
    {1, 3, "ROUGH WATER"}, {1, 1, "PLOUGHED FIELD"},
    {5, 5, "OUTSIDE A WINDMILL"}, {4, 2, "LOWER FLOOR OF THE MILL"},
    {4, 4, "ICY PATH"}, {4, 1, "SCREE SLOPE"},
    {1, 2, "SILVER CHAMBER"}, {1, 2, "WIZARD'S LAIR"},
    {1, 1, "MOSAIC-FLOORED HALL"}, {1, 2, "SILVER THRONE ROOM"},
    {1, 2, "MIDDLE OF THE LAKE"}, {4, 2, "EDGE OF AN ICY LAKE"},
    {4, 1, "PITTED TRACK"}, {4, 1, "HIGH PINNACLE"},
    {5, 5, "ABOVE A GLACIER"}, {2, 1, "HUGE FALLEN OAK"},
    {1, 1, "TURRET ROOM WITH A SLOT MACHINE"}, {1, 1, "COBWEBBY ROOM"},
    {3, 1, "SAFE IN OGBAN'S CHAMBER"}, {3, 1, "CUPBOARD IN A CORNER"},
    {1, 1, "NARROW PASSAGE"}, {1, 6, "CAVE"},
    {1, 1, "WOODMAN'S HUT"}, {4, 2, "SIDE OF A WOODED VALLEY"},
    {2, 1, "STREAM IN A VALLEY BOTTOM"}, {1, 1, "DEEP DARK WOOD"},
    {1, 1, "SHADY HOLLOW"}, {3, 4, "ANCIENT STONE CIRCLE"},
    {1, 6, "STABLE"}, {1, 4, "ATTIC BEDROOM"},
    {1, 1, "DAMP WELL BOTTOM"}, {3, 2, "TOP OF A DEEP WELL"},
    {3, 1, "BURNT-OUT CAMPFIRE"}, {1, 6, "ORCHARD"},
    {6, 2, "END OF A BRIDGE"}, {6, 2, "END OF A BRIDGE"},
    {6, 1, "CROSSROADS"}, {4, 1, "WINDING ROAD"},
    {1, 1, "VILLAGE OF RUSTIC HOUSES"}, {1, 1, "WHITE COTTAGE"}
};

static const Noun nouns[NOUN_COUNT + 1] = {
    {0, NULL},
    {3, "COINS"}, {1, "SHEET"}, {3, "BOOTS"}, {1, "HORSESHOE"},
    {3, "APPLES"}, {1, "BUCKET"}, {4, "AXE"}, {1, "BOAT"},
    {1, "PHIAL"}, {3, "REEDS"}, {1, "BONE"}, {1, "SHIELD"},
    {3, "PLANKS"}, {1, "ROPE"}, {1, "RING"}, {1, "JUG"},
    {1, "NET"}, {1, "SWORD"}, {1, "SILVER PLATE"},
    {1, "UNIFORM"}, {1, "KEY"}, {3, "SEEDS"}, {1, "LAMP"},
    {3, "BREAD"}, {1, "BROOCH"}, {3, "MATCHES"},
    {2, "STONE OF DESTINY"}, {4, "APPLE"},
    {0, "BED"}, {0, "CUPBOARD"}, {0, "BRIDGE"}, {0, "TREES"},
    {0, "SAIL"}, {0, "KILN"}, {0, "KETCH"}, {0, "BRICKS"},
    {0, "WINDMILL"}, {0, "SACKS"}, {0, "OGBAN'S BOAR"}, {0, "WHEEL"},
    {0, "PONY"}, {0, "GRAVESTONES"}, {0, "POOL"}, {0, "GATES"},
    {0, "HANDLE"}, {0, "HUT"}, {0, "VINE"}, {0, "INSCRIPTIONS"},
    {0, "TROLL"}, {0, "RUBBLE"}, {0, "HOUND"}, {0, "FOUNTAIN"},
    {0, "CIRCLE"}, {0, "MOSAICS"}, {0, "BOOKS"}, {0, "CASKS"},
    {0, "WELL"}, {0, "WALLS"}, {0, "RATS"}, {0, "SAFE"},
    {0, "COBWEBS"}, {0, "COIN"}, {0, "BELL"}, {0, "UP SILVER PLATE"},
    {0, "STONES"}, {0, "KITCHENS"}, {0, "GOBLET"}, {0, "WINE"},
    {0, "GRARGS"}, {0, "DOOR"}, {0, "AWAKE"}, {0, "GUIDE"},
    {0, "PROTECT"}, {0, "LEAD"}, {0, "HELP"}, {0, "CHEST"},
    {0, "WATER"}, {0, "STABLES"}, {0, "SLUICE GATES"},
    {0, "POT"}, {0, "STATUE"}, {0, "PINNACLE"}, {0, "MUSIC"},
    {0, "MAGIC WORDS"}, {0, "MISTY POOL"}, {0, "WELL BOTTOM"},
    {0, "OLD KILN"}, {0, "MOUNTAIN HUT"}
};

static const char *initial_exits[ROOM_COUNT + 1] = {
    NULL,
    "E", "ESW", "WE", "EW", "EW", "ESW", "ESW", "ES", "EW", "SW",
    "S", "N", "ES", "SW", "S", "NW", "N", "N", "ES", "NSW",
    "NS", "E", "NSW", "N", "NES", "EW", "W", "S", "NS", "N",
    "NES", "W", "NS", "D", "NES", "SW", "E", "NW", "NS", "S",
    "NS", "E", "NSEW", "WU", "UD", "NS", "E", "SW", "NSE", "NW",
    "NE", "EW", "NSW", "E", "WN", "S", "E", "NEW", "NW", "S",
    "ES", "SW", "NES", "EW", "SW", "NE", "EW", "ESW", "SW", "ND",
    " ", "E", "NEW", "EW", "NEW", "EW", "EW", "NEW", "NEW", "WU"
};

static const int initial_locations[CARRIED_OBJECTS + 1] = {
    ROOM_NONE, ROOM_WHITE_COTTAGE, ROOM_ATTIC_BEDROOM, ROOM_CUPBOARD_ROOM,
    ROOM_STABLE, ROOM_ORCHARD, ROOM_TOP_OF_WELL, ROOM_WOODMANS_HUT,
    ROOM_ICY_LAKE_EDGE, ROOM_KITCHENS, ROOM_MARSHY_INLET,
    ROOM_HALF_DUG_GRAVE, ROOM_MISTY_POOL, ROOM_MOUNTAIN_HUT,
    ROOM_HIGH_PINNACLE, ROOM_NARROW_PASSAGE, ROOM_OLD_KILN,
    ROOM_BEACHED_KETCH, ROOM_GUARD_ROOM, ROOM_SILVER_THRONE_ROOM,
    ROOM_BANQUETING_HALL, ROOM_OGBANS_SAFE, ROOM_SACKS_UPPER_FLOOR,
    ROOM_GAMEKEEPERS_COTTAGE, ROOM_WHITE_COTTAGE, ROOM_PALACE_BATTLEMENTS,
    GONE, ROOM_SILVER_CHAMBER, ROOM_ORCHARD
};

static const int initially_hidden[] = {
    FLAGV_COINS_HIDDEN, FLAGV_SHEET_HIDDEN, FLAGV_BOOTS_HIDDEN,
    FLAGV_HORSESHOE_HIDDEN, FLAGV_APPLES_HIDDEN, FLAGV_PHIAL_HIDDEN,
    FLAGV_SHIELD_HIDDEN, FLAGV_PLANKS_HIDDEN, FLAGV_JUG_HIDDEN,
    FLAGV_NET_HIDDEN, FLAGV_UNIFORM_HIDDEN, FLAGV_KEY_HIDDEN,
    FLAGV_SEEDS_HIDDEN
};
static const char *verb_tokens[VERB_COUNT + 1] = {
    NULL, "NOO", "EOO", "SOO", "WOO", "UOO", "DOO", "INV", "GET", "TAK",
    "EXA", "REA", "GIV", "SAY", "PIC", "WEA", "TIE", "CLI", "RIG", "USE",
    "OPE", "LIG", "FIL", "PLA", "WAT", "SWI", "EMP", "ENT", "CRO", "REM",
    "FEE", "TUR", "DIV", "BAI", "LEA", "THR", "INS", "BLO", "DRO", "EAT",
    "MOV", "INT", "RIN", "CUT", "HOL", "BUR", "POI", "SHO", "UNL", "WIT",
    "DRI", "COU", "PAY", "MAK", "BRE", "STE", "GAT", "REF"
};

static int loc[CARRIED_OBJECTS + 1];
static int flagv[FLAG_COUNT + 1];
static char exits[ROOM_COUNT + 1][8];
static char maze_path[2][16];
static FILE *game_input;
static int curses_active;
static int (*ui_read_line)(const char *prompt, char *buf, size_t size);

static void ui_add_event(const char *s);

static char *read_input(char *buf, size_t size) {
    if (ui_read_line) {
        return ui_read_line("Input", buf, size) ? buf : NULL;
    }
    return fgets(buf, (int)size, game_input ? game_input : stdin);
}

static void set_response(Game *g, const char *s) {
    snprintf(g->response, sizeof(g->response), "%s", s);
}

static void append_response(Game *g, const char *a, const char *b) {
    snprintf(g->response, sizeof(g->response), "%s%s", a, b);
}

static void title(void) {
    if (curses_active) return;
    putchar('\n');
    puts("MYSTERY OF SILVER");
    puts("    MOUNTAIN");
    puts("========================================");
    putchar('\n');
}

static void wait_return(void) {
    char buf[32];
    if (curses_active) return;
    puts("PRESS RETURN TO CONTINUE");
    (void)read_input(buf, sizeof(buf));
}

static void wrap_print(const char *s) {
    int col = 0;
    const char *word = s;
    while (*word) {
        while (*word == ' ') {
            putchar(' ');
            col++;
            word++;
        }
        const char *end = word;
        while (*end && *end != ' ') {
            end++;
        }
        int len = (int)(end - word);
        if (col > 0 && col + len > LINE_LEN) {
            putchar('\n');
            col = 0;
        }
        fwrite(word, 1, (size_t)len, stdout);
        col += len;
        word = end;
    }
}

static void uppercase_trim(char *s) {
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\r' || isspace((unsigned char)s[n - 1]))) {
        s[--n] = '\0';
    }
    size_t start = 0;
    while (s[start] && isspace((unsigned char)s[start])) {
        start++;
    }
    if (start) {
        memmove(s, s + start, strlen(s + start) + 1);
    }
    for (size_t i = 0; s[i]; i++) {
        s[i] = (char)toupper((unsigned char)s[i]);
    }
}

static int hcode(const Game *g) {
    if (g->noun < 10) {
        return g->room * 10 + g->noun;
    }
    return g->room * 100 + g->noun;
}

static void random_maze(void) {
    const char dirs[] = "NESW";
    char reverse[9];
    maze_path[0][0] = '\0';
    maze_path[1][0] = '\0';
    for (int i = 0; i < 8; i++) {
        char d = dirs[rand() % 4];
        maze_path[0][i] = d;
        if (d == 'N') reverse[7 - i] = 'S';
        if (d == 'S') reverse[7 - i] = 'N';
        if (d == 'E') reverse[7 - i] = 'W';
        if (d == 'W') reverse[7 - i] = 'E';
    }
    maze_path[0][8] = '\0';
    reverse[8] = '\0';
    snprintf(maze_path[1], sizeof(maze_path[1]), "%s", reverse);
}

static void new_game(Game *g) {
    memset(flagv, 0, sizeof(flagv));
    for (int i = 1; i <= ROOM_COUNT; i++) {
        snprintf(exits[i], sizeof(exits[i]), "%s", initial_exits[i]);
    }
    for (int i = 1; i <= CARRIED_OBJECTS; i++) {
        loc[i] = initial_locations[i];
    }
    for (size_t i = 0; i < sizeof(initially_hidden) / sizeof(initially_hidden[0]); i++) {
        flagv[initially_hidden[i]] = 1;
    }
    flagv[FLAGV_SAFE_CODE] = rand() % 900 + 100;
    flagv[FLAGV_BELL_RING_COUNT] = rand() % 3 + 2;
    flagv[FLAGV_COIN_COUNT] = 4;
    flagv[FLAGV_THIRD_WORD_ROOM_PROTECT] = ROOM_ANCIENT_STONE_CIRCLE;
    flagv[FLAGV_THIRD_WORD_ROOM_LEAD] = ROOM_HIGH_PINNACLE;
    flagv[FLAGV_THIRD_WORD_ROOM_HELP] = ROOM_HIGH_WALLED_GARDEN;
    flagv[FLAGV_THIRD_WORD_SELECTOR] = rand() % 3;
    random_maze();
    g->room = ROOM_CROSSROADS;
    g->running = 1;
    set_response(g, "GOOD LUCK ON YOUR QUEST!");
}

static int save_game(const Game *g, const char *name) {
    FILE *fp = fopen(name, "w");
    if (!fp) {
        return 0;
    }
    fprintf(fp, "SILVERM 1\n%d\n", g->room);
    for (int i = 1; i <= ROOM_COUNT; i++) {
        fprintf(fp, "%s\n", exits[i]);
    }
    for (int i = 1; i <= CARRIED_OBJECTS; i++) {
        fprintf(fp, "%d\n", loc[i]);
    }
    for (int i = 1; i <= FLAG_COUNT; i++) {
        fprintf(fp, "%d\n", flagv[i]);
    }
    fprintf(fp, "%s\n%s\n", maze_path[0], maze_path[1]);
    fclose(fp);
    return 1;
}

static int load_game(Game *g, const char *name) {
    FILE *fp = fopen(name, "r");
    char line[128];
    if (!fp) {
        return 0;
    }
    if (!fgets(line, sizeof(line), fp) || strncmp(line, "SILVERM 1", 9) != 0) {
        fclose(fp);
        return 0;
    }
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return 0;
    }
    g->room = atoi(line);
    for (int i = 1; i <= ROOM_COUNT; i++) {
        if (!fgets(exits[i], sizeof(exits[i]), fp)) {
            fclose(fp);
            return 0;
        }
        uppercase_trim(exits[i]);
    }
    for (int i = 1; i <= CARRIED_OBJECTS; i++) {
        if (!fgets(line, sizeof(line), fp)) {
            fclose(fp);
            return 0;
        }
        loc[i] = atoi(line);
    }
    for (int i = 1; i <= FLAG_COUNT; i++) {
        if (!fgets(line, sizeof(line), fp)) {
            fclose(fp);
            return 0;
        }
        flagv[i] = atoi(line);
    }
    if (!fgets(maze_path[0], sizeof(maze_path[0]), fp) || !fgets(maze_path[1], sizeof(maze_path[1]), fp)) {
        fclose(fp);
        return 0;
    }
    uppercase_trim(maze_path[0]);
    uppercase_trim(maze_path[1]);
    fclose(fp);
    g->running = 1;
    set_response(g, "OK. CARRY ON");
    return g->room >= ROOM_HALF_DUG_GRAVE && g->room <= ROOM_WHITE_COTTAGE;
}

static int ask_filename(char *name, size_t size) {
    printf("\nPLEASE ENTER FILE NAME\n");
    if (!read_input(name, size)) {
        return 0;
    }
    size_t n = strlen(name);
    while (n && (name[n - 1] == '\n' || name[n - 1] == '\r')) {
        name[--n] = '\0';
    }
    return n > 0;
}

static void SILVERM_UNUSED describe_room(const Game *g) {
    char line[2048];
    title();
    snprintf(line, sizeof(line), "%s. YOU ARE %s %s %s ", g->response,
             prep_x[rooms[g->room].x], article_y[rooms[g->room].y], rooms[g->room].name);
    wrap_print(line);
    char visible[1024] = "";
    for (int i = 1; i < CARRIED_OBJECTS; i++) {
        if (flagv[i] == 0 && loc[i] == g->room) {
            char item[80];
            snprintf(item, sizeof(item), " %s %s,", article_y[nouns[i].article], nouns[i].name);
            strncat(visible, item, sizeof(visible) - strlen(visible) - 1);
        }
    }
    if (g->room == ROOM_BANQUETING_HALL && flagv[FLAGV_GRARG_SLEEPING] == 0) strncat(visible, " GRARGS FEASTING,", sizeof(visible) - strlen(visible) - 1);
    if (g->room == ROOM_BANQUETING_HALL && flagv[FLAGV_GRARG_SLEEPING] == 1) strncat(visible, " A SLEEPING GRARG,", sizeof(visible) - strlen(visible) - 1);
    if (g->room == ROOM_RUSTY_GATES || g->room == ROOM_OVERGROWN_TRACK) strncat(visible, " A PONY,", sizeof(visible) - strlen(visible) - 1);
    if (g->room == ROOM_WOODED_VALLEY_SIDE) strncat(visible, " A HERMIT,", sizeof(visible) - strlen(visible) - 1);
    if (g->room == ROOM_DANK_CORRIDOR && strcmp(exits[ROOM_DANK_CORRIDOR], "N") == 0) strncat(visible, " AN OAK DOOR,", sizeof(visible) - strlen(visible) - 1);
    if (g->room == ROOM_OGBANS_SAFE && flagv[FLAGV_OGBAN_DEAD] == 1) strncat(visible, " OGBAN (DEAD),", sizeof(visible) - strlen(visible) - 1);
    if (visible[0]) {
        snprintf(line, sizeof(line), ", YOU CAN SEE%s", visible);
        wrap_print(line);
    }
    wrap_print(" AND YOU CAN GO ");
    for (size_t i = 0; exits[g->room][i]; i++) {
        printf(" %c", exits[g->room][i]);
    }
    printf("\n\n========================================\n\n\n");
}

static int find_verb(const char *verb) {
    char token[4] = {'O', 'O', 'O', '\0'};
    char temp[32];
    snprintf(temp, sizeof(temp), "%s", verb);
    if (strcmp(temp, "PLAY") == 0) {
        strcpy(temp, "BLO");
    }
    for (int i = 0; i < 3 && temp[i]; i++) {
        token[i] = temp[i];
    }
    for (int i = 1; i <= VERB_COUNT; i++) {
        if (strcmp(token, verb_tokens[i]) == 0) {
            return i;
        }
    }
    return VERB_COUNT + 1;
}

static int find_noun_once(const char *noun) {
    for (int i = 1; i <= NOUN_COUNT; i++) {
        if (strcmp(noun, nouns[i].name) == 0) {
            return i;
        }
    }
    return 0;
}

static int find_noun(const char *noun) {
    int b = find_noun_once(noun);
    if (b == 0 && noun[0]) {
        char plural[128];
        snprintf(plural, sizeof(plural), "%sS", noun);
        b = find_noun_once(plural);
    }
    return b;
}

static void parse_command(Game *g, char *input) {
    char *space;
    g->verb = 0;
    g->noun = NOUN_NONE;
    g->verb_text[0] = '\0';
    g->noun_text[0] = '\0';
    uppercase_trim(input);
    space = strchr(input, ' ');
    if (space) {
        *space++ = '\0';
        while (*space == ' ') {
            space++;
        }
        snprintf(g->verb_text, sizeof(g->verb_text), "%s", input);
        snprintf(g->noun_text, sizeof(g->noun_text), "%s", space);
    } else {
        snprintf(g->verb_text, sizeof(g->verb_text), "%s", input);
    }
    g->verb = find_verb(g->verb_text);
    if (g->noun_text[0]) {
        g->noun = find_noun(g->noun_text);
    }
}

static void do_fill(Game *g);
static void do_give(Game *g);
static void do_break(Game *g);
static void do_drop(Game *g);
static void do_blow(Game *g);
static void do_reflect(Game *g);
static void do_get(Game *g);
static void do_examine(Game *g);
static void do_enter(Game *g);
static void do_use(Game *g);
static void do_light(Game *g);
static void do_plant(Game *g);
static void do_water(Game *g);
static void do_move(Game *g);
static void do_drink(Game *g);
static void do_wear(Game *g);
static void do_tie(Game *g);
static void do_climb(Game *g);

static void do_tunnel_maze(Game *g, int d) {
    char trail[9] = "SSSSSSSS";
    int misses = 0;
    int idx = d / 2 - 1;
    while (strcmp(trail, maze_path[idx]) != 0) {
        char input[64];
        if (curses_active) {
            if (!ui_read_line("Lost in tunnels - which way? (N,S,W,E or G)", input, sizeof(input))) {
                flagv[FLAGV_PLAYER_DEAD] = 1;
                return;
            }
        } else {
            title();
            puts("YOU ARE LOST IN THE");
            puts("     TUNNELS");
            puts("WHICH WAY? (N,S,W OR E)");
            if (misses > 15) {
                puts("(OR G TO GIVE UP!)");
            }
            putchar('\n');
            if (!read_input(input, sizeof(input))) {
                flagv[FLAGV_PLAYER_DEAD] = 1;
                return;
            }
        }
        if (!input[0]) {
            flagv[FLAGV_PLAYER_DEAD] = 1;
            return;
        }
        uppercase_trim(input);
        char c = input[0] ? input[strlen(input) - 1] : ' ';
        memmove(trail, trail + 1, 7);
        trail[7] = c;
        trail[8] = '\0';
        if (strcmp(input, "G") == 0) {
            flagv[FLAGV_PLAYER_DEAD] = 1;
            return;
        }
        if (strcmp(trail, maze_path[idx]) != 0) {
            misses++;
        }
    }
    (void)g;
}

static void do_move_direction(Game *g) {
    int d = g->verb;
    if (d == 5) d = 1;
    if (d == 6) d = 3;
    if (!(((g->room == ROOM_BRIDGE_WEST_END && d == 2) || (g->room == ROOM_BRIDGE_EAST_END && d == 4))) || flagv[FLAGV_TROLL_TOLL_PAID] == 1) {
    } else {
        set_response(g, "A TROLL STOPS YOU CROSSING");
        return;
    }
    if (flagv[FLAGV_TROLL_TOLL_PAID] == 1) flagv[FLAGV_TROLL_TOLL_PAID] = 0;
    if (!(flagv[FLAGV_UNIFORM_WORN] == 1 || flagv[FLAGV_BOOTS_WORN] == 1)) {
        if (flagv[FLAGV_GRARG_PATROL_WARNING] == 1) {
            flagv[FLAGV_PLAYER_DEAD] = 1;
            set_response(g, "GRARGS HAVE GOT YOU!");
            return;
        }
        if (g->room == ROOM_BANQUETING_HALL && flagv[FLAGV_GRARG_SLEEPING] == 0) {
            set_response(g, "GRARGS WILL SEE YOU!");
            return;
        }
        if (g->room == ROOM_BURNT_OUT_CAMPFIRE || g->room == ROOM_PLOUGHED_FIELD || g->room == ROOM_GRARG_SENTRY_POST || g->room == ROOM_GUARD_ROOM) {
            set_response(g, "A GRARG PATROL APPROACHES");
            flagv[FLAGV_GRARG_PATROL_WARNING] = 1;
            return;
        }
    }
    if (loc[NOUN_BOAT] == INVENTORY && ((g->room == ROOM_ICY_LAKE_EDGE && d == 2) || (g->room == ROOM_ISLAND_SHORE && d != 3))) {
        set_response(g, "THE BOAT IS TOO HEAVY");
        return;
    }
    if (loc[NOUN_BOAT] != INVENTORY && ((g->room == ROOM_ICY_LAKE_EDGE && d == 4) || (g->room == ROOM_ISLAND_SHORE && d == 3))) {
        set_response(g, "YOU CANNOT SWIM");
        return;
    }
    if (g->room == ROOM_ICY_LAKE_EDGE && loc[NOUN_BOAT] == INVENTORY && d == 4 && flagv[FLAGV_BOAT_POWERED] == 0) {
        set_response(g, "NO POWER!");
        return;
    }
    if (g->room == ROOM_ROUGH_WATER && d == 3 && flagv[FLAGV_BOAT_KEPT_AFLOAT] == 0) {
        set_response(g, "THE BOAT IS SINKING!");
        return;
    }
    if (g->room == ROOM_BARREN_COUNTRYSIDE && d == 1 && flagv[FLAGV_BOAR_CAUGHT] == 0) {
        set_response(g, "OGBAN'S BOAR BLOCKS YOUR PATH");
        return;
    }
    if (((g->room == ROOM_HALLOW_TOMB && d == 2) || (g->room == ROOM_STALACTITES_AND_STALAGMITES && d == 4)) && flagv[FLAGV_RUBBLE_MOVED] == 0) {
        set_response(g, "A PILE OF RUBBLE BLOCKS YOUR PATH");
        return;
    }
    if (g->room == ROOM_FROZEN_POND && loc[NOUN_PLANKS] != g->room) {
        set_response(g, "THE ICE IS BREAKING!");
        return;
    }
    if (g->room == ROOM_MAZE_OF_TUNNELS && (d == 2 || d == 4)) {
        do_tunnel_maze(g, d);
        if (flagv[FLAGV_PLAYER_DEAD]) return;
    }
    if (g->room == ROOM_STALACTITES_AND_STALAGMITES && d == 4) {
        set_response(g, "PASSAGE IS TOO STEEP");
        return;
    }
    if (g->room == ROOM_HIGH_GLASS_GATES && d == 2 && flagv[FLAGV_HOUND_DISTRACTED] == 0) {
        set_response(g, "A HUGE HOUND BARS YOUR WAY");
        return;
    }
    if ((g->room == ROOM_WINE_CELLAR || g->room == ROOM_ROW_OF_CASKS) && flagv[FLAGV_LAMP_LIT] == 0) {
        set_response(g, "IT IS TOO DARK");
        return;
    }
    if (g->room == ROOM_MOSAIC_HALL && d == 2 && flagv[FLAGV_CHARM_TRIO_ASSEMBLED] == 0) {
        set_response(g, "MYSTERIOUS FORCES HOLD YOU BACK");
        return;
    }
    if (g->room == ROOM_MOSAIC_HALL && d == 3 && flagv[FLAGV_OGBAN_DEAD] == 0) {
        set_response(g, "YOU MET OGBAN!!!");
        flagv[FLAGV_PLAYER_DEAD] = 1;
        return;
    }
    if (g->room == ROOM_WINE_CELLAR && flagv[FLAGV_RATS_GONE] == 0) {
        set_response(g, "RATS NIBBLE YOUR ANKLES");
        return;
    }
    if (g->room == ROOM_COBWEBBY_ROOM && (d == 1 || d == 4) && flagv[FLAGV_COBWEBS_CLEARED] == 0) {
        set_response(g, "YOU GET CAUGHT IN THE WEBS!");
        return;
    }
    if (g->room == ROOM_WIZARDS_LAIR && d == 4 && flagv[FLAGV_WIZARD_DOOR_UNLOCKED] == 0) {
        set_response(g, "THE DOOR DOES NOT OPEN");
        return;
    }
    if (g->room == ROOM_DUSTY_LIBRARY && flagv[FLAGV_POISON_PLACED] == 1) flagv[FLAGV_OGBAN_DEAD] = 1;
    if (g->room == ROOM_ROW_OF_CASKS && d == 4 && strcmp(exits[ROOM_ROW_OF_CASKS], "EW") == 0) {
        g->room = ROOM_SHADY_HOLLOW;
        set_response(g, "THE PASSAGE WAS STEEP!");
        return;
    }
    if (g->room == ROOM_BANQUETING_HALL && d == 3) {
        flagv[FLAGV_GRARG_SLEEPING] = 1;
        flagv[FLAGV_UNIFORM_HIDDEN] = 0;
    }
    if (g->room == ROOM_PALACE_ENTRANCE_HALL && d == 2) flagv[FLAGV_HOUND_DISTRACTED] = 0;
    int old = g->room;
    for (size_t i = 0; exits[old][i]; i++) {
        char k = exits[old][i];
        if ((k == 'N' || k == 'U') && d == 1) g->room = old - 10;
        if (k == 'E' && d == 2) g->room = old + 1;
        if ((k == 'S' || k == 'D') && d == 3) g->room = old + 10;
        if (k == 'W' && d == 4) g->room = old - 1;
    }
    set_response(g, "OK");
    if (g->room == old) set_response(g, "YOU CANNOT GO THAT WAY");
    if ((old == ROOM_BRIDGE_WEST_END && d == 2) || (old == ROOM_BRIDGE_EAST_END && d == 4)) set_response(g, "OK. YOU CROSSED");
    if (flagv[FLAGV_BOOTS_WORN] == 1) flagv[FLAGV_BOOT_WEAR_COUNTER]++;
    if (flagv[FLAGV_BOOT_WEAR_COUNTER] > 5 && flagv[FLAGV_BOOTS_WORN] == 1) {
        set_response(g, "BOOTS HAVE WORN OUT");
        flagv[FLAGV_BOOTS_WORN] = 0;
        loc[NOUN_BOOTS] = GONE;
    }
}

static void do_inventory(Game *g) {
    int any = 0;
    set_response(g, "OK");
    if (curses_active) {
        char line[512] = "YOU HAVE ";
        for (int i = 1; i <= CARRIED_OBJECTS; i++) {
            if (i == CARRIED_OBJECTS && loc[NOUN_APPLES] == INVENTORY) {
                continue;
            }
            if (loc[i] == INVENTORY) {
                const char *name = nouns[i].name;
                if (i == NOUN_COINS && flagv[FLAGV_COIN_COUNT] == 1) name = "COIN";
                strncat(line, name, sizeof(line) - strlen(line) - 1);
                strncat(line, ", ", sizeof(line) - strlen(line) - 1);
                any = 1;
            }
        }
        if (!any) {
            strncat(line, "NOTHING", sizeof(line) - strlen(line) - 1);
        } else {
            size_t n = strlen(line);
            if (n >= 2) line[n - 2] = '\0';
        }
        set_response(g, line);
        return;
    }
    printf("YOU HAVE ");
    for (int i = 1; i <= CARRIED_OBJECTS; i++) {
        if (i == CARRIED_OBJECTS && loc[NOUN_APPLES] == INVENTORY) {
            continue;
        }
        if (loc[i] == INVENTORY) {
            const char *name = nouns[i].name;
            if (i == 1 && flagv[FLAGV_COIN_COUNT] == 1) name = "COIN";
            printf("%s,", name);
            any = 1;
        }
    }
    if (!any) printf("NOTHING");
    putchar('\n');
    wait_return();
}

static void do_get(Game *g) {
    int h = hcode(g);
    if (h == 6577) {
        set_response(g, "HOW?");
        return;
    }
    if (h == 4177 || h == 5177) {
        g->noun = NOUN_JUG;
        do_fill(g);
        return;
    }
    if (g->noun == NOUN_SACKS) {
        set_response(g, "TOO HEAVY!");
        return;
    }
    if (g->noun == NOUN_HORSESHOE && flagv[FLAGV_HORSESHOE_LOOSENED] == 0) {
        set_response(g, "IT IS FIRMLY NAILED ON!");
        return;
    }
    int count = 0;
    for (int i = 1; i < CARRIED_OBJECTS; i++) {
        if (loc[i] == INVENTORY) count++;
    }
    if (count > 13) {
        set_response(g, "YOU CANNOT CARRY ANY MORE");
        return;
    }
    if (g->noun > CARRIED_OBJECTS) {
        snprintf(g->response, sizeof(g->response), "YOU CANNOT GET THE %s", g->noun_text);
        return;
    }
    if (g->noun == NOUN_NONE) return;
    if (loc[g->noun] != g->room) set_response(g, "IT IS NOT HERE!");
    if (flagv[g->noun] == 1) {
        snprintf(g->response, sizeof(g->response), "WHAT %s?", g->noun_text);
    }
    if (loc[g->noun] == INVENTORY) set_response(g, "YOU ALREADY HAVE IT");
    if (loc[g->noun] == g->room && flagv[g->noun] == 0) {
        loc[g->noun] = INVENTORY;
        snprintf(g->response, sizeof(g->response), "YOU HAVE THE %s", g->noun_text);
    }
    if (g->noun == NOUN_APPLE) loc[NOUN_APPLES] = GONE;
    if (g->noun == NOUN_APPLES) loc[NOUN_APPLE] = INVENTORY;
    if (loc[NOUN_HORSESHOE] == INVENTORY && loc[NOUN_SHIELD] == INVENTORY && loc[NOUN_RING] == INVENTORY) flagv[FLAGV_CHARM_TRIO_ASSEMBLED] = 1;
    if (g->noun == NOUN_BOAT && flagv[FLAGV_BOAT_POWERED] == 1) loc[NOUN_SHEET] = INVENTORY;
    if (g->noun == NOUN_SHEET) flagv[FLAGV_BOAT_POWERED] = 0;
}

static void do_examine(Game *g) {
    int h = hcode(g);
    set_response(g, "YOU SEE WHAT YOU MIGHT EXPECT!");
    if (g->noun > NOUN_NONE) set_response(g, "NOTHING SPECIAL");
    if (g->noun == NOUN_HUT || g->noun == NOUN_MOUNTAIN_HUT) do_enter(g);
    if (h == 8076) set_response(g, "IT IS EMPTY");
    if (h == 8080) { set_response(g, "AHA!"); flagv[FLAGV_COINS_HIDDEN] = 0; }
    if (h == 7029) { set_response(g, "OK"); flagv[FLAGV_SHEET_HIDDEN] = 0; }
    if (g->noun == NOUN_UNIFORM) { set_response(g, "MATCHES IN POCKET"); loc[NOUN_MATCHES] = INVENTORY; }
    if (h == 1648) snprintf(g->response, sizeof(g->response), "THERE ARE SOME LETTERS '%s'", maze_path[1]);
    if (h == 7432) { set_response(g, "THEY ARE APPLE TREES"); flagv[FLAGV_APPLES_HIDDEN] = 0; }
    if (h == 2134 || h == 2187) { set_response(g, "OK"); flagv[FLAGV_JUG_HIDDEN] = 0; }
    if (g->noun == NOUN_KETCH) { set_response(g, "IT IS FISHY!"); flagv[FLAGV_NET_HIDDEN] = 0; }
    if (h == 3438) { set_response(g, "OK"); flagv[FLAGV_SEEDS_HIDDEN] = 0; }
    if (h == 242) set_response(g, "A FADED INSCRIPTION");
    if ((h == 1443 || h == 1485) && flagv[FLAGV_POOL_EMPTIED] == 0) set_response(g, "A GLIMMERING FROM THE DEPTHS");
    if ((h == 1443 || h == 1485) && flagv[FLAGV_POOL_EMPTIED] == 1) { set_response(g, "SOMETHING HERE..."); flagv[FLAGV_SHIELD_HIDDEN] = 0; }
    if (h == 2479 || h == 2444) set_response(g, "THERE IS A HANDLE");
    if (g->noun == NOUN_PHIAL) set_response(g, "THE LABEL READS 'POISON'");
    if (h == 4055) {
        int r = flagv[flagv[FLAGV_THIRD_WORD_SELECTOR] + FLAGV_THIRD_WORD_ROOM_PROTECT];
        snprintf(g->response, sizeof(g->response), "MAGIC WORDS LIE AT THE CROSSROADS, THE FOUNTAIN AND THE %s", rooms[r].name);
    }
    if (h == 2969 && flagv[FLAGV_GRARG_SLEEPING] == 1) set_response(g, "VERY UGLY!");
    if (h == 7158 || h == 7186) set_response(g, "THERE ARE LOOSE BRICKS");
    if (g->room == ROOM_MOSAIC_HALL) set_response(g, "VERY INTERESTING!");
    if (g->noun == NOUN_FOUNTAIN || g->noun == NOUN_PINNACLE || g->noun == NOUN_STATUE) set_response(g, "INTERESTING!");
    if (h == 6978) set_response(g, "THERE IS A WOODEN DOOR");
    if (h == 6970) { set_response(g, "YOU FOUND SOMETHING"); flagv[FLAGV_HORSESHOE_HIDDEN] = 0; }
    if (h == 2066) set_response(g, "A LARGE CUPBOARD IN THE CORNER");
    if (h == 6865 || h == 6853) set_response(g, "THERE ARE NINE STONES");
    if (h == 248) set_response(g, "A FADED WORD - 'M R H S'");
}

static void do_give(Game *g) {
    int h = hcode(g);
    if (g->room == ROOM_WOODED_VALLEY_SIDE) set_response(g, "HE GIVES IT BACK!");
    if (h == 6425) {
        snprintf(g->response, sizeof(g->response), "HE TAKES IT AND SAYS '%d RINGS ARE NEEDED'", flagv[FLAGV_BELL_RING_COUNT]);
        loc[NOUN_BROOCH] = GONE;
    }
    if (g->room == ROOM_BRIDGE_WEST_END || g->room == ROOM_BRIDGE_EAST_END) set_response(g, "HE DOES NOT WANT IT");
    if (g->noun == NOUN_COIN && flagv[FLAGV_COIN_COUNT] == 0) set_response(g, "YOU HAVE RUN OUT!");
    if ((h == 7562 || h == 7662) && flagv[FLAGV_COIN_COUNT] > 0 && loc[NOUN_COINS] == INVENTORY) {
        set_response(g, "HE TAKES IT");
        flagv[FLAGV_TROLL_TOLL_PAID] = 1;
    }
    if (flagv[FLAGV_TROLL_TOLL_PAID] == 1) {
        flagv[FLAGV_COIN_COUNT]--;
        if (flagv[FLAGV_COIN_COUNT] == 0) loc[NOUN_COINS] = GONE;
    }
    if (g->noun == NOUN_COINS) {
        set_response(g, "HE TAKES THEM ALL!");
        loc[NOUN_COINS] = GONE;
        flagv[FLAGV_TROLL_TOLL_PAID] = 1;
        flagv[FLAGV_COIN_COUNT] = 0;
    }
    if (h == 2228 && loc[NOUN_APPLES] == GONE) { append_response(g, "HE LEADS YOU ", "NORTH"); loc[NOUN_APPLE] = GONE; g->room = ROOM_RUSTY_GATES; }
    if ((h == 2228 && loc[NOUN_APPLES] == INVENTORY) || h == 225) { append_response(g, "HE LEADS YOU ", "NORTH"); g->room = ROOM_RUSTY_GATES; }
    if ((h == 1228 && loc[NOUN_APPLES] == INVENTORY) || h == 125) { append_response(g, "HE LEADS YOU ", "SOUTH"); g->room = ROOM_OVERGROWN_TRACK; }
    if (g->room == ROOM_HIGH_GLASS_GATES || g->room == ROOM_BARREN_COUNTRYSIDE) { set_response(g, "HE EATS IT!"); loc[g->noun] = GONE; }
    if (h == 711) { flagv[FLAGV_HOUND_DISTRACTED] = 1; set_response(g, "HE IS DISTRACTED"); }
    if (h == 385 || h == 3824) { set_response(g, "THEY SCURRY AWAY"); loc[g->noun] = GONE; flagv[FLAGV_RATS_GONE] = 1; }
}

static void do_say(Game *g) {
    set_response(g, "YOU SAID IT");
    if (g->noun == NOUN_MAGIC_WORDS) {
        set_response(g, "YOU MUST SAY THEM ONE BY ONE!");
        return;
    }
    if (g->room != ROOM_SILVER_CHAMBER || g->noun < NOUN_AWAKE || g->noun > NOUN_HELP || loc[NOUN_STONE_OF_DESTINY] != INVENTORY) return;
    if (g->noun == NOUN_AWAKE && flagv[FLAGV_SAID_AWAKE] == 0) { set_response(g, "THE MOUNTAIN RUMBLES!"); flagv[FLAGV_SAID_AWAKE] = 1; return; }
    if (g->noun == NOUN_GUIDE && flagv[FLAGV_SAID_AWAKE] == 1 && flagv[FLAGV_SAID_GUIDE] == 0) { set_response(g, "TOWERS FALL DOWN!"); flagv[FLAGV_SAID_GUIDE] = 1; return; }
    if (g->noun == flagv[FLAGV_THIRD_WORD_SELECTOR] + NOUN_PROTECT && flagv[FLAGV_SAID_AWAKE] == 1 && flagv[FLAGV_SAID_GUIDE] == 1) { flagv[FLAGV_QUEST_COMPLETE] = 1; return; }
    set_response(g, "THE WRONG SACRED WORD!");
    flagv[FLAGV_PLAYER_DEAD] = 1;
}

static void do_wear(Game *g) {
    if (g->noun == NOUN_APPLES || g->noun == NOUN_REEDS) do_get(g);
    if (g->noun == NOUN_BOOTS) {
        flagv[FLAGV_BOOTS_WORN] = 1;
        flagv[FLAGV_GRARG_PATROL_WARNING] = 0;
        set_response(g, "YOU ARE INVISIBLE");
    }
    if (g->noun == NOUN_UNIFORM) {
        flagv[FLAGV_UNIFORM_WORN] = 1;
        flagv[FLAGV_GRARG_PATROL_WARNING] = 0;
        set_response(g, "YOU ARE DISGUISED");
    }
}

static void do_remove(Game *g) {
    if (g->noun == NOUN_BOOTS && flagv[FLAGV_BOOTS_WORN] == 1) { set_response(g, "TAKEN OFF"); flagv[FLAGV_BOOTS_WORN] = 0; }
    if (g->noun == NOUN_UNIFORM && flagv[FLAGV_UNIFORM_WORN] == 1) { set_response(g, "OK"); flagv[FLAGV_UNIFORM_WORN] = 0; }
    if (g->noun == NOUN_BRICKS || g->noun == NOUN_RUBBLE) do_move(g);
}

static void do_tie(Game *g) {
    int h = hcode(g);
    if (g->noun == NOUN_SHEET || g->noun == NOUN_ROPE) set_response(g, "NOTHING TO TIE IT TO!");
    if (h == 7214) { set_response(g, "IT IS TIED"); loc[NOUN_ROPE] = ROOM_TOP_OF_WELL; flagv[FLAGV_ROPE_TIED_AT_WELL] = 1; }
    if (h == 722) { set_response(g, "OK"); flagv[FLAGV_SHEET_TIED_AT_WELL] = 1; loc[NOUN_SHEET] = ROOM_TOP_OF_WELL; }
}

static void do_climb(Game *g) {
    int h = hcode(g);
    if (h == 1547 && flagv[FLAGV_VINE_GROWN] == 1) { set_response(g, "ALL RIGHT"); g->room = ROOM_INSCRIBED_CAVERN; }
    if (g->noun == NOUN_ROPE || g->noun == NOUN_SHEET) set_response(g, "NOT ATTACHED TO ANYTHING!");
    if (h == 5414 && loc[NOUN_ROPE] == ROOM_HIGH_PINNACLE) set_response(g, "YOU ARE AT THE TOP");
    if (h == 7214 && flagv[FLAGV_ROPE_TIED_AT_WELL] == 1) { set_response(g, "GOING DOWN"); g->room = ROOM_WELL_BOTTOM; }
    if (h == 722 && flagv[FLAGV_SHEET_TIED_AT_WELL] == 1) { g->room = ROOM_WELL_BOTTOM; set_response(g, "IT IS TORN"); loc[NOUN_SHEET] = GONE; flagv[FLAGV_SHEET_TIED_AT_WELL] = 0; }
    if (h == 7114 && flagv[FLAGV_ROPE_TIED_AT_WELL] == 1) { loc[NOUN_ROPE] = ROOM_WELL_BOTTOM; flagv[FLAGV_ROPE_TIED_AT_WELL] = 0; set_response(g, "IT FALLS DOWN-BUMP!"); }
}

static void do_use(Game *g) {
    int h = hcode(g);
    if (h == 522) { set_response(g, "OK"); flagv[FLAGV_BOAT_POWERED] = 1; }
    if (g->noun == NOUN_COINS || g->noun == NOUN_COIN || g->noun == NOUN_APPLES || g->noun == NOUN_APPLE || g->noun == NOUN_BONE || g->noun == NOUN_BREAD) do_give(g);
    if (h == 416) { set_response(g, "YOU HAVE KEPT AFLOAT"); flagv[FLAGV_BOAT_KEPT_AFLOAT] = 1; return; }
    if (h == 4116) { set_response(g, "IT IS NOT BIG ENOUGH!"); return; }
    if (g->noun == NOUN_SWORD || g->noun == NOUN_AXE) do_break(g);
    if (g->noun == NOUN_PLANKS) do_drop(g);
    if (g->noun == NOUN_SILVER_PLATE) do_reflect(g);
    if (g->noun == NOUN_REEDS) do_blow(g);
    if (g->noun == NOUN_JUG || g->noun == NOUN_BUCKET) do_fill(g);
}

static void do_open(Game *g) {
    int h = hcode(g);
    if (g->noun == NOUN_CHEST || g->noun == NOUN_SACKS) do_examine(g);
    if (h == 2030) { flagv[FLAGV_PHIAL_HIDDEN] = 0; set_response(g, "OK"); }
    if (h == 6030) { set_response(g, "OK"); flagv[FLAGV_BOOTS_HIDDEN] = 0; }
    if (h == 2444 || h == 1870) set_response(g, "YOU ARE NOT STRONG ENOUGH");
    if (h == 3756) { set_response(g, "A PASSAGE!"); strcpy(exits[ROOM_ROW_OF_CASKS], "EW"); }
    if (h == 5960) {
        char input[64];
        int ok;
        set_response(g, "WHAT IS THE CODE");
        if (!curses_active) puts(g->response);
        ok = curses_active ? ui_read_line("What is the code?", input, sizeof(input)) : (read_input(input, sizeof(input)) != NULL);
        if (ok && atoi(input) == flagv[FLAGV_SAFE_CODE]) {
            set_response(g, "IT OPENS");
            flagv[FLAGV_KEY_HIDDEN] = 0;
        } else {
            set_response(g, "WRONG!");
        }
    }
    if (h == 6970) set_response(g, "IT FALLS OFF ITS HINGES");
    if (h == 4870) set_response(g, "IT IS LOCKED");
}

static void do_light(Game *g) {
    int h = hcode(g);
    if (g->noun > CARRIED_OBJECTS) set_response(g, "IT DOES NOT BURN");
    if (g->noun == NOUN_MATCHES) set_response(g, "YOU LIT THEM");
    if (h == 3826) set_response(g, "NOT BRIGHT ENOUGH");
    if ((g->noun == NOUN_LAMP || h == 6970) && loc[NOUN_MATCHES] != INVENTORY) set_response(g, "NO MATCHES");
    if (g->noun == NOUN_LAMP && loc[NOUN_MATCHES] == INVENTORY) { snprintf(g->response, sizeof(g->response), "A BRIGHT %s", g->verb_text); flagv[FLAGV_LAMP_LIT] = 1; }
    if (h == 6970 && loc[NOUN_MATCHES] == INVENTORY) { flagv[FLAGV_HORSESHOE_LOOSENED] = 1; set_response(g, "IT HAS TURNED TO ASHES"); }
}

static void do_fill(Game *g) {
    int h = hcode(g);
    if ((g->noun == NOUN_JUG || g->noun == NOUN_BUCKET) && (g->room == ROOM_ROUGH_WATER || g->room == ROOM_MIDDLE_OF_LAKE)) {
        set_response(g, "YOU CAPSIZED!");
        flagv[FLAGV_PLAYER_DEAD] = 1;
    }
    if (h == 6516 && loc[NOUN_JUG] == INVENTORY) { set_response(g, "IT IS NOW FULL"); flagv[FLAGV_JUG_FULL] = 1; }
    if (h == 656) set_response(g, "IT LEAKS OUT!");
}

static void do_plant(Game *g) {
    if (g->noun != NOUN_SEEDS || g->room != ROOM_HIGH_WALLED_GARDEN) {
        set_response(g, "DOES NOT GROW!");
        return;
    }
    set_response(g, "OK");
    flagv[FLAGV_SEEDS_PLANTED] = 1;
}

static void do_water(Game *g) {
    if (g->noun == NOUN_SEEDS && flagv[FLAGV_SEEDS_PLANTED] == 1 && flagv[FLAGV_JUG_FULL] == 1) {
        set_response(g, "A LARGE VINE GROWS IN SECONDS!");
        flagv[FLAGV_VINE_GROWN] = 1;
    }
}

static void do_break(Game *g) {
    int h = hcode(g);
    if (g->noun == NOUN_AXE || g->noun == NOUN_SWORD) set_response(g, "THWACK!");
    if (h == 5818) { set_response(g, "YOU CLEARED THE WEBS"); flagv[FLAGV_COBWEBS_CLEARED] = 1; }
    if (h == 187) { set_response(g, "THE DOOR BROKE!"); strcpy(exits[ROOM_DANK_CORRIDOR], "NS"); strcpy(exits[ROOM_DUNGEONS], "NS"); }
    if (h == 717) { set_response(g, "YOU BROKE THROUGH"); strcpy(exits[ROOM_WELL_BOTTOM], "N"); }
}

static void do_swing(Game *g) {
    if (g->noun == NOUN_JUG) { g->noun = NOUN_SEEDS; do_water(g); }
    if (hcode(g) == 499) set_response(g, "WHERE?");
}

static void do_enter(Game *g) {
    if (hcode(g) == 4337) { g->verb = 2; do_move_direction(g); return; }
    if (g->room == ROOM_MOUNTAIN_HUT) { set_response(g, "YOU FOUND SOMETHING"); flagv[FLAGV_PLANKS_HIDDEN] = 0; }
}

static void do_cross(Game *g) {
    if (g->room == ROOM_BRIDGE_EAST_END) { g->verb = 4; do_move_direction(g); return; }
    if (g->room == ROOM_BRIDGE_WEST_END) { g->verb = 2; do_move_direction(g); }
}

static void do_rig(Game *g) {
    int h = hcode(g);
    if (h == 3859 || h == 3339 || h == 1241 || h == 2241 || h == 751) set_response(g, "WITH WHAT?");
}

static void do_turn(Game *g) {
    int h = hcode(g);
    if (h == 2340) set_response(g, "IT GOES ROUND");
    if (h == 2445) { set_response(g, "THE GATES OPEN, THE POOL EMPTIES"); flagv[FLAGV_POOL_EMPTIED] = 1; }
}

static void do_dive(Game *g) {
    if (g->room == ROOM_MISTY_POOL || g->room == ROOM_MIDDLE_OF_LAKE) {
        set_response(g, "YOU HAVE DROWNED");
        flagv[FLAGV_PLAYER_DEAD] = 1;
    }
}

static void do_bail(Game *g) {
    set_response(g, "HOW?");
}

static void do_drop(Game *g) {
    int h = hcode(g);
    if (g->noun == NOUN_NONE || g->noun > CARRIED_OBJECTS) return;
    loc[g->noun] = g->room;
    set_response(g, "DONE");
    if (h == 418 || h == 518) { set_response(g, "YOU DROWNED!"); flagv[FLAGV_PLAYER_DEAD] = 1; }
    if (g->noun == NOUN_BOAT && flagv[FLAGV_BOAT_POWERED] == 1) loc[NOUN_SHEET] = g->room;
    if (g->noun == NOUN_JUG && flagv[FLAGV_JUG_FULL] == 1) { set_response(g, "YOU LOST THE WATER!"); flagv[FLAGV_JUG_FULL] = 0; }
    if (g->noun == NOUN_SHEET && flagv[FLAGV_BOAT_POWERED] == 1) flagv[FLAGV_BOAT_POWERED] = 0;
}

static void do_insert(Game *g) {
    if (g->noun == NOUN_COIN && flagv[FLAGV_COIN_COUNT] == 0) set_response(g, "YOU DO NOT HAVE ANY");
    if (hcode(g) == 5762 && loc[NOUN_COINS] == INVENTORY && flagv[FLAGV_COIN_COUNT] > 0) {
        flagv[FLAGV_COIN_COUNT]--;
        snprintf(g->response, sizeof(g->response), "A NUMBER APPEARS - %d", flagv[FLAGV_SAFE_CODE]);
        if (flagv[FLAGV_COIN_COUNT] == 0) loc[NOUN_COINS] = GONE;
    }
}

static void do_throw(Game *g) {
    int h = hcode(g);
    if (g->noun == NOUN_NONE || g->noun > CARRIED_OBJECTS) return;
    set_response(g, "DID NOT GO FAR!");
    loc[g->noun] = g->room;
    if (h == 3317) {
        set_response(g, "YOU CAUGHT THE BOAR");
        flagv[FLAGV_BOAR_CAUGHT] = 1;
    }
}

static void do_blow(Game *g) {
    int h = hcode(g);
    if (g->noun == NOUN_REEDS) set_response(g, "A NICE TUNE");
    if (h == 5233) set_response(g, "WHAT WITH?");
    if (g->noun == NOUN_MUSIC) set_response(g, "HOW, O MUSICAL ONE?");
    if (h == 5610) { flagv[FLAGV_GHOST_FREED] = 1; set_response(g, "THE GHOST OF THE GOBLIN GUARDIAN IS FREE!"); strcpy(exits[ROOM_HUGE_FALLEN_OAK], "NS"); }
}

static void do_eat(Game *g) {
    if (g->noun == NOUN_NONE || g->noun > CARRIED_OBJECTS) return;
    if (g->noun == NOUN_APPLES || g->noun == NOUN_BREAD) { set_response(g, "YUM YUM!"); loc[g->noun] = GONE; }
}

static void do_move(Game *g) {
    int h = hcode(g);
    if (g->room == ROOM_STALACTITES_AND_STALAGMITES && g->noun == NOUN_RUBBLE) { flagv[FLAGV_RUBBLE_MOVED] = 1; set_response(g, "YOU REVEALED A STEEP PASSAGE"); }
    if (g->room == ROOM_HALLOW_TOMB && g->noun == NOUN_RUBBLE) set_response(g, "YOU CANNOT MOVE RUBBLE FROM HERE");
    if (h == 7136) set_response(g, "THEY ARE WEDGED IN!");
}

static void do_into_or_poison(Game *g) {
    if ((g->noun == NOUN_GOBLET || g->noun == NOUN_WINE) && loc[NOUN_PHIAL] == INVENTORY && g->room == ROOM_MOSAIC_HALL) {
        set_response(g, "OK");
        flagv[FLAGV_POISON_PLACED] = 1;
    }
}

static void do_ring(Game *g) {
    char input[64];
    int mr;
    if (g->room != ROOM_SILVER_BELL || g->noun != NOUN_BELL) return;
    do {
        int ok;
        if (!curses_active) printf("\nHOW MANY TIMES?\n");
        ok = curses_active ? ui_read_line("How many times?", input, sizeof(input)) : (read_input(input, sizeof(input)) != NULL);
        if (!ok) return;
        mr = atoi(input);
        if (mr == 0 && !curses_active) puts("A NUMBER");
    } while (mr == 0);
    if (mr == flagv[FLAGV_BELL_RING_COUNT]) {
        set_response(g, "A ROCK DOOR OPENS");
        strcpy(exits[ROOM_SILVER_BELL], "EW");
        return;
    }
    set_response(g, "YOU HAVE MISTREATED THE BELL!");
    flagv[FLAGV_PLAYER_DEAD] = 1;
}

static void do_cut(Game *g) {
    if (hcode(g) == 5861) {
        g->noun = NOUN_SWORD;
        do_break(g);
    }
}

static void do_reflect(Game *g) {
    int h = hcode(g);
    if ((h == 4864 || h == 4819) && loc[NOUN_SILVER_PLATE] == INVENTORY) {
        set_response(g, "YOU REFLECTED THE WIZARDS GLARE! HE IS DEAD");
        flagv[FLAGV_WIZARD_DEFEATED] = 1;
    }
    if (g->noun == NOUN_STONE_OF_DESTINY) do_get(g);
}

static void do_with(Game *g) {
    if (hcode(g) == 7549 || hcode(g) == 7649) set_response(g, "WHAT WITH?");
    if (g->noun == NOUN_COINS || g->noun == NOUN_COIN) do_give(g);
}

static void do_unlock(Game *g) {
    if (hcode(g) == 4870 && loc[NOUN_KEY] == INVENTORY) {
        set_response(g, "THE KEY TURNS!");
        flagv[FLAGV_WIZARD_DOOR_UNLOCKED] = 1;
    }
}

static void do_break_verb(Game *g) {
    if (hcode(g) == 1870) set_response(g, "HOW?");
}

static void do_reflect_verb(Game *g) {
    if (g->room == ROOM_WIZARDS_LAIR) set_response(g, "HOW?");
}

static void dispatch(Game *g) {
    if (g->noun > CARRIED_OBJECTS || g->noun == NOUN_NONE) {
    } else if (!(g->verb == 8 || g->verb == 9 || g->verb == 14 || g->verb == 17 || g->verb == 44 || g->verb > 54)) {
        if (g->verb < VERB_COUNT && loc[g->noun] != INVENTORY) {
            snprintf(g->response, sizeof(g->response), "YOU DO NOT HAVE THE %s", g->noun_text);
            return;
        }
    }
    if (g->room == ROOM_HUGE_FALLEN_OAK && flagv[FLAGV_GHOST_FREED] == 0 && g->verb != 37 && g->verb != 53) {
        set_response(g, "THE GHOST OF THE GOBLIN GUARDIAN HAS GOT YOU!");
        return;
    }
    if (!(g->verb == 44 || g->verb == 47 || g->verb == 19 || g->verb == 57 || g->verb == 49)) {
        if (g->room == ROOM_WIZARDS_LAIR && flagv[FLAGV_WIZARD_DEFEATED] == 0) {
            set_response(g, "THE WIZARD HAS YOU IN HIS GLARE");
            return;
        }
    }
    switch (g->verb) {
        case 1: case 2: case 3: case 4: case 5: case 6: do_move_direction(g); break;
        case 7: do_inventory(g); break;
        case 8: case 9: case 14: case 55: case 56: do_get(g); break;
        case 10: case 11: do_examine(g); break;
        case 12: do_give(g); break;
        case 13: do_say(g); break;
        case 15: do_wear(g); break;
        case 16: do_tie(g); break;
        case 17: do_climb(g); break;
        case 18: do_blow(g); break;
        case 19: do_use(g); break;
        case 20: do_open(g); break;
        case 21: case 45: do_light(g); break;
        case 22: do_fill(g); break;
        case 23: do_plant(g); break;
        case 24: do_water(g); break;
        case 25: do_break(g); break;
        case 26: do_swing(g); break;
        case 32: do_dive(g); break;
        case 27: do_enter(g); break;
        case 28: do_cross(g); break;
        case 29: do_remove(g); break;
        case 30: do_rig(g); break;
        case 31: do_turn(g); break;
        case 33: do_bail(g); break;
        case 34: do_drop(g); break;
        case 35: do_throw(g); break;
        case 36: do_insert(g); break;
        case 37: do_blow(g); break;
        case 38: do_drop(g); break;
        case 39: do_eat(g); break;
        case 40: do_move(g); break;
        case 41: do_into_or_poison(g); break;
        case 42: do_ring(g); break;
        case 43: do_cut(g); break;
        case 44: do_reflect(g); break;
        case 46: do_into_or_poison(g); break;
        case 47: do_reflect(g); break;
        case 48: do_unlock(g); break;
        case 49: do_use(g); break;
        case 50: do_drink(g); break;
        case 51: do_examine(g); break;
        case 52: do_with(g); break;
        case 53: do_blow(g); break;
        case 54: do_break_verb(g); break;
        case 57: do_reflect_verb(g); break;
        default: break;
    }
}

static void do_drink(Game *g) {
    set_response(g, "ARE YOU THIRSTY?");
}

static int SILVERM_UNUSED choose_start(Game *g) {
    char input[64], filename[256];
    for (;;) {
        title();
        puts("DO YOU WANT TO\n");
        puts("   1. START A NEW GAME");
        puts("OR 2. CONTINUE A SAVED GAME\n");
        puts("TYPE IN EITHER 1 OR 2");
        if (!read_input(input, sizeof(input))) return 0;
        if (atoi(input) == 1) {
            new_game(g);
            return 1;
        }
        if (atoi(input) == 2) {
            if (!ask_filename(filename, sizeof(filename))) continue;
            printf("OK. SEARCHING FOR %s\n", filename);
            if (load_game(g, filename)) {
                puts("OK. LOADING");
                return 1;
            }
            puts("COULD NOT LOAD THAT FILE");
            wait_return();
        }
    }
}

static void after_turn(Game *g) {
    if (flagv[FLAGV_QUEST_COMPLETE] == 1) {
        if (curses_active) {
            set_response(g, "HOOOOORRRRRRAAAAAYYYYYY! YOU HAVE SUCCEEDED IN YOUR QUEST AND BROUGHT PEACE TO THE LAND");
            g->running = 0;
            return;
        }
        title();
        puts("HOOOOORRRRRRAAAAAYYYYYY!\n");
        puts("YOU HAVE SUCCEEDED IN YOUR");
        puts("QUEST AND BROUGHT PEACE TO");
        puts("THE LAND");
        g->running = 0;
        return;
    }
    if (g->room == ROOM_ROUGH_WATER) {
        flagv[FLAGV_ROUGH_WATER_TURN_COUNTER]++;
        if (flagv[FLAGV_ROUGH_WATER_TURN_COUNTER] == 10) {
            flagv[FLAGV_PLAYER_DEAD] = 1;
            set_response(g, "YOU SANK!");
        }
    }
    if (g->room == ROOM_HUGE_FALLEN_OAK && flagv[FLAGV_GHOST_FREED] == 0 && loc[NOUN_REEDS] != INVENTORY) {
        set_response(g, "THE GHOST OF THE GOBLIN GUARDIAN GETS YOU!");
        flagv[FLAGV_PLAYER_DEAD] = 1;
    }
    if (flagv[FLAGV_PLAYER_DEAD]) {
        if (curses_active) {
            char line[768];
            snprintf(line, sizeof(line), "%s YOU HAVE FAILED IN YOUR QUEST! BUT YOU ARE GRANTED ANOTHER TRY", g->response);
            new_game(g);
            set_response(g, line);
            return;
        }
        title();
        puts(g->response);
        puts("YOU HAVE FAILED IN YOUR QUEST!");
        puts("\nBUT YOU ARE GRANTED ANOTHER TRY");
        wait_return();
        new_game(g);
    }
}

static int stdio_main(void) {
    Game game;
    char input[256], filename[256];
    srand((unsigned int)time(NULL));
    memset(&game, 0, sizeof(game));
    if (!choose_start(&game)) {
        return 0;
    }
    while (game.running) {
        describe_room(&game);
        set_response(&game, "PARDON?");
        puts("WHAT WILL YOU DO NOW ");
        if (!read_input(input, sizeof(input))) break;
        char savecheck[256];
        snprintf(savecheck, sizeof(savecheck), "%s", input);
        uppercase_trim(savecheck);
        if (strcmp(savecheck, "SAVE GAME") == 0) {
            flagv[FLAGV_SAVED_ROOM] = game.room;
            if (ask_filename(filename, sizeof(filename))) {
                puts(save_game(&game, filename) ? "OK. SAVING" : "COULD NOT SAVE");
            }
            puts("BYE...");
            break;
        }
        parse_command(&game, input);
        if (game.noun_text[0] == '\0') set_response(&game, "I NEED TWO WORDS");
        if (game.verb > VERB_COUNT) set_response(&game, "TRY SOMETHING ELSE");
        if (game.verb > VERB_COUNT && game.noun == 0) {
            snprintf(game.response, sizeof(game.response), "YOU CANNOT %s", savecheck);
        }
        dispatch(&game);
        after_turn(&game);
    }
    return 0;
}

#define MAX_EVENTS 200
#define MAX_INPUT 255

static char ui_events[MAX_EVENTS][512];
static int ui_event_count;
static int inventory_scroll;

static void ui_add_event(const char *s) {
    if (!s || !*s) return;
    if (ui_event_count == MAX_EVENTS) {
        memmove(ui_events, ui_events + 1, sizeof(ui_events[0]) * (MAX_EVENTS - 1));
        ui_event_count--;
    }
    snprintf(ui_events[ui_event_count++], sizeof(ui_events[0]), "%s", s);
}

static void ui_add_command_event(const char *s) {
    char line[512];
    snprintf(line, sizeof(line), "> %s", s);
    ui_add_event(line);
}

static void ui_collect_visible(const Game *g, char *buf, size_t size) {
    buf[0] = '\0';
    for (int i = 1; i < CARRIED_OBJECTS; i++) {
        if (flagv[i] == 0 && loc[i] == g->room) {
            if (buf[0]) strncat(buf, ", ", size - strlen(buf) - 1);
            strncat(buf, nouns[i].name, size - strlen(buf) - 1);
        }
    }
    if (g->room == ROOM_BANQUETING_HALL && flagv[FLAGV_GRARG_SLEEPING] == 0) strncat(buf, buf[0] ? ", GRARGS FEASTING" : "GRARGS FEASTING", size - strlen(buf) - 1);
    if (g->room == ROOM_BANQUETING_HALL && flagv[FLAGV_GRARG_SLEEPING] == 1) strncat(buf, buf[0] ? ", A SLEEPING GRARG" : "A SLEEPING GRARG", size - strlen(buf) - 1);
    if (g->room == ROOM_RUSTY_GATES || g->room == ROOM_OVERGROWN_TRACK) strncat(buf, buf[0] ? ", A PONY" : "A PONY", size - strlen(buf) - 1);
    if (g->room == ROOM_WOODED_VALLEY_SIDE) strncat(buf, buf[0] ? ", A HERMIT" : "A HERMIT", size - strlen(buf) - 1);
    if (g->room == ROOM_DANK_CORRIDOR && strcmp(exits[ROOM_DANK_CORRIDOR], "N") == 0) strncat(buf, buf[0] ? ", AN OAK DOOR" : "AN OAK DOOR", size - strlen(buf) - 1);
    if (g->room == ROOM_OGBANS_SAFE && flagv[FLAGV_OGBAN_DEAD] == 1) strncat(buf, buf[0] ? ", OGBAN (DEAD)" : "OGBAN (DEAD)", size - strlen(buf) - 1);
    if (!buf[0]) snprintf(buf, size, "nothing obvious");
}

static void ui_collect_exits(const Game *g, char *buf, size_t size) {
    buf[0] = '\0';
    for (size_t i = 0; exits[g->room][i]; i++) {
        char part[8];
        snprintf(part, sizeof(part), "%s%c", buf[0] ? ", " : "", exits[g->room][i]);
        strncat(buf, part, size - strlen(buf) - 1);
    }
    if (!buf[0]) snprintf(buf, size, "none");
}

static int ui_collect_inventory(const char **items, int max_items) {
    int count = 0;
    for (int i = 1; i <= CARRIED_OBJECTS && count < max_items; i++) {
        if (i == CARRIED_OBJECTS && loc[NOUN_APPLES] == INVENTORY) continue;
        if (loc[i] == INVENTORY) {
            items[count++] = (i == NOUN_COINS && flagv[FLAGV_COIN_COUNT] == 1) ? "COIN" : nouns[i].name;
        }
    }
    return count;
}

static int ui_wrap_lines(const char *text, char lines[][256], int max_lines, int width) {
    int count = 0;
    const char *p = text;
    if (width < 4) width = 4;
    while (*p && count < max_lines) {
        while (*p == ' ') p++;
        int len = (int)strlen(p);
        if (len <= width) {
            snprintf(lines[count++], 256, "%s", p);
            break;
        }
        int cut = width;
        while (cut > 0 && p[cut] != ' ') cut--;
        if (cut <= 0) cut = width;
        snprintf(lines[count], 256, "%.*s", cut, p);
        count++;
        p += cut;
    }
    if (count == 0 && max_lines > 0) {
        lines[count++][0] = '\0';
    }
    return count;
}

static int ui_add_wrapped(WINDOW *win, int y, int x, int width, int max_y, const char *text) {
    char lines[32][256];
    int n = ui_wrap_lines(text, lines, 32, width);
    for (int i = 0; i < n && y < max_y; i++, y++) {
        mvwaddnstr(win, y, x, lines[i], width);
    }
    return y;
}

static void ui_frame(int loc_bottom, int input_top, int split_col, int use_sidebar) {
    int rows = LINES, cols = COLS;
    erase();
    box(stdscr, 0, 0);
    mvhline(2, 1, ACS_HLINE, cols - 2);
    mvaddch(2, 0, ACS_LTEE);
    mvaddch(2, cols - 1, ACS_RTEE);
    mvhline(loc_bottom, 1, ACS_HLINE, cols - 2);
    mvaddch(loc_bottom, 0, ACS_LTEE);
    mvaddch(loc_bottom, cols - 1, ACS_RTEE);
    mvhline(input_top, 1, ACS_HLINE, cols - 2);
    mvaddch(input_top, 0, ACS_LTEE);
    mvaddch(input_top, cols - 1, ACS_RTEE);
    if (use_sidebar) {
        mvvline(loc_bottom + 1, split_col, ACS_VLINE, input_top - loc_bottom - 1);
        mvaddch(loc_bottom, split_col, ACS_TTEE);
        mvaddch(input_top, split_col, ACS_BTEE);
    }
    (void)rows;
}

static void ui_draw(const Game *g, const char *input) {
    int rows = LINES, cols = COLS;
    int compact = rows < 20 || cols < 70;
    int loc_bottom = compact ? 9 : rows / 3;
    int input_top = rows - 3;
    int split_col = compact ? cols - 1 : cols - 25;
    int use_sidebar = !compact && split_col > 45;
    if (loc_bottom < 7) loc_bottom = 7;
    if (input_top <= loc_bottom + 3) input_top = loc_bottom + 4;
    if (input_top >= rows - 1) input_top = rows - 2;

    ui_frame(loc_bottom, input_top, split_col, use_sidebar);
    attron(A_BOLD);
    mvaddnstr(1, (cols - 31) / 2, "THE MYSTERY OF SILVER MOUNTAIN", cols - 2);
    attroff(A_BOLD);

    char line[1024], exits_line[128], visible[1024];
    snprintf(line, sizeof(line), "LOCATION: %s", rooms[g->room].name);
    attron(A_BOLD);
    mvaddnstr(3, 2, line, cols - 4);
    attroff(A_BOLD);
    snprintf(line, sizeof(line), "You are %s %s %s.", prep_x[rooms[g->room].x], article_y[rooms[g->room].y], rooms[g->room].name);
    int y = ui_add_wrapped(stdscr, 5, 2, cols - 4, loc_bottom, line);
    ui_collect_exits(g, exits_line, sizeof(exits_line));
    ui_collect_visible(g, visible, sizeof(visible));
    snprintf(line, sizeof(line), "Exits: %s", exits_line);
    y = ui_add_wrapped(stdscr, y + 1, 2, cols - 4, loc_bottom, line);
    snprintf(line, sizeof(line), "Visible: %.1000s", visible);
    ui_add_wrapped(stdscr, y, 2, cols - 4, loc_bottom, line);

    int event_width = (use_sidebar ? split_col : cols - 1) - 3;
    int event_top = loc_bottom + 1;
    int event_bottom = input_top;
    attron(A_BOLD);
    mvaddnstr(event_top, 2, "RECENT EVENTS", event_width);
    attroff(A_BOLD);
    char wrapped[MAX_EVENTS * 4][256];
    int wrapped_count = 0;
    for (int i = 0; i < ui_event_count && wrapped_count < MAX_EVENTS * 4; i++) {
        wrapped_count += ui_wrap_lines(ui_events[i], wrapped + wrapped_count, MAX_EVENTS * 4 - wrapped_count, event_width);
    }
    int event_lines = event_bottom - event_top - 2;
    int start = wrapped_count > event_lines ? wrapped_count - event_lines : 0;
    y = event_top + 2;
    for (int i = start; i < wrapped_count && y < event_bottom; i++, y++) {
        mvaddnstr(y, 2, wrapped[i], event_width);
    }

    if (use_sidebar) {
        const char *items[CARRIED_OBJECTS];
        int count = ui_collect_inventory(items, CARRIED_OBJECTS);
        int inv_top = loc_bottom + 1;
        int inv_x = split_col + 2;
        int inv_w = cols - split_col - 4;
        int inv_lines = input_top - loc_bottom - 4;
        if (inv_lines < 1) inv_lines = 1;
        if (inventory_scroll > count - inv_lines) inventory_scroll = count - inv_lines;
        if (inventory_scroll < 0) inventory_scroll = 0;
        char titlebuf[64];
        if (count > inv_lines) {
            snprintf(titlebuf, sizeof(titlebuf), "INVENTORY %d-%d/%d", inventory_scroll + 1, inventory_scroll + inv_lines < count ? inventory_scroll + inv_lines : count, count);
        } else {
            snprintf(titlebuf, sizeof(titlebuf), "INVENTORY");
        }
        attron(A_BOLD);
        mvaddnstr(inv_top, inv_x, titlebuf, inv_w);
        attroff(A_BOLD);
        for (int i = 0; i < inv_lines && inventory_scroll + i < count; i++) {
            mvaddnstr(inv_top + 2 + i, inv_x, items[inventory_scroll + i], inv_w);
        }
        if (count == 0) mvaddnstr(inv_top + 2, inv_x, "nothing", inv_w);
        if (inventory_scroll > 0) mvaddnstr(input_top - 2, inv_x, "PgUp for more", inv_w);
        if (inventory_scroll + inv_lines < count) mvaddnstr(input_top - 1, inv_x, "PgDn for more", inv_w);
    }

    mvaddnstr(input_top + 1, 2, "WHAT WILL YOU DO NOW? > ", cols - 4);
    mvaddnstr(input_top + 1, 25, input, cols - 27);
    move(input_top + 1, 25 + (int)strlen(input));
    refresh();
}

static int ui_modal_input(const char *title_text, const char *prompt, const char *initial, char *out, size_t out_size) {
    char input[256];
    int pos;
    snprintf(input, sizeof(input), "%s", initial ? initial : "");
    pos = (int)strlen(input);
    for (;;) {
        int w = COLS < 54 ? COLS - 4 : 50;
        int h = 7;
        int y = (LINES - h) / 2;
        int x = (COLS - w) / 2;
        if (w < 30) w = COLS - 2;
        if (y < 1) y = 1;
        WINDOW *win = newwin(h, w, y, x);
        keypad(win, TRUE);
        box(win, 0, 0);
        mvwhline(win, 2, 1, ACS_HLINE, w - 2);
        mvwaddch(win, 2, 0, ACS_LTEE);
        mvwaddch(win, 2, w - 1, ACS_RTEE);
        wattron(win, A_BOLD);
        mvwaddnstr(win, 1, 2, title_text, w - 4);
        wattroff(win, A_BOLD);
        mvwprintw(win, 3, 2, "%s %.*s", prompt, w - (int)strlen(prompt) - 4, input);
        mvwaddnstr(win, 5, 2, "Enter = confirm    Esc = cancel", w - 4);
        wmove(win, 3, 2 + (int)strlen(prompt) + 1 + pos);
        wrefresh(win);
        int ch = wgetch(win);
        delwin(win);
        if (ch == 27) return 0;
        if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) {
            snprintf(out, out_size, "%s", input);
            return 1;
        }
        if ((ch == KEY_BACKSPACE || ch == 127 || ch == 8) && pos > 0) {
            input[--pos] = '\0';
        } else if (isprint(ch) && pos < (int)sizeof(input) - 1) {
            input[pos++] = (char)ch;
            input[pos] = '\0';
        }
    }
}

static int ui_confirm_exit(void) {
    for (;;) {
        int w = COLS < 48 ? COLS - 4 : 44;
        int h = 7;
        int y = (LINES - h) / 2;
        int x = (COLS - w) / 2;
        if (w < 28) w = COLS - 2;
        if (y < 1) y = 1;
        WINDOW *win = newwin(h, w, y, x);
        keypad(win, TRUE);
        box(win, 0, 0);
        mvwhline(win, 2, 1, ACS_HLINE, w - 2);
        mvwaddch(win, 2, 0, ACS_LTEE);
        mvwaddch(win, 2, w - 1, ACS_RTEE);
        wattron(win, A_BOLD);
        mvwaddnstr(win, 1, 2, "Quit Game", w - 4);
        wattroff(win, A_BOLD);
        mvwaddnstr(win, 3, 2, "Are you sure you want to exit?", w - 4);
        mvwaddnstr(win, 5, 2, "Y = quit    N/Esc = cancel", w - 4);
        wmove(win, 5, 2);
        wrefresh(win);
        int ch = wgetch(win);
        delwin(win);
        if (ch == 'y' || ch == 'Y') return 1;
        if (ch == 'n' || ch == 'N' || ch == 27 || ch == '\n' || ch == '\r' || ch == KEY_ENTER) return 0;
    }
}

static int ui_choose_start(Game *g) {
    char status[128] = "";
    for (;;) {
        int w = COLS < 58 ? COLS - 4 : 54;
        int h = 9;
        int y = (LINES - h) / 2;
        int x = (COLS - w) / 2;
        if (w < 36) w = COLS - 2;
        if (y < 1) y = 1;
        erase();
        box(stdscr, 0, 0);
        attron(A_BOLD);
        mvaddnstr(1, (COLS - 31) / 2, "THE MYSTERY OF SILVER MOUNTAIN", COLS - 2);
        attroff(A_BOLD);

        WINDOW *win = newwin(h, w, y, x);
        keypad(win, TRUE);
        box(win, 0, 0);
        mvwhline(win, 2, 1, ACS_HLINE, w - 2);
        mvwaddch(win, 2, 0, ACS_LTEE);
        mvwaddch(win, 2, w - 1, ACS_RTEE);
        wattron(win, A_BOLD);
        mvwaddnstr(win, 1, 2, "Start Game", w - 4);
        wattroff(win, A_BOLD);
        mvwaddnstr(win, 3, 2, "1. Start a new game", w - 4);
        mvwaddnstr(win, 4, 2, "2. Continue a saved game", w - 4);
        mvwaddnstr(win, 6, 2, "Press 1 or 2    Esc = exit", w - 4);
        if (status[0]) {
            mvwaddnstr(win, 7, 2, status, w - 4);
        }
        wrefresh(win);
        int ch = wgetch(win);
        delwin(win);
        if (ch == '1') {
            new_game(g);
            return 1;
        }
        if (ch == '2') {
            char filename[256];
            if (ui_modal_input("Load Game", "File name:", "silverm.sav", filename, sizeof(filename))) {
                if (load_game(g, filename)) {
                    return 1;
                }
                snprintf(status, sizeof(status), "Could not load that file.");
            } else {
                snprintf(status, sizeof(status), "Load cancelled.");
            }
        } else if (ch == 27) {
            return 0;
        } else if (ch == KEY_RESIZE) {
            continue;
        } else {
            snprintf(status, sizeof(status), "Type 1 or 2.");
        }
    }
}

static int ui_prompt_line_impl(const char *prompt, char *buf, size_t size) {
    char out[256];
    int ok = ui_modal_input(prompt, "Answer:", "", out, sizeof(out));
    if (!ok) return 0;
    snprintf(buf, size, "%s\n", out);
    return 1;
}

static void ui_execute_command(Game *g, const char *command) {
    char input[256], check[256];
    snprintf(input, sizeof(input), "%s", command);
    snprintf(check, sizeof(check), "%s", command);
    uppercase_trim(check);
    ui_add_command_event(command);
    set_response(g, "PARDON?");
    if (strcmp(check, "LOOK") == 0 || strcmp(check, "L") == 0) {
        set_response(g, "OK");
        ui_add_event(g->response);
        return;
    }
    parse_command(g, input);
    if (g->noun_text[0] == '\0') set_response(g, "I NEED TWO WORDS");
    if (g->verb > VERB_COUNT) set_response(g, "TRY SOMETHING ELSE");
    if (g->verb > VERB_COUNT && g->noun == NOUN_NONE) {
        snprintf(g->response, sizeof(g->response), "YOU CANNOT %s", check);
    }
    dispatch(g);
    after_turn(g);
    ui_add_event(g->response);
}

static int curses_main(void) {
    Game game;
    char input[MAX_INPUT + 1] = "";
    int input_len = 0;
    srand((unsigned int)time(NULL));
    memset(&game, 0, sizeof(game));

    setlocale(LC_ALL, "");
    initscr();
    curses_active = 1;
    ui_read_line = ui_prompt_line_impl;
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr, FALSE);
    if (has_colors()) {
        start_color();
    }
    if (!ui_choose_start(&game)) {
        endwin();
        curses_active = 0;
        ui_read_line = NULL;
        return 0;
    }
    ui_add_event(game.response);

    while (game.running) {
        ui_draw(&game, input);
        int ch = getch();
        if (ch == KEY_RESIZE) continue;
        if (ch == KEY_PPAGE) {
            inventory_scroll -= 3;
            if (inventory_scroll < 0) inventory_scroll = 0;
            continue;
        }
        if (ch == KEY_NPAGE) {
            inventory_scroll += 3;
            continue;
        }
        if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) {
            char check[256], filename[256];
            if (input_len == 0) continue;
            snprintf(check, sizeof(check), "%s", input);
            uppercase_trim(check);
            if (strcmp(check, "QUIT") == 0 || strcmp(check, "EXIT") == 0) {
                ui_add_command_event(input);
                if (ui_confirm_exit()) {
                    ui_add_event("BYE...");
                    break;
                }
                ui_add_event("EXIT CANCELLED");
            } else if (strcmp(check, "SAVE") == 0 || strcmp(check, "SAVE GAME") == 0) {
                ui_add_command_event(input);
                if (ui_modal_input("Save Game", "File name:", "silverm.sav", filename, sizeof(filename))) {
                    flagv[FLAGV_SAVED_ROOM] = game.room;
                    ui_add_event(save_game(&game, filename) ? "OK. SAVING" : "COULD NOT SAVE");
                } else {
                    ui_add_event("SAVE CANCELLED");
                }
            } else if (strcmp(check, "LOAD") == 0 || strcmp(check, "LOAD GAME") == 0) {
                ui_add_command_event(input);
                if (ui_modal_input("Load Game", "File name:", "silverm.sav", filename, sizeof(filename))) {
                    ui_add_event(load_game(&game, filename) ? "OK. LOADING" : "COULD NOT LOAD THAT FILE");
                } else {
                    ui_add_event("LOAD CANCELLED");
                }
            } else {
                ui_execute_command(&game, input);
            }
            input[0] = '\0';
            input_len = 0;
            continue;
        }
        if ((ch == KEY_BACKSPACE || ch == 127 || ch == 8) && input_len > 0) {
            input[--input_len] = '\0';
            continue;
        }
        if (isprint(ch) && input_len < MAX_INPUT) {
            input[input_len++] = (char)ch;
            input[input_len] = '\0';
        }
    }
    if (!game.running) {
        ui_draw(&game, input);
        getch();
    }
    endwin();
    curses_active = 0;
    ui_read_line = NULL;
    return 0;
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--stdio") == 0) {
        return stdio_main();
    }
    return curses_main();
}
