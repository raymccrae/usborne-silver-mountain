#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "silverm.h"

#define ROOM_COUNT 80
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
    0, 80, 70, 60, 69, 74, 72, 63, 52, 20, 11, 1, 14, 36, 54,
    61, 21, 32, 10, 50, 29, 59, 34, 13, 80, 30, 81, 47, 74
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

static char *read_input(char *buf, size_t size) {
    return fgets(buf, (int)size, game_input ? game_input : stdin);
}

static void set_response(Game *g, const char *s) {
    snprintf(g->response, sizeof(g->response), "%s", s);
}

static void append_response(Game *g, const char *a, const char *b) {
    snprintf(g->response, sizeof(g->response), "%s%s", a, b);
}

static void title(void) {
    putchar('\n');
    puts("MYSTERY OF SILVER");
    puts("    MOUNTAIN");
    puts("========================================");
    putchar('\n');
}

static void wait_return(void) {
    char buf[32];
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
    flagv[FLAGV_THIRD_WORD_ROOM_PROTECT] = 68;
    flagv[FLAGV_THIRD_WORD_ROOM_LEAD] = 54;
    flagv[FLAGV_THIRD_WORD_ROOM_HELP] = 15;
    flagv[FLAGV_THIRD_WORD_SELECTOR] = rand() % 3;
    random_maze();
    g->room = 77;
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
    return g->room >= 1 && g->room <= ROOM_COUNT;
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
    if (g->room == 29 && flagv[FLAGV_GRARG_SLEEPING] == 0) strncat(visible, " GRARGS FEASTING,", sizeof(visible) - strlen(visible) - 1);
    if (g->room == 29 && flagv[FLAGV_GRARG_SLEEPING] == 1) strncat(visible, " A SLEEPING GRARG,", sizeof(visible) - strlen(visible) - 1);
    if (g->room == 12 || g->room == 22) strncat(visible, " A PONY,", sizeof(visible) - strlen(visible) - 1);
    if (g->room == 64) strncat(visible, " A HERMIT,", sizeof(visible) - strlen(visible) - 1);
    if (g->room == 18 && strcmp(exits[18], "N") == 0) strncat(visible, " AN OAK DOOR,", sizeof(visible) - strlen(visible) - 1);
    if (g->room == 59 && flagv[FLAGV_OGBAN_DEAD] == 1) strncat(visible, " OGBAN (DEAD),", sizeof(visible) - strlen(visible) - 1);
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
    g->noun = 0;
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
    if (!(((g->room == 75 && d == 2) || (g->room == 76 && d == 4))) || flagv[FLAGV_TROLL_TOLL_PAID] == 1) {
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
        if (g->room == 29 && flagv[FLAGV_GRARG_SLEEPING] == 0) {
            set_response(g, "GRARGS WILL SEE YOU!");
            return;
        }
        if (g->room == 73 || g->room == 42 || g->room == 9 || g->room == 10) {
            set_response(g, "A GRARG PATROL APPROACHES");
            flagv[FLAGV_GRARG_PATROL_WARNING] = 1;
            return;
        }
    }
    if (loc[8] == INVENTORY && ((g->room == 52 && d == 2) || (g->room == 31 && d != 3))) {
        set_response(g, "THE BOAT IS TOO HEAVY");
        return;
    }
    if (loc[8] != INVENTORY && ((g->room == 52 && d == 4) || (g->room == 31 && d == 3))) {
        set_response(g, "YOU CANNOT SWIM");
        return;
    }
    if (g->room == 52 && loc[8] == INVENTORY && d == 4 && flagv[FLAGV_BOAT_POWERED] == 0) {
        set_response(g, "NO POWER!");
        return;
    }
    if (g->room == 41 && d == 3 && flagv[FLAGV_BOAT_KEPT_AFLOAT] == 0) {
        set_response(g, "THE BOAT IS SINKING!");
        return;
    }
    if (g->room == 33 && d == 1 && flagv[FLAGV_BOAR_CAUGHT] == 0) {
        set_response(g, "OGBAN'S BOAR BLOCKS YOUR PATH");
        return;
    }
    if (((g->room == 3 && d == 2) || (g->room == 4 && d == 4)) && flagv[FLAGV_RUBBLE_MOVED] == 0) {
        set_response(g, "A PILE OF RUBBLE BLOCKS YOUR PATH");
        return;
    }
    if (g->room == 35 && loc[13] != g->room) {
        set_response(g, "THE ICE IS BREAKING!");
        return;
    }
    if (g->room == 5 && (d == 2 || d == 4)) {
        do_tunnel_maze(g, d);
        if (flagv[FLAGV_PLAYER_DEAD]) return;
    }
    if (g->room == 4 && d == 4) {
        set_response(g, "PASSAGE IS TOO STEEP");
        return;
    }
    if (g->room == 7 && d == 2 && flagv[FLAGV_HOUND_DISTRACTED] == 0) {
        set_response(g, "A HUGE HOUND BARS YOUR WAY");
        return;
    }
    if ((g->room == 38 || g->room == 37) && flagv[FLAGV_LAMP_LIT] == 0) {
        set_response(g, "IT IS TOO DARK");
        return;
    }
    if (g->room == 49 && d == 2 && flagv[FLAGV_CHARM_TRIO_ASSEMBLED] == 0) {
        set_response(g, "MYSTERIOUS FORCES HOLD YOU BACK");
        return;
    }
    if (g->room == 49 && d == 3 && flagv[FLAGV_OGBAN_DEAD] == 0) {
        set_response(g, "YOU MET OGBAN!!!");
        flagv[FLAGV_PLAYER_DEAD] = 1;
        return;
    }
    if (g->room == 38 && flagv[FLAGV_RATS_GONE] == 0) {
        set_response(g, "RATS NIBBLE YOUR ANKLES");
        return;
    }
    if (g->room == 58 && (d == 1 || d == 4) && flagv[FLAGV_COBWEBS_CLEARED] == 0) {
        set_response(g, "YOU GET CAUGHT IN THE WEBS!");
        return;
    }
    if (g->room == 48 && d == 4 && flagv[FLAGV_WIZARD_DOOR_UNLOCKED] == 0) {
        set_response(g, "THE DOOR DOES NOT OPEN");
        return;
    }
    if (g->room == 40 && flagv[FLAGV_POISON_PLACED] == 1) flagv[FLAGV_OGBAN_DEAD] = 1;
    if (g->room == 37 && d == 4 && strcmp(exits[37], "EW") == 0) {
        g->room = 67;
        set_response(g, "THE PASSAGE WAS STEEP!");
        return;
    }
    if (g->room == 29 && d == 3) {
        flagv[FLAGV_GRARG_SLEEPING] = 1;
        flagv[FLAGV_UNIFORM_HIDDEN] = 0;
    }
    if (g->room == 8 && d == 2) flagv[FLAGV_HOUND_DISTRACTED] = 0;
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
    if ((old == 75 && d == 2) || (old == 76 && d == 4)) set_response(g, "OK. YOU CROSSED");
    if (flagv[FLAGV_BOOTS_WORN] == 1) flagv[FLAGV_BOOT_WEAR_COUNTER]++;
    if (flagv[FLAGV_BOOT_WEAR_COUNTER] > 5 && flagv[FLAGV_BOOTS_WORN] == 1) {
        set_response(g, "BOOTS HAVE WORN OUT");
        flagv[FLAGV_BOOTS_WORN] = 0;
        loc[3] = GONE;
    }
}

static void do_inventory(Game *g) {
    int any = 0;
    set_response(g, "OK");
    printf("YOU HAVE ");
    for (int i = 1; i <= CARRIED_OBJECTS; i++) {
        if (i == CARRIED_OBJECTS && loc[5] == INVENTORY) {
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
        g->noun = 16;
        do_fill(g);
        return;
    }
    if (g->noun == 38) {
        set_response(g, "TOO HEAVY!");
        return;
    }
    if (g->noun == 4 && flagv[FLAGV_HORSESHOE_LOOSENED] == 0) {
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
    if (g->noun == 0) return;
    if (loc[g->noun] != g->room) set_response(g, "IT IS NOT HERE!");
    if (flagv[g->noun] == 1) {
        snprintf(g->response, sizeof(g->response), "WHAT %s?", g->noun_text);
    }
    if (loc[g->noun] == INVENTORY) set_response(g, "YOU ALREADY HAVE IT");
    if (loc[g->noun] == g->room && flagv[g->noun] == 0) {
        loc[g->noun] = INVENTORY;
        snprintf(g->response, sizeof(g->response), "YOU HAVE THE %s", g->noun_text);
    }
    if (g->noun == 28) loc[5] = GONE;
    if (g->noun == 5) loc[28] = INVENTORY;
    if (loc[4] == INVENTORY && loc[12] == INVENTORY && loc[15] == INVENTORY) flagv[FLAGV_CHARM_TRIO_ASSEMBLED] = 1;
    if (g->noun == 8 && flagv[FLAGV_BOAT_POWERED] == 1) loc[2] = INVENTORY;
    if (g->noun == 2) flagv[FLAGV_BOAT_POWERED] = 0;
}

static void do_examine(Game *g) {
    int h = hcode(g);
    set_response(g, "YOU SEE WHAT YOU MIGHT EXPECT!");
    if (g->noun > 0) set_response(g, "NOTHING SPECIAL");
    if (g->noun == 46 || g->noun == 88) do_enter(g);
    if (h == 8076) set_response(g, "IT IS EMPTY");
    if (h == 8080) { set_response(g, "AHA!"); flagv[FLAGV_COINS_HIDDEN] = 0; }
    if (h == 7029) { set_response(g, "OK"); flagv[FLAGV_SHEET_HIDDEN] = 0; }
    if (g->noun == 20) { set_response(g, "MATCHES IN POCKET"); loc[26] = INVENTORY; }
    if (h == 1648) snprintf(g->response, sizeof(g->response), "THERE ARE SOME LETTERS '%s'", maze_path[1]);
    if (h == 7432) { set_response(g, "THEY ARE APPLE TREES"); flagv[FLAGV_APPLES_HIDDEN] = 0; }
    if (h == 2134 || h == 2187) { set_response(g, "OK"); flagv[FLAGV_JUG_HIDDEN] = 0; }
    if (g->noun == 35) { set_response(g, "IT IS FISHY!"); flagv[FLAGV_NET_HIDDEN] = 0; }
    if (h == 3438) { set_response(g, "OK"); flagv[FLAGV_SEEDS_HIDDEN] = 0; }
    if (h == 242) set_response(g, "A FADED INSCRIPTION");
    if ((h == 1443 || h == 1485) && flagv[FLAGV_POOL_EMPTIED] == 0) set_response(g, "A GLIMMERING FROM THE DEPTHS");
    if ((h == 1443 || h == 1485) && flagv[FLAGV_POOL_EMPTIED] == 1) { set_response(g, "SOMETHING HERE..."); flagv[FLAGV_SHIELD_HIDDEN] = 0; }
    if (h == 2479 || h == 2444) set_response(g, "THERE IS A HANDLE");
    if (g->noun == 9) set_response(g, "THE LABEL READS 'POISON'");
    if (h == 4055) {
        int r = flagv[flagv[FLAGV_THIRD_WORD_SELECTOR] + FLAGV_THIRD_WORD_ROOM_PROTECT];
        snprintf(g->response, sizeof(g->response), "MAGIC WORDS LIE AT THE CROSSROADS, THE FOUNTAIN AND THE %s", rooms[r].name);
    }
    if (h == 2969 && flagv[FLAGV_GRARG_SLEEPING] == 1) set_response(g, "VERY UGLY!");
    if (h == 7158 || h == 7186) set_response(g, "THERE ARE LOOSE BRICKS");
    if (g->room == 49) set_response(g, "VERY INTERESTING!");
    if (g->noun == 52 || g->noun == 82 || g->noun == 81) set_response(g, "INTERESTING!");
    if (h == 6978) set_response(g, "THERE IS A WOODEN DOOR");
    if (h == 6970) { set_response(g, "YOU FOUND SOMETHING"); flagv[FLAGV_HORSESHOE_HIDDEN] = 0; }
    if (h == 2066) set_response(g, "A LARGE CUPBOARD IN THE CORNER");
    if (h == 6865 || h == 6853) set_response(g, "THERE ARE NINE STONES");
    if (h == 248) set_response(g, "A FADED WORD - 'M R H S'");
}

static void do_give(Game *g) {
    int h = hcode(g);
    if (g->room == 64) set_response(g, "HE GIVES IT BACK!");
    if (h == 6425) {
        snprintf(g->response, sizeof(g->response), "HE TAKES IT AND SAYS '%d RINGS ARE NEEDED'", flagv[FLAGV_BELL_RING_COUNT]);
        loc[25] = GONE;
    }
    if (g->room == 75 || g->room == 76) set_response(g, "HE DOES NOT WANT IT");
    if (g->noun == 62 && flagv[FLAGV_COIN_COUNT] == 0) set_response(g, "YOU HAVE RUN OUT!");
    if ((h == 7562 || h == 7662) && flagv[FLAGV_COIN_COUNT] > 0 && loc[1] == INVENTORY) {
        set_response(g, "HE TAKES IT");
        flagv[FLAGV_TROLL_TOLL_PAID] = 1;
    }
    if (flagv[FLAGV_TROLL_TOLL_PAID] == 1) {
        flagv[FLAGV_COIN_COUNT]--;
        if (flagv[FLAGV_COIN_COUNT] == 0) loc[1] = GONE;
    }
    if (g->noun == 1) {
        set_response(g, "HE TAKES THEM ALL!");
        loc[1] = GONE;
        flagv[FLAGV_TROLL_TOLL_PAID] = 1;
        flagv[FLAGV_COIN_COUNT] = 0;
    }
    if (h == 2228 && loc[5] == GONE) { append_response(g, "HE LEADS YOU ", "NORTH"); loc[28] = GONE; g->room = 12; }
    if ((h == 2228 && loc[5] == INVENTORY) || h == 225) { append_response(g, "HE LEADS YOU ", "NORTH"); g->room = 12; }
    if ((h == 1228 && loc[5] == INVENTORY) || h == 125) { append_response(g, "HE LEADS YOU ", "SOUTH"); g->room = 22; }
    if (g->room == 7 || g->room == 33) { set_response(g, "HE EATS IT!"); loc[g->noun] = GONE; }
    if (h == 711) { flagv[FLAGV_HOUND_DISTRACTED] = 1; set_response(g, "HE IS DISTRACTED"); }
    if (h == 385 || h == 3824) { set_response(g, "THEY SCURRY AWAY"); loc[g->noun] = GONE; flagv[FLAGV_RATS_GONE] = 1; }
}

static void do_say(Game *g) {
    set_response(g, "YOU SAID IT");
    if (g->noun == 84) {
        set_response(g, "YOU MUST SAY THEM ONE BY ONE!");
        return;
    }
    if (g->room != 47 || g->noun < 71 || g->noun > 75 || loc[27] != INVENTORY) return;
    if (g->noun == 71 && flagv[FLAGV_SAID_AWAKE] == 0) { set_response(g, "THE MOUNTAIN RUMBLES!"); flagv[FLAGV_SAID_AWAKE] = 1; return; }
    if (g->noun == 72 && flagv[FLAGV_SAID_AWAKE] == 1 && flagv[FLAGV_SAID_GUIDE] == 0) { set_response(g, "TOWERS FALL DOWN!"); flagv[FLAGV_SAID_GUIDE] = 1; return; }
    if (g->noun == flagv[FLAGV_THIRD_WORD_SELECTOR] + 73 && flagv[FLAGV_SAID_AWAKE] == 1 && flagv[FLAGV_SAID_GUIDE] == 1) { flagv[FLAGV_QUEST_COMPLETE] = 1; return; }
    set_response(g, "THE WRONG SACRED WORD!");
    flagv[FLAGV_PLAYER_DEAD] = 1;
}

static void do_wear(Game *g) {
    if (g->noun == 5 || g->noun == 10) do_get(g);
    if (g->noun == 3) {
        flagv[FLAGV_BOOTS_WORN] = 1;
        flagv[FLAGV_GRARG_PATROL_WARNING] = 0;
        set_response(g, "YOU ARE INVISIBLE");
    }
    if (g->noun == 20) {
        flagv[FLAGV_UNIFORM_WORN] = 1;
        flagv[FLAGV_GRARG_PATROL_WARNING] = 0;
        set_response(g, "YOU ARE DISGUISED");
    }
}

static void do_remove(Game *g) {
    if (g->noun == 3 && flagv[FLAGV_BOOTS_WORN] == 1) { set_response(g, "TAKEN OFF"); flagv[FLAGV_BOOTS_WORN] = 0; }
    if (g->noun == 20 && flagv[FLAGV_UNIFORM_WORN] == 1) { set_response(g, "OK"); flagv[FLAGV_UNIFORM_WORN] = 0; }
    if (g->noun == 36 || g->noun == 50) do_move(g);
}

static void do_tie(Game *g) {
    int h = hcode(g);
    if (g->noun == 2 || g->noun == 14) set_response(g, "NOTHING TO TIE IT TO!");
    if (h == 7214) { set_response(g, "IT IS TIED"); loc[14] = 72; flagv[FLAGV_ROPE_TIED_AT_WELL] = 1; }
    if (h == 722) { set_response(g, "OK"); flagv[FLAGV_SHEET_TIED_AT_WELL] = 1; loc[2] = 72; }
}

static void do_climb(Game *g) {
    int h = hcode(g);
    if (h == 1547 && flagv[FLAGV_VINE_GROWN] == 1) { set_response(g, "ALL RIGHT"); g->room = 16; }
    if (g->noun == 14 || g->noun == 2) set_response(g, "NOT ATTACHED TO ANYTHING!");
    if (h == 5414 && loc[14] == 54) set_response(g, "YOU ARE AT THE TOP");
    if (h == 7214 && flagv[FLAGV_ROPE_TIED_AT_WELL] == 1) { set_response(g, "GOING DOWN"); g->room = 71; }
    if (h == 722 && flagv[FLAGV_SHEET_TIED_AT_WELL] == 1) { g->room = 71; set_response(g, "IT IS TORN"); loc[2] = GONE; flagv[FLAGV_SHEET_TIED_AT_WELL] = 0; }
    if (h == 7114 && flagv[FLAGV_ROPE_TIED_AT_WELL] == 1) { loc[14] = 71; flagv[FLAGV_ROPE_TIED_AT_WELL] = 0; set_response(g, "IT FALLS DOWN-BUMP!"); }
}

static void do_use(Game *g) {
    int h = hcode(g);
    if (h == 522) { set_response(g, "OK"); flagv[FLAGV_BOAT_POWERED] = 1; }
    if (g->noun == 1 || g->noun == 62 || g->noun == 5 || g->noun == 28 || g->noun == 11 || g->noun == 24) do_give(g);
    if (h == 416) { set_response(g, "YOU HAVE KEPT AFLOAT"); flagv[FLAGV_BOAT_KEPT_AFLOAT] = 1; return; }
    if (h == 4116) { set_response(g, "IT IS NOT BIG ENOUGH!"); return; }
    if (g->noun == 18 || g->noun == 7) do_break(g);
    if (g->noun == 13) do_drop(g);
    if (g->noun == 19) do_reflect(g);
    if (g->noun == 10) do_blow(g);
    if (g->noun == 16 || g->noun == 6) do_fill(g);
}

static void do_open(Game *g) {
    int h = hcode(g);
    if (g->noun == 76 || g->noun == 38) do_examine(g);
    if (h == 2030) { flagv[FLAGV_PHIAL_HIDDEN] = 0; set_response(g, "OK"); }
    if (h == 6030) { set_response(g, "OK"); flagv[FLAGV_BOOTS_HIDDEN] = 0; }
    if (h == 2444 || h == 1870) set_response(g, "YOU ARE NOT STRONG ENOUGH");
    if (h == 3756) { set_response(g, "A PASSAGE!"); strcpy(exits[37], "EW"); }
    if (h == 5960) {
        char input[64];
        set_response(g, "WHAT IS THE CODE");
        puts(g->response);
        if (read_input(input, sizeof(input)) && atoi(input) == flagv[FLAGV_SAFE_CODE]) {
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
    if (g->noun == 26) set_response(g, "YOU LIT THEM");
    if (h == 3826) set_response(g, "NOT BRIGHT ENOUGH");
    if ((g->noun == 23 || h == 6970) && loc[26] != INVENTORY) set_response(g, "NO MATCHES");
    if (g->noun == 23 && loc[26] == INVENTORY) { snprintf(g->response, sizeof(g->response), "A BRIGHT %s", g->verb_text); flagv[FLAGV_LAMP_LIT] = 1; }
    if (h == 6970 && loc[26] == INVENTORY) { flagv[FLAGV_HORSESHOE_LOOSENED] = 1; set_response(g, "IT HAS TURNED TO ASHES"); }
}

static void do_fill(Game *g) {
    int h = hcode(g);
    if ((g->noun == 16 || g->noun == 6) && (g->room == 41 || g->room == 51)) {
        set_response(g, "YOU CAPSIZED!");
        flagv[FLAGV_PLAYER_DEAD] = 1;
    }
    if (h == 6516 && loc[16] == INVENTORY) { set_response(g, "IT IS NOW FULL"); flagv[FLAGV_JUG_FULL] = 1; }
    if (h == 656) set_response(g, "IT LEAKS OUT!");
}

static void do_plant(Game *g) {
    if (g->noun != 22 || g->room != 15) {
        set_response(g, "DOES NOT GROW!");
        return;
    }
    set_response(g, "OK");
    flagv[FLAGV_SEEDS_PLANTED] = 1;
}

static void do_water(Game *g) {
    if (g->noun == 22 && flagv[FLAGV_SEEDS_PLANTED] == 1 && flagv[FLAGV_JUG_FULL] == 1) {
        set_response(g, "A LARGE VINE GROWS IN SECONDS!");
        flagv[FLAGV_VINE_GROWN] = 1;
    }
}

static void do_break(Game *g) {
    int h = hcode(g);
    if (g->noun == 7 || g->noun == 18) set_response(g, "THWACK!");
    if (h == 5818) { set_response(g, "YOU CLEARED THE WEBS"); flagv[FLAGV_COBWEBS_CLEARED] = 1; }
    if (h == 187) { set_response(g, "THE DOOR BROKE!"); strcpy(exits[18], "NS"); strcpy(exits[28], "NS"); }
    if (h == 717) { set_response(g, "YOU BROKE THROUGH"); strcpy(exits[71], "N"); }
}

static void do_swing(Game *g) {
    if (g->noun == 16) { g->noun = 22; do_water(g); }
    if (hcode(g) == 499) set_response(g, "WHERE?");
}

static void do_enter(Game *g) {
    if (hcode(g) == 4337) { g->verb = 2; do_move_direction(g); return; }
    if (g->room == 36) { set_response(g, "YOU FOUND SOMETHING"); flagv[FLAGV_PLANKS_HIDDEN] = 0; }
}

static void do_cross(Game *g) {
    if (g->room == 76) { g->verb = 4; do_move_direction(g); return; }
    if (g->room == 75) { g->verb = 2; do_move_direction(g); }
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
    if (g->room == 14 || g->room == 51) {
        set_response(g, "YOU HAVE DROWNED");
        flagv[FLAGV_PLAYER_DEAD] = 1;
    }
}

static void do_bail(Game *g) {
    set_response(g, "HOW?");
}

static void do_drop(Game *g) {
    int h = hcode(g);
    if (g->noun == 0 || g->noun > CARRIED_OBJECTS) return;
    loc[g->noun] = g->room;
    set_response(g, "DONE");
    if (h == 418 || h == 518) { set_response(g, "YOU DROWNED!"); flagv[FLAGV_PLAYER_DEAD] = 1; }
    if (g->noun == 8 && flagv[FLAGV_BOAT_POWERED] == 1) loc[2] = g->room;
    if (g->noun == 16 && flagv[FLAGV_JUG_FULL] == 1) { set_response(g, "YOU LOST THE WATER!"); flagv[FLAGV_JUG_FULL] = 0; }
    if (g->noun == 2 && flagv[FLAGV_BOAT_POWERED] == 1) flagv[FLAGV_BOAT_POWERED] = 0;
}

static void do_insert(Game *g) {
    if (g->noun == 62 && flagv[FLAGV_COIN_COUNT] == 0) set_response(g, "YOU DO NOT HAVE ANY");
    if (hcode(g) == 5762 && loc[1] == INVENTORY && flagv[FLAGV_COIN_COUNT] > 0) {
        flagv[FLAGV_COIN_COUNT]--;
        snprintf(g->response, sizeof(g->response), "A NUMBER APPEARS - %d", flagv[FLAGV_SAFE_CODE]);
        if (flagv[FLAGV_COIN_COUNT] == 0) loc[1] = GONE;
    }
}

static void do_throw(Game *g) {
    int h = hcode(g);
    if (g->noun == 0 || g->noun > CARRIED_OBJECTS) return;
    set_response(g, "DID NOT GO FAR!");
    loc[g->noun] = g->room;
    if (h == 3317) {
        set_response(g, "YOU CAUGHT THE BOAR");
        flagv[FLAGV_BOAR_CAUGHT] = 1;
    }
}

static void do_blow(Game *g) {
    int h = hcode(g);
    if (g->noun == 10) set_response(g, "A NICE TUNE");
    if (h == 5233) set_response(g, "WHAT WITH?");
    if (g->noun == 83) set_response(g, "HOW, O MUSICAL ONE?");
    if (h == 5610) { flagv[FLAGV_GHOST_FREED] = 1; set_response(g, "THE GHOST OF THE GOBLIN GUARDIAN IS FREE!"); strcpy(exits[56], "NS"); }
}

static void do_eat(Game *g) {
    if (g->noun == 0 || g->noun > CARRIED_OBJECTS) return;
    if (g->noun == 5 || g->noun == 24) { set_response(g, "YUM YUM!"); loc[g->noun] = GONE; }
}

static void do_move(Game *g) {
    int h = hcode(g);
    if (g->room == 4 && g->noun == 50) { flagv[FLAGV_RUBBLE_MOVED] = 1; set_response(g, "YOU REVEALED A STEEP PASSAGE"); }
    if (g->room == 3 && g->noun == 50) set_response(g, "YOU CANNOT MOVE RUBBLE FROM HERE");
    if (h == 7136) set_response(g, "THEY ARE WEDGED IN!");
}

static void do_into_or_poison(Game *g) {
    if ((g->noun == 67 || g->noun == 68) && loc[9] == INVENTORY && g->room == 49) {
        set_response(g, "OK");
        flagv[FLAGV_POISON_PLACED] = 1;
    }
}

static void do_ring(Game *g) {
    char input[64];
    int mr;
    if (g->room != 27 || g->noun != 63) return;
    do {
        printf("\nHOW MANY TIMES?\n");
        if (!read_input(input, sizeof(input))) return;
        mr = atoi(input);
        if (mr == 0) puts("A NUMBER");
    } while (mr == 0);
    if (mr == flagv[FLAGV_BELL_RING_COUNT]) {
        set_response(g, "A ROCK DOOR OPENS");
        strcpy(exits[27], "EW");
        return;
    }
    set_response(g, "YOU HAVE MISTREATED THE BELL!");
    flagv[FLAGV_PLAYER_DEAD] = 1;
}

static void do_cut(Game *g) {
    if (hcode(g) == 5861) {
        g->noun = 18;
        do_break(g);
    }
}

static void do_reflect(Game *g) {
    int h = hcode(g);
    if ((h == 4864 || h == 4819) && loc[19] == INVENTORY) {
        set_response(g, "YOU REFLECTED THE WIZARDS GLARE! HE IS DEAD");
        flagv[FLAGV_WIZARD_DEFEATED] = 1;
    }
    if (g->noun == 27) do_get(g);
}

static void do_with(Game *g) {
    if (hcode(g) == 7549 || hcode(g) == 7649) set_response(g, "WHAT WITH?");
    if (g->noun == 1 || g->noun == 62) do_give(g);
}

static void do_unlock(Game *g) {
    if (hcode(g) == 4870 && loc[21] == INVENTORY) {
        set_response(g, "THE KEY TURNS!");
        flagv[FLAGV_WIZARD_DOOR_UNLOCKED] = 1;
    }
}

static void do_break_verb(Game *g) {
    if (hcode(g) == 1870) set_response(g, "HOW?");
}

static void do_reflect_verb(Game *g) {
    if (g->room == 48) set_response(g, "HOW?");
}

static void dispatch(Game *g) {
    if (g->noun > CARRIED_OBJECTS || g->noun == 0) {
    } else if (!(g->verb == 8 || g->verb == 9 || g->verb == 14 || g->verb == 17 || g->verb == 44 || g->verb > 54)) {
        if (g->verb < VERB_COUNT && loc[g->noun] != INVENTORY) {
            snprintf(g->response, sizeof(g->response), "YOU DO NOT HAVE THE %s", g->noun_text);
            return;
        }
    }
    if (g->room == 56 && flagv[FLAGV_GHOST_FREED] == 0 && g->verb != 37 && g->verb != 53) {
        set_response(g, "THE GHOST OF THE GOBLIN GUARDIAN HAS GOT YOU!");
        return;
    }
    if (!(g->verb == 44 || g->verb == 47 || g->verb == 19 || g->verb == 57 || g->verb == 49)) {
        if (g->room == 48 && flagv[FLAGV_WIZARD_DEFEATED] == 0) {
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
        title();
        puts("HOOOOORRRRRRAAAAAYYYYYY!\n");
        puts("YOU HAVE SUCCEEDED IN YOUR");
        puts("QUEST AND BROUGHT PEACE TO");
        puts("THE LAND");
        g->running = 0;
        return;
    }
    if (g->room == 41) {
        flagv[FLAGV_ROUGH_WATER_TURN_COUNTER]++;
        if (flagv[FLAGV_ROUGH_WATER_TURN_COUNTER] == 10) {
            flagv[FLAGV_PLAYER_DEAD] = 1;
            set_response(g, "YOU SANK!");
        }
    }
    if (g->room == 56 && flagv[FLAGV_GHOST_FREED] == 0 && loc[10] != INVENTORY) {
        set_response(g, "THE GHOST OF THE GOBLIN GUARDIAN GETS YOU!");
        flagv[FLAGV_PLAYER_DEAD] = 1;
    }
    if (flagv[FLAGV_PLAYER_DEAD]) {
        title();
        puts(g->response);
        puts("YOU HAVE FAILED IN YOUR QUEST!");
        puts("\nBUT YOU ARE GRANTED ANOTHER TRY");
        wait_return();
        new_game(g);
    }
}

#ifndef SILVERM_TEST
int main(void) {
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
#endif
