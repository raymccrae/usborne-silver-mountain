#define _POSIX_C_SOURCE 200809L
#define SILVERM_TEST
#include "silverm.c"
#include <unistd.h>

static int failures;

static void check(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        failures++;
    }
}

static void set_scripted_input(const char *text) {
    FILE *fp = tmpfile();
    check(fp != NULL, "tmpfile() failed");
    if (!fp) {
        return;
    }
    fputs(text, fp);
    rewind(fp);
    game_input = fp;
}

static void clear_scripted_input(void) {
    if (game_input) {
        fclose(game_input);
        game_input = NULL;
    }
}

static void deterministic_new_game(Game *g) {
    srand(1);
    memset(g, 0, sizeof(*g));
    new_game(g);
    snprintf(maze_path[0], sizeof(maze_path[0]), "NNNNNNNN");
    snprintf(maze_path[1], sizeof(maze_path[1]), "SSSSSSSS");
    flagv[FLAGV_SAFE_CODE] = 123;
    flagv[FLAGV_BELL_RING_COUNT] = 3;
    flagv[FLAGV_THIRD_WORD_SELECTOR] = 0;
}

static void command(Game *g, const char *text) {
    char input[128];
    int before = g->room;
    snprintf(input, sizeof(input), "%s", text);
    parse_command(g, input);
    if (g->noun_text[0] == '\0') set_response(g, "I NEED TWO WORDS");
    if (g->verb > VERB_COUNT) set_response(g, "TRY SOMETHING ELSE");
    dispatch(g);
    after_turn(g);
    if (getenv("TRACE_SILVERM_TEST")) {
        fprintf(stderr, "%3d -> %3d  %-24s  %s\n", before, g->room, text, g->response);
    }
}

static void command_with_input(Game *g, const char *text, const char *answers) {
    set_scripted_input(answers);
    command(g, text);
    clear_scripted_input();
}

static void expect_room(Game *g, int room, const char *after) {
    if (g->room != room) {
        fprintf(stderr, "FAIL: after %s expected room %d, got %d (%s)\n",
                after, room, g->room, g->response);
        failures++;
    }
}

static void run_problem_checks(void) {
    Game g;
    char filename[128];

    deterministic_new_game(&g);
    g.room = 80;
    command(&g, "EXAMINE POT");
    command(&g, "GET COINS");
    check(loc[NOUN_COINS] == INVENTORY && flagv[FLAGV_COIN_COUNT] == 4, "getting coins gives four coins");
    snprintf(filename, sizeof(filename), "/tmp/silverm-test-save-%ld.sav", (long)getpid());
    check(save_game(&g, filename), "save_game succeeds");
    check(flagv[FLAGV_COIN_COUNT] == 4 && loc[NOUN_COINS] == INVENTORY, "saving does not lose coins");
    remove(filename);

    deterministic_new_game(&g);
    g.room = 80;
    command(&g, "EXAMINE POT");
    command(&g, "GET COINS");
    g.room = 75;
    command(&g, "GIVE COIN");
    check(flagv[FLAGV_COIN_COUNT] == 3 && loc[NOUN_COINS] == INVENTORY, "GIVE COIN spends one coin");

    deterministic_new_game(&g);
    g.room = 80;
    command(&g, "EXAMINE POT");
    command(&g, "GET COINS");
    g.room = 75;
    command(&g, "GIVE COINS");
    check(flagv[FLAGV_COIN_COUNT] == 0 && loc[NOUN_COINS] == GONE, "GIVE COINS keeps original all-coins behavior");

    deterministic_new_game(&g);
    g.room = 60;
    flagv[FLAGV_BOOTS_HIDDEN] = 0;
    command(&g, "GET BOOTS");
    command(&g, "WEAR BOOTS");
    command(&g, "N");
    command(&g, "E");
    command(&g, "W");
    command(&g, "E");
    command(&g, "W");
    command(&g, "E");
    check(flagv[FLAGV_BOOTS_WORN] == 0 && loc[NOUN_BOOTS] == GONE, "boots wear out after too many moves");

    deterministic_new_game(&g);
    g.room = 80;
    command(&g, "GET COINS");
    check(flagv[FLAGV_COINS_HIDDEN] == 1 && loc[NOUN_COINS] == 80, "coins are hidden before examining pot");
    command(&g, "EXAMINE POT");
    command(&g, "GET COINS");
    check(flagv[FLAGV_COINS_HIDDEN] == 0 && loc[NOUN_COINS] == INVENTORY, "examining pot reveals coins");
}

static void run_full_playthrough(void) {
    Game g;
    deterministic_new_game(&g);

    const char *commands[] = {
        "E", "E", "E", "EXAMINE CHEST", "EXAMINE POT", "GET COINS", "GET BREAD",
        "U", "EXAMINE BED", "GET SHEET", "N", "OPEN CUPBOARD", "GET BOOTS",
        "S", "D", "W", "W", "W", "W", "W", "GIVE COIN", "CROSS BRIDGE",
        "W", "EXAMINE TREES", "GET APPLES", "W", "W", "WEAR BOOTS", "W",
        "REMOVE BOOTS", "GET BUCKET", "E", "WEAR BOOTS", "N", "REMOVE BOOTS",
        "GET AXE", "N", "W", "GET BOAT", "W", "USE SHEET", "W", "N", "N",
        "DROP BOAT", "N", "EXAMINE KILN", "GET JUG", "N", "GET REEDS",
        "S", "S", "E", "EXAMINE KETCH", "GET NET", "W", "GET BOAT",
        "S", "S", "USE BUCKET", "S", "E", "DROP BOAT", "DROP BUCKET",
        "GET SHEET", "E", "N", "E", "U", "GET SACKS", "EXAMINE SACKS",
        "GET SEEDS", "D", "W", "N", "N", "THROW NET", "N", "TURN WHEEL",
        "N", "GET LAMP", "E", "EXAMINE POOL", "S", "EXAMINE SLUICE GATES",
        "TURN HANDLE", "N", "EXAMINE POOL", "GET SHIELD", "W", "S", "W",
        "GIVE APPLE", "N", "READ INSCRIPTIONS", "E", "E", "MOVE RUBBLE",
        "W", "W", "GET BONE", "E", "S", "GIVE APPLE", "E", "S", "S", "S",
        "S", "E", "E", "FILL JUG", "S", "E", "GIVE COIN", "CROSS BRIDGE",
        "E", "E", "N", "EXAMINE STONES", "W", "W", "N", "BLOW REEDS",
        "DROP REEDS", "N", "N", "ENTER HUT", "GET PLANKS", "W", "DROP PLANKS",
        "S", "D", "W", "GET ROPE", "E", "N", "U", "N", "N", "PLANT SEEDS",
        "WATER SEEDS", "DROP JUG", "DROP SEEDS", "CLIMB VINE", "READ INSCRIPTION",
        "N", "W", "W", "W", "MOVE RUBBLE", "E",
        NULL
    };

    for (int i = 0; commands[i]; i++) {
        command(&g, commands[i]);
    }
    command_with_input(&g, "E", "N\nN\nN\nN\nN\nN\nN\nN\n");
    expect_room(&g, 6, "first return through maze");

    const char *palace[] = {
        "E", "S", "N", "E", "GIVE BONE", "E", "E", "E", "WEAR BOOTS", "E",
        "REMOVE BOOTS", "GET SWORD", "WEAR BOOTS", "S", "REMOVE BOOTS",
        "EXAMINE KITCHENS", "OPEN CUPBOARD", "GET PHIAL", "EXAMINE PHIAL",
        "S", "GET BROOCH", "N", "W", "S", "EXAMINE GRARGS", "WEAR BOOTS",
        "S", "REMOVE BOOTS", "S", "POISON WINE", "E", "N", "N", "DROP BOOTS",
        "GET UNIFORM", "WEAR UNIFORM", "EXAMINE UNIFORM", "N", "E", "N",
        "W", "W", "S", "SWING AXE", "S", "S", "W", "LIGHT LAMP", "W",
        "GIVE BREAD", "W", "OPEN CASK", "W", "E", "E", "EXAMINE STABLES",
        "EXAMINE DOOR", "GET HORSESHOE", "OPEN DOOR", "BURN DOOR",
        "GET HORSESHOE", "S", "W", "W", "W", "W", "GIVE COIN", "CROSS BRIDGE",
        "W", "W", "W", "TIE SHEET", "CLIMB SHEET", "EXAMINE WELL BOTTOM",
        "SWING AXE", "N", "GET RING", "E", "S", "E", "N", "E", "GIVE BROOCH",
        "W", "N", "N", "N", "N", "W", "GIVE APPLE", "N", "E", "E", "E",
        NULL
    };
    for (int i = 0; palace[i]; i++) {
        command(&g, palace[i]);
    }
    command_with_input(&g, "E", "N\nN\nN\nN\nN\nN\nN\nN\n");
    expect_room(&g, 6, "second return through maze");

    const char *finale[] = {
        "E", "E", "W", "S", "W", "S", "E", "E",
        NULL
    };
    for (int i = 0; finale[i]; i++) {
        command(&g, finale[i]);
    }
    command_with_input(&g, "RING BELL", "3\n");

    const char *endgame[] = {
        "E", "N", "N", "E", "E", "S", "W", "S", "S", "S", "E",
        "GET SILVER PLATE", "N", "READ BOOK", "S", "W", "S", "W", "W",
        "CUT COBWEBS", "W", "INSERT COIN", "E", "E",
        NULL
    };
    for (int i = 0; endgame[i]; i++) {
        command(&g, endgame[i]);
    }
    command_with_input(&g, "OPEN SAFE", "123\n");

    const char *win[] = {
        "GET KEY", "W", "N", "W", "HOLD UP SILVER PLATE", "W", "UNLOCK DOOR",
        "DROP KEY", "W", "GET STONE OF DESTINY", "SAY MAGIC WORDS",
        "SAY AWAKE", "SAY GUIDE", "SAY PROTECT",
        NULL
    };
    for (int i = 0; win[i]; i++) {
        command(&g, win[i]);
    }

    check(flagv[FLAGV_QUEST_COMPLETE] == 1, "full playthrough reaches victory flag");
    check(g.running == 0, "full playthrough stops after victory");
}

int main(void) {
    int saved_stdout = dup(fileno(stdout));
    FILE *null_stdout = fopen("/dev/null", "w");
    if (saved_stdout >= 0 && null_stdout) {
        fflush(stdout);
        dup2(fileno(null_stdout), fileno(stdout));
    }

    run_problem_checks();
    run_full_playthrough();

    if (saved_stdout >= 0 && null_stdout) {
        fflush(stdout);
        dup2(saved_stdout, fileno(stdout));
        close(saved_stdout);
        fclose(null_stdout);
    }

    if (failures) {
        fprintf(stderr, "%d test(s) failed\n", failures);
        return 1;
    }
    puts("All silverm tests passed");
    return 0;
}
